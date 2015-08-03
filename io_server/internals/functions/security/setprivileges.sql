create or replace function applyAclTemplateIO (int4, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idTemplate alias for $2;

    r record;
    ier int4;
begin

    for r in
        select * from getPrivilegiesFromTemplate (idTemplate)
    loop
        ier := setPrivileges (r.id_role, idObject, r.allow_readlist, r.allow_read, r.allow_delete, r.allow_update, r.allow_send);
        if (ier < 0) then
            return -1;
        end if;
    end loop;

    return 1;
end
$BODY$
language 'plpgsql';

--this function only for DLs
create or replace function setPrivileges(varchar, int4, bool, bool, bool, bool, bool) returns int4 as
$BODY$
declare
    uidRole alias for $1;
    idObject alias for $2;
    bReadList alias for $3;
    bRead alias for $4;
    bDelete alias for $5;
    bUpdate alias for $6;
    bSend alias for $7;

    res int4;
    idRole int4;
begin
    select id into idRole from "position" where unique_id = uidRole;
    if(idRole isnull) then
        return -1;
    end if;
        
    res = setPrivileges(idRole, idObject, bReadList, bRead, bDelete, bUpdate, bSend);
    return res;
end
$BODY$
language 'plpgsql';

create or replace function setPrivileges(int4, int4, bool, bool, bool, bool, bool) returns int4 as
$BODY$
declare
    idRole alias for $1;
    idObject alias for $2;
    bReadList alias for $3;
    bRead alias for $4;
    bDelete alias for $5;
    bUpdate alias for $6;
    bSend alias for $7;

    idAuthor int4;
    theUser varchar;
    tableName varchar;

    query varchar;
    rec record;
    res int4;

    r bool;
    rl bool;
    d bool;
    u bool;
    s bool;

    currUser int4;
begin

    query = 'select author, table_name from tbl_io_objects where id = ' || idObject;
    for rec in execute query
    loop
        idAuthor = rec.author;
        tableName = rec.table_name;
    end loop;

    currUser = getCurrentUser();
    if(currUser <> 1 and currUser <> 2) then -- not admin, jupiter
        if(idAuthor isnull) then
            return -1;
        end if;

        if(idAuthor <> getCurrentUser()) then 
            raise exception 'You are not author of the IO, so you cannot delegate any privileges for it to another roles!';
            return -1;
        end if;
    end if;


    for rec in 
        select id, role_name from ONLY kks_roles where id = idRole
    loop
        idAuthor = rec.id;
        theUser = rec.role_name;
        if(idAuthor = -10) then
            theUser = 'public';
        end if;
    end loop;

    if(idAuthor isnull) then
        return -1;
    end if;

    rl := bReadList;
    if(rl isnull) then
        rl := false;
    end if;
    r := bRead;
    if(r isnull) then
        r := false;
    end if;
    u := bUpdate;
    if(u isnull) then
        u := false;
    end if;
    d := bDelete;
    if(d isnull) then
        d := false;
    end if;
    s := bSend;
    if (s is null) then
        s := false;
    end if;

    select count(id_role) into idAuthor from access_table where id_role = idRole and id_io_object = idObject;

    if(idAuthor = 0) then
        query := 'insert into access_table (id_io_object, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_send) 
                   values (' || 
                       idObject           || ', ' || 
                       idRole             || ', ' ||
                       asString(r,false)  || ', ' ||
                       asString(rl,false) || ', ' ||
                       asString(d,false)  || ', ' ||
                       asString(u,false)  || ', ' ||
                       asString(s,false)  || ')';
    else
        query := 'update access_table set allow_read = ' || asString(r,false) || 
                                       ', allow_readlist = ' || asString(rl,false) ||
                                       ', allow_delete = ' || asString(d,false) ||
                                       ', allow_update = ' || asString(u,false) ||
                                       ', allow_send = ' || asString (s,false) ||
                  ' where id_io_object = ' || idObject || ' and id_role = ' || idRole;
    end if;

    execute query;
    if(FOUND = FALSE) then
        raise exception 'Cannot modify access_table! The query is %', query;
        return -1;
    end if;

--Поскольку права доступа проверяются при обращении к таблице через механизм правил и переименования таблицы, то выполнять нижеприведенный код не надо.
/*
    if(tableName is not null) then
        if(theUser = 'public') then
            select setGrants(tableName) into res;
        else
            select setGrants(tableName, theUser) into res;
        end if;
        
        if(res <> 1) then
            raise exception 'Cannot set grants for table % to %', tableName, theUser;
        end if;
        for rec in  
            select table_name from object_ref_tables where id_io_object = idObject
        loop
            if(theUser = 'public') then
                select setGrants(tableName) into res;
            else
                select setGrants(rec.table_name, theUser) into res;
            end if;
            
            if(res <> 1) then
                raise exception 'Cannot set grants for table % to %', rec.table_name, theUser;
            end if;
        end loop;
    end if;
*/

    return 1;
end
$BODY$
language 'plpgsql' security definer;
