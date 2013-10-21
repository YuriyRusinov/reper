select f_safe_drop_type('h_eio_get_indicators');
create type h_eio_get_indicators as(
                                  id_rec int8,
                                  id_io_category int4,
                                  id_io_attribute int4,
                                  value varchar,
                                  attr_code varchar,  --при информационном обмене (в ф-и ioGetObjectAttrsEx() в качестве значения данного поля используется unique_id)
                                  id_attr_type int4,
                                  id_attr_category int4,
                                  id_attr_value int8, 
                                  start_time timestamp,
                                  stop_time timestamp,
                                  insert_time timestamp,
                                  id_io_object_src int4,
                                  id_io_object_src1 int4,
                                  is_actual boolean,
                                  description varchar);

create or replace function eioGetIndicators(int8, bool, timestamp, timestamp) returns setof h_eio_get_indicators as
$BODY$
declare
    idRec alias for $1;
    isActual alias for $2;
    startTime alias for $3;
    stopTime alias for $4;

    r h_eio_get_indicators%rowtype;

    iStartTime timestamp;
    iStopTime timestamp;
    iIsActual boolean;
begin

    if(isActual isnull) then
        iIsActual := true;
    else
        iIsActual := isActual;
    end if;

    if(startTime isnull) then
        iStartTime := '-infinity'::timestamp;
    else 
        iStartTime := startTime;
    end if;

    if(stopTime isnull) then
        iStopTime := 'infinity'::timestamp;
    else 
        iStopTime := stopTime;
    end if;

    for r in 
        select 
            rav.id_record, 
            ac.id_io_category, 
            ac.id_io_attribute, 
            (case when session_user = 'jupiter' then getUniqueValue(rav.value, a.id_a_type, a.table_name) else rav.value end ) as value,
            a.code,
            a.id_a_type,
            rav.id_attr_category,
            rav.id,
            rav.start_time,
            rav.stop_time,
            rav.insert_time,
            rav.id_io_object_src,
            rav.id_io_object_src1,
            rav.is_actual,
            rav.description
        from 
            ( f_sel_rec_attrs_values(idRec) rav inner join attrs_categories ac on (rav.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and rav.id_record = idRec))
            --( rec_attrs_values rav inner join attrs_categories ac on (rav.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and rav.id_record = idRec))
        where 
            case when isActual = true then rav.is_actual = true else (rav.start_time >= iStartTime and rav.stop_time <= iStopTime) end
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';


--используется в информационном обмене
create or replace function eioGetIndicatorsEx(int8) returns setof h_eio_get_indicators as
$BODY$
declare
    idRecord alias for $1;
    r h_eio_get_indicators%rowtype;
begin

    for r in 
        select 
            rav.id_record, 
            ac.id_io_category, 
            ac.id_io_attribute,--!!! 
            getUniqueValue(rav.value, a.id_a_type, a.table_name) as value,
            a.unique_id,
            a.id_a_type,
            rav.id_attr_category,
            rav.id,
            rav.start_time,
            rav.stop_time,
            rav.insert_time,
            rav.id_io_object_src,
            rav.id_io_object_src1,
            rav.is_actual,
            rav.description
        from 
            (f_sel_rec_attrs_values(idRecord) rav inner join attrs_categories ac on (rav.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and rav.id_record = idRecord))
        where 
            rav.is_actual = true 
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
