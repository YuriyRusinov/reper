create or replace function lifeCycleStatesCheck() returns trigger as 
$BODY$
declare
    cnt int4;
begin

    select count(*) into cnt from life_cycle_io_states where id_life_cycle = new.id_life_cycle and id_io_states = new.id_io_states;
    if(cnt > 0) then
        return NULL;
    end if;

    return new;
end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trglifecycle_states', 'life_cycle_io_states');

select f_create_trigger('trglifecycle_states', 'before', 'insert or update', 'life_cycle_io_states', 'lifecyclestatescheck()');
