select f_safe_drop_type('h_out_queue');

create type h_out_queue as(full_address varchar, 
                           id_queue int8, 
                           id_org int4, 
                           id_entity int8, 
                           entityUID varchar,
                           entity_io_UID varchar,
                           entity_table varchar, 
                           entity_type int4, 
                           sync_type int4, 
                           sync_result int4,
                           receiver_uid varchar, --receiver organization email_prefix
                           port int4, --port of the transport (for IP-transports)
                           use_gateway bool); 

create or replace function uQueryOutQueue() returns setof h_out_queue as
$BODY$
declare
    r h_out_queue%rowtype;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in 
        select             
            (select address from uGetAddressExOrg(q.id_organization, idTransport)) as full_address,
            q.id,
            q.id_organization,
            q.id_entity,
            q.entity_uid,
            q.entity_io_uid,
            q.entity_table,
            q.entity_type,
            q.sync_type,
            q.sync_result,
            o.email_prefix,
            (select port from uGetAddressExOrg(q.id_organization, idTransport)) as port,
            (select use_gateway from uGetAddressExOrg(q.id_organization, idTransport)) as use_gateway

        from
            out_sync_queue q,
            organization o
        where
            q.sync_result = 1 --new records
            and q.entity_type not in (12, 13) --files (transferred by parts)
            and q.id_organization = o.id
        order by 2
        limit 10 --чтобы не перенагружать данными клиента будем брать исходящие сообщения порциями не более чем по 10 штук
    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
