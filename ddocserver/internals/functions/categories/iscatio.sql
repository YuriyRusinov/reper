create or replace function isIOBycat (bigint) returns boolean as
$BODY$
declare
    id_cat alias for $1;

    cnt bigint;
    res boolean;
begin

    select into cnt count(*) from io_objects where id_io_category = id_cat;

    res := cnt > 0;
    return res;
end
$BODY$
language 'plpgsql';
