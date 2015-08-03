create or replace function lcUpdateState(int4, int4) returns int4 as
$BODY$
declare
    idLifeCycle alias for $1;
    idState alias for $2;

    cnt int4;
begin

    select count(*) into cnt from life_cycle_io_states where id_life_cycle = idLifeCycle and id_io_states = idState;
    if(cnt = 0) then
        insert into life_cycle_io_states (id_life_cycle, id_io_states) values (idLifeCycle, idState);
    end if;

    return 1;
end
$BODY$
language 'plpgsql';
