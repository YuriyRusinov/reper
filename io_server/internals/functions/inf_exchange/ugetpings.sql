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
    returnedEmails varchar[];
    remainedEmails varchar[];

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

    returnedEmails := ARRAY[]::varchar[];
    remainedEmails := ARRAY[]::varchar[];

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
        returnedEmails := array_append(returnedEmails, r.org_email_prefix);
        return next r;
    end loop;

    --in case of first sync procedure, full organizations list is empty in organization table
    --so, returnedEmails does not equal org_emails
    --but we must send ping request to the master organization, but we doesnt know it ID and other characteristics
    --so we must generate ping using info from queue_results table and ID of org = 0
    -- see also uGetPingResults()
    for rr in 
        select t_all.uid
        from 
            ( select unnest(org_emails::varchar[]) as uid ) t_all
            left join  ( select unnest(returnedEmails::varchar[]) as uid ) t_ret  on (t_ret.uid = t_all.uid)
        where t_ret.uid isnull
    loop
        remainedEmails := array_append(remainedEmails, rr.uid);
    end loop;

    if(array_upper(remainedEmails, 1) <= 0) then
        return;
    end if;

    for r in select * from uGetPingsFromQueueResults(remainedEmails::varchar[])
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
/**/
create or replace function uGetPingsFromQueueResults(varchar[]) returns setof h_get_pings as
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
        select distinct on (qr.org_uid)
            0, 
            qr.org_uid as org_name, 
            qr.org_uid as org_short_name, 
            qr.org_uid as org_email_prefix, 
            
            t.id as id_transport, 
            t.name as transport_name, 
            qr.address as transport_addr, 
            t.is_active as transport_active,
            qr.port as transport_port,
            qr.use_gateway,

            1 as is_created,
            true as is_completed,
            NULL,

            currentVersion,
            idFrom,
            nameFrom,
            addressFrom,
            portFrom,
            useGateway,
            emailPrefixFrom
        from queue_results qr
             left join transport t on (qr.id_transport = t.id)
        where  
            qr.is_read = 1
            and qr.org_uid = ANY (org_emails)
        order by qr.org_uid
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

