select f_safe_drop_type('h_c_get_life_cycle_info');
create type h_c_get_life_cycle_info as(id int4, 
                                       name varchar, 
                                       description varchar,
                                       id_state_state int4,
                                       start_state_name varchar,
                                       start_state_desc varchar,
                                       start_state_is_system boolean); 

create or replace function cGetLifeCycleInfo(int4) returns setof h_c_get_life_cycle_info as
$BODY$
declare
    idLifeCycle alias for $1;
    r h_c_get_life_cycle_info%rowtype;
begin

    for r in
        select 
            lc.id, 
            lc.name, 
            lc.description, 
            lc.id_start_state, 
            s.name as start_state_name,
            s.description as start_state_desc,
            s.is_system as start_state_is_system 
        from life_cycle lc
             left join io_states s on (lc.id_start_state = s.id)
        where 
            lc.id = idLifeCycle
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';

