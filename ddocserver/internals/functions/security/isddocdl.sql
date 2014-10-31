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
    elsif(idType = 2) then --�� �� ������� ����������� �������
        return false;
    end if;

    return true; --�� � ������� DynamicDocs
    
end
$BODY$
language 'plpgsql';
