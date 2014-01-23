create or replace function createHistogramFilterQuery(int4) returns varchar as
$BODY$
declare
    idStreamParams alias for $1;
    r record;
    q varchar;
    ioObjects int4[];
    ioCategories int4[];
begin


    for r in select scenarios, variants, io_categories, io_objects, partition_lows from histogram_params_streams where id = idStreamParams
    loop

        q = 'select m_ser.id as id_seria, m_str.id as id_stream, m_ser.time_step
             from 
                 message_streams m_str,
                 message_series m_ser
             where
                 m_str.id = m_ser.id_message_stream ';

        if(r.partition_lows is not null and array_upper(r.partition_lows, 1) > 0) then
            q = q || '
                 and m_str.id_partition_low = ANY(' || asString(r.partition_lows, true) || ') ';
        end if;
        if(r.scenarios is not null and array_upper(r.scenarios, 1) > 0) then
            q = q || '
                 and m_str.id_processing_scenario = ANY(' || asString(r.scenarios, true) || ') ';
        end if;
        if(r.variants is not null and array_upper(r.variants, 1) > 0) then
            q = q || '
                 and m_str.id_processing_variant = ANY(' || asString(r.variants, true) || ') ';
        end if;

        if(r.io_objects is not null and array_upper(r.io_objects, 1) > 0) then 
            --in that case io_categories will be ignored
            q = q || '
                 and m_str.id_io_object = ANY(' || asString(r.io_objects, true) || ') ';
        else
            --in that case io_objects will be ignored
            if(r.io_categories is not null and array_upper(r.io_categories, 1) > 0) then 
                select array_agg(id) into ioObjects from io_objects where id_io_category = ANY(r.io_categories);
                if(ioObjects is not null and array_upper(ioObjects, 1) > 0) then
                    q = q || '
                        and m_str.id_io_object = ANY(' || asString(ioObjects, true) || ') ';
                end if;
            end if;
        end if;
        
    end loop;


    return q;

end
$BODY$
LANGUAGE 'plpgsql';

select f_safe_drop_type('h_get_histogram_data');
create type h_get_histogram_data as(h_order int4, h_x float8, h_y float8);


create or replace function getHistogramData(int4, int4) returns setof h_get_histogram_data as
$BODY$
declare
    idHistogramParams alias for $1;
    iCount alias for $2;
    
    r h_get_histogram_data%rowtype;
    q varchar;

    q_min varchar;
    q_max varchar;
    v_min float8;
    v_max float8;

    q_hist_data varchar;
begin

    q = createHistogramFilterQuery(idHistogramParams);    
    if(q isnull) then
        return;
    end if;

    execute 'create temp table t_hist_filtered_data as ' || q;
    
    q_min = 'select min(time_step) from t_hist_filtered_data';
    execute q_min into v_min;
    
    q_max = 'select max(time_step) from t_hist_filtered_data';
    execute q_max into v_max;

    if(v_min = v_max or iCount <= 0) then
        return;
    end if;


    q_hist_data = '
        select r.r_index, r.step, h.bin 
        from 
            getRanges(' || asString(v_min, false) || ', ' || asString(v_max, false) || ', ' || asString(iCount, false) || ') r, 
            histogram(' || quote_literal('select time_step from t_hist_filtered_data;') || ', ' || asString(v_min, false) || ', ' || asString(v_max, false) || ', ' || asString(iCount, false) || ') h
        where
            h.inum = r.r_index';

    for r in execute q_hist_data
    loop
        return next r;
    end loop;

    drop table t_hist_filtered_data;
    
    return;
end
$BODY$
language 'plpgsql';


select f_safe_drop_type('h_get_ranges');
create type h_get_ranges as(r_index int4, step float8);

create or replace function getRanges(float8, float8, int4) returns setof h_get_ranges as
$BODY$
declare
    v_min alias for $1;
    v_max alias for $2;
    v_count alias for $3;

    r h_get_ranges%rowtype;

    i int4;
    step float8;
begin

    if(v_count <= 0 or v_min = v_max) then
        raise exception 'Incorrect input data in getRanges() function!';
        return;
    end if;
    
    for i in 1..v_count
    loop
        step = ((v_max-v_min)/v_count)*(i);
        r.r_index = i-1; --counting from 0
        r.step = step; 
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
