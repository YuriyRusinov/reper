create or replace function replaceLocalOrg(int4, varchar, int4, varchar) returns int4 as
$BODY$
declare
    oldId alias for $1;
    oldPrefix alias for $2;
    newId alias for $3;
    newPrefix alias for $4;
begin

    if(getCurrentUser() <> 2) then -- 'jupiter'
        return -1;
    end if;

    if(oldId isnull or oldId <= 0 or oldPrefix isnull or newId isnull or newId <= 0 or newPrefix isnull) then
        return -1;
    end if;

    update organization set id_parent = newId where id_parent = oldId;
    update organization set id_parent1 = newId where id_parent1 = oldId;
    update organization set id_substitutor = newId where id_substitutor = oldId;

    --!!! update out_sync_queue set id_organization = newId where id_organization = oldId;
    update coworkers set id_organization1 = newId  where id_organization1 = oldId;
    update coworkers set id_organization2 = newId  where id_organization2 = oldId;

    update report_organization set id_organization = newId  where id_organization = oldId;    
    update user_chains_organization set id_organization = newId  where id_organization = oldId;

    delete from organization_work_mode where id_organization = oldId;

    update units set id_organization = newId where id_organization = oldId;
    update receivers set id_organization = newId where id_organization = oldId;

    update tbl_io_objects set id_owner_org = newId where id_owner_org = oldId;
    update io_objects_organization set id_organization = newId where id_organization = oldId;
    update io_sync_archive set id_organization = newId where id_organization = oldId;
    update io_last_sync set id_organization = newId where id_organization = oldId;

    delete from organization_transport where id_organization = oldId;
    create unique index ot_addr_port on organization_transport (address, port);

    update root_table set 
        unique_id = overlay(unique_id placing newPrefix from 1 for char_length(oldPrefix)) 
    where 
        unique_id not in (select unique_id from tbl_io_objects where id < 300) --not apply for system qualifiers! they MUST have unique_id starts with localorg
        and substring(unique_id from 1 for char_length(oldPrefix)) = oldPrefix;

    return newId;

end
$BODY$
language 'plpgsql';
