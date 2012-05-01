create or replace function firstSyncIO() returns trigger as
$BODY$
declare
    res int4;
    isGlobal bool;
    r record;
begin
    
    --if syncronization is stopped
    if(processSync() = FALSE) then
        raise warning 'Syncronization daemod is stopped!';
        return NULL;
    end if;

    if(getPrivilege(getCurrentUser(), new.id_io_objects, 5, true) = false) then 
        raise exception 'You have insufficient permissions to make IO syncronized!'; 
        return NULL; 
    end if;

    for r in 
        select io.is_global, io.id_sync_type, io.id_owner_org from f_sel_io_objects(new.id_io_objects) io where io.id = new.id_io_objects
    loop
        isGlobal = r.is_global;
        if(r.id_owner_org <> getLocalOrgId() and r.id_sync_type = 4) then --global, intergated (no need to first sync - already done)
            return new;
        end if;
    end loop;
    
    if(isGlobal isnull or isGlobal = FALSE) then
        raise exception 'You cannot syncronize nonglobal objects!';
        return NULL;
    end if;
    
    res = processFirstSync(new.id_io_objects, new.id_organization);
    if(res <= 0) then
        raise exception 'First syncronization of io chain was failed!';
        return NULL;
    end if;
    
    return new;
end
$BODY$
language 'plpgsql' security definer;

