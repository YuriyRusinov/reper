select f_safe_drop_type('h_get_indicator_value');

create type h_get_indicator_value as( id_indicator int4, 
                                      ind_name varchar,
                                      ind_desc varchar,
                                      id_indicator_type int4,
                                      ind_type_name varchar,
                                      id_indicator_parent int4,
                                      id_a_type int4,
                                      attr_type_name varchar,
                                      attr_type_code varchar,
                                      id_indicator_value int4,
                                      id_io_object int4,
                                      id_io_object_src int4,
                                      id_io_object_src1 int4,
                                      meas_time timestamp,
                                      insert_time timestamp,
                                      the_value varchar,
                                      start_time timestamp,
                                      stop_time timestamp,
                                      id_a_view int4,
                                      a_view_name varchar,
                                      ind_table_name varchar,
                                      ind_column_name varchar,
                                      id_ref_attr_type int4);

create or replace function ioGetIndicatorsValues(int4) returns setof h_get_indicator_value as
$BODY$
declare
    idObject alias for $1;
    r h_get_indicator_value%rowtype;
begin

    for r in 

        select 
            i.id,
            i.name,
            i.description,
            i.id_indicator_type,
            it.name,
            i.id_parent,
            i.id_a_type,
            a.name,
            a.code,
            iv.id,
            iv.id_io_object,
            iv.id_io_object_src,
            iv.id_io_object_src1,
            iv.meas_time,
            iv.insert_time,
            iv.the_value,
            iv.start_time,
            iv.stop_time,
            av.id,
            av.name, 
            i.table_name,
            i.column_name,
            i.id_ref_attr_type
        from 
            indicator i,
            indicators_values iv,
            indicator_type it,
            a_types a,
            a_views av

        where 
            i.id = iv.id_indicator
            and iv.id_io_object = idObject
            and i.id_indicator_type = it.id
            and i.id_a_type = a.id
            and a.id_a_view = av.id
            
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';
