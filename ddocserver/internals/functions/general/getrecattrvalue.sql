create or replace function getRecAttrValue(int8) returns setof h_eio_get_indicators as
$BODY$
declare
    idAttrValue alias for $1;

    r h_eio_get_indicators%rowtype;
    rr record;

    idRecord int8;
    idAttrCategory int8;
begin
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
            (case when session_user = 'jupiter' then getUniqueValue(av.value, a.id_a_type, a.table_name) else case when a.id_a_type = 2 or a.id_a_type = 3 or a.id_a_type=19 or a.id_a_type=26 or a.id_a_type = 39 then aGetRefValue (a.id, a.table_name, av.value) else av.value end end ) as value,
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
            (f_sel_rec_attrs_values(idRecord) av 
                inner join attrs_categories ac on (av.id_attr_category = ac.id) 
                inner join attributes a on (ac.id_io_attribute=a.id and av.id_record = idRecord and av.id_attr_category = idAttrCategory and av.id=idAttrValue)
            )
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
