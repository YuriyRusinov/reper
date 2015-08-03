create or replace function ioInsertEmptyRecord(int4, varchar) returns int4 as
$BODY$
declare
    idObject alias for $1;
    tableName alias for $2;
    
    res int4;

    tName varchar;
    idCategory int4;
    idChild int4;

    query varchar;
    insert_q varchar;
    values_q varchar;
    r record;
    rr record;
begin
    res = 0;

    if(idObject is null or idObject <= 0 or tableName isnull) then
        return res;
    end if;

    for r in 
        select table_name, id_io_category from f_sel_io_objects(idObject) where id = idObject
    loop
        tName = r.table_name;
        idCategory = r.id_io_category;
        if(tName isnull or tName <> tableName) then
            for rr in
                select table_name, id_io_category from object_ref_tables where id_io_object = idObject and table_name = tableName
            loop
                tName = rr.table_name;
                idCategory = rr.id_io_category;
            end loop;
        end if;
    end loop;

    if(FOUND = FALSE) then
        return res;
    end if;

    if(tName isnull or idCategory isnull) then
        return res;
    end if;

    select id_child into idChild from io_categories where id = idCategory;
    if(idChild isnull) then
        return res;
    end if;

    idCategory = idChild;

    query := 'select 
                   a.id, 
                   a.code, 
                   ac.is_mandatory, 
                   ac.def_value, 
                   at.code as atype, 
                   at.id as atypeid, 
                   a.table_name as atabname 
               from 
                   attributes a 
                   inner join attrs_categories ac on (a.id = ac.id_io_attribute and ac.id_io_category=' || idCategory || ' and a.id > 1) 
                   inner join a_types at on (a.id_a_type = at.id);';

    raise warning '%', query;
    
    select getNextSeq(tName, 'id') into res;
    if(res <= 0) then
        return 0;
    end if;
    
    insert_q = 'insert into ' || tName || ' ( id ';
    values_q = ' ' || res;

    for r in
        execute query
    loop
        if (r.is_mandatory) then
            if(r.def_value isnull) then
                insert_q := insert_q || ', "' || r.code || '"';
            else 
                continue;
            end if;
        else
            continue;
        end if;

         --mandatory and def_value isnull

        if (r.atypeid = 4 or r.atypeid = 5 or r.atypeid = 27) then --DATE OR TIMESTAMP or TIME
            values_q := values_q || ', current_timestamp';
        elsif (r.atypeid = 10) then --INTERVAL
            values_q := values_q || ', {1, 1}';

        elsif (r.atypeid = 12) then -- INT4[]
            values_q := values_q || ', {1}';
        elsif (r.atypeid = 1) then --BOOL
            values_q := values_q || ', TRUE';
        elsif (r.atypeid = 2 or r.atypeid = 3 or r.atypeid = 6 or r.atypeid = 7 or r.atypeid = 8 or r.atypeid = 18 or r.atypeid = 19 or r.atypeid = 25 or r.atypeid = 26) then
            --INT4, FLOAT8
            values_q := values_q || ', 1::' || r.atype;
        else
            values_q := values_q || ', ' || quote_literal(' ');
        end if;

    end loop;

    raise warning '%', insert_q;
    insert_q = insert_q || ') values (' || values_q || ')';
    execute insert_q;

    return res;

    --exception when others
    --then
     --   return -1;
end
$BODY$
language 'plpgsql';
