select f_safe_drop_type('h_out_queue');

create type h_out_queue as(full_address varchar, 
                           id_queue int4, 
                           id_org int4, 
                           id_entity int4, 
                           entityUID varchar,
                           entity_io_UID varchar,
                           entity_table varchar, 
                           entity_type int4, 
                           sync_type int4, 
                           sync_result int4,
                           receiver_uid varchar);--receiver organization email_prefix

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
            (uGetAddressExOrg(q.id_organization, idTransport)) as full_address,
            q.id,
            q.id_organization,
            q.id_entity,
            q.entity_uid,
            q.entity_io_uid,
            q.entity_table,
            q.entity_type,
            q.sync_type,
            q.sync_result,
            o.email_prefix
        from
            out_sync_queue q,
            organization o
        where
            q.sync_result = 1 --new records
            and q.id_organization = o.id
        order by 2
    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
