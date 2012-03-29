create or replace function checkIOForOwner(int4) returns boolean as
$BODY$
declare
    idObject alias for $1;
    idOrganization int4;

    idUser int4;
    r record;
begin

    idUser = getCurrentUser();
    if(idUser <> 1 and idUser <> 2 and idUser <> -100) then --not admin and jupiter and not postgres
    
        for r in 
            select id_owner_org, id_sync_type from tbl_io_objects where id = idObject
        loop
            if(r.id_sync_type = 4) then
                return true; --если тип синхронизации ИО равен объектового ведения, интегрируемые, то вносить ихменения в ИО разрешаем
            end if;

            if(r.id_owner_org is null or r.id_owner_org <> getLocalOrgId()) then
                raise exception 'ONLY OWNER ORGANIZATION CAN MODIFY THE OBJECT';
                return false;
            end if;
        end loop;
    end if;

    return true;

end
$BODY$
language 'plpgsql' security definer;
