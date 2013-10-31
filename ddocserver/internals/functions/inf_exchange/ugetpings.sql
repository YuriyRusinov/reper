select f_safe_drop_type('h_get_pings');
create type h_get_pings as (id int4,
                            org_name varchar,
                            org_short_name varchar,
                            org_email_prefix varchar,
                            id_transport int4,
                            transport_name varchar,
                            transport_addr varchar,
                            transport_active boolean,
                            transport_port int4,
                            transport_use_gateway bool,
                            org_created int4, 
                            org_completed bool,
                            org_is_main bool,
                            current_version varchar,
                            id_from int4,
                            name_from varchar,
                            address_from varchar,
                            port_from int4,
                            use_gateway bool,
                            email_prefix_from varchar
                            );

create or replace function uGetPings (int4) returns setof h_get_pings as
$BODY$
declare
    idOrganization alias for $1;

    currentVersion varchar;
    idFrom int4;
    nameFrom varchar;
    addressFrom varchar;
    portFrom int4;
    useGateway bool;
    emailPrefixFrom varchar;

    r h_get_pings%rowtype;
    rr record;
begin

    if(idOrganization isnull) then
        return;
    end if;
    
    currentVersion = kkssitoVersion1();
    
    for rr in select * from getLocalOrg(NULL)
    loop
        idFrom = rr.id_organization;
        nameFrom = rr.org_name;
        addressFrom = rr.address;
        portFrom = rr.port;
        useGateway = rr.use_gateway;
        emailPrefixFrom = rr.the_uid;
    end loop;

    for r in 
        select 
            o.id, 
            o.name as org_name, 
            o.short_name as org_short_name, 
            o.email_prefix as org_email_prefix, 
            t.id as id_transport, 
            t.name as transport_name, 
            otr.address as transport_addr, 
            (otr.is_active and t.is_active) as transport_active,
            otr.port as transport_port,
            otr.use_gateway,
            o.is_created,
            o.is_completed,
            o.is_main,
            currentVersion,
            idFrom,
            nameFrom,
            addressFrom,
            portFrom,
            useGateway,
            emailPrefixFrom
        from organization o
             left join organization_transport otr on (o.id=otr.id_organization) 
             left join transport t on (otr.id_transport=t.id)
        where o.id = idOrganization
        order by id
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function uGetPings (varchar) returns setof h_get_pings as
$BODY$
declare
    emailPrefix alias for $1;

    currentVersion varchar;
    idFrom int4;
    nameFrom varchar;
    addressFrom varchar;
    portFrom int4;
    useGateway bool;
    emailPrefixFrom varchar;

    r h_get_pings%rowtype;
    rr record;
begin

    if(emailPrefix isnull) then
        return;
    end if;
    

    currentVersion = kkssitoVersion1();
    
    for rr in select * from getLocalOrg(NULL)
    loop
        idFrom = rr.id_organization;
        nameFrom = rr.org_name;
        addressFrom = rr.address;
        portFrom = rr.port;
        useGateway = rr.use_gateway;
        emailPrefixFrom = rr.the_uid;
    end loop;


    for r in 
        select 
            o.id, 
            o.name as org_name, 
            o.short_name as org_short_name, 
            o.email_prefix as org_email_prefix, 
            t.id as id_transport, 
            t.name as transport_name, 
            otr.address as transport_addr, 
            (otr.is_active and t.is_active) as transport_active,
            otr.port as transport_port,
            otr.use_gateway,
            o.is_created,
            o.is_completed,
            o.is_main,
            currentVersion,
            idFrom,
            nameFrom,
            addressFrom,
            portFrom,
            useGateway,
            emailPrefixFrom
        from organization o
             left join organization_transport otr on (o.id=otr.id_organization) 
             left join transport t on (otr.id_transport=t.id)
        where o.email_prefix = emailPrefix
        order by id
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function uGetPings() returns setof h_get_pings as
$BODY$
declare
    currentVersion varchar;
    idFrom int4;
    nameFrom varchar;
    addressFrom varchar;
    portFrom int4;
    useGateway bool;
    emailPrefixFrom varchar;

    r h_get_pings%rowtype;
    rr record;
