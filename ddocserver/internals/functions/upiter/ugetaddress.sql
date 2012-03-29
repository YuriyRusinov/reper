/*
create or replace function uGetAddress(varchar) returns varchar as
$BODY$
declare
    inAddr alias for $1;
    outAddr varchar;
begin

    outAddr := inAddr;

    if(outAddr isnull or outAddr = 'local') then
        select str_parameter into outAddr from system_table where id_system = 2;
    end if;
    
    if(outAddr isnull) then
        raise exception E'Upiter local address cannot be NULL!\nYou MUST set address in TABLE system_table in column str_parameter with identifier id_system = 2!'; 
    end if;

    return outAddr;
end
$BODY$
language 'plpgsql';

create or replace function uGetAddress(varchar, int4) returns varchar as
$BODY$
declare
    inAddr alias for $1;
    aType alias for $2;

    outAddr varchar;
begin

    outAddr := inAddr;

    if(outAddr isnull or outAddr = 'local' or outAddr = '') then
        if(aType = 0) then 
            select str_parameter into outAddr from system_table where id_system = 2;
        else
            outAddr := '$$';
        end if;
    end if;
    
    if(outAddr isnull or outAddr = '') then
        raise exception E'Upiter local address cannot be NULL!\nYou MUST set address in TABLE system_table in column str_parameter with identifier id_system = 2!'; 
    end if;

    return outAddr;
end
$BODY$
language 'plpgsql';
*/
