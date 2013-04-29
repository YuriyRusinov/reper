select f_safe_drop_type ('h_transports');

create type h_transports as (unique_id varchar,
                             id_transport int4,
                             transport_name varchar,
                             local_address varchar,
                             is_active boolean,
                             local_port int4);

create or replace function getTransportAddresses (varchar) returns setof h_transports as
$BODY$
declare
    receiverUID alias for $1;

    r h_transports%rowtype;
begin

    if (receiverUID is null) then
        raise notice 'Incorrect organization Unique_id';
        return;
    end if;

    for r in
        select t.unique_id,
               t.id,
               t.name,
               otr.address,
               (t.is_active and otr.is_active) as tis_active,
               otr.port
        from
            transport t left join (organization_transport otr
            inner join organization o on 
            (otr.id_organization=o.id and o.email_prefix = receiverUID))
            on (t.id = otr.id_transport)
        order by t.id    
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
