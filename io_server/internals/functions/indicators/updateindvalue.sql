create or replace function updateIndValue(int4, int4, int4, timestamp, varchar) returns int4 as
$BODY$
declare
    idIndValue alias for $1;
    idObjSrc alias for $2;
    idObjSrc1 alias for $3;
    measTime alias for $4;
    theValue alias for $5;
begin
    
    update "indicators_values" 
    set 
        id_io_object_src = idObjSrc,
        id_io_object_src1 = idObjSrc1,
        meas_time = measTime,
        the_value = theValue
    where 
        id = idIndValue;

    return 1;
end
$BODY$
language 'plpgsql';
