select f_safe_drop_type('h_get_out_cmds');
create type h_get_out_cmds as (full_address varchar,
                               id int4,
                               dl_from_id int4,
                               dl_from_name varchar,
                               dl_executor_id int4,
                               dl_executor_name varchar,
                               dl_executor_address varchar,
                               exec_org_address varchar,
                               dl_to_id int4,
                               dl_to_name varchar,
                               id_io_category int4,
                               categoty_code varchar,
                               id_jr_state int4,
                               insert_time timestamp,
                               exec_datetime timestamp,
                               exec_period int4[2],
                               message_body varchar,
                               unique_id varchar,
                               input_number varchar,
                               output_number varchar,
                               id_urgency_level int4,
                               id_io_object_att int4,
                               dl_from_uid varchar,
                               dl_executor_uid varchar,
                               dl_to_uid varchar,
                               receive_datetime timestamp,
                               accepted_datetime timestamp,
                               urgency_level_code varchar,
                               id_organization int4,
                               port int4,
                               org_uid varchar,
                               use_gateway bool
                               );

create or replace function uGetOutCmds() returns setof h_get_out_cmds as
$BODY$
declare
    r h_get_out_cmds%rowtype;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in
        select 
--            (uGetAddress(o.address, 0) || uGetAddress(p2.address, 1)) as full_address,
            (case 
                  when cmd.id_jr_state = 7 then (select address from uGetAddressEx(cmd.id_dl_from, idTransport))
                  --when cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_executor) = FALSE then (uGetAddressEx(cmd.id_dl_executor, idTransport) )
                  --when cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_to) = FALSE then (uGetAddressEx(cmd.id_dl_to, idTransport) )
                  else (select address from uGetAddressEx(cmd.id_dl_executor, idTransport) ) 
            end ) as full_address,
            cmd.id,
            cmd.id_dl_from,
            p1.name,
            cmd.id_dl_executor,
            p2.name,
            NULL as dls_addr, --uGetAddress(p2.address, 1) as dls_addr,
            NULL as ex_addr, --uGetAddress(o.address, 0) as ex_addr,
            cmd.id_dl_to,
            p3.name,
            cmd.id_io_category,
            c.code,
            cmd.id_jr_state,
            cmd.insert_time,
            cmd.exec_datetime,
            cmd.exec_period,
            cmd.message_body,
            cmd.unique_id,
            cmd.input_number,
            cmd.output_number,
            cmd.id_urgency_level,
            cmd.id_io_object_att,
            p1.unique_id,
            p2.unique_id,
            p3.unique_id,
            cmd.receive_datetime,
            cmd.accepted_datetime,
            ul.code,
            (case 
                 when cmd.id_jr_state = 7 then u1.id_organization
                 else u2.id_organization 
            end) as id_organization,
            (case 
                  when cmd.id_jr_state = 7 then (select port from uGetAddressEx(cmd.id_dl_from, idTransport))
                  else (select port from uGetAddressEx(cmd.id_dl_executor, idTransport) ) 
            end ) as port,
            (case 
                  when cmd.id_jr_state = 7 then (select org_uid from uGetAddressEx(cmd.id_dl_from, idTransport))
                  else (select org_uid from uGetAddressEx(cmd.id_dl_executor, idTransport) ) 
            end ) as org_uid,
            (case 
                  when cmd.id_jr_state = 7 then (select use_gateway from uGetAddressEx(cmd.id_dl_from, idTransport))
                  else (select use_gateway from uGetAddressEx(cmd.id_dl_executor, idTransport) ) 
            end ) as use_gateway

            
        from
            command_journal cmd,
            "position" p1,
            "position" p2,
            "position" p3,
            io_categories c,
            urgency_levels ul,
            units u1,
            units u2
--            units u,
--            organization o
        where
            c.id = cmd.id_io_category
            and cmd.id_urgency_level = ul.id
            and p1.id = cmd.id_dl_from
            and p2.id = cmd.id_dl_executor
            and p3.id = cmd.id_dl_to
            and (
                (cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_executor) = FALSE)
                --or (cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_to) = FALSE)
                or (cmd.id_jr_state = 7 and isLocalDl(cmd.id_dl_executor) = TRUE) --virtual also transferred
                )            
            and p1.id_unit = u1.id
            and p2.id_unit = u2.id

    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function uGetOutCmds1() returns setof h_get_out_cmds as
$BODY$
declare
    r h_get_out_cmds%rowtype;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in
        select 
