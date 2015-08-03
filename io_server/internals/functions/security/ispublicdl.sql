create or replace function isPublicDl(int4) returns bool as
$BODY$
declare
    idDl alias for $1;
    isPublic bool;
begin

    select is_public into isPublic from "position" where id = idDl;
    if(FOUND = FALSE) then
        return false;
    end if;

    if(isPublic isnull) then
        return false;
    end if;

    return isPublic;

end
$BODY$
language 'plpgsql';
