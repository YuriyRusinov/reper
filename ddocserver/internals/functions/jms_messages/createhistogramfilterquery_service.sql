create or replace function createHistogramFilterQueryService(int4) returns varchar as
$BODY$
declare
    idServiceParams alias for $1;
    r record;
    q varchar;
    ioObjects int4[];
    ioCategories int4[];
begin


    for r in select scenarios, variants, io_categories, services from histogram_params_chains where id = idServiceParams
    loop

        q = 'select cd.id as id, cd.id_chain as id_chain, getSeconds(cd.end_service_time - cd.start_service_time) as time_step
             from 
                 chains_data cd,
                 chains c,
                 tbl_io_objects io
             where
                 cd.start_service_time is not null
                 and cd.end_service_time is not null
                 and c.id = cd.id_chain
                 and io.id = cd.id_io_object ';

        if(r.scenarios is not null and array_upper(r.scenarios, 1) > 0) then
            q = q || '
                 and cd.id_processing_scenario = ANY(' || asString(r.scenarios, true) || ') ';
        end if;
        if(r.variants is not null and array_upper(r.variants, 1) > 0) then
            q = q || '
                 and cd.id_processing_variant = ANY(' || asString(r.variants, true) || ') ';
        end if;
        if(r.io_categories is not null and array_upper(r.io_categories, 1) > 0) then 
            q = q || '
                 and io.id_io_category = ANY(' || asString(r.io_categories, true) || ') ';
        end if;
        if(r.services is not null and array_upper(r.services, 1) > 0) then 
            q = q || '
                 and c.id_handler = ANY(' || asString(r.services, true) || ') ';
        end if;
        
    end loop;


    return q;

end
$BODY$
LANGUAGE 'plpgsql' security definer;

select f_safe_drop_type('h_get_histogram_data_service');
create type h_get_histogram_data_service as(h_order int4, h_x float8, h_y float8);


create or replace function getHistogramDataForService(int4, int4) returns setof h_get_histogram_data_service as
$BODY$
declare
    idHistogramParams alias for $1;
    iCount alias for $2;
    
    r h_get_histogram_data_service%rowtype;
    q varchar;

    q_min varchar;
    q_max varchar;
    v_min float8;
    v_max float8;

    q_hist_data varchar;
begin

    q = createHistogramFilterQueryService(idHistogramParams);
    if(q isnull) then
        return;
    end if;

    execute 'create temp table t_hist_filtered_data_s as ' || q;
    
    q_min = 'select min(time_step) from t_hist_filtered_data_s';
    execute q_min into v_min;
    
    q_max = 'select max(time_step) from t_hist_filtered_data_s';
    execute q_max into v_max;

    if(v_min = v_max or iCount <= 0) then
        return;
    end if;


    q_hist_data = '
        select r.r_index, r.step, h.bin 
        from 
            getRanges(' || asString(v_min, false) || ', ' || asString(v_max, false) || ', ' || asString(iCount, false) || ') r, 
            histogram(' || quote_literal('select time_step from t_hist_filtered_data_s;') || ', ' || asString(v_min, false) || ', ' || asString(v_max, false) || ', ' || asString(iCount, false) || ') h
        where
            h.inum = r.r_index';

    for r in execute q_hist_data
    loop
        return next r;
    end loop;

    drop table t_hist_filtered_data_s;
    
    return;
end
$BODY$
language 'plpgsql';


create or replace function getMilliseconds(interval) returns float8 as
$BODY$
declare
    iInt alias for $1;
    mSecs float8;
begin

    SELECT 
        ((60*(EXTRACT(HOURS FROM iInt))
        +
        EXTRACT(MINUTES FROM iInt) ) * 60
        +
        EXTRACT(SECOND FROM iInt) ) * 1000
    into mSecs;

    return mSecs;
end
$BODY$
language 'plpgsql';

create or replace function getSeconds(interval) returns float8 as
$BODY$
declare
    iInt alias for $1;
    mSecs float8;
begin

    SELECT 
        ((60*(EXTRACT(HOURS FROM iInt))
        +
        EXTRACT(MINUTES FROM iInt) ) * 60
        +
        EXTRACT(SECOND FROM iInt) )
    into mSecs;

    return mSecs;
end
$BODY$
language 'plpgsql';

/*
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
*/
--select * from getHistogramData(3, 123);
/*
select r.r_index, r.step, h.inum, h.bin 
from 
    getRanges(0.11, 0.6, 10) r, 
    histogram('select t_step from aaa(3);;;;;;;;;;;;;;;;;;;;;;;;', 0.11, 0.6, 10) h
where
    h.inum = r.r_index
    */

    --select getMilliseconds(('12.01.2014 12:10:00.12'::timestamp - '12.01.2014 12:00:00.11'::timestamp)::interval )
/*
    SELECT 
        ((60*(EXTRACT(HOURS FROM ('12.01.2014 12:10:00.12'::timestamp - '12.01.2014 12:00:00.11'::timestamp)::interval))
        +
        EXTRACT(MINUTES FROM ('12.01.2014 12:10:00.12'::timestamp - '12.01.2014 12:00:00.11'::timestamp)::interval) ) * 60
        +
        EXTRACT(SECOND FROM ('12.01.2014 12:10:00.12'::timestamp - '12.01.2014 12:00:00.11'::timestamp)::interval) ) * 1000*/
        --+
        --select EXTRACT(MILLISECONDS FROM ('12.01.2014 12:10:00.12'::timestamp - '12.01.2014 12:00:00.11'::timestamp)::interval);


--select inum, bin from histogram('select t_step from aaa(3);;;;;;;;;;;;;;;;;;;;;;;;', 0.11, 0.6, 10);
--select max(t_step) from aaa(3)
--select min(t_step) from aaa(3)
--select t_step from aaa(3)
--select * from message_streams
--update message_streams set id_processing_variant = 1, id_processing_scenario = 1
--select createHistogramFilterQuery(3)
--select scenarios, variants, dl_froms, dl_tos, io_categories, io_objects, partition_lows from histogram_params_streams where id = 3

--select min(time_step) from message_series where id in (select m_s.id from (select m_ser.id, m_ser.time_step from message_series m_ser) as m_s)
--create temp table v as select * from aaa(3)
--select * from v
--select * from chains_data
--update chains_data set start_service_time = insert_time, end_service_time = handled_time

--select * from createHistogramFilterQueryService(5);
--select * from getHistogramDataForService(5, 10);