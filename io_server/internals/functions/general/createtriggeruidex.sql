--more safe function for createTriggerUID. Can be used when update database to newer version or on ordinary work
create or replace function createTriggerUIDEx(varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    query varchar;
begin

    perform f_safe_drop_trigger('trgsetuid', tableName);

    query := 'create trigger trgsetuid
              before insert or update
              on ' || tableName || '
              for each row 
              execute procedure uidCheck();';

    execute query;

    return 1;
end
$BODY$
language 'plpgsql';

