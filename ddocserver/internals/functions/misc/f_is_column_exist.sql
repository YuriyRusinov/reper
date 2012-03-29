create or replace function f_is_column_exist(varchar, varchar) returns bool as
$BODY$
declare
    tableName alias for $1;
    columnName alias for $2;

    attCount int4;
begin

    select count(a.attname)  into attCount
    from
        pg_class c,
        pg_attribute a
    where
        c.oid = a.attrelid
        and c.relname = tableName
        and a.attname = columnName;   

    if(attCount >= 1) then
        return TRUE;
    else
        return FALSE;
    end if;

    return FALSE;
    
    exception when others then
        return FALSE;

end
$BODY$
language 'plpgsql';
