select f_safe_drop_type('h_lc_get_states');
create type h_lc_get_states as(id int4, 
                               name varchar, 
                               description varchar,
                               is_system boolean); 

create or replace function lcGetStates(int4) returns setof h_lc_get_states as
$BODY$
declare
    idLifeCycle alias for $1;

    r h_lc_get_states%rowtype;
begin

    for r in
        select s.id, s.name, s.description, s.is_system 
        from io_states s, life_cycle_io_states lc
        where
            lc.id_life_cycle = idLifeCycle
            and lc.id_io_states = s.id
    loop   
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
