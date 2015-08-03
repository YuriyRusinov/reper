create or replace function setRubricPrivileges(int4, int4, bool, bool, bool, bool) returns int4 as
$BODY$
declare
    idRole alias for $1;
    idRubric alias for $2;
    bReadList alias for $3;
    bRead alias for $4;
    bDelete alias for $5;
    bUpdate alias for $6;

    idUser int4;
    theUser varchar;
    tableName varchar;

    query varchar;
    rec record;
    res int4;

    r bool;
    rl bool;
    d bool;
    u bool;
begin

    select into idUser getCurrentUser();
    if (idUser is null or (idUser != 1 and idUser != 2)) then
        raise notice 'Only admin may change privilegies for rubrics';
        return -1;
    end if;

    for rec in 
        select id, role_name from ONLY kks_roles where id = idRole
    loop
        idUser = rec.id;
        theUser = rec.role_name;
        if(idUser = -10) then
            theUser = 'public';
        end if;
    end loop;

    if(idUser isnull) then
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

    select count(id_role) into idUser from access_rubric_table where id_role = idRole and id_rubric = idRubric;

    if(idUser = 0) then
        query := 'insert into access_rubric_table (id_rubric, id_role, allow_read, allow_readlist, allow_delete, allow_update) 
                   values (' || 
                       idRubric           || ', ' || 
                       idRole             || ', ' ||
                       asString(r,true)  || ', ' ||
                       asString(rl,true) || ', ' ||
                       asString(d,true)  || ', ' ||
                       asString(u,true)  || ')';
    else
        query := 'update access_rubric_table set allow_read = ' || asString(r,true) || 
                                       ', allow_readlist = ' || asString(rl,true) ||
                                       ', allow_delete = ' || asString(d,true) ||
                                       ', allow_update = ' || asString(u,true) ||
                  ' where id_rubric = ' || idRubric || ' and id_role = ' || idRole;
    end if;

    execute query;
    if (not FOUND) then
        return -1;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';
