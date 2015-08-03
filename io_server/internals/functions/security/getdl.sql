select f_safe_drop_type('h_get_dl');
create type h_get_dl as(id int4, 
                        id_unit int4, 
                        id_user int4, 
                        id_user_vrio int4, 
                        id_maclabel int4, 
                        name varchar, 
                        short_name varchar, 
                        is_io bool);

create or replace function getDl(int4) returns setof h_get_dl as
$BODY$
declare
    idPosition alias for $1;

    r record;
begin

    for r in 
        select 
            id, 
            id_unit, 
            id_user, 
            id_user_vrio, 
            id_maclabel, 
            name, 
            short_name, 
            is_io
        from 
            "position"
        where
            id = idPosition

    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
