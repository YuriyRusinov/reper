create or replace function getPrimaryKey(varchar) returns varchar as
$BODY$
declare
    tableName alias for $1;
    pkName varchar;
    tName varchar;
    isExist int4;
begin

    if(substr(tableName, 1, 4) = 'tbl_' or
        tableName = 'rec_attrs_values' or
        tableName = 'attrs_values' 
      ) then
        tName := tableName;
    else

        select f_is_view_exist(tableName) into isExist; --if view with given name is exist this means that the real table has name with prefix tbl_
        if(isExist = 1) then
            tName := 'tbl_' || tableName;
        else
            tName := tableName;
        end if;

    end if;

    SELECT a.attname into pkName
    FROM 
        pg_class c, 
        pg_class c2, 
        pg_index i, 
        pg_attribute a
    WHERE 
        c.relname = tName 
        AND c.oid = i.indrelid 
        AND i.indexrelid = c2.oid
        AND i.indisprimary 
        AND i.indisunique 
        AND a.attrelid=c2.oid
        AND a.attnum>0
    LIMIT 1;

    return pkName;
end
$BODY$
language 'plpgsql';

create or replace function getPrimaryKeys(varchar) returns setof varchar as
$BODY$
declare
    tableName alias for $1;
    pkName varchar;
    tName varchar;
    r record;
    isExist int4;
begin

    if(substr(tableName, 1, 4) = 'tbl_' or
        tableName = 'rec_attrs_values' or
        tableName = 'attrs_values' 
      ) then
        tName := tableName;
    else

        select f_is_view_exist(tableName) into isExist; --if view with given name is exist this means that the real table has name with prefix tbl_
        if(isExist = 1) then
            tName := 'tbl_' || tableName;
        else
            tName := tableName;
        end if;

    end if;



    for r in 
        SELECT a.attname
        FROM 
            pg_class c, 
            pg_class c2, 
            pg_index i, 
            pg_attribute a
        WHERE 
            c.relname = tName 
            AND c.oid = i.indrelid 
            AND i.indexrelid = c2.oid
            AND i.indisprimary 
            AND i.indisunique 
            AND a.attrelid=c2.oid
            AND a.attnum>0
    loop
        return next r.attname;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
