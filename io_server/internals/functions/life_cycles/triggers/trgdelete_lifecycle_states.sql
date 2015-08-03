create or replace function deleteLifeCycleStatesCheck() returns trigger as 
$BODY$
declare
    cnt int4;
begin
    
    update life_cycle set id_start_state = NULL where id = old.id_life_cycle and id_start_state = old.id_io_states;
    delete from state_crosses where id_life_cycle = old.id_life_cycle and id_state_src = old.id_io_states;
    delete from state_crosses where id_life_cycle = old.id_life_cycle and id_state_dest = old.id_io_states;

    return old;
end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trgdelete_lifecycle_states', 'life_cycle_io_states');

select f_create_trigger('trgdelete_lifecycle_states', 'before', 'delete', 'life_cycle_io_states', 'deletelifecyclestatescheck()');
