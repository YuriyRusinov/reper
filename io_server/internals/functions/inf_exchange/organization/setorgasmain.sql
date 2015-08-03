create or replace function setOrgAsMain(int4) returns int4 as
$BODY$
declare
    isMain alias for $1;
begin

    if(isMain <> 1) then
        return 1;
    end if;

    create table main_org_table (is_main int4);
    insert into main_org_table VALUES (1);

    return 1;

end
$BODY$
language 'plpgsql';
