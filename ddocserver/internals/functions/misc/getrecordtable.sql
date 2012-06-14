create or replace function getRecordTable(int8) returns varchar as
$BODY$
declare
    idRecord alias for $1;
    tName varchar;
begin
    SELECT p.relname into tName
    FROM q_base_table q, pg_class p
    WHERE q.id = idRecord AND q.tableoid = p.oid;

    return tName;
end
$BODY$
language 'plpgsql';
