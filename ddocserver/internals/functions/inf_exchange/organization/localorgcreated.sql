create or replace function localOrgCreated() returns int4 as
$BODY$
declare
    idOrg int4;
begin
    select id into idOrg from local_org_table;
    if(FOUND = FALSE) then
        return 0;
    end if;
    if(idOrg isnull or idOrg <= 0) then
        return 0;
    end if;

    return idOrg;

    exception when others
    then
        return 0;
end
$BODY$
language 'plpgsql';
 