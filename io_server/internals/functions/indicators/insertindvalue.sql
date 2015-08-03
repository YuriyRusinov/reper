create or replace function insertIndValue (int4, int4, int4, int4, timestamp, varchar, timestamp) returns int4 as
$BODY$
declare
    idIndex alias for $1;
    idObject alias for $2;
    idObjectSrc alias for $3;
    idObjectSrc1 alias for $4;
    measDatetime alias for $5;
    indValue alias for $6;
    startDatemime alias for $7;

    idIndicator int4;
    
begin

    select getNextSeq('indicators_values', 'id') into idIndicator;
    
    insert into indicators_values (id, 
                                   id_io_object, 
                                   id_indicator, 
                                   id_io_object_src, 
                                   id_io_object_src1, 
                                   meas_time, 
                                   inserted_time, 
                                   value, 
                                   start_time, 
                                   stop_time) 
                           values (idIndicator, 
                                   idObject, 
                                   idObjectSrc, 
                                   idObjectSrc1, 
                                   measDatetime, 
                                   current_timestamp, 
                                   indValue, 
                                   startDatemime, 
                                   current_timestamp); 

    return idIndicator;
end
$BODY$
language 'plpgsql';

