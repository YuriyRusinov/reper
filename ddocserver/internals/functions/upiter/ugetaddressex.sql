select f_safe_drop_type('h_get_address_ex');
create type h_get_address_ex as (address varchar, port int4);

create or replace function uGetAddressEx() returns setof h_get_address_ex as 
$BODY$
declare
    r h_get_address_ex%rowtype;
begin

    for r in select * from uGetAddressEx(getCurrentDl())
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';


create or replace function uGetAddressEx(int4) returns setof h_get_address_ex as
$BODY$
declare
    r h_get_address_ex%rowtype;
    idDl alias for $1;

begin

    for r in
       select 
            ot.address,
            ot.port
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
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function uGetAddressEx(int4, int4) returns setof h_get_address_ex as 
$BODY$
declare
    idDl alias for $1;
    idTransport alias for $2;

    r h_get_address_ex%rowtype;
begin

    for r  in
        select 
            ot.address,
            ot.port
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
            and t.is_active = TRUE
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function uGetAddressExOrg(int4, int4) returns setof h_get_address_ex as
$BODY$
declare
    idOrg alias for $1;
    idTransport alias for $2;

    r h_get_address_ex%rowtype;
begin

    for r in 
        select 
            ot.address,
            ot.port
        from 
            transport t inner join
            organization_transport ot on (ot.id_transport = t.id and ot.is_active and t.is_active and ot.id_transport = idTransport) inner join organization o on (ot.id_organization=o.id and o.id = idOrg)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
