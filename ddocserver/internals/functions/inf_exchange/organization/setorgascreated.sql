create or replace function setOrgAsCreated(int4, int4) returns int4 as
$BODY$
declare
    idOrg alias for $1;
    isCreated alias for $2;
begin
    update organization set is_created = isCreated where id = idOrg;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';
