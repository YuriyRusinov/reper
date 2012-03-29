create or replace function cSetPrivileges(int4, int4, bool, bool, bool, bool, bool) returns int4 as
$BODY$
declare
    idRole alias for $1;
    idCategory alias for $2;
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

    currUser = getCurrentUser();
    if(currUser <> 1 and currUser <> 2) then -- not admin, jupiter
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

    select count(id_role) into idAuthor from access_categories_table where id_role = idRole and id_io_category = idCategory;

    if(idAuthor = 0) then
        query := 'insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use) 
                   values (' || 
                       idCategory         || ', ' || 
                       idRole             || ', ' ||
                       asString(r,false)  || ', ' ||
                       asString(rl,false) || ', ' ||
                       asString(d,false)  || ', ' ||
                       asString(u,false)  || ', ' ||
                       asString(s,false)  || ')';
    else
        query := 'update access_categories_table set allow_read = ' || asString(r,false) || 
                                       ', allow_readlist = ' || asString(rl,false) ||
                                       ', allow_delete = ' || asString(d,false) ||
                                       ', allow_update = ' || asString(u,false) ||
                                       ', allow_use = ' || asString (s,false) ||
                  ' where id_io_category = ' || idCategory || ' and id_role = ' || idRole;
    end if;

    execute query;
    if(FOUND = FALSE) then
        raise exception 'Cannot modify access_table! The query is %', query;
        return -1;
    end if;

    return 1;
end
$BODY$
language 'plpgsql' security definer;
