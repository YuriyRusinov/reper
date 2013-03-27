select f_safe_drop_type('h_c_get_state');
create type h_c_get_state as(id int4, 
                             name varchar, 
                             description varchar,
                             is_system boolean); 

create or replace function cGetState(int4) returns setof h_c_get_state as
$BODY$
declare
    idState alias for $1;

    r h_c_get_state%rowtype;
begin

    for r in 
        select 
               s1.id, 
               s1.name, 
               s1.description,
               s1.is_system
        from io_states s1
        where 
            s1.id = idState 
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function cGetState() returns setof h_c_get_state as
$BODY$
declare
    r h_c_get_state%rowtype;
begin

    for r in 
        select 
               s1.id, 
               s1.name, 
               s1.description,
               s1.is_system
        from io_states s1
        where is_system = false --запрещаем работу с системными состояниями      
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
