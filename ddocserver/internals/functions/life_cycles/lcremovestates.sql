create or replace function lcRemoveStates(int4, int4[]) returns int4 as
$BODY$
declare
    idLifeCycle alias for $1;
    excludes alias for $2;

    cnt int4;
    r RECORD;
begin

    if(excludes isnull) then
        delete from life_cycle_io_states where id_life_cycle = idLifeCycle;
        return 1;
    end if;

    delete from life_cycle_io_states 
    where 
        id_life_cycle = idLifeCycle 
        and id_io_states not in (select id from io_states where id = ANY(excludes));

    return 1;

end
$BODY$
language 'plpgsql';
