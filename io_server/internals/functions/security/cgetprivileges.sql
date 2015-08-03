
create or replace function cGetPrivilege(int4, int4, int4, bool) returns bool as
$BODY$
declare
    idRole alias for $1;
    idCategory alias for $2;
    whatPrivilege alias for $3; -- readlist = 1, read = 2, delete = 3, update = 4, use = 5, error if other value given
    withInherit alias for $4;   --if true than inspected inherited roles and if thay have privilege then return true.
                                --if role has attribute with_inheritance = false that parameter is ignored
    result boolean;
    idUser int4;
    --isAuthor boolean;
begin
    select getCurrentUser() into idUser;
    if(idUser = 1 or idUser = 2) then --admin and jupiter
        return TRUE;--ýòèì ïîëüçîâàòåëÿì âñåãäà ðàçðåøåí ïîëíûé äîñòóï
    end if;

    --ÅÑËÈ ÒÅÊÓÙÈÉ ÏÎËÜÇÎÂÀÒÅËÜ ßÂËßÅÒÑß ÀÂÒÎÐÎÌ, ÒÎ ÑÎÎÒÂÅÒÑÒÂÅÍÍÎ Â ÒÀÁËÈÖÅ ACCESS_TABLE ÁÓÄÓÒ ÍÅÎÁÕÎÄÈÌÛÅ ÇÍÀ×ÅÍÈß (ÏÐÀÂÀ).
    --ÏÎÝÒÎÌÓ ÏÐÎÂÅÐÊÓ ÍÈÆÅÏÐÈÂÅÄÅÍÍÓÞ ÂÛÏÎËÍßÒÜ ÍÅ ÍÀÄÎ.
    --ÊÐÎÌÅ ÒÎÃÎ, ÀÄÌÈÍÈÑÒÐÀÒÎÐ ÌÎÆÅÒ ÓÁÐÀÒÜ ÐßÄ ÏÎËÍÎÌÎ×ÈÉ Ñ ÀÂÒÎÐÀ
    
    --select into isAuthor (author = idUser) from tbl_io_objects io where io.id=idObject;
    --if (isAuthor) then
    --    return true;
    --end if;

    select isControlPrivileges() into result;
    if(result = false) then
        return TRUE;
    end if;

    if(whatPrivilege = 1) then
        select allow_readlist into result from cGetPrivileges(idRole, idCategory, withInherit);
        return result;
    end if;
    if(whatPrivilege = 2) then
        select allow_read into result from cGetPrivileges(idRole, idCategory, withInherit);
        return result;
    end if;
    if(whatPrivilege = 3) then
        select allow_delete into result from cGetPrivileges(idRole, idCategory, withInherit);
        return result;
    end if;
    if(whatPrivilege = 4) then
        select allow_update into result from cGetPrivileges(idRole, idCategory, withInherit);
        return result;
    end if;
    if(whatPrivilege = 5) then
        select allow_use into result from cGetPrivileges(idRole, idCategory, withInherit);
        return result;
    end if;

    return NULL; --maybe should return FALSE?
end
$BODY$
language 'plpgsql';

select f_safe_drop_type('h_get_c_privileges');
create type h_get_c_privileges as(id_role int4,
                                   allow_readlist bool,
                                   allow_read bool, 
                                   allow_delete bool, 
                                   allow_update bool,
                                   allow_use bool,
                                   was_found bool
                                   --record in access_roles was found with/without inheritance
                                   );
                                

create or replace function getCPrivileges(int4) returns setof h_get_c_privileges as
$BODY$
declare
    idCategory alias for $1;

    r h_get_c_privileges%rowtype;
begin

    for r in 
        select id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_use 
        from access_categories_table
        where
            id_io_category = idCategory
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql' security definer;

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

select f_safe_drop_type('h_get_c_privileges1');
create type h_get_c_privileges1 as(allow_readlist bool, 
                                   allow_read bool, 
                                   allow_delete bool, 
                                   allow_update bool,
                                   allow_use bool,
                                   was_found bool --çàïèñü â òàáëèöå access_roles â òîì ÷èñëå ñ ïðèìåíåíèåì íàñëåäîâàíèÿ áûëà íàéäåíà  
                                   );
                                
