create or replace function getSyncResult(int4) returns int4 as
$BODY$
declare
    idQueue alias for $1;
    status int4;
begin

    select sync_result into status from out_sync_queue where id = idQueue;

    return status;

end
$BODY$
language 'plpgsql';
