select f_safe_drop_type('h_out_file_parts');

create type h_out_file_parts as(full_address varchar, 
                           id_queue int4, 
                           id_org int4, 
                           id_io_url int4, 
                           io_url_uid varchar,
                           abs_url varchar,
                           entity_type int4, 
                           sync_type int4, 
                           receiver_uid varchar);--receiver organization email_prefix

create or replace function getOutFileParts() returns setof h_out_file_parts as
$BODY$
declare
    r h_out_file_parts%rowtype;
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
            (rGetAbsUrl(q.id_entity) ) as abs_url,
            q.entity_type,
            q.sync_type,
            o.email_prefix
        from
            out_sync_queue q,
            organization o
        where
            q.sync_result = 1 --new records
            and q.entity_type in (12, 13) --files (transferred by parts)
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
