create or replace function uInsertTransport(varchar, varchar, varchar, bool) returns int4 as
$BODY$
declare
    tUniqueId alias for $1;
    tName alias for $2;
    tAddress alias for $3;
    tActive alias for $4;

    cnt int4;
    tId int4;
    transportAddr varchar;
begin

    select id into tId from transport where unique_id = tUniqueId;
    raise notice 'local address is %', tAddress;
    if (tAddress is null) then
        transportAddr = 'Not available';
    else
        transportAddr = tAddress;
    end if;

    if(tId isnull) then

        tId = getNextSeq('transport', 'id');

        insert into transport (unique_id, id, name, local_address, is_active) values (tUniqueId, tId, tName, transportAddr, tActive);
        if (not FOUND) then
            raise notice 'Cannot insert new transport and set local address for that!';
            return 0;
        end if;
        return tId;
    end if;

    update transport set local_address = tAddress where id = tId;

    return tId;

    return 1;

end
$BODY$
language 'plpgsql';
