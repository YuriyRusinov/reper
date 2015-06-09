/*
--определяется в файле eiogetindicators.sql

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
                                  description varchar,
                                  attr_name varchar,
                                  attr_order int4,
                                  attr_directives varchar,
                                  attr_def_value varchar,
                                  attr_is_mandatory bool,
                                  attr_is_read_only bool,
                                  attr_def_width int4,
                                  id_a_view int4,
                                  a_type_code varchar,
                                  a_type_name varchar,
                                  attr_title varchar,
                                  displayed_value varchar
                                  );
*/

create or replace function getRecAttrValueHistory(int8, timestamp, timestamp) returns setof h_eio_get_indicators as
$BODY$
declare
    idAttrValue alias for $1;
    startTime alias for $2;
    stopTime alias for $3;

    r h_eio_get_indicators%rowtype;
    rr record;

    iStartTime timestamp;
    iStopTime timestamp;
    idRecord int4;
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
        select id_record, id_attr_category from getIdRecordForIdAttrValue(idAttrValue)
    loop
        idRecord := rr.id_record;
        idAttrCategory := rr.id_attr_category;
    end loop;

    for r in 
        select 
            av.id_record, 
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
            av.description,
            a.name,
            ac."order",
            ac.directives,
            ac.def_value,
            ac.is_mandatory,
            ac.is_read_only,
            a.def_width,
            att.id_a_view,
            att.code as a_type_code,
            att.name as a_type_name,
            a.title,
            ioGetAttrValueEx(av.value, a.id_a_type, a.table_name, idRecord, a.column_name) as displayed_value
        from 
            f_sel_rec_attrs_values(idRecord::int8) av 
            inner join attrs_categories ac on (av.id_attr_category = ac.id) 
            inner join attributes a on (ac.id_io_attribute=a.id and av.id_record = idRecord)
            inner join a_types att on (a.id_a_type = att.id)
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


select f_safe_drop_type('h_get_id_rec_id_cat_attr');
create type h_get_id_rec_id_cat_attr as(
                                  id_record int8,
                                  id_attr_category int4);

--функция нужна для ускорения получения данной информации. Чтобы читать с правами admin'a
create or replace function getIdRecordForIdAttrValue(int8) returns h_get_id_rec_id_cat_attr as
$BODY$
declare
    idAttrValue alias for $1;
    

    r h_get_id_rec_id_cat_attr%rowtype;
begin

    for r in 
        select id_record, id_attr_category from tbl_rec_attrs_values where id = idAttrValue
    loop
        return r;
    end loop;

    return r;

end
$BODY$
language 'plpgsql' security definer;
