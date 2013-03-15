create or replace function hStartHandler(varchar, int4) returns int4 as
$BODY$
declare
    iService alias for $1;
    iId alias for $2;

    q varchar;
begin

    q = 'select ' || iService || '(' || iId || ')';
    execute q;

    return 1;

end
$BODY$
language 'plpgsql';
