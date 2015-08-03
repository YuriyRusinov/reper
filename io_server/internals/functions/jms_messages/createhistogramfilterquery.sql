drop function if exists createHistogramFilterQuery(int4);
create or replace function createHistogramFilterQuery(int4[], int4[], int4[], int4[], int4[]) returns varchar as
$BODY$
declare
    scenarios alias for $1;
    variants alias for $2;
    io_categories alias for $3;
    io_objects alias for $4;
    part_lows alias for $5;
    
    --r record;
    q varchar;
    ioObjects int4[];
    --ioCategories int4[];
begin


    --for r in select scenarios, variants, io_categories, io_objects, partition_lows from histogram_params_streams where id = idStreamParams
    --loop

    q = 'select m_ser.id as id_seria, m_str.id as id_stream, m_ser.time_step
         from 
             message_streams m_str,
             message_series m_ser
         where
             m_str.id = m_ser.id_message_stream ';

    if(part_lows is not null and array_upper(part_lows, 1) > 0) then
        q = q || '
             and m_str.id_partition_low = ANY(' || asString(part_lows, true) || ') ';
    end if;

    if(scenarios is not null and array_upper(scenarios, 1) > 0) then
        q = q || '
             and m_str.id_processing_scenario = ANY(' || asString(scenarios, true) || ') ';
    end if;

    if(variants is not null and array_upper(variants, 1) > 0) then
        q = q || '
             and m_str.id_processing_variant = ANY(' || asString(variants, true) || ') ';
    end if;

    if(io_objects is not null and array_upper(io_objects, 1) > 0) then 
        --in that case io_categories will be ignored
        q = q || '
             and m_str.id_io_object = ANY(' || asString(io_objects, true) || ') ';
    else
        --in that case io_objects will be ignored
        if(io_categories is not null and array_upper(io_categories, 1) > 0) then 
            select array_agg(id) into ioObjects from io_objects where id_io_category = ANY(io_categories);
            if(ioObjects is not null and array_upper(ioObjects, 1) > 0) then
                q = q || '
                    and m_str.id_io_object = ANY(' || asString(ioObjects, true) || ') ';
            end if;
        end if;
    end if;
        
    return q;

end
$BODY$
LANGUAGE 'plpgsql';

