select f_safe_drop_type('h_local_org_desc');

create type h_local_org_desc as(id_organization int4,
                                org_name varchar,
                                address varchar,
                                the_uid varchar,
                                port int4
                               );

create or replace function getLocalOrg() returns setof h_local_org_desc as
$BODY$
declare
    r h_local_org_desc%rowtype;

begin

    -- get first organization on local address
    for r in 
        select 
            o.id, 
            o.name, 
            ot.address, 
            o.email_prefix,
            ot.port
        from
            organization o,
            organization_transport ot,
            transport t
        where
            o.id = ot.id_organization
            and ot.id_transport = t.id
            and t.local_address = ot.address
            and (
                  (ot.port isnull and t.local_port isnull)
                  or (ot.port = t.local_port)
                )
    loop
        return next r;
        return;
    end loop;

    return;

end 
$BODY$
language 'plpgsql' security definer;

create or replace function getLocalOrgId() returns int4 as
$BODY$
declare
    r record;
begin

    for r in select id_organization from getLocalOrg()
    loop
        return r.id_organization;
    end loop;

    return -1;
end
$BODY$
language 'plpgsql';
