create or replace function updateUserTemplate(int4, int4, int4, int4) returns int4 as
$BODY$
declare
    idUser alias for $1;
    idObject alias for $2;
    idTemplate alias for $3;
    iType alias for $4;
 
    cnt int4;
begin
    
    select count(*) into cnt from user_templates where id_user = idUser and id_io_object = idObject and type = iType;
    if(cnt > 0) then
        update user_templates set id_template = idTemplate where id_user = idUser and id_io_object = idObject and type = iType;
        if(FOUND = false) then
            return 0;
        end if;
     
        return 1;
    end if;

    insert into user_templates (id_user, id_template, id_io_object, type) values (idUser, idTemplate, idObject, iType);
    if(FOUND = false) then
        return 0;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function clearUserTemplate(int4, int4, int4) returns int4 as
$BODY$
declare
    idUser alias for $1;
    idObject alias for $2;
    iType alias for $3;
 
    cnt int4;
begin
    
    delete from user_templates where id_user = idUser and id_io_object = idObject and type = iType;
    if(FOUND = FALSE) then
        raise notice 'There are no user templates for the object found!';
    end if;

    return 1;
end
$BODY$
language 'plpgsql';
