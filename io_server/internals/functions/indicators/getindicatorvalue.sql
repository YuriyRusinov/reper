--тип возвращаемых данных описан в iogetindicatorsvalues.sql

create or replace function getIndicatorValue(int4) returns setof h_get_indicator_value as
$BODY$
declare
    idIndicatorValue alias for $1;
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
            and iv.id = idIndicatorValue
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
