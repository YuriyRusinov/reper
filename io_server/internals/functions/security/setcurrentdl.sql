create or replace function setCurrentDl(int4) returns int4 as
$BODY$
declare
    idDl alias for $1;
    idUser int4;
    cnt int4;
begin

    select count(id) into cnt from "position" where id_user = getCurrentUser() and id = idDl;
    if(cnt <> 1) then
        return -1;
    end if;

    --DL MUST BE LOCAL!
    if(isLocalDl(idDl) <> TRUE) then
        return -2;
    end if;

    select count(id_dl) into cnt from curr_dl;
    if(cnt > 0) then
        update curr_dl set id_dl = idDl;
    else
        insert into curr_dl select idDl;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';
