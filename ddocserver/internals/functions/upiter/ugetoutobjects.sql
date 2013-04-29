select f_safe_drop_type('h_get_out_objects');
create type h_get_out_objects as( full_address varchar,
                                  id_io_object int4,
                                  insert_time timestamp, 
                                  planned_exec timestamp,
                                  real_exec timestamp,
                                  command_id int4,
                                  command_extra_id int4,
                                  id_dl_executor int4,
                                  control_journal_id int4,
                                  id_organization int4,
                                  port int4  
                                  );

create or replace function uGetOutObjects() returns setof h_get_out_objects as
$BODY$
declare
    r RECORD;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in
        select 
--            (uGetAddress(o.address, 0) || uGetAddress(p1.address, 1))::varchar as full_address,
            (select address from uGetAddressEx(cmd.id_dl_to, idTransport)) as full_address,
            tcj.id_io_object,
            tcj.insert_time,
            tcj.planned_exec,
            tcj.real_exec,
            cmd.id,
            cmd.extra_id,
            cmd.id_dl_executor,
            tcj.id,
            u.id_organization,
            (select port from uGetAddressEx(cmd.id_dl_to, idTransport)) as port

        from 
            command_journal cmd,
            tsd_control_journal tcj,
--            organization o,
            "position" p1,
            units u
        where
            tcj.id_journal = cmd.id
            and tcj.is_outed = false
            and cmd.id_jr_state <> 6 --filter for documents saved as draft
            and isLocalDl(cmd.id_dl_to) = FALSE
            and cmd.id_dl_to = p1.id
            and p1.id_unit = u.id
    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;
            
    return;
    
end
$BODY$
language 'plpgsql';
