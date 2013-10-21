create or replace function setSyncResult(int8, int4) returns int4 as
$BODY$
declare
    idQueue alias for $1;
    status alias for $2;

    entityType int4;
    r record;
begin

    update out_sync_queue set sync_result = status where id = idQueue;
    if(FOUND = FALSE) then
        return -1;
    end if;

    for r in select entity_type, id_organization from out_sync_queue where id = idQueue
    loop
        if(r.entity_type = 6) then
            perform setOrgAsCreated(r.id_organization, status-2);--in out_sync_queue field sync_result has value 3 if OK and 4 if ERROR, so we should adapt the value
        end if;
    end loop;

    return 1;

end
$BODY$
language 'plpgsql';
