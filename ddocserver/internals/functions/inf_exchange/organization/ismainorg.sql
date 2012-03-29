
create or replace function isMainOrg() returns bool as 
$BODY$
declare
    isMain bool;
begin

    isMain = false;

    select is_main into isMain from organization where id = getLocalOrgId();

    if(isMain isnull) then
        return false;
    end if;

    return isMain;    

end
$BODY$
language 'plpgsql';
