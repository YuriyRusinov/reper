select f_safe_drop_type('h_c_get_state_crosses');
create type h_c_get_state_crosses as(id int4, 
                                     name varchar, 
                                     id_state_src int4, 
                                     src_name varchar,
                                     src_desc varchar,
                                     src_is_system boolean,
                                     id_state_dst int4,
                                     dst_name varchar,
                                     dst_desc varchar,
                                     dst_is_system boolean); 

create or replace function cGetStateCrosses(int4) returns setof h_c_get_state_crosses as
$BODY$
declare
    idLifeCycle alias for $1;

    r h_c_get_state_crosses%rowtype;
begin

    for r in 
        select sc.id, sc.name,
               s1.id as id_state_src, s1.name as src_name, s1.description as src_desc, s1.is_system as src_is_system,
               s2.id as id_state_dst, s2.name as dst_name, s2.description as dst_desc, s2.is_system as dst_is_system
        from state_crosses sc, io_states s1, io_states s2
        where 
            sc.id_life_cycle = idLifeCycle 
            and sc.id_state_src = s1.id 
            and sc.id_state_dest = s2.id
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
