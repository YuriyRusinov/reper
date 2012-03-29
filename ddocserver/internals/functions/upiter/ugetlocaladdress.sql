--get local address for first active transport
create or replace function uGetLocalAddress() returns varchar as
$BODY$
declare
    addr varchar;
    cnt int4;
begin

    select local_address into addr from transport where is_active = TRUE limit 1;
    
    if(addr isnull) then
        raise notice 'Cannot get local address from table transport!';
        return NULL;
    end if;

    return addr;

end
$BODY$
language 'plpgsql';

--get local address for given transport
create or replace function uGetLocalAddress(int4) returns varchar as
$BODY$
declare
    idTransport alias for $1;

    addr varchar;
    cnt int4;
begin

    select local_address into addr from transport where id = idTransport;
    
    if(addr isnull) then
        raise notice 'Cannot get local address for given transport from table transport!';
        return NULL;
    end if;

    return addr;

end
$BODY$
language 'plpgsql';
