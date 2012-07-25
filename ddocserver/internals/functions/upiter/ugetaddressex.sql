create or replace function uGetAddressEx() returns varchar as 
$BODY$
declare
    addr varchar;
begin

    addr := uGetAddressEx(getCurrentDl());

    return addr;
end
$BODY$
language 'plpgsql';

create or replace function uGetAddressEx(int4) returns varchar as
$BODY$
declare
    idDl alias for $1;

    addr varchar;
    r record;
begin

    for r in
       select 
            ot.address 
        from 
            transport t,
            organization_transport ot,
            organization o,
            units u,
            position p
        where 
            p.id = idDl 
            and p.id_unit = u.id
            and u.id_organization = o.id
            and o.id = ot.id_organization
            and ot.is_active = TRUE
            and ot.id_transport = t.id
            and t.is_active = TRUE
    loop
        return r.address;
    end loop;

    return addr;

end
$BODY$
language 'plpgsql';

create or replace function uGetAddressEx(int4, int4) returns varchar as 
$BODY$
declare
    idDl alias for $1;
    idTransport alias for $2;

    addr varchar;
    r record;
begin

    select 
        ot.address into addr
    from 
        transport t,
        organization_transport ot,
        organization o,
        units u,
        position p
    where 
        p.id = idDl 
        and p.id_unit = u.id
        and u.id_organization = o.id
        and o.id = ot.id_organization
        and ot.id_transport = idTransport
        and ot.is_active = TRUE
        and ot.id_transport = t.id
        and t.is_active = TRUE;

    if(addr is not null) then
        return addr;
    end if;

    return addr;

end
$BODY$
language 'plpgsql';

create or replace function uGetAddressExOrg(int4, int4) returns varchar as
$BODY$
declare
    idOrg alias for $1;
    idTransport alias for $2;

    addr varchar;
    r record;
begin

    select 
        ot.address into addr
    from 
        transport t inner join
        organization_transport ot on (ot.id_transport = t.id and ot.is_active and t.is_active and ot.id_transport = idTransport) inner join organization o on (ot.id_organization=o.id and o.id = idOrg);

    if(addr is not null) then
        return addr;
    end if;

    return addr;

end
$BODY$
language 'plpgsql';
