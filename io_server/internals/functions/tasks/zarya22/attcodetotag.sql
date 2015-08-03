create or replace function attCodeToTag(varchar) returns varchar as
$BODY$
declare
    aCode alias for $1;
    safeACode varchar;
begin

    safeACode = replace(aCode, '-', '___');

    return 'z_' || safeACode;
end
$BODY$
LANGUAGE 'plpgsql';