begin

    currentVersion = kkssitoVersion1();
    
    for rr in select * from getLocalOrg(NULL)
    loop
        idFrom = rr.id_organization;
        nameFrom = rr.org_name;
        addressFrom = rr.address;
        portFrom = rr.port;
        useGateway = rr.use_gateway;
        emailPrefixFrom = rr.the_uid;
    end loop;

    for r in 
        select 
            o.id, 
            o.name as org_name, 
            o.short_name as org_short_name, 
            o.email_prefix as org_email_prefix, 
            t.id as id_transport, 
            t.name as transport_name, 
            otr.address as transport_addr, 
            (otr.is_active and t.is_active) as transport_active,
            otr.port as transport_port,
            otr.use_gateway,
            o.is_created,
            o.is_completed,
            o.is_main,
            currentVersion,
            idFrom,
            nameFrom,
            addressFrom,
            portFrom,
            useGateway,
            emailPrefixFrom
        from organization o
             left join organization_transport otr on (o.id=otr.id_organization) 
             left join transport t on (otr.id_transport=t.id)
        where o.id <> getLocalOrgId()
        order by id
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function uGetPings(int4[]) returns setof h_get_pings as
$BODY$
declare
    idOrgs alias for $1;

    currentVersion varchar;
    idFrom int4;
    nameFrom varchar;
    addressFrom varchar;
    portFrom int4;
    useGateway bool;
    emailPrefixFrom varchar;

    r h_get_pings%rowtype;
    rr record;
begin

    if(idOrgs isnull or array_upper(idOrgs, 1) = 0) then
        return;
    end if;

    currentVersion = kkssitoVersion1();
    
    for rr in select * from getLocalOrg(NULL)
    loop
        idFrom = rr.id_organization;
        nameFrom = rr.org_name;
        addressFrom = rr.address;
        portFrom = rr.port;
        useGateway = rr.use_gateway;
        emailPrefixFrom = rr.the_uid;
    end loop;

    for r in 
        select 
            o.id, 
            o.name as org_name, 
            o.short_name as org_short_name, 
            o.email_prefix as org_email_prefix, 
            t.id as id_transport, 
            t.name as transport_name, 
            otr.address as transport_addr, 
            (otr.is_active and t.is_active) as transport_active,
            otr.port as transport_port,
            otr.use_gateway,
            o.is_created,
            o.is_completed,
            o.is_main,
            currentVersion,
            idFrom,
            nameFrom,
            addressFrom,
            portFrom,
            useGateway,
            emailPrefixFrom
        from organization o
             left join organization_transport otr on (o.id=otr.id_organization) 
             left join transport t on (otr.id_transport=t.id)
        where o.id = ANY (idOrgs)
        order by id
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function uGetPings(varchar[]) returns setof h_get_pings as
$BODY$
declare
    org_emails alias for $1;

    currentVersion varchar;
    idFrom int4;
    nameFrom varchar;
    addressFrom varchar;
    portFrom int4;
    useGateway bool;
    emailPrefixFrom varchar;

    r h_get_pings%rowtype;
    rr record;
begin

    if(org_emails isnull or array_upper(org_emails, 1) = 0) then
        return;
    end if;

    currentVersion = kkssitoVersion1();
 
    for rr in select * from getLocalOrg(NULL)
    loop
        idFrom = rr.id_organization;
        nameFrom = rr.org_name;
        addressFrom = rr.address;
        portFrom = rr.port;
        useGateway = rr.use_gateway;
        emailPrefixFrom = rr.the_uid;
    end loop;

    for r in 
        select 
            o.id, 
            o.name as org_name, 
            o.short_name as org_short_name, 
            o.email_prefix as org_email_prefix, 
            t.id as id_transport, 
            t.name as transport_name, 
            otr.address as transport_addr, 
            (otr.is_active and t.is_active) as transport_active,
            otr.port as transport_port,
            otr.use_gateway,
            o.is_created,
            o.is_completed,
            o.is_main,
            currentVersion,
            idFrom,
            nameFrom,
            addressFrom,
            portFrom,
            useGateway,
            emailPrefixFrom
        from organization o
             left join organization_transport otr on (o.id=otr.id_organization) 
             left join transport t on (otr.id_transport=t.id)
        where o.email_prefix = ANY (org_emails)
        order by id
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
