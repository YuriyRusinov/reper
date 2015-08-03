create or replace function xmlText (xml) returns text as
$BODY$
declare
    iValue alias for $1;

    result text;
    w_str varchar;
    ipos int4;
begin

    w_str := iValue ::varchar;
    --raise warning '% %', value, w_str;

    if (iValue is null) then
        return null;
    end if;

    w_str := iValue::varchar;
    ipos := position ('encoding' in w_str);
    --raise warning '%', ipos;

    if (ipos <= 0) then
        result := replace (w_str, '<?xml version="1.0" ' , '<?xml version="1.0" encoding="utf8" ');
        --raise warning 'w_str is %, result is %', w_str, result;
    end if;

    --result := w_str::text;
    return result;

end
$BODY$
language 'plpgsql';
