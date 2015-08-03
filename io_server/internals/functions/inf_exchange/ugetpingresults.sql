select f_safe_drop_type('h_ping_result');

create type h_ping_result as (full_address varchar,
                              id_reception int8,
                              id_transport int4,
                              id_external_queue int8,
                              sync_result int4,
                              port int4,
                              org_uid varchar,
                              use_gateway bool);

create or replace function uGetPingResults() returns setof h_ping_result as
$BODY$
declare
    r h_ping_result%rowtype;
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
            and id_external_queue <= 0 --if negative - see description in database scheme in powerdesigner
                                       --in case of zero -- first sync procedure is processing
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
