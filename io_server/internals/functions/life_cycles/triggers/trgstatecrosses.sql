create or replace function stateCrossesCheck() returns trigger as 
$BODY$
declare
    cnt int4;
begin
    
    if(new.id_state_src = new.id_state_dest) then
        raise exception 'id_state_src and id_state_dest must be diferrent!';
        return NULL;
    end if;

    select count(*) into cnt from life_cycle_io_states where id_life_cycle = new.id_life_cycle and id_io_states = new.id_state_src;
    if(cnt = 0) then
        raise exception 'id_state_src does not present in life_cycle_io_states table for the given id_life_cycle!';
        return NULL;
    end if;

    select count(*) into cnt from life_cycle_io_states where id_life_cycle = new.id_life_cycle and id_io_states = new.id_state_dest;
    if(cnt = 0) then
        raise exception 'id_state_dest does not present in life_cycle_io_states table for the given id_life_cycle!';
        return NULL;
    end if;

    return new;
end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trgstatecrosses', 'state_crosses');

select f_create_trigger('trgstatecrosses', 'before', 'insert or update', 'state_crosses', 'statecrossescheck()');
