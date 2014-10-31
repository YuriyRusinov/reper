select f_safe_drop_type('h_get_msg_conf');

create type h_get_msg_conf as(full_address varchar, 
                              id int4, 
                              extra_id int4, 
                              read_datetime timestamp,
                              receive_datetime timestamp,
                              port int4, 
                              org_uid varchar,
                              use_gateway bool);

create or replace function uGetMsgsConfirm() returns setof h_get_msg_conf as
$BODY$
declare
    r h_get_msg_conf%rowtype;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in
        select 
            (select address from uGetAddressEx(msg.id_dl_sender, idTransport)) as full_address,
            msg.id,
            msg.extra_id,
            msg.read_datetime,
            msg.receive_datetime,
            (select port from uGetAddressEx(msg.id_dl_sender, idTransport)) as port,
            (select org_uid from uGetAddressEx(msg.id_dl_sender, idTransport)) as org_uid,
            (select use_gateway from uGetAddressEx(msg.id_dl_sender, idTransport)) as use_gateway

        from
            message_journal msg
        where
            msg.is_outed = false 
            and msg.extra_id is not null --if is_outed = false but extra_id isnull that means  that message was created from XML!
            and (msg.read_datetime is not null or msg.receive_datetime is not null)
            and isLocalDl(msg.id_dl_sender) = FALSE
            and isDDocDl(msg.id_dl_sender) = true --отправляем только ДЛ, которые имеют тип "ДЛ в системе DynamicDocs"

    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
        
end
$BODY$
language 'plpgsql';
