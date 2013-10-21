select f_safe_drop_type('h_get_local_address');
create type h_get_local_address as(address varchar, port int4, use_gateway bool);

--get local address for first active transport
/*
create or replace function uGetLocalAddress() returns setof h_get_local_address as
$BODY$
declare
    r h_get_local_address%rowtype;
    cnt int4;
begin

    for r in 
        select local_address, local_port from transport where is_active = TRUE limit 1
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';
*/
--get local address for given transport

create or replace function uGetLocalAddress(int4) returns setof h_get_local_address as
$BODY$
declare
    idTransport alias for $1;

    r h_get_local_address%rowtype;
    cnt int4;
begin

    for r in 
        select 
            t.local_address, 
            t.local_port,
            t.use_gateway
        from 
            transport t
        where 
            t.id = idTransport
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';
