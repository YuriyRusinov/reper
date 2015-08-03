create or replace function getCurrentTransport() returns int4 as
$BODY$
declare
    cnt int4;
    idTransport int4;
begin

    select f_is_temp_table_exist('curr_transport') into cnt;
    if(cnt = 0) then
        return NULL;
    end if;

    select id_transport into idTransport from curr_transport limit 1;

    return idTransport;

end
$BODY$
language 'plpgsql';
