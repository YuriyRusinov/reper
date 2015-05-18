create or replace function insertOrgAddress(int4, int4, varchar, int4, bool, bool) returns int4 as
$BODY$
declare
    idOrg alias for $1;
    idTransport alias for $2;
    oAddress alias for $3;
    oPort alias for $4;
    isActive alias for $5;
    useGateway alias for $6;

    idAddr int4;
begin


    select getNextSeq('organization_transport', 'id') into idAddr;
    insert into organization_transport (id, id_organization, id_transport, address, port, is_active, use_gateway) values
                                       (idAddr,
                                        idOrg,
                                        idTransport,
                                        oAddress,
                                        oPort,
                                        isActive,
                                        useGateway);

    return idAddr;

end
$BODY$
language 'plpgsql';

--in information exchange
create or replace function insertOrgAddressEx(varchar, varchar, varchar, int4, bool, bool) returns int4 as
$BODY$
declare
    uidOrg alias for $1;
    uidTransport alias for $2;

    oAddress alias for $3;
    oPort alias for $4;
    isActive alias for $5;
    useGateway alias for $6;

    idAddr int4;
    idOrg int4;
    idTransport int4;
begin

    if(getCurrentUser() <> 2) then --'jupiter'
        return -1;
    end if;

    select id into idOrg from organization where email_prefix = uidOrg;
    if(idOrg is null) then
        return -1;
    end if;

    select id into idTransport from transport where unique_id = uidTransport;
    if(idTransport is null) then
        return -1;
    end if;

    select id into idAddr from organization_transport where id_organization = idOrg and id_transport = idTransport;
    if(idAddr is not null) then
        update organization_transport set address = oAddress, port = oPort, is_active = isActive, use_gateway = useGateway where id = idAddr;
        return idAddr;
    end if;

    select getNextSeq('organization_transport', 'id') into idAddr;
    insert into organization_transport (id, id_organization, id_transport, address, port, is_active, use_gateway) values
                                       (idAddr,
                                        idOrg,
                                        idTransport,
                                        oAddress,
                                        oPort,
                                        isActive,
                                        useGateway);

    return idAddr;

end
$BODY$
language 'plpgsql';
