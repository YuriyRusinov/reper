create or replace function getMilliseconds(interval) returns float8 as
$BODY$
declare
    iInt alias for $1;
    mSecs float8;
begin

    SELECT 
        ((60*(EXTRACT(HOURS FROM iInt))
        +
        EXTRACT(MINUTES FROM iInt) ) * 60
        +
        EXTRACT(SECOND FROM iInt) ) * 1000
    into mSecs;

    return mSecs;
end
$BODY$
language 'plpgsql';

create or replace function getSeconds(interval) returns float8 as
$BODY$
declare
    iInt alias for $1;
    mSecs float8;
begin

    SELECT 
        ((60*(EXTRACT(HOURS FROM iInt))
        +
        EXTRACT(MINUTES FROM iInt) ) * 60
        +
        EXTRACT(SECOND FROM iInt) )
    into mSecs;

    return mSecs;
end
$BODY$
language 'plpgsql';
