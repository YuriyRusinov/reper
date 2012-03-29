create or replace function dropPrivileges(int4) returns int4 as
$BODY$
declare
    idObject alias for $1;

    idAuthor int4;
    idUser int4;
begin
    idUser = getCurrentUser();

    select author into idAuthor from tbl_io_objects where id = idObject;
    if(idAuthor isnull and idUser <> 1 and idUser <> 2) then
        return -1;
    end if;

    if(idAuthor = idUser or idUser = 1 or idUser = 2) then
        delete from access_table where id_io_object = idObject;
        return 1;
    end if;

    return -1;

end
$BODY$
language 'plpgsql' security definer;

create or replace function dropRubricPrivileges(int4) returns int4 as
$BODY$
declare
    idRubric alias for $1;

    idAuthor int4;
    idUser int4;
begin

    idUser = getCurrentUser();

    select id_role into idAuthor from access_rubric_table where id_rubric = idRubric;
    if(idAuthor isnull and idUser <> 1 and idUser <> 2) then
        return -1;
    end if;

    if(idAuthor = idUser or idUser = 1 or idUser = 2) then
        delete from access_rubric_table where id_rubric = idRubric and id_role <> idUser;
        return 1;
    end if;

    return -1;
end
$BODY$
language 'plpgsql' security definer;
