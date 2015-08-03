create or replace function isDDocDl(int4) returns bool as
$BODY$
declare
    idDl alias for $1;
    isDDoc bool;
    idType int4;
begin

    select id_position_type into idType from "position" where id = idDl;

    if(idType isnull) then
        return NULL;
    elsif(idType = 2) then --ДЛ во внешней сопрягаемой системе
        return false;
    end if;

    return true; --ДЛ в системе DynamicDocs
    
end
$BODY$
language 'plpgsql';
