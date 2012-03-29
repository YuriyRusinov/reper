create or replace function startControlPrivileges() returns int4 as
$BODY$
declare
    cnt int4;
begin
    select count(*) into cnt from control_privileges;
    if(cnt > 0) then
        update control_privileges set is_used = true;
    else
        insert into control_privileges (is_used) values (true);
    end if;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function stopControlPrivileges() returns int4 as
$BODY$
declare
    cnt int4;
begin
    select count(*) into cnt from control_privileges;
    if(cnt > 0) then
        update control_privileges set is_used = false;
    else
        insert into control_privileges (is_used) values (false);
    end if;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function isControlPrivileges()  returns bool as
$BODY$
declare
   isUsed bool;
begin

    select is_used into isUsed from control_privileges limit 1;
    if(isUsed isnull) then
        insert into control_privileges (is_used) values (true);
        return true;
    end if;

    return isUsed;
end
$BODY$
language 'plpgsql';
