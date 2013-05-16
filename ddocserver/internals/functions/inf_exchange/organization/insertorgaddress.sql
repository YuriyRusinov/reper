create or replace function insertOrgAddress(int4, int4, varchar, int4, bool) returns int4 as
$BODY$
declare
    idOrg alias for $1;
    idTransport alias for $2;
    oAddress alias for $3;
    oPort alias for $4;
    isActive alias for $5;

    idAddr int4;
begin


    select getNextSeq('organization_transport', 'id') into idAddr;
    insert into organization_transport (id, id_organization, id_transport, address, port, is_active) values
                                       (idAddr,
                                        idOrg,
                                        idTransport,
                                        oAddress,
                                        oPort,
                                        isActive);

    return idAddr;

end
$BODY$
language 'plpgsql';