create or replace function processFirstSync(int4, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idOrganization alias for $2;
    
    res int4;
    r record;
    q varchar;
    pTable varchar;
    lTable varchar;
    tExist int4;
begin
    raise warning E'\nIn processFirstSync. idObject = %', idObject;

    pTable = 'tmp_processed_objects_' || session_user;
    lTable = 'tmp_looked_objects_' || session_user;
    
    tExist = f_is_temp_table_exist(pTable);
    if(tExist = 0) then
        q = 'create temp table ' || pTable || ' (id_object int4); ';
        execute q;
    end if;
    
    tExist = f_is_temp_table_exist(lTable);
    if(tExist = 0) then
        q = 'create temp table ' || lTable || ' (id_object int4); ';
        execute q;
    end if;

    res = processFirstSyncInt(idObject, idOrganization);
    if(res = 1) then
        
        q = 'update io_objects set id_io_state = 3 where id in (select id_object from ' || pTable || ')';
        execute q;

        --this trigger will not fired on this command because of the object is inserted in tmp_processed_objects
        q = 'insert into io_objects_organization (id_io_objects, id_organization) select id_object, ' || idOrganization || ' from ' || pTable || ' where id_object <> ' || idObject;
        execute q;

        q = 'drop table ' || pTable;
        execute q;

        q = 'drop table ' || lTable;
        execute q;

    end if;
    
    return res;
end
$BODY$
language 'plpgsql';

create or replace function processFirstSyncInt(int4, int4) returns int4 as 
$BODY$
declare
    idObject alias for $1;
    idOrganization alias for $2;
    
    idCat int4;
    ioUID varchar; 
    catUID varchar;
    isGlobal bool;
    idOrg int4;

    r record;
    res int4;
    pTable varchar;
    lTable varchar;
    q varchar;
begin

    pTable = 'tmp_processed_objects_' || session_user;
    lTable = 'tmp_looked_objects_' || session_user;

    select id_organization into idOrg from io_objects_organization where id_io_objects = idObject and id_organization = idOrganization;
    if(idOrg is not null) then
        raise notice 'current object is already sync with given organization';
        return 2; -- current object is already sync with given organization
    end if;
    
    idOrg = NULL;
    q = 'select id_object from ' || pTable || ' where id_object = ' || idObject;
    for r in execute q
    loop
        idOrg = r.id_object;
    end loop;
    
    if(idOrg is not null) then
        raise notice 'object is already processed';
        return 3;--object is already processed
    end if;
    
    for r in
        select 
            io.id_io_category, 
            io.unique_id, 
            io.is_global, 
            io.is_system,
            c.unique_id as cat_uid 
        from 
            f_sel_io_objects(idObject) io, 
            io_categories c 
        where 
            io.id = idObject 
            and io.id_io_category = c.id
    loop
        idCat = r.id_io_category;
        ioUID = r.unique_id;
        isGlobal = r.is_global;
        catUID = r.cat_uid;
    end loop;
    
    if(isGlobal <> TRUE) then
        raise exception 'we cannot sync nonglobal objects. Aborted';
        return -1;--we cannot sync nonglobal objects
    end if;

    idOrg = NULL;
    q = 'select id_object from ' || lTable || ' where id_object = ' || idObject;
    for r in execute q
    loop
        idOrg = r.id_object;
    end loop;
    
    if(idOrg is not null) then
        raise exception 'Cycling was found! This is not allowed! Aborted! ID_OBJECT = %', idOrg;
        return -2;
    end if;

    q = 'insert into ' || lTable || ' (id_object) values(' || idObject || ')';
    execute q;
    
    for r in select id_io_object from getRefIO(idCat)
    loop
        if(r.id_io_object <> 27 and r.id_io_object <> 26) then --organizations are syncronyzed separately
                                      --positions are syncronized separately
            res = processFirstSyncInt(r.id_io_object, idOrganization);
            if(res <= 0) then
                return res;
            end if;
        end if;
    end loop;
    
    --add category for sync
    perform addSyncRecord(idOrganization, idCat, catUID, catUID, 'io_categories', 1, 1);
        --perform addSyncRecord(r.id, new.id, new.unique_id, new.unique_id, 'io_categories', 1, 1);

    --if the IO has no references to another IO
    perform addSyncRecord(idOrganization, idObject, ioUID, ioUID, 'io_objects', 1, 2);
    -- add all existing records in all ref tables of the IO
    --ONLY if io_objects::id_sync_type <> 4! (global, integrated)
    perform addAllRecordsInt(idObject, idOrganization, ioUID);

    
    q = 'insert into ' || pTable || ' (id_object) values(' || idObject || ')';
    execute q;

    return 1;
end
$BODY$
language 'plpgsql';

select f_safe_drop_type('h_getrefio');
create type h_getrefio as(id_io_object int4, table_name varchar, attr_id int4, attr_type int4, is_global bool);
create or replace function getRefIO(int4) returns setof h_getrefio as
$BODY$
declare
    idCat alias for $1;
    idChildCat int4;
    r h_getrefio%rowtype;
begin

    for r in
        select
            distinct io.id,
            io.table_name,
            a.id,
            a.id_a_type,
            io.is_global
        from
            io_objects io,
            io_categories c,
            attrs_categories ac,
            attributes a
        where
            c.id = idCat
            and (c.id = ac.id_io_category or c.id_child = ac.id_io_category)
            and ac.id_io_attribute = a.id
            and a.table_name = io.table_name
        order by 1
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function addAllRecordsInt(int4, int4, varchar) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idOrg alias for $2;
    ioUID alias for $3;
    
    tableName varchar;
    r record;
    rr record;
    q varchar;
begin

    select table_name into tableName from f_sel_io_objects(idObject) where id = idObject;
    if(tableName is null) then
        return 0;
    end if;

    q = 'select unique_id, id from ' || tableName || ' order by 2';
    for r in execute q
    loop
        perform addSyncRecord(idOrg, r.id::int4, r.unique_id, ioUID, tableName, 1, 3);
    end loop;

    for r in 
        select table_name from object_ref_tables where id_io_object = idObject
    loop
        q = 'select unique_id, id from ' || r.table_name || ' order by 2';
        for rr in execute q
        loop
            perform addSyncRecord(idOrg, rr.id, rr.unique_id, ioUID, r.table_name, 1, 4);
        end loop;
    end loop;
    
    return 1;
end
$BODY$
language 'plpgsql';
