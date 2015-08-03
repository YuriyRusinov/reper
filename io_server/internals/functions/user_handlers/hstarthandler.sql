create or replace function hStartHandler(varchar, int4) returns int4 as
$BODY$
declare
    iService alias for $1;
    iId alias for $2;

    q varchar;
begin

    q = 'select * from ' || iService || '(' || iId || ')';
    execute q;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function hStartSheduledHandler(int4, varchar, varchar) returns int4 as
$BODY$
declare
    idShedule alias for $1;
    iService alias for $2;
    inData alias for $3;

    q varchar;
begin

    if(idShedule isnull or idShedule <= 0) then
        return -1;
    end if;

    --invoked in getSheduledHandlers()
    --update sheduled_handlers set last_exec = clock_timestamp() where id = idShedule;

    if(inData is not null) then 
        q = 'select * from ' || iService || '(' || quote_literal(inData) || ')';
    else
        q = 'select * from ' || iService || '(NULL::varchar)';
    end if;

    execute q;

    return 1;

end
$BODY$
language 'plpgsql';
