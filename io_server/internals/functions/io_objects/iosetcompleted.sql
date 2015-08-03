create or replace function ioSetCompleted(int4, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    oper alias for $2;
begin

    update io_objects set is_completed = oper where id = idObject;    -- oper = 1 - after insert
                                                                      -- oper = 2 - after update 
    --if(FOUND = FALSE) then
    --    return 0;
    --end if;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function ioSetUpdating(int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    cnt int4;
begin

    update io_objects set is_completed = 0 where id = idObject;
    --GET DIAGNOSTICS cnt = ROW_COUNT;
    --if(cnt = 0) then
    --    return 0;
    --end if;

    return 1;

end
$BODY$
language 'plpgsql';
