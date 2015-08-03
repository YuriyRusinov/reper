create or replace function getSystemParamStr(int4) returns varchar as
$BODY$
declare
    idSystem alias for $1;
    val varchar;
begin

    select str_parameter into val from system_table where id_system = idSystem;

    return val;

end
$BODY$
language 'plpgsql' security definer;

create or replace function getSystemParamInt(int4) returns int4 as
$BODY$
declare
    idSystem alias for $1;
    val int4;
begin

    select int_parameter into val from system_table where id_system = idSystem;

    return val;

end
$BODY$
language 'plpgsql' security definer;

