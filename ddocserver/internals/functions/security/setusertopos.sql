create or replace function setUserToPos (int4, int4) returns int4 as
$BODY$
declare
    idPos alias for $1;
    idUser alias for $2;

    cnt int4;
begin
    if (idPos is null) then
        return -1;
    end if;
    select into cnt count(*) from "position" p where p.id=idPos;
    if (cnt is null or cnt = 0) then
        raise warning 'Position % does not exist', idPos;
        return -2;
    end if;

    update "position" set id_user = idUser where id=idPos;
    if (not FOUND) then
        return -3;
    end if;

    return idPos;
end
$BODY$
language 'plpgsql';
