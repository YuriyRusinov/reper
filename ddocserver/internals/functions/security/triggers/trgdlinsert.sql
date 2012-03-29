CREATE OR REPLACE FUNCTION dlInsertCheck() RETURNS trigger AS
$BODY$
DECLARE

    gname varchar;    
    uname varchar;

    created int4;
    res int4;
    query varchar;
    idOrg int4;
    idLocalOrg int4;
    idOrgType int4;
    unitName varchar;
    r record;
    io_uid varchar;

BEGIN

    if(new.id isnull) then
        new.id = getNextSeq('position', 'id');
    end if;

    if(new.email_prefix isnull) then
        new.email_prefix = generateDlEmailPrefix(new.id);
    end if;

    for r in select u.id_organization, u.name, o.id_type_org from units u, organization o where u.id = new.id_unit and u.id_organization = o.id
    loop
        idOrg = r.id_organization;
        idOrgType = r.id_type_org;
        unitName = r.name;
    end loop;

    --получаем идентификатор локальной организации
    idLocalOrg = getLocalOrgId();

    --if jupiter insert this record then id_user and id_user_vrio MUST BE NULL!!!
    --this block check that local user not assigned to nonlocal unit
    if(new.id_user is not null or new.id_user_vrio is not null) then
        if(idOrg is not null) then
            if(idOrg <> idLocalOrg) then
                raise exception 'You cannot set local user to nonlocal unit!';
                return NULL;
            end if;
        end if;
    end if;

    --запрещаем создавать должностных лиц в подразделени€х нелокальных организаций
    --однако если нелокальна€ организаци€ имеет тип 3 (внешн€€ унаследованна€ система)
    --то мы разрешаем создавать публичных (только публичных) ƒЋ в подразделении
    -- ѕубличные должностные лица.
    if(idOrg <> idLocalOrg) then
        if(getCurrentUser() <> 2 and (idOrgType <> 3 or unitName <> 'ѕубличные должностные лица')) then
            raise exception 'You cannot create position in nonlocal units (only in Public)';
            return NULL;
        else
            new.is_public = true;
        end if;
    end if;

    --запрещаем создавать ƒЋ в подразделении "ѕубличные должностные лица" локальной организации
    --они должны находитс€ каждый в своем реальном подразделении
    if(idOrg = idLocalOrg and unitName = 'ѕубличные должностные лица') then
        raise exception 'You cannot create position in Public of local organization!';
        return NULL;
    end if;

    --public positions send
    --данные отправл€ем только если ƒЋ создали в локальной организации.
    if(TG_OP = 'INSERT' or TG_OP = 'UPDATE' ) then
        if(new.is_public = TRUE and getCurrentUser() <> 2 and idOrg = idLocalOrg) then --not jupiter
            for r in select id from organization where is_created = 1 and id not in (select getLocalOrgId())
            loop
                select unique_id into io_uid from io_objects where id = 26; --position qualifier
                perform addSyncRecord(r.id, new.id, new.unique_id, io_uid, 'position', 1, 10);
            end loop;
        end if;
        if(TG_OP = 'UPDATE') then
            if(old.is_public = TRUE and new.is_public = FALSE and getCurrentUser() <> 2 and idOrg = idLocalOrg) then --not jupiter
                for r in select id from organization where is_created = 1 and id not in (select getLocalOrgId())
                loop
                    select unique_id into io_uid from io_objects where id = 26; --position qualifier
                    perform addSyncRecord(r.id, new.id, new.unique_id, io_uid, 'position', 3, 11);
                end loop;
            end if;
        end if;
    end if;

    if(TG_OP = 'UPDATE') then
        if(new.role_name <> old.role_name) then
            raise exception 'You cannot change field ROLE_NAME of the record';
        end if;

        if(new.id_user <> old.id_user) then
            if(new.id_user is not null) then
                perform uAddToGroup(new.id_user, new.role_name);
            end if;
            if(old.id_user is not null) then
                perform uRemoveFromGroup(old.id_user, new.role_name);
            end if;
        end if;
        if(new.id_user_vrio <> old.id_user_vrio) then
            if(new.id_user_vrio is not null) then
                perform uAddToGroup(new.id_user_vrio, new.role_name);
            end if;
            if(old.id_user_vrio is not null) then
                perform uRemoveFromGroup(old.id_user_vrio, new.role_name);
            end if;
        end if;

--        if(new.id_unit <> old.id_unit) then
--            perform gAddToGroup(new.role_name, new.id_unit);
--            perform gRemoveFromGroup(new.role_name, old.id_unit);
--        end if; 

        return new;
    end if;

    gname = new.role_name;
    
    if(gname isnull) then
	raise exception 'Input parameters cannot be NULL. Check input parameters!';
    end if;

    if(gname = 'postgres') then
	raise exception 'group name cannot be postgres!';
    end if;

    if(gname = 'admins') then
	--raise notice 'Group name cannot be admins!';
        return new;
    end if;

    --создаем группу в таблице pg_group
    select safeCreateGroup(gname) into res;
    if(res = 0) then
	select count(groname) into res from pg_group where groname = gname;
	if(res = 0) then 
	    raise exception 'Clucter already contains ROLE with specified name. New GROUP must have another name!';
	end if;
    end if;

    if(new.id_user is not null) then
        perform uAddToGroup(new.id_user, new.role_name);
    end if;

    if(new.id_user_vrio is not null) then
        perform uAddToGroup(new.id_user_vrio, new.role_name);
    end if;

    --perform gAddToGroup(new.role_name, new.id_unit);

    RETURN new;
END;
$BODY$
LANGUAGE 'plpgsql';

select f_safe_drop_trigger('trgdlinsert', 'position');

create trigger trgDlInsert
after insert or update
on "position"
for each row 
execute procedure dlInsertCheck();
