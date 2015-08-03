create or replace function isLogging(int4) returns boolean as
$BODY$
declare
    idOperType alias for $1;
    cnt int4;
begin
    select count(id) into cnt from notlogging_oper_table where id = idOperType;
    if(cnt > 0) then
        return false;
    end if;

    return true;
end
$BODY$
language plpgsql;

create or replace function setAsLogging(int4) returns int4 as
$BODY$
declare
    idOperType alias for $1;
begin

    delete from notlogging_oper_table where id = idOperType;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function setAsNotLogging(int4) returns int4 as
$BODY$
declare
    idOperType alias for $1;
begin

    insert into notlogging_oper_table values(idOperType);

    return 1;

end
$BODY$
language 'plpgsql';
