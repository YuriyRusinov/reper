create or replace function getIdObjectByRecordUID(varchar) returns int4 as
$BODY$
declare
    recUID alias for $1;

    tableName varchar;
    tName varchar;
    idObject int4;
begin

    SELECT p.relname into tableName
    FROM q_base_table qq, pg_class p
    WHERE qq.unique_id = recUID and qq.tableoid = p.oid;

    if(substr(tableName, 1, 4) = 'tbl_') then
        tName = substr(tableName, 5);
    else
        tName = tableName;
    end if;

    select id into idObject from tbl_io_objects where table_name = tName;

    return idObject;

end
$BODY$
language 'plpgsql' security definer;
