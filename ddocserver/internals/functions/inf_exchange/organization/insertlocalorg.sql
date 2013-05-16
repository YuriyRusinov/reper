create or replace function insertLocalOrg(varchar, varchar, varchar, varchar, int4, int4, int4, varchar, int4) returns int4 as
$BODY$
declare
    oName alias for $1;
    oShortName alias for $2;
    oCodeName alias for $3;
    oEmailPrefix alias for $4;
    oType alias for $5;
    oParent alias for $6;

    idTransport alias for $7;
    oAddress alias for $8;
    oPort alias for $9;

    idOrg int4;
    idOrgAddr int4;
    
begin

    select insertOrganization(oName, oShortName, oCodeName, oEmailPrefix, oType, oParent) into idOrg;
    if(idOrg is not null and idOrg > 0) then
        select insertOrgAddress(idOrg, idTransport, oAddress, oPort, true) into idOrgAddr;
        if(idOrgAddr isnull or idOrgAddr <= 0) then
            return -2;
        end if;
    else
        return -1;
    end if;

    return idOrg;
    
end
$BODY$
language 'plpgsql';
