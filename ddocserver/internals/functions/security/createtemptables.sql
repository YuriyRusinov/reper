create or replace function createTempTables() returns void as
$BODY$
begin
    perform createLastErrorTable();
    perform createTableCurrDl();
    perform createControlPrivTable();

    return;
end
$BODY$
language plpgsql;
