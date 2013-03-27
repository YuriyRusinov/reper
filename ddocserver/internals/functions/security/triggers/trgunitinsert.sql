CREATE OR REPLACE FUNCTION unitInsertCheck() RETURNS trigger AS
$BODY$
DECLARE

    gname varchar;    

    created int4;
    res int4;
    query varchar;
    idOrg int4;
    parentName varchar;
BEGIN

    if(TG_OP = 'UPDATE') then
        if(new.role_name <> old.role_name) then
            raise exception 'You cannot change field ROLE_NAME of the record';
        end if;

        return new;
    end if;

    if(new.id_organization is not null) then
        if(getCurrentUser() <> 2) then --jupiter
            if(new.id_organization <> getLocalOrgId() and new.name <> 'Публичные должностные лица') then
                raise exception 'You cannot create units in remote organizations!';
                return NULL;
            end if;
        end if;
    end if;
    
    if(new.id_parent is not null) then
        select name into parentName from units where id = new.id_parent;
        if(parentName = 'Публичные должностные лица') then
            raise exception 'You cannot create units with parent as Public!';
            return NULL;
        end if;
    end if;


    gname = new.role_name;
    
    if(gname isnull) then
	raise exception 'Input parameters cannot be NULL. Check input parameters!';
    end if;

    if(gname = 'postgres') then
	raise exception 'group name cannot be postgres!';
    end if;

    if(gname = 'admins') then
	raise notice 'Group name cannot be admins!';
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

    RETURN new;
END;
$BODY$
LANGUAGE 'plpgsql';

select f_safe_drop_trigger('trgunitinsert', 'units');

create trigger trgUnitInsert
after insert or update
on "units"
for each row 
execute procedure unitInsertCheck();
