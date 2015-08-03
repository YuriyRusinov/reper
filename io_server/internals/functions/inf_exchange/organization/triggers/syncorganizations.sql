create or replace function syncOrganizations() returns trigger as 
$BODY$
declare
    theTable varchar;
    idEntity int4;
    entityUID varchar;
    entityIOUID varchar;
    syncType int4;
    idObject int4;

    entityType int4;
    r record;
    idLocalOrg int4;
    isMain int4;
begin


    --if syncronization is stopped
    if(processSync() = FALSE) then
        if(TG_OP = 'DELETE') then
            return old;
        else
            return new;
        end if;
    end if;

    theTable := TG_RELNAME; --organization
    idObject = 27; --id of organization qualifier in io_objects table
    entityIOUID = 'not_used'; --new.unique_id;--in this context not used

    if(theTable <> 'organization') then
        raise exception 'syncOrganizations was invoked not on ORGANIZATION table!';
        return NULL;
    end if;

    if(TG_OP = 'INSERT') then
        return new;
    end if;

   
    idLocalOrg = getLocalOrgId();

    --raise notice '%', TG_OP;
    
    if(TG_OP = 'UPDATE') then
        --raise exception E'\n\nasasasas\n\n';
        if(old.id <> new.id) then
            raise exception 'You cannot change id of the records!';
            return NULL;
        end if;
        if(old.email_prefix <> new.email_prefix) then
            raise exception 'You cannot change EMAIL_PREFIX field of organization!';
            return NULL;
        end if;
    
        if(old.is_completed = FALSE and new.is_completed = TRUE) then
            if(new.id = idLocalOrg or idLocalOrg = -1) then -- is local organization
                raise warning 'new.id = idLocalOrg';
                return new;
            end if;

            if(getCurrentUser() = 2) then --jupiter
                return new;
            end if;

            syncType = 1;--create new entity on target object
            idEntity = new.id;
            entityUID = 'not_used'; --for organization table this field is not used
            entityType = 6;--start check of organization existance
            
            --raise exception E'\n\nasasasas\n\n';
            perform addSyncRecord(idEntity, idEntity, entityUID, entityIOUID, theTable, syncType, entityType);
            
            return new;
            
        else
            if(old.is_created = 0 and new.is_created = 1) then --check of org existance was successfull
                entityType = 8;--send to new organization full qualifier of organizations
                syncType = 2;--update exist entity (organization table) on target object
                

                if(getCurrentUser() <> 2) then --jupiter. ONLY JUPITER CAN CHANGE THIS FIELD
                    return new;
                end if;

                --only main organization can send organizations ot other objects
                if(isMainOrg() <> true) then
                    return new;
                end if;
                

                if(new.id <> idLocalOrg) then
                    for r in select unique_id from f_sel_io_objects(idObject) where id = idObject
                    loop
                        perform addSyncRecord(new.id, idObject, r.unique_id, r.unique_id, 'io_objects', syncType, entityType);
                    end loop;
                end if;
                
                entityType = 9;--send to all others organizations new record about created organization
                syncType = 1;--insert new record
                idEntity = new.id;
                entityUID = 'not_used'; --for organization table this field is not used

                for r in select id from organization where id not in (new.id, idLocalOrg) and is_created = 1
                loop
                    perform addSyncRecord(r.id, idEntity, entityUID, entityIOUID, theTable, syncType, entityType);
                end loop;

                --send to new organization all global categories that are not system
                --!!!DO NOT NEED TO DO THAT!!
                --Categories are alwayes syncronized when IO syncronizing
                /*
                if(new.id <> idLocalOrg) then
                    for r in 
                        select id, unique_id from io_categories where is_global = true and is_system = false
                    loop
                        perform addSyncRecord(new.id, r.id, r.unique_id, r.unique_id, 'io_categories', 1, 1);
                    end loop;
                end if;
                */
                
                return new;

            elsif(new.is_created <> 1) then

               return new;

            end if;
            
            --обновляем информацию об изменениях  в описании организации
            --однако есть ряд условий
            --1) если организация была объявлена как имеющая тип "внешняя унаследованная система", то никаких обновлений слать не будем
            --2) если изменилась нелокальная организация на не главной машине, то никаких обновлений слать не будем (измениться мог только тип организации на "внешняя унаследованная система" (триггер trgorginsertcheckbefore))

            if(isMainOrg() = false and new.id <> idLocalOrg) then
                return new;
            end if;
            if(new.id_type_org = 3) then
                return new;
            end if;
            
            --just will send to all organizations updated record
            syncType = 2; --update exist entity on target object
            idEntity = new.id;
            entityUID = new.email_prefix; 
            
            if(getCurrentUser() = 2) then --jupiter
                return new;
            end if;

            for r in select id from organization where id not in ( idLocalOrg )  and is_created = 1
            loop
                perform addSyncRecord(r.id, idEntity, entityUID, entityIOUID, theTable, syncType, 3);
            end loop;
            
            return new;
        end if;
    
    elsif (TG_OP = 'DELETE') then
        syncType = 3;
        idEntity = old.id;
        entityUID = old.email_prefix; 
        
        if(getCurrentUser() = 2) then --jupiter
            return new;
        end if;

        for r in --send message to all organizations except local and deleting
            select id from organization where id not in (old.id, idLocalOrg) and is_created = 1
        loop
            perform addSyncRecord(r.id, idEntity, entityUID, entityIOUID, theTable, syncType, 9);
        end loop;

        return old;
    end if;

    return new;
end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trgupdateorganization', 'organization');

select f_create_trigger('trgupdateorganization', 'before', 'update or delete', 'organization', 'syncorganizations()');

/*
create trigger trgUpdateOrganization
before update or delete
on organization
for each row 
execute procedure syncOrganizations();
*/
