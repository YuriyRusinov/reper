create or replace function deleteIndValue(int4, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idIndicator alias for $2;
begin

    delete from indicators_values where id_io_object = idObject and id_indicator = idIndicator;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function deleteIndValue(int4) returns int4 as
$BODY$
declare
    idIndicatorValue alias for $1;
begin
    delete from indicators_values where id = idIndicatorValue;

    return 1;
end
$BODY$
language 'plpgsql';
