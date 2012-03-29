create or replace function tagToAttCode(varchar) returns varchar as
$BODY$
declare
    aTag alias for $1;
    safeACode varchar;
begin

    safeACode = replace(aTag, '___', '-');

    return safeACode;
end
$BODY$
LANGUAGE 'plpgsql';
