
create or replace function isMainOrg() returns bool as 
$BODY$
declare
    isMain int4;
begin

    isMain = 0;
    
    select f_is_table_exist('main_org_table', NULL) into isMain;

    if(isMain isnull or isMain = 0) then
        return false;
    end if;

    return true;    

end
$BODY$
language 'plpgsql';
