
create or replace function translateTablesToSchema(varchar[]) returns int4 as
$BODY$
declare
    theSchemas alias for $1;

    schemas varchar[];
    r record;
    currentTable varchar;
    query varchar;

    c int4;
    i int4;
    res int4;

    idCategory int4;
    idChild int4;
    idObject int4;

    idAttr int4;

    idAttrType int4;
    tableOID varchar;
    columnName varchar;
    defValue varchar;
    isMandatory bool;
    isReadOnly bool;

    attLength int4;
    defWidth int4;
    
begin

    if(theSchemas isnull) then
        schemas[1] = 'public';
    else
        schemas = theSchemas;
    end if;

    c = array_upper(schemas, 1);
    if(c = 0) then
        return -1;
    end if;

    query = 'set search_path to public';

    for i in 1..c
    loop
        if(schemas[i] = 'public') then
            return -2;
        end if;
        query = query || ', ' || schemas[i];
    end loop;

    execute query;

    create table public.ex_table () inherits (root_table, q_base_table);

    currentTable = '';
    isReadOnly = false;
    isMandatory = false;
    defValue = NULL;

    create temp table t_cat_created(id_cat int4);
    
    perform f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
    perform f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

    for r in 
        select * 
        from translateColumnsToAttrs(schemas) --where relname <> 'rubr_group'
    loop

        if(currentTable <> r.relname) then
            idChild = cInsert(r.rel_desc, NULL, r.rel_desc, 10, NULL, false, NULL, true, 1);--child
            if(idChild isnull or idChild <= 0) then
                drop table t_cat_created;
                return -4;
            end if;
            insert into t_cat_created values(idChild);
            
            idCategory = cInsert('Справочник для ' || r.rel_desc, NULL, 'Справочник для ' || r.rel_desc, 1, idChild, true, NULL, true, 1);
            if(idCategory isnull or idCategory <= 0) then
                drop table t_cat_created;
                return -4;
            end if;
            insert into t_cat_created values(idCategory);

            idObject = ioInsert(r.rel_desc, idCategory, 1, 1, r.relname, r.rel_desc, NULL, 1, NULL, 1, NULL, true, NULL, NULL, NULL, NULL, 1); --create category and IO  select * from io_sync_types
            if(idObject isnull or idObject <= 0) then
                drop table t_cat_created;
                return -5;
            end if;

            query = 'alter table ' || r.relname || ' owner to admin';
            execute query;
            query = 'alter table ' || r.relname || ' add column id bigserial, add column uuid_t uuid, add column unique_id varchar, add column last_update timestamp';
            execute query;
            
            query = 'update ' || r.relname || ' set id = pg_catalog.nextval(' || quote_literal('q_base_table_id_seq') || ')';
            execute query;

            query = 'update ' || r.relname || ' set unique_id = generateUID(id, ' || quote_literal(r.relname) || '), last_update = current_timestamp';
            execute query;

            query = 'update ' || r.relname || ' set uuid_t = generateUUID()';
            execute query;

            query = 'alter table ' || r.relname || ' alter column id set not null';
            execute query;
            query = 'alter table ' || r.relname || ' alter column unique_id set not null';
            execute query;
            query = 'alter table ' || r.relname || ' alter column uuid_t set not null';
            execute query;
            query = 'alter table ' || r.relname || ' alter column last_update set not null';
            execute query;
            query = 'alter table ' || r.relname || ' alter column last_update set default current_timestamp';
            execute query;


            query = 'alter table ' || r.relname || ' inherit ex_table;';
            execute query;

            query = 'create unique index i_' || r.relname || '_id on ' || r.relname || ' using btree (id);';
            execute query;

            query = 'create unique index i_' || r.relname || '_unique_id on ' || r.relname || ' using btree (unique_id);';
            execute query;

            perform setMacToNULL(r.relname);
            perform createTriggerUID(r.relname);
            perform createTriggerUUID(r.relname);
            perform createTriggerQBaseTableCheck1(r.relname);
        end if;
        
        currentTable = r.relname;
        columnName = r.attfname;

        attLength = 0;
        defWidth = 100;

        if( /*(r.att_type = 'int2' or r.att_type = 'int4' or r.att_type = 'int8') and*/ r.relfname is not null) then
            if(r.relname = r.relfname) then
                idAttrType = 3;
            else
                idAttrType = 2;
            end if;
        elsif(r.att_type = 'int2' or r.att_type = 'int4' or r.att_type = 'int8') then
            idAttrType = 8;
        elsif(r.att_type = 'bool') then
            idAttrType = 1;
        elsif(r.att_type = 'time') then
            idAttrType = 27;
        elsif(r.att_type = 'date') then
            idAttrType = 4;
        elsif(r.att_type = 'timestamp') then
            idAttrType = 5;
        elsif(r.att_type = 'numeric' or r.att_type = 'float8' or r.att_type = 'double') then
            idAttrType = 6;
        else
            if(r.att_typlen is not null and r.att_typlen > 4) then
                attLength = r.att_typlen - 4;
            elsif(r.att_len is not null and r.att_len > 4) then
                attLength = r.att_len - 4;
            else
                attLength = 0;
            end if;
            
            if(attLength > 0) then
                idAttrType = 14;
                defWidth = attLength;
            else
                idAttrType = 9; --varchar
            end if;
        end if;

        idAttr = acInsert1(idChild, 
                          idAttrType, 
                          r.attname, --code
                          r.description,
                          r.description,
                          r.relfname,
                          columnName,
                          defWidth,
                          defValue,
                          isMandatory,
                          isReadOnly,
                          NULL, -- unique_id
                          columnName);

        if(idAttr isnull or idAttr <= 0) then
            drop table t_cat_created;
            return -3;
        end if;
        
    end loop;

    update io_categories set is_completed = 1 where id in (select id_cat from t_cat_created);

    drop table t_cat_created;

    perform f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');
    perform f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');

    return 1;

end
$BODY$
language 'plpgsql' security definer;
