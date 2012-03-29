create or replace function ioDelete(int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idUser int4;
    hasPriv bool;
    tableName varchar;
    ok int4;
begin

    idUser = getCurrentUser();

    select getPrivilege(idUser, idObject, 3, TRUE) into hasPriv;
    if(hasPriv <> TRUE) then
        raise exception 'You does not have privileges to delete the object!';
        return -1;
    end if;
    
    select table_name into tableName from tbl_io_objects where id = idObject;
    if(tableName is not null) then
        select count(id) into ok from attributes where table_name = tableName;
        if(ok <> 0) then
            raise warning '% attributes has reference to the object! You cannot delete that', ok;
            return -1;
        end if;
    end if;

    delete from attrs_values where id_io_object = idObject;

    --delete from access_table where id_io_object = idObject;

    delete from log where id_io_object = idObject;

    delete from user_templates where id_io_object = idObject;

    --files
    select rRemoveObjUrl(idObject, TRUE) into ok;
    if(ok <> 1) then
        rollback;
        return -1;
    end if;

    --includes;
    perform ioDeleteIncludes(idObject);

    delete from io_rubricator where id_io_object = idObject;

    delete from access_table where id_io_object = idObject;

    delete from tbl_io_objects where id = idObject;
    if(FOUND = FALSE) then
        rollback;
        return -1;
    end if;

    return 1;
end
$BODY$
language 'plpgsql' security definer;
