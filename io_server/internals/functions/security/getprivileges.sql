create or replace function getPrivilege(int4, int4, int4, bool) returns bool as
$BODY$
declare
    idRole alias for $1;
    idObject alias for $2;
    whatPrivilege alias for $3; -- readlist = 1, read = 2, delete = 3, update = 4, send = 5, error if other value given
    withInherit alias for $4;   --if true than inspected inherited roles and if thay have privilege then return true.
                                --if role has attribute with_inheritance = false that parameter is ignored
    result boolean;
    idUser int4;
    isAuthor boolean;
begin
    select getCurrentUser() into idUser;
    if(idUser = 1 or idUser = 2) then --admin and jupiter
        return TRUE;--этим пользователям всегда разрешен полный доступ
    end if;

    --ЕСЛИ ТЕКУЩИЙ ПОЛЬЗОВАТЕЛЬ ЯВЛЯЕТСЯ АВТОРОМ, ТО СООТВЕТСТВЕННО В ТАБЛИЦЕ ACCESS_TABLE БУДУТ НЕОБХОДИМЫЕ ЗНАЧЕНИЯ (ПРАВА).
    --ПОЭТОМУ ПРОВЕРКУ НИЖЕПРИВЕДЕННУЮ ВЫПОЛНЯТЬ НЕ НАДО.
    --КРОМЕ ТОГО, АДМИНИСТРАТОР МОЖЕТ УБРАТЬ РЯД ПОЛНОМОЧИЙ С АВТОРА
    
    --select into isAuthor (author = idUser) from tbl_io_objects io where io.id=idObject;
    --if (isAuthor) then
    --    return true;
    --end if;

    select isControlPrivileges() into result;
    if(result = false) then
        return TRUE;
    end if;

    if(whatPrivilege = 1) then
        select allow_readlist into result from getPrivileges(idRole, idObject, withInherit);
        return result;
    end if;
    if(whatPrivilege = 2) then
        select allow_read into result from getPrivileges(idRole, idObject, withInherit);
        return result;
    end if;
    if(whatPrivilege = 3) then
        select allow_delete into result from getPrivileges(idRole, idObject, withInherit);
        return result;
    end if;
    if(whatPrivilege = 4) then
        select allow_update into result from getPrivileges(idRole, idObject, withInherit);
        return result;
    end if;
    if(whatPrivilege = 5) then
        select allow_send into result from getPrivileges(idRole, idObject, withInherit);
        return result;
    end if;

    return NULL; --maybe should return FALSE?
end
$BODY$
language 'plpgsql';

select f_safe_drop_type('h_get_io_privileges');
create type h_get_io_privileges as(id_role int4,
                                   allow_readlist bool, 
                                   allow_read bool, 
                                   allow_delete bool, 
                                   allow_update bool,
                                   allow_send bool,
                                   was_found bool
                                   --record in access_roles was found with/without inheritance
                                   );
                                
--испльзуется в диалоге назначения прав (т.е. загружаются прямо заданные права, без наследования)
create or replace function getIOPrivileges(int4) returns setof h_get_io_privileges as
$BODY$
declare
    idObject alias for $1;

    r h_get_io_privileges%rowtype;
begin

    for r in 
        select id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send 
        from access_table
        where
            id_io_object = idObject
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql' security definer;

--испльзуется в диалоге назначения прав (т.е. загружаются прямо заданные права, без наследования)
/*
create or replace function getRubricPrivileges(int4) returns setof h_get_io_privileges as
$BODY$
declare
    idRubric alias for $1;

    r h_get_io_privileges%rowtype;
    cnt int4;
begin

    select into cnt count (*) from access_rubric_table art where art.id_rubric = idRubric;
    if (cnt > 0) then
        for r in 
            select id_role, allow_readlist, allow_read, allow_delete, allow_update, false, true 
            from access_rubric_table
            where
                id_rubric = idRubric
        loop
            return next r;
        end loop;
    else
        for r in
            select distinct -10, true, true, true, true, false, false from rubricator rubr left join access_rubric_table art on (rubr.id=art.id_rubric and rubr.id=idRubric)
        loop
            return next r;
        end loop;
    end if;

    return;
end
$BODY$
language 'plpgsql' security definer;
*/

select f_safe_drop_type('h_get_privileges');
create type h_get_privileges as(allow_readlist bool, 
                                allow_read bool, 
                                allow_delete bool, 
                                allow_update bool,
                                allow_send bool,
                                was_found bool --запись в таблице access_roles в том числе с применением наследования была найдена  
                                );
                                
create or replace function getPrivileges(int4, int4, bool) returns setof h_get_privileges as
$BODY$
declare
    idRole alias for $1;
    idObject alias for $2;
    withInherit alias for $3;

    r h_get_privileges%rowtype;
    rr h_get_privileges%rowtype;
    bWithInheritance bool;
