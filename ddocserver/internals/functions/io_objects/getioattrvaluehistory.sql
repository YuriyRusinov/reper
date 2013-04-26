/*
--определяется в файле iogetobjectattrs.sql

select f_safe_drop_type('h_get_object_attrs');
create type h_get_object_attrs as(
                                  id_io_object int4,
                                  id_io_category int4,
                                  id_io_attribute int4,
                                  value varchar,
                                  attr_code varchar,  --при информационном обмене (в ф-и ioGetObjectAttrsEx() в качестве значения данного поля используется unique_id)
                                  id_attr_type int4,
                                  id_attr_category int4,
                                  id_attr_value int4, 
                                  start_time timestamp,
                                  stop_time timestamp,
                                  insert_time timestamp,
                                  id_io_object_src int4,
                                  id_io_object_src1 int4,
                                  is_actual boolean,
                                  description varchar);
*/

create or replace function getIOAttrValueHistory(int4, timestamp, timestamp) returns setof h_get_object_attrs as
$BODY$
declare
    idAttrValue alias for $1;
    startTime alias for $2;
    stopTime alias for $3;

    r h_get_object_attrs%rowtype;
    rr record;

    iStartTime timestamp;
    iStopTime timestamp;
    idObject int4;
    idAttrCategory int4;
begin

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


    for rr in
        select id_io_object, id_attr_category from getIdObjectForIdAttrValue(idAttrValue)
    loop
        idObject := rr.id_io_object;
        idAttrCategory := rr.id_attr_category;
    end loop;

    for r in 
        select 
            av.id_io_object, 
            ac.id_io_category, 
            ac.id_io_attribute, 
            (case when session_user = 'jupiter' then getUniqueValue(av.value, a.id_a_type, a.table_name) else av.value end ) as value,
            a.code,
            a.id_a_type,
            av.id_attr_category,
            av.id,
            av.start_time,
            av.stop_time,
            av.insert_time,
            av.id_io_object_src,
            av.id_io_object_src1,
            av.is_actual,
            av.description
        from 
            (f_sel_attrs_values(idObject) av inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and av.id_io_object = idObject))
        where 
            av.start_time >= iStartTime 
            and (av.stop_time isnull or av.stop_time <= iStopTime)
            and av.id_attr_category = idAttrCategory
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';


select f_safe_drop_type('h_get_id_obj_id_cat_attr');
create type h_get_id_obj_id_cat_attr as(
                                  id_io_object int4,
                                  id_attr_category int4);

--функция нужна для ускорения получения данной информации. Чтобы читать с правами admin'a
create or replace function getIdObjectForIdAttrValue(int4) returns h_get_id_obj_id_cat_attr as
$BODY$
declare
    idAttrValue alias for $1;
    

    r h_get_id_obj_id_cat_attr%rowtype;
begin

    for r in 
        select id_io_object, id_attr_category from tbl_attrs_values where id = idAttrValue
    loop
        return r;
    end loop;

    return r;

end
$BODY$
language 'plpgsql' security definer;
