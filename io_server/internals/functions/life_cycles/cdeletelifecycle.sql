create or replace function cDeleteLifeCycle(int4) returns int4 as
$BODY$
declare
    idLifeCycle alias for $1;
begin

    update io_categories set id_life_cycle = NULL where id_life_cycle = idLifeCycle;

    --отработает триггер 
    --delete from state_crosses where id_life_cycle = idLifeCycle;
    delete from life_cycle_io_states where id_life_cycle = idLifeCycle;
    delete from life_cycle where id = idLifeCycle;

    return 1;
    
end
$BODY$
language 'plpgsql';