begin

    select with_inheritance into bWithInheritance from ONLY kks_roles where id = idRole;
    if(bWithInheritance isnull) then
        r.allow_readlist := false;
        r.allow_read := false;
        r.allow_delete := false;
        r.allow_update := false;
        r.allow_send := false;
        r.was_found := true;
        raise exception 'You are not registered in the database! Please, contact your administrator.';
        return next r;
        return;
    end if;

--    raise warning '%, %', bWithInheritance, withInherit;
    
    if(withInherit = false or bWithInheritance = false) then
        for r in 
            select * from getPrivilegesOnly(idRole, idObject)
        loop
            return next r;
        end loop;

        return;
    end if;

    for r in select * from getPrivilegesInherit(idRole, idObject)
    loop
        if(r.was_found = true) then
            return next r;
        else
            for rr in select * from getPrivilegesOnly(-10, idObject)
            loop
               return next rr;
            end loop;
        end if;
    end loop;

    return;

end
$BODY$
language 'plpgsql' security definer;

create or replace function getPrivilegesOnly(int4, int4) returns setof h_get_privileges as
$BODY$
declare
    idRole alias for $1;
    idObject alias for $2;

    r h_get_privileges%rowtype;
begin

    for r in
        select    
            allow_readlist,
            allow_read,
            allow_delete,
            allow_update,
            allow_send,
            true
        from access_table
        where 
            id_role = idRole
            and id_io_object = idObject
    loop
        return next r;
    end loop;

    if(FOUND = FALSE) then
        r.allow_readlist := false;
        r.allow_read := false;
        r.allow_delete := false;
        r.allow_update := false;
        r.allow_send := false;
        r.was_found := false;

        return next r;
    end if;

    return;
end
$BODY$
language 'plpgsql' security definer;

--эта функция работает таким образом, что если в качестве параметра ей задан ПОЛЬЗОВАТЕЛЬ
--и  он назначен на несколько должностей, то она вернет в случае наследования прав пользователем
--права, просмотрев все возможные цепочки.
--В итоге лучше всего на вход ей НЕ подавать в качестве роли отдельного ПОЛЬЗОВАТЕЛЯ. 
--Ограничится должностным лицом
create or replace function getPrivilegesInherit(int4, int4) returns setof h_get_privileges as
$BODY$
declare
    idRole alias for $1;
    idObject alias for $2;

    r h_get_privileges%rowtype;
    rr RECORD;
    rrr h_get_privileges%rowtype;
    inheritedRole int4;
begin

    for r in 
        select * from getPrivilegesOnly(idRole, idObject)
    loop
        for rr in select id_role from getInheritedRoles(idRole)
        loop
            inheritedRole := rr.id_role;
            if(inheritedRole > 0) then
                for rrr in select * from getPrivilegesInherit(inheritedRole, idObject)
                loop
                    r.allow_readlist := r.allow_readlist or rrr.allow_readlist; 
                    r.allow_read := r.allow_read or rrr.allow_read;
                    r.allow_delete := r.allow_delete or rrr.allow_delete;
                    r.allow_update := r.allow_update or rrr.allow_update;
                    r.allow_send := r.allow_send or rrr.allow_send;
                    r.was_found := r.was_found or rrr.was_found;
                end loop;
            end if;
        end loop;

        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_type('h_get_inherited_roles');
create type h_get_inherited_roles as (id_role int4);

--эта функция возвращает наследуемую роль только в случае, если у наследующей роли (т.е. переданной в качестве параметра)
--стоит флаг with_inheritance = TRUE. В противном случае наследовать права не надо и функция вернет только -1 в списке возврата.
create or replace function getInheritedRoles(int4) returns setof h_get_inherited_roles as 
$BODY$
declare
    idRole alias for $1;
    r h_get_inherited_roles%rowtype;
    rr RECORD;

    inheritedRole int4;
    tableName varchar;
begin

    inheritedRole := -1;

    SELECT distinct
        p.relname into tableName
    FROM 
        kks_roles c, pg_class p
    WHERE 
        c.tableoid = p.oid
        and c.id = idRole
        and p.relname <> 'kks_roles'
        and c.with_inheritance = true;

    if(FOUND = FALSE) then
        r.id_role := -1;
        return next r;
        return;
    end if;
    
    if(tableName = 'users') then
        for rr in select id from getUserDls(idRole)
        loop
            r.id_role := rr.id;
            return next r;
        end loop;
    elsif (tableName = 'position') then
        select id_unit into inheritedRole from "position" where id = idRole;
        r.id_role := inheritedRole;
        return next r;
    elsif (tableName = 'units') then
        select id_parent into inheritedRole from units where id = idRole;
        if(inheritedRole isnull) then
            inheritedRole := -1;
        end if;
        r.id_role := inheritedRole;
        return next r;
    else
        r.id_role := -1;
        return next r;
        return;
    end if;
        
    
    return;
end
$BODY$
language 'plpgsql';
