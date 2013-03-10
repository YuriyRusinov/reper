create or replace function lcInsertState(int4, int4) returns int4 as
$BODY$
declare
    idLifeCycle alias for $1;
    idState alias for $2;
begin

    insert into life_cycle_io_states (id_life_cycle, id_io_states) values(idLifeCycle, idState);
    return 1;
end
$BODY$
language 'plpgsql';
