create or replace function setOrgAsCompleted() returns trigger as 
$BODY$
declare
    lAddress varchar;
    lPort int4;
    isMain int4;

    idOldLocalOrg int4;
    oldEmailPrefix varchar;

    idNewLocalOrg int4;
    newEmailPrefix varchar;

    r record;
begin

    --if syncronization is stopped
    if(processSync() = FALSE) then
        update organization set is_completed = true, is_created = 1 where id = new.id_organization and is_completed = false;
        return new;
    end if;

    if(new.is_active = false) then --is transport is inactive then nothing to do
        return new;
    end if;
    
    for r in select t.local_address, t.local_port from transport t where t.id = new.id_transport
    loop
        lAddress = r.local_address;
        lPort = r.local_port;
    end loop;
    
    if(lAddress is not null and lAddress = new.address and ( ( lPort isnull and new.port isnull) or (lPort = new.port) ) ) then
        select f_is_table_exist('local_org_table', NULL) into isMain;
        if(isMain <> 1) then
            create table local_org_table (id int4, unique_name varchar);
        end if;
        
        for r in select id, email_prefix from organization where id = new.id_organization
        loop
            idNewLocalOrg := r.id;
            newEmailPrefix := r.email_prefix;
        end loop;

        if(getCurrentUser() = 2) then --jupiter
            for r in select id, unique_name from local_org_table limit 1
            loop
                idOldLocalOrg := r.id;
                oldEmailPrefix := r.unique_name;
            end loop;

            if(idOldLocalOrg is not null and oldEmailPrefix = 'localorg_prefix_slave') then
                idNewLocalOrg := replaceLocalOrg(idOldLocalOrg, oldEmailPrefix, idNewLocalOrg, newEmailPrefix);
                if(idNewLocalOrg is null or idNewLocalOrg <= 0) then
                    raise exception 'Cannot replace old and new local organizations!';
                    return NULL;
                end if;

                perform f_safe_drop_trigger('trgupdateorganization', 'organization');
                delete from organization where id = idOldLocalOrg;
                perform f_create_trigger('trgupdateorganization', 'before', 'update or delete', 'organization', 'syncorganizations()');

                --if(FOUND = FALSE) then
                --    raise exception 'Cannot delete old local organization from organization table!';
                --    return NULL;
                --end if;
            end if;

        end if;

        delete from local_org_table;
        insert into local_org_table (id, unique_name) values(idNewLocalOrg, newEmailPrefix); 
        if(FOUND = FALSE) then
            raise exception  'Created local organization but an error was occured in insert into local_org_table';
            return NULL;
        end if;
    end if;

    if(getCurrentUser() = 2) then --jupiter
        update organization set is_completed = true where id = new.id_organization and is_completed = false;
        return new;
    end if;

    if(lAddress is not null and lAddress = new.address and ( ( lPort isnull and new.port isnull) or (lPort = new.port) )) then

        select f_is_table_exist('main_org_table', NULL) into isMain;
        if(isMain <> 1) then
            update organization set is_created = 1 where id = new.id_organization;
            return new;
        end if;

        update organization set is_main = true, is_created = 1 where id = new.id_organization;

    end if;

    update organization set is_completed = true where id = new.id_organization and is_completed = false;
   
    return new;

end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trgupdateorgtransport', 'organization_transport');

select f_create_trigger('trgupdateorgtransport', 'after', 'insert', 'organization_transport', 'setorgascompleted()');

/*
create trigger trgUpdateOrgTransport
after insert
on organization_transport
for each row 
execute procedure setOrgAsCompleted();
*/

create or replace function setOrgAsDisconnected() returns trigger as
$BODY$
declare
    idOrg int4;
    cnt int4;
begin

    idOrg = old.id_organization;
    select count(*) into cnt from organization_transport where id_organization = idOrg;
    if(cnt <> 0) then
        return old;
    end if;
    
    update organization set is_created = 0, is_completed = false where id = idOrg;

    return old;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgdeleteorgtransport', 'organization_transport');

select f_create_trigger('trgdeleteorgtransport', 'after', 'delete', 'organization_transport', 'setorgasdisconnected()');

/*
create trigger trgDeleteOrgTransport
after delete
on organization_transport
for each row 
execute procedure setOrgAsDisconnected();
*/
