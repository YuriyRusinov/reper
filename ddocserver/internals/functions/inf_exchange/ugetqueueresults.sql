select f_safe_drop_type('h_out_queue_result');

create type h_out_queue_result as (full_address varchar,
                                   id_reception int8,
                                   id_transport int4,
                                   id_external_queue int8,
                                   sync_result int4,
                                   port int4,
                                   org_uid varchar,
                                   use_gateway bool);

create or replace function uGetQueueResults() returns setof h_out_queue_result as
$BODY$
declare
    r h_out_queue_result%rowtype;
    idTransport int4;
begin
    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in
        select 
            qr.address as full_address,
            qr.id,
            qr.id_transport,
            qr.id_external_queue,
            qr.sync_result,
            qr.port,
            qr.org_uid,
            qr.use_gateway
        from
            queue_results qr 
        where
            qr.is_read = 1
        order by 2
    loop
        if (r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
