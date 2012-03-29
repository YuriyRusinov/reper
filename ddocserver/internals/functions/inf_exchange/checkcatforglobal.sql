create or replace function checkCatForGlobal() returns trigger as
$BODY$
declare
    r record;
    s bool;
begin

    if(getCurrentUser() = 2) then --jupiter
        return new;
    end if;

    if(TG_OP = 'INSERT') then
        if(new.is_global = false) then
            return new;
        end if;
        --new.is_global = true
        --currently the category is empty! We cannot check that for global

        raise exception 'On insert into table IO_CATEGORY new record cannot have is_global = TRUE!';
        return NULL;
    end if;
    
    if(TG_OP = 'UPDATE') then
        if(new.is_completed = 0 and new.is_global = true) then
            raise exception 'current category is still empty (is_completed = 0), so you cannot have is_global = TRUE!';
            return NULL;
        end if;
        if(old.is_global = true and new.is_global = false) then
            raise exception 'You cannot set GLOBAL category to NONGLOBAL!';
            return NULL;
        end if;
        if(new.is_global = false) then
            return new;
        end if;
    end if;
    
    for r in select is_global, table_name, id_io_object, attr_id from getRefIO(new.id)
    loop
        if(r.is_global = false) then
            raise exception 'Found link to nonglobal object! IO_ID = %, TABLE_NAME = %, ATTR_ID = % ', r.id_io_object, r.table_name, r.attr_id;
            return NULL;
        end if;
    end loop;

/*    
    s = false;
    for r in 
        select id from organization where id <> getLocalOrgId() and is_created = 1
    loop
        perform addSyncRecord(r.id, new.id, new.unique_id, new.unique_id, 'io_categories', 1, 1);
        s = true;
    end loop;
    
    if(s = true) then
        new.id_io_state = 3;
    end if;
*/
    
    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');

/*
create trigger trgCheckCatForGlobal
before insert or update
on io_categories
for each row 
execute procedure checkCatForGlobal();
*/
