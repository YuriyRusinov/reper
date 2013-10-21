create or replace function uSetLocalAddress(varchar, int4, bool) returns int4 as
$BODY$
declare
    uAddress alias for $1;
    uPort alias for $2;
    useGateway alias for $3;

    cnt int4;
begin

    select count(*) into cnt from transport;
    if(cnt = 0) then
        insert into transport (name, local_address, local_port, use_gateway) values('Транспорт TCP/IP', uAddress, uPort, useGateway);
        if(FOUND = FALSE) then
            raise notice 'Cannot insert new transport and set local address for that!';
            return 0;
        end if;
    else --update all transports
        update transport set local_address = uAddress, local_port = uPort, use_gateway = useGateway;
        if(FOUND = FALSE) then
            raise notice 'Cannot set local address to system tranports!';
            return 0;
        end if;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';


create or replace function uSetLocalAddress(int4, varchar, int4, bool) returns int4 as
$BODY$
declare
    uTransport alias for $1;
    uAddress alias for $2;
    uPort alias for $3;
    useGateway alias for $4;

    cnt int4;
begin

    select count(*) into cnt from transport;

    if(cnt = 0) then
        insert into transport (name, local_address, local_port, use_gateway) values('Транспорт TCP/IP', uAddress, uPort, useGateway);
        if(FOUND = FALSE) then
            raise notice 'Cannot insert new transport and set local address for that!';
            return 0;
        end if;
    else --update all transports
        update transport set local_address = uAddress, local_port = uPort, use_gateway = useGateway where id = uTransport;
        if(FOUND = FALSE) then
            raise notice 'Cannot set local address to system tranports!';
            return 0;
        end if;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';

