create or replace function hSimpleHandler(int8) returns int4 as
$BODY$
declare

    iId alias for $1;
    iCount int4;

    i int8;

    a float8;
    b float8;
    c float8;
begin

    update chains_data set start_service_time = clock_timestamp() where id = iId;
    iCount = 1;

    a = 21.2;
    b = 22.1;

    for i in 1..5000000 
    loop
        c = a / i;
    end loop;

    update chains_data set end_service_time = clock_timestamp(), return_code = iCount, is_handled = 1 where id = iId;

    return iCount;
    

end
$BODY$
language 'plpgsql';


create or replace function hSimpleHandler(varchar) returns int4 as
$BODY$
declare

    inData alias for $1;

begin

    return 20;   

end
$BODY$
language 'plpgsql';

--select hStartHandler('hsimplehandler', 2146);