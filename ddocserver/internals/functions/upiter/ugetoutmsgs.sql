select f_safe_drop_type('h_get_out_msgs');
create type h_get_out_msgs as (full_address varchar,
                               id int4,
                               dl_sender_id int4,
                               dl_sender_name varchar,
                               dl_receiver_id int4,
                               dl_receiver_name varchar,
                               dl_receiver_address varchar,
                               recv_org_address varchar,
                               id_io_object int4,
                               insert_time timestamp,
                               message_body varchar,
                               unique_id varchar,
                               input_number varchar,
                               output_number varchar,
                               id_urgency_level int4,
                               dl_sender_uid varchar,
                               dl_receiver_uid varchar,
                               urgency_level_code varchar,
                               id_organization int4,
                               port int4
                               );

create or replace function uGetOutMsgs() returns setof h_get_out_msgs as
$BODY$
declare
    r h_get_out_msgs%rowtype;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in
        select 
--            (uGetAddress(o.address, 0) || uGetAddress(p2.address, 1)) as full_address,
            (select address from uGetAddressExOrg(u.id_organization, idTransport)) as full_address,
            msg.id,
            msg.id_dl_sender,
            p1.name,
            msg.id_dl_receiver,
            p2.name,
            NULL as dls_addr, --uGetAddress(p2.address, 1) as dls_addr,
            NULL as recv_org_address, --uGetAddress(o.address, 0) as recv_org_address,
            msg.id_io_object,
            msg.insert_time,
            msg.message_body,
            msg.unique_id,
            msg.input_number,
            msg.output_number,
            msg.id_urgency_level,
            p1.unique_id,
            p2.unique_id,
            ul.code,
            u.id_organization, --ИД организации, на которую отправляется сообщение (нужно для механизма пересылки больших файлов блоками)
            (select port from uGetAddressExOrg(u.id_organization, idTransport)) as port

        from
            message_journal msg,
            "position" p1,
            "position" p2,
            urgency_levels ul,
            units u
--            organization o
        where
            p1.id = msg.id_dl_sender
            and msg.id_urgency_level = ul.id
            and p2.id = msg.id_dl_receiver
            and p2.id_unit = u.id
            and msg.is_outed = false
            and isLocalDl(msg.id_dl_receiver) = FALSE
    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function uGetNextOutMsg() returns setof h_get_out_msgs as
$BODY$
declare
    r h_get_out_msgs%rowtype;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in
        select 
--            (uGetAddress(o.address, 0) || uGetAddress(p2.address, 1)) as full_address,
            (select address from uGetAddressExOrg(u.id_organization, idTransport)) as full_address,
            msg.id,
            msg.id_dl_sender,
            p1.name,
            msg.id_dl_receiver,
            p2.name,
            NULL as dls_addr, --uGetAddress(p2.address, 1) as dls_addr,
            NULL as ex_addr, --uGetAddress(o.address, 0) as ex_addr,
            msg.id_io_object,
            msg.insert_time,
            msg.message_body,
            msg.unique_id,
            msg.input_number,
            msg.output_number,
            msg.id_urgency_level,
            p1.unique_id,
            p2.unique_id,
            ul.code,
            u.id_organization,
            (select port from uGetAddressExOrg(u.id_organization, idTransport)) as port

        from
            message_journal msg,
            urgency_levels ul,
            "position" p1,
            "position" p2,
            units u
--            organization o
        where
            p1.id = msg.id_dl_sender
            and msg.id_urgency_level = ul.id
            and p2.id = msg.id_dl_receiver
            and p2.id_unit = u.id
            and msg.is_outed = false
            and isLocalDl(msg.id_dl_receiver) = FALSE
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