--            (uGetAddress(o.address, 0) || uGetAddress(p2.address, 1)) as full_address,
            (case 
                  when cmd.id_jr_state = 7 then (select address from uGetAddressEx(cmd.id_dl_from, idTransport))
                  --when cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_executor) = FALSE then (uGetAddressEx(cmd.id_dl_executor, idTransport) )
                  --when cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_to) = FALSE then (uGetAddressEx(cmd.id_dl_to, idTransport) )
                  else (select address from uGetAddressEx(cmd.id_dl_to, idTransport) ) 
            end ) as full_address,
            cmd.id,
            cmd.id_dl_from,
            p1.name,
            cmd.id_dl_executor,
            p2.name,
            NULL as dls_addr, --uGetAddress(p2.address, 1) as dls_addr,
            NULL as ex_addr, --uGetAddress(o.address, 0) as ex_addr,
            cmd.id_dl_to,
            p3.name,
            cmd.id_io_category,
            c.code,
            cmd.id_jr_state,
            cmd.insert_time,
            cmd.exec_datetime,
            cmd.exec_period,
            cmd.message_body,
            cmd.unique_id,
            cmd.input_number,
            cmd.output_number,
            cmd.id_urgency_level,
            cmd.id_io_object_att,
            p1.unique_id,
            p2.unique_id,
            p3.unique_id,
            cmd.receive_datetime,
            cmd.accepted_datetime,
            ul.code,
            (case 
                 when cmd.id_jr_state = 7 then u1.id_organization
                 else u3.id_organization 
            end) as id_organization,
            (case 
                  when cmd.id_jr_state = 7 then (select port from uGetAddressEx(cmd.id_dl_from, idTransport))
                  --when cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_executor) = FALSE then (uGetAddressEx(cmd.id_dl_executor, idTransport) )
                  --when cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_to) = FALSE then (uGetAddressEx(cmd.id_dl_to, idTransport) )
                  else (select port from uGetAddressEx(cmd.id_dl_to, idTransport) ) 
            end ) as port,
            (case 
                  when cmd.id_jr_state = 7 then (select org_uid from uGetAddressEx(cmd.id_dl_from, idTransport))
                  else (select org_uid from uGetAddressEx(cmd.id_dl_to, idTransport) ) 
            end ) as org_uid,
            (case 
                  when cmd.id_jr_state = 7 then (select use_gateway from uGetAddressEx(cmd.id_dl_from, idTransport))
                  else (select use_gateway from uGetAddressEx(cmd.id_dl_to, idTransport) ) 
            end ) as use_gateway
            
        from
            command_journal cmd,
            "position" p1,
            "position" p2,
            "position" p3,
            io_categories c,
            urgency_levels ul,
            units u1,
            units u3

--            units u,
--            organization o
        where
            c.id = cmd.id_io_category
            and cmd.id_urgency_level = ul.id
            and p1.id = cmd.id_dl_from
            and p2.id = cmd.id_dl_executor
            and p3.id = cmd.id_dl_to
            and (
                --( cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_executor) = FALSE)
                (cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_to) = FALSE)
                or (cmd.id_jr_state = 7 and isLocalDl(cmd.id_dl_executor) = TRUE) --virtual also transferred
                )            
            and p1.id_unit = u1.id
            and p3.id_unit = u3.id

    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function uGetNextOutCmd() returns setof h_get_out_cmds as
$BODY$
declare
    r h_get_out_cmds%rowtype;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in
        select 
--            (uGetAddress(o.address, 0) || uGetAddress(p2.address, 1)) as full_address,
            --(uGetAddressEx(cmd.id_dl_executor, idTransport)) as full_address,
            (case when id_jr_state = 7 then (select address from uGetAddressEx(cmd.id_dl_from, idTransport)) else (select address from uGetAddressEx(cmd.id_dl_executor, idTransport)) end) as full_address,
            cmd.id,
            cmd.id_dl_from,
            p1.name,
            cmd.id_dl_executor,
            p2.name,
            NULL as dls_addr, --uGetAddress(p2.address, 1) as dls_addr,
            NULL as ex_addr, --uGetAddress(o.address, 0) as ex_addr,
            cmd.id_dl_to,
            p3.name,
            cmd.id_io_category,
            c.code,
            cmd.id_jr_state,
            cmd.insert_time,
            cmd.exec_datetime,
            cmd.exec_period,
            cmd.message_body,
            cmd.unique_id,
            cmd.input_number,
            cmd.output_number,
            cmd.id_urgency_level,
            cmd.id_io_object_att,
            p1.unique_id,
            p2.unique_id,
            p3.unique_id,
            cmd.receive_datetime,
            cmd.accepted_datetime,
            ul.code
            (case 
                 when cmd.id_jr_state = 7 then u1.id_organization
                 else u2.id_organization 
            end) as id_organization,                                                                                                                                                         
            (case when id_jr_state = 7 then (select port from uGetAddressEx(cmd.id_dl_from, idTransport)) else (select port from uGetAddressEx(cmd.id_dl_executor, idTransport)) end) as port,
            (case when id_jr_state = 7 then (select org_uid from uGetAddressEx(cmd.id_dl_from, idTransport)) else (select org_uid from uGetAddressEx(cmd.id_dl_executor, idTransport)) end) as org_uid,
            (case when id_jr_state = 7 then (select use_gateway from uGetAddressEx(cmd.id_dl_from, idTransport)) else (select use_gateway from uGetAddressEx(cmd.id_dl_executor, idTransport)) end) as use_gateway

        from
            command_journal cmd,
            "position" p1,
            "position" p2,
            "position" p3,
            io_categories c,
            urgency_levels ul,
            units u1,
            units u2

--            units u,
--            organization o
        where
            c.id = cmd.id_io_category
            and cmd.id_urgency_level = ul.id
            and p1.id = cmd.id_dl_from
            and p2.id = cmd.id_dl_executor
            and p3.id = cmd.id_dl_to
            and (
                (cmd.id_jr_state = 1 and isLocalDl(cmd.id_dl_executor) = FALSE)
                or (cmd.id_jr_state = 7 and isLocalDl(cmd.id_dl_executor) = TRUE) --virtual also transferred
                )
            and p1.id_unit = u1.id
            and p2.id_unit = u2.id
        limit 1
    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
