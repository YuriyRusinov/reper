
/*
применяется к следующим таблицам:
io_objects
urls_objects
object_ref_tables
rubricator
attrs_values
*/
create or replace function acl_ioCtrl() returns trigger as
$BODY$
declare
    idOperation int4; --1 - insert, 2 - update, 3 - delete
    idUser int4;
    idObject int4;
    isAuthor bool;
    ok bool;
    hasPriv boolean;
begin
    if(TG_OP = 'INSERT') then
        idOperation = 1;
    elsif(TG_OP = 'UPDATE') then
        idOperation = 2;
    else --delete
        idOperation = 3;
    end if;
    


    select getCurrentUser() into idUser;
    if(idUser = 1 or idUser = 2) then --admin and jupiter --этим пользователям всегда разрешен полный доступ
        if(idOperation = 3) then
            return old; 
        else
            return new;
        end if;
    end if;

    --
    --ВЫБИРАЕМ ИНФОРМАЦИОННЫЙ ОБЪЕКТ, ПО ОТНОШЕНИЮ К КОТОРОМУ ОСУЩЕСТВЛЯЕТСЯ РЕДАКТИРОВАНИЕ (УДАЛЕНИЕ)
    --
    if(idOperation = 3) then
        if(TG_RELNAME = 'tbl_io_objects') then
            idObject = old.id;
        elsif(TG_RELNAME = 'rubricator') then
            idObject = getIOForRubric(old.id);
            if(idObject isnull) then
                return old; --если редактируется (удаляется) рубрика, не являющаяся частью информационного объекта, то разрешаем действие (проверка для такого случая осуществляется в другом триггере)
            end if;
        else
            idObject = old.id_io_object;
        end if;
    else
        if(TG_RELNAME = 'tbl_io_objects') then
            idObject = new.id;
        elsif(TG_RELNAME = 'rubricator') then
            idObject = getIOForRubric(new.id);
            if(idObject isnull) then
                return new; --если редактируется (удаляется) рубрика, не являющаяся частью информационного объекта, то разрешаем действие (проверка для такого случая осуществляется в другом триггере)
            end if;
        else
            idObject = new.id_io_object;
        end if;
    end if;
 
    ---
    ---ОПРЕДЕЛЯЕМ, ЯВЛЯЕТСЯ ЛИ ТЕКУЩИЙ ПОЛЬЗОВАТЕЛЬ АВТОРОМ ИНФОРМАЦИОННОГО ОБЪЕКТА
    ---
    if(TG_RELNAME = 'tbl_io_objects') then    
        if(idOperation = 3) then --delete
            if(idUser = old.author) then
                isAuthor = true;
            else
                isAuthor = false;
            end if; 
        else
            if(idUser = new.author) then
                isAuthor = true;
            else
                isAuthor = false;
            end if; 
        end if;
  
        if(idOperation = 1) then --если делаем insert то необходимо проверить, чтобы ИО не создавался не от своего имени
            if(isAuthor = false) then
                raise exception 'ERROR! You cannot set another user as author of your IO!';
                return NULL;
            end if;
        end if;
    else
        isAuthor = acl_isAuthorIO(idObject);
    end if;

    if(isAuthor = true) then -- автору ИО разрешен полный доступ
        if(idOperation = 3) then
            return old; 
        else
            return new;
        end if;
    end if;

    --
    -- ПОЛЬЗОВАТЕЛЬ НЕ ЯВЛЯЕТСЯ АВТОРОМ, ПРОВЕРЯЕМ ЕГО ПОЛНОМОЧИЯ
    --
    if(idOperation = 3) then
        hasPriv = true;
        select getPrivilege(getCurrentUser(), idObject, 4, true) into hasPriv;
        raise warning E'\n\n%\n\n', hasPriv;
        if(not hasPriv) then
            hasPriv = getPrivilege(idUser, idObject, 3, true);
        end if;
    else
        hasPriv = getPrivilege(idUser, idObject, 4, true); --пользователь должен иметь право на изменение ИО
    end if;

    if(hasPriv = true) then
        if(idOperation = 3) then
            return old;
        else
            return new;
        end if;
    else
        raise exception 'You have insufficient privileges for that operation!';
        return NULL;
    end if;

    return NULL;
end
$BODY$
language 'plpgsql';


create or replace function acl_isAuthorIO(int4) returns bool as
$BODY$
declare
    idObject alias for $1;
    idUser int4;
    isAuthor bool;
begin

    select getCurrentUser() into idUser;

    select into isAuthor (author = idUser) from tbl_io_objects io where io.id = idObject;
    if (isAuthor) then
        return true;
    end if;
    
    return false;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trg_acl_ioctrl', 'io_objects');

select f_create_trigger('trg_acl_ioctrl', 'before', 'insert or update or delete', 'io_objects', 'acl_ioctrl()');

/*create trigger trg_acl_ioCtrl 
before insert or update or delete
on tbl_io_objects
for each row 
execute procedure acl_ioCtrl();
*/

select f_safe_drop_trigger('trg_acl_ioctrl', 'urls_objects');

create trigger trg_acl_ioCtrl 
before insert or update or delete
on urls_objects
for each row 
execute procedure acl_ioCtrl();

select f_safe_drop_trigger('trg_acl_ioctrl', 'object_ref_tables');

create trigger trg_acl_ioCtrl 
before insert or update or delete
on object_ref_tables
for each row 
execute procedure acl_ioCtrl();

select f_safe_drop_trigger('trg_acl_ioctrl', 'rubricator');

create trigger trg_acl_ioCtrl 
before insert or update or delete
on rubricator
for each row 
execute procedure acl_ioCtrl();

select f_safe_drop_trigger('trg_acl_ioctrl', 'attrs_values');

select f_create_trigger('trg_acl_ioctrl', 'before', 'insert or update or delete', 'attrs_values', 'acl_ioctrl()');

/*
create trigger trg_acl_ioCtrl 
before insert or update or delete
on tbl_attrs_values
for each row 
execute procedure acl_ioCtrl();
*/
