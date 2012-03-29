create or replace function getTableByUID (varchar) returns varchar as
$BODY$
declare
    table_uid alias for $1;

    query varchar;
    tableName varchar;
begin

    query := 'select io.table_name from f_sel_io_objects(' ||  quote_literal (table_uid) || ') io where io.unique_id=' || quote_literal (table_uid);
    raise notice '%', query;
    execute query into tableName;
    return tableName;
end
$BODY$
language 'plpgsql';
