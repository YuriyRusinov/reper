select f_safe_drop_type('h_rec_sys_params');
create type h_rec_sys_params as (id int8, 
                                 id_io_state int4, 
                                 uuid_t uuid, 
                                 unique_id varchar, 
                                 r_icon varchar, 
                                 record_fill_color int8, 
                                 record_text_color int8,
                                 last_update timestamp);

create or replace function recGetSysParams(int8) returns setof h_rec_sys_params as
$BODY$
declare
    idRec alias for $1;

    r h_rec_sys_params%rowtype;
begin

    for r in 
        select 
            id,
            id_io_state,
            uuid_t,
            unique_id,
            r_icon,
            record_fill_color,
            record_text_color,
            last_update
        from 
            q_base_table
        where
            id = idRec
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


