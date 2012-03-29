create or replace function uSetRecordAsSended (int4) returns int4 as
$BODY$
declare
    ide alias for $1;
begin
    --RAISE WARNING 'send inform exchange %', ide;
    update out_sync_queue set sync_result=2 where id = ide;
    if (not FOUND) then
        return 0;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';
