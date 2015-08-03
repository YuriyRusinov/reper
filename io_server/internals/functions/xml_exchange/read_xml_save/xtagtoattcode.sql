create or replace function xTagToAttCode(varchar) returns varchar as
$BODY$
declare
    aTag alias for $1;
    safeACode varchar;
begin

    safeACode = replace(aTag, '___', '-');

    return substring(safeACode from 3);--remove z_
end
$BODY$
LANGUAGE 'plpgsql';
