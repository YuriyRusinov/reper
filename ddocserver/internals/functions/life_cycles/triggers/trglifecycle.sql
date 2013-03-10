create or replace function lifeCycleCheck() returns trigger as 
$BODY$
declare
    cnt int4;
begin

    if(new.id_start_state is not null) then
        select count(*) into cnt from life_cycle_io_states where id_life_cycle = new.id and id_io_states = new.id_start_state;
        if(cnt = 0) then
            insert into life_cycle_io_states (id_life_cycle, id_io_states) values (new.id, new.id_start_state);
        end if;
    end if;

    return new;
end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trglifecycle', 'life_cycle');

select f_create_trigger('trglifecycle', 'after', 'insert or update', 'life_cycle', 'lifecyclecheck()');
