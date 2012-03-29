create or replace function deleteIndicator(int4) returns int4 as
$BODY$
declare
    idIndicator alias for $1;
begin

    delete from "indicator" where id = idIndicator;

    return 1;
end
$BODY$
language 'plpgsql';

