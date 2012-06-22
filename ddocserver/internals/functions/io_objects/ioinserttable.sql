create or replace function ioInsertTable (varchar, 
                                          int4, 
                                          int4, 
                                          varchar, 
                                          varchar,
                                          int4,
                                          varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    idObject alias for $2;
    idCategory alias for $3;
    tName alias for $4;
    uniqueid alias for $5;
    idSearchTemplate alias for $6;
    refTableName alias for $7;

    res int4;
    idTable int4;
    tNameJ varchar;
begin
    if (tableName is null or idObject is null or idCategory is null or tName is null) then
        raise notice 'All parameters have to be not null';
        return -1;
    end if;

    select getNextSeq('object_ref_tables', 'id') into idTable;

    if (uniqueid is not null) then
        tNameJ := 'eio_table_' || idObject || '_' || idTable;
    else
        tNameJ := tableName;
    end if;
    insert into object_ref_tables 
    (
         id,
         table_name,
         id_io_object,
         id_io_category,
         title,
         unique_id,
         id_search_template,
         ref_table_name
    ) values (
         idTable,
         tNameJ,
         idObject,
         idCategory,
         tName,
         uniqueid,
         idSearchTemplate,
         refTableName
    );

    if (not FOUND) then
        return -1;
    end if;

    return idTable;
end
$BODY$ 
language 'plpgsql';

create or replace function createIOTable (varchar, int4, bool) returns varchar as
$BODY$
declare
    table_name alias for $1;
    id_io_category alias for $2; --CHILD CATEGORY, NOT MAIN!!!
    bInheritRootTable alias for $3;

    create_query varchar; --creates the table
    alter_query varchar; --alter the table
    create_ref_table varchar; --create some of reference tables of the category have some attributes with type 17 (atCheckListEx)
    
    query varchar;
    r RECORD;
    res int4;
    amount int4;
    what int4;
    unit varchar;
    c_query varchar;

    s_user varchar; -- session_user
    c_user varchar; -- current user
    
    tName varchar; --for atCheckListEx
    q varchar; --for atCheckListEx
    rr RECORD; --for atCheckListEx
    isExist int4;

    refColumnName varchar;
begin
    select into res count (*) from pg_tables as t where t.tablename = table_name;
    if (res > 0) then
        raise exception 'entity % (table) already exists in database', table_name;
        return NULL;
    end if;

    select into res count (*) from pg_views as v where v.viewname = table_name;
    if (res > 0) then
        raise exception 'entity % (view) already exists in database', table_name;
        return NULL;
    end if;

    create_query := 'create table ' || table_name || ' (id int8 not null default pg_catalog.nextval(' || quote_literal('q_base_table_id_seq') || '), ';

    query := 'select 
                   a.id, 
                   a.code, 
                   ac.is_mandatory, 
                   ac.def_value, 
                   at.code as atype, 
                   at.id as atypeid, 
                   a.table_name as atabname,
                   a.ref_column_name as refcol 
               from 
                   attributes a 
                   inner join attrs_categories ac on (a.id = ac.id_io_attribute and ac.id_io_category=' || id_io_category || ' and a.id > 1) 
                   inner join a_types at on (a.id_a_type = at.id);';

    alter_query = '';
    create_ref_table = '';

    for r in
        execute query
    loop
        
        refColumnName = r.refcol;
        if(r.refcol isnull) then
            refColumnName = 'id';
        end if;

        --atCheckListEx
        if(r.atypeid = 17) then --atCheckListEx

            tName := table_name || '_' || r.atabname || '_ref_' || r.id;

            --if table is already exist then we just return and do nothing
            q := 'select f_is_table_exist(' || quote_literal(tName) || ', NULL) as is_exist';
            for rr in execute q
            loop
                --TODO: need add check that if the table exist that has the structure aka (id_1 int4, id_2 int4)
                --else we should create the new table
                if(rr.is_exist <> 1) then
                    create_ref_table := create_ref_table || ' create table ' || tName || ' (id_' || table_name || ' int8, id_' || r.atabname || ' int8); ';
                    --Ќа эту таблицу пока проверку на префикс tbl_ делать не надо, ибо она еще не переименована (находитс€ в процессе создани€ в данном триггере)
                    create_ref_table := create_ref_table || ' alter table '  || tName || ' ADD CONSTRAINT FK_ID_' || tName || '_1 FOREIGN KEY (ID_' || table_name || ') REFERENCES ' || table_name || ' (ID) ON DELETE CASCADE ON UPDATE CASCADE; ';

                    select f_is_view_exist(r.atabname) into isExist; --≈сли представление с таким названием существует, то это означает, что реальна€ таблица имеет название с префиксом tbl_
                    if(isExist = 1) then
                        create_ref_table := create_ref_table || ' alter table '  || tName || ' ADD CONSTRAINT FK_ID_' || tName || '_2 FOREIGN KEY (ID_'|| r.atabname || ') REFERENCES tbl_' || r.atabname || ' (ID) ON DELETE RESTRICT ON UPDATE CASCADE; ';
                    else
                        create_ref_table := create_ref_table || ' alter table '  || tName || ' ADD CONSTRAINT FK_ID_' || tName || '_2 FOREIGN KEY (ID_'|| r.atabname || ') REFERENCES ' || r.atabname || ' (ID) ON DELETE RESTRICT ON UPDATE CASCADE; ';
                    end if;
                    create_ref_table := create_ref_table || ' select setGrants1(' || quote_literal(tName) || '); ';
                end if;
            end loop;

            --continue;
        --end if;
        else

        create_query := create_query || '"' || r.code || '" ' || r.atype;

        --mandatory
        
        if (r.is_mandatory) then
            create_query := create_query || ' not null';
        else
            create_query := create_query || ' null';
        end if;

        --def_value
        
        if (r.def_value is not null) then
            create_query := create_query || ' default ';

            if (r.atypeid = 4 or r.atypeid = 5) then --DATE OR TIMESTAMP
                if (r.def_value = 'current_timestamp') then
                    create_query := create_query || ' current_timestamp';
                else
                    create_query := create_query || ' to_timestamp( ' || quote_literal (r.def_value) || ', ' || quote_literal ('DD.MM.YYYY HH24:MI:SS') || '::timestamp)';
                end if;
            elsif (r.atypeid = 10) then --INTERVAL
                if (not is_int4 (split_part (r.def_value, ' ', 1))) then
                    raise exception 'Interval value is invalid';
                    return NULL;
                end if;
                amount := split_part (r.def_value, ' ', 1)::int4;
                unit := split_part (r.def_value, ' ', 2);
                what := 0;
                if (lower (unit) = 'years') then
                    what := 1;
                elsif (lower (unit) = 'half-years') then
                    what := 2;
                elsif (lower (unit) = 'quarters') then
                    what := 3;
                elsif (lower (unit) = 'months') then
                    what := 4;
                elsif (lower (unit) = 'weeks') then
                    what := 5;
                elsif (lower (unit) = 'days') then
                    what := 6;
                elsif (lower (unit) = 'hours') then
                    what := 7;
                else
                    raise exception 'Invalid unit';
                    return NULL;
                end if;
                create_query := create_query || '{' || amount || ',' || what || '}';
                ---r.def_value;
                ---TODO : need working.

            elsif (r.atypeid = 12) then -- INT4[]
                if(substr(r.def_value, 1, 1) <> '{') then
                    create_query := create_query || quote_literal ('{' || r.def_value || '}');
                else
                    create_query := create_query || quote_literal (r.def_value);
                end if;
            elsif (r.atypeid = 9 or r.atypeid = 11 or r.atypeid = 14 or r.atypeid = 15) then
                create_query := create_query || quote_literal (r.def_value);
            else
                create_query := create_query || r.def_value;
            end if;
        end if;  --defvalue

        create_query = create_query || ', ';
        --atList or atParent
        
        if (r.atypeid = 2 or r.atypeid = 3) then --atList, atParent
            if (alter_query is null) then
                alter_query := 'ALTER TABLE "' || table_name || '" ADD CONSTRAINT "FK_' || table_name || '_REF_' || r.code || '" foreign key ("' || r.code || '") references ';
            else
                alter_query := alter_query || 'ALTER TABLE "' || table_name || '" ADD CONSTRAINT "FK_' || table_name || '_REF_' || r.code || '" foreign key ("' || r.code || '") references ';
            end if;
            if (r.atypeid = 3) then
                alter_query := alter_query || table_name;
            else
                select f_is_view_exist(r.atabname) into isExist; --≈сли представление с таким названием существует, то это означает, что реальна€ таблица имеет название с префиксом tbl_
                if(isExist = 1) then
                    alter_query := alter_query || 'tbl_' || r.atabname;
                else
                    alter_query := alter_query || r.atabname;
                end if;
            end if;
            alter_query := alter_query || ' ("' || refColumnName || '") on delete restrict on update cascade;';
        end if;

        --i := i+1;
        end if;
    end loop; --main loop

    create_query := create_query || ' constraint PK_' || table_name || ' primary key (id)) ';
    if (bInheritRootTable = TRUE) then
        create_query := create_query || ' inherits (root_table, q_base_table); ';
        
        create_query := create_query || ' create trigger trgSetUID before insert or update on ' || table_name || ' for each row execute procedure uidCheck(); ';
        create_query := create_query || ' create trigger trgSyncRecords before insert or update or delete on ' || table_name || ' for each row execute procedure syncRecords(); ';
        create_query := create_query || ' create trigger trgCheckTableForOwner before insert or update or delete on ' || table_name || ' for each row execute procedure checkTableForOwner(); ';
        create_query := create_query || ' create trigger trgSetUUID before insert or update on ' || table_name || ' for each row execute procedure uuidCheck(); ';
        create_query := create_query || ' create trigger trg_fk_q_base_table_check1 before update or delete on ' || table_name || ' for each row execute procedure fkQBaseTableCheck1(); ';
        create_query := create_query || ' create unique index i_unique_id_' || table_name || ' on ' || table_name || ' using BTREE (unique_id); ';
    end if;
 
    create_query := create_query || ';';

    s_user = session_user;

    create_query := create_query || ' alter table ' || table_name || ' owner to ' || s_user;
    --create_query := create_query || '; grant all on ' || table_name || '_id_seq to ' || s_user;
    

    execute create_query;
    if (not FOUND) then
        raise exception 'Cannot create table %', table_name;
        return NULL;
    end if;

    --for user_chains
    c_query := 'select count(uc.id_io_category) from user_chains uc where uc.id_io_category = ' || id_io_category;
    execute c_query into amount;
--    raise warning '%', amount;
    if(amount > 0) then
        perform createTriggerUserHandler(table_name);
    end if;
   

    if (alter_query is not null and length (alter_query) > 0) then
        execute alter_query;
        if (not FOUND) then
            raise exception 'Cannot create reference';
            return NULL;
        end if;
    end if;

    if(create_ref_table is not null and length (create_ref_table) > 5) then
        execute create_ref_table;
        if(not FOUND) then
            raise exception 'Cannot create reference tables!';
            return NULL;
        end if;
    end if;

    --ѕримен€ем подсистему дискреционого разграничени€ доступа
    perform acl_secureTable(table_name);

    return table_name;
end
$BODY$ 
language 'plpgsql' security definer;



create or replace function createIOView (varchar, int4, int4, varchar) returns varchar as
$BODY$
declare
    viewName alias for $1;
    idCategory alias for $2; --CHILD CATEGORY, NOT MAIN!!!
    idSearchTemplate alias for $3;
    refTableName alias for $4;

    res int4;
    wherePart varchar;
    whatPart varchar;
    columnsPart varchar;
    q varchar;

begin

    select into res count (*) from pg_tables as t where t.tablename = viewName;
    if (res > 0) then
        raise exception 'entity % (table) already exists in database', viewName;
        return NULL;
    end if;

    select into res count (*) from pg_views as v where v.viewname = viewName;
    if (res > 0) then
        raise exception 'entity % (view) already exists in database', viewName;
        return NULL;
    end if;

    wherePart = generateFilterQuery(refTableName, idSearchTemplate);

    select 
        array_to_string(array_agg(refTableName || asString('."', false) || a.code || asString('"', false) ),  ',') into whatPart 
    from 
        attributes a, 
        attrs_categories ac 
    where 
        a.id = ac.id_io_attribute 
        and ac.id_io_category = idCategory;

    if(whatPart isnull) then
        raise exception 'Nothing to select from view! category does not contain any attributes';
        return NULL;
    end if;

    q = 'create view ' || viewName || ' as select ' || whatPart || ' from ' || refTableName || ' where 1=1 ' || wherePart;

    raise notice '%', q;

    execute q;
    if (not FOUND) then
        raise exception 'Cannot create view %', viewName;
        return NULL;
    end if;


    select 
        array_to_string(array_agg(asString('new."', false) || a.code || asString('"', false)), ',') into whatPart 
    from 
        attributes a, 
        attrs_categories ac 
    where 
        a.id = ac.id_io_attribute 
        and ac.id_io_category = idCategory;

    select 
        array_to_string(array_agg(asString('"', false) || a.code || asString('"', false)), ',') into columnsPart 
    from 
        attributes a, 
        attrs_categories ac 
    where 
        a.id = ac.id_io_attribute 
        and ac.id_io_category = idCategory;

    q = 'create rule i_rule_' || viewName || ' as on INSERT TO ' || viewName || 
          ' do instead insert into ' || refTableName || '(' || columnsPart || ') values (' || whatPart || ');';
    raise notice '%', q;

    execute q;
    if (not FOUND) then
        raise exception 'Cannot create rule for INSERT on view %', viewName;
        return NULL;
    end if;



    select 
        array_to_string(array_agg('"' || a.code || '" = new."' || a.code || '"'), ',') into whatPart 
    from 
        attributes a, 
        attrs_categories ac 
    where 
        a.id = ac.id_io_attribute 
        and ac.id_io_category = idCategory
        and a.code <> 'id';

    q = 'create rule u_rule_' || viewName || ' as on UPDATE TO ' || viewName || 
          ' do instead update ' || refTableName || ' set ' || whatPart || ' where id = old.id;';
    raise notice '%', q;

    execute q;
    if (not FOUND) then
        raise exception 'Cannot create rule for UPDATE on view %', viewName;
        return NULL;
    end if;


    q = 'create rule d_rule_' || viewName || ' as on DELETE TO ' || viewName || 
          ' do instead delete from ' || refTableName || ' where id = old.id;';
    raise notice '%', q;

    execute q;
    if (not FOUND) then
        raise exception 'Cannot create rule for DELETE on view %', viewName;
        return NULL;
    end if;

    --ѕримен€ем подсистему дискреционого разграничени€ доступа
    perform acl_secureTable(viewName);

    return viewName;
end
$BODY$
language 'plpgsql';
