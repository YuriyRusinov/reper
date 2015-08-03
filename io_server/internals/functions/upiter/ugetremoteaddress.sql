create or replace function uGetRemoteAddress() returns varchar as
$BODY$
declare
    addr varchar;
    cnt int4;
begin

    select address into addr from test_table;

    if(addr isnull) then
        raise notice 'Cannot get Upiter remote address from system_table!';
        return NULL;
    end if;

    return addr;

end
$BODY$
language 'plpgsql';

create or replace function uSetRemoteAddress(varchar) returns int4 as
$BODY$
declare
    addr alias for $1;
begin

    perform f_safe_drop_table('test_table');

    create table test_table (address varchar);
 
    insert into test_table (address) values (addr);

    return 1;

end
$BODY$
language 'plpgsql';
