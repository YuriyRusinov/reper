create or replace function uuidCheck() returns trigger as
$BODY$
declare
    tableName varchar;
    uuid_t uuid;
begin

    if(new.uuid_t isnull) then
        new.uuid_t = uuid_generate_v1();
    end if;

    return new;
end
$BODY$
language 'plpgsql';


create or replace function createTriggerUUID(varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    query varchar;
begin

    --select f_safe_drop_trigger('trgsetuid', tableName);

    query = 'create trigger trgSetUUID
             before insert or update
             on ' || tableName || '
             for each row 
             execute procedure uuidCheck();';

    execute query;

    return 1;
end
$BODY$
language 'plpgsql';