create or replace function cGetPrivileges(int4, int4, bool) returns setof h_get_c_privileges1 as
$BODY$
declare
    idRole alias for $1;
    idCategory alias for $2;
    withInherit alias for $3;

    r h_get_c_privileges1%rowtype;
    rr h_get_c_privileges1%rowtype;
    bWithInheritance bool;
begin

    select with_inheritance into bWithInheritance from ONLY kks_roles where id = idRole;
    if(bWithInheritance isnull) then
        r.allow_readlist := false;
        r.allow_read := false;
        r.allow_delete := false;
        r.allow_update := false;
        r.allow_use := false;
        r.was_found := true;
        raise exception 'You are not registered in the database! Please, contact your administrator.';
        return next r;
        return;
    end if;

--    raise warning '%, %', bWithInheritance, withInherit;
    
    if(withInherit = false or bWithInheritance = false) then
        for r in 
            select * from cGetPrivilegesOnly(idRole, idCategory)
        loop
            return next r;
        end loop;

        return;
    end if;

    for r in select * from cGetPrivilegesInherit(idRole, idCategory)
    loop
        if(r.was_found = true) then
            return next r;
        else
            for rr in select * from cGetPrivilegesOnly(-10, idCategory)
            loop
               return next rr;
            end loop;
        end if;
    end loop;

    return;

end
$BODY$
language 'plpgsql' security definer;

create or replace function cGetPrivilegesOnly(int4, int4) returns setof h_get_c_privileges1 as
$BODY$
declare
    idRole alias for $1;
    idCategory alias for $2;

    r h_get_c_privileges1%rowtype;
begin

    for r in
        select    
            allow_readlist,
            allow_read,
            allow_delete,
            allow_update,
            allow_use,
            true
        from access_categories_table
        where 
            id_role = idRole
            and id_io_category = idCategory
    loop
        return next r;
    end loop;

    if(FOUND = FALSE) then
        r.allow_readlist := false;
        r.allow_read := false;
        r.allow_delete := false;
        r.allow_update := false;
        r.allow_use := false;
        r.was_found := false;

        return next r;
    end if;

    return;
end
$BODY$
language 'plpgsql' security definer;

--ýòà ôóíêöèÿ ðàáîòàåò òàêèì îáðàçîì, ÷òî åñëè â êà÷åñòâå ïàðàìåòðà åé çàäàí ÏÎËÜÇÎÂÀÒÅËÜ
--è  îí íàçíà÷åí íà íåñêîëüêî äîëæíîñòåé, òî îíà âåðíåò â ñëó÷àå íàñëåäîâàíèÿ ïðàâ ïîëüçîâàòåëåì
--ïðàâà, ïðîñìîòðåâ âñå âîçìîæíûå öåïî÷êè.
--Â èòîãå ëó÷øå âñåãî íà âõîä åé ÍÅ ïîäàâàòü â êà÷åñòâå ðîëè îòäåëüíîãî ÏÎËÜÇÎÂÀÒÅËß. 
--Îãðàíè÷èòñÿ äîëæíîñòíûì ëèöîì
create or replace function cGetPrivilegesInherit(int4, int4) returns setof h_get_c_privileges1 as
$BODY$
declare
    idRole alias for $1;
    idCategory alias for $2;

    r h_get_c_privileges1%rowtype;
    rr RECORD;
    rrr h_get_c_privileges1%rowtype;
    inheritedRole int4;
begin

    for r in 
        select * from cGetPrivilegesOnly(idRole, idCategory)
    loop
        for rr in select id_role from getInheritedRoles(idRole)
        loop
            inheritedRole := rr.id_role;
            if(inheritedRole > 0) then
                for rrr in select * from cGetPrivilegesInherit(inheritedRole, idCategory)
                loop
                    r.allow_readlist := r.allow_readlist or rrr.allow_readlist; 
                    r.allow_read := r.allow_read or rrr.allow_read;
                    r.allow_delete := r.allow_delete or rrr.allow_delete;
                    r.allow_update := r.allow_update or rrr.allow_update;
                    r.allow_use := r.allow_use or rrr.allow_use;
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



