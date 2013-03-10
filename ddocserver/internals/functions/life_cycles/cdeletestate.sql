create or replace function cDeleteState(int4) returns int4 as
$BODY$
declare
    idState alias for $1;
begin

    delete from io_states where id = idState;

    return 1;
    
end
$BODY$
language 'plpgsql';
