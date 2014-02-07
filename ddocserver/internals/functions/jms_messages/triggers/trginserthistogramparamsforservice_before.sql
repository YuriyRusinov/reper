create or replace function insertHistogramParamsForServiceBefore() returns trigger as
$BODY$
declare
    q varchar;

    q_min varchar;
    q_max varchar;
    v_min float8;
    v_max float8;

    q_hist_data varchar;
begin

    q = createHistogramFilterQueryService(new.scenarios, new.variants, new.io_categories, new.services);    
    if(q isnull) then
        return new;
    end if;

    execute 'create temp table t_hist_filtered_data_s as ' || q;
    --raise exception '%', q;
    
    q_min = 'select min(time_step) from t_hist_filtered_data_s';
    execute q_min into v_min;
    
    q_max = 'select max(time_step) from t_hist_filtered_data_s';
    execute q_max into v_max;

    if(v_min = v_max or new.h_count <= 0) then
        return new;
    end if;

    new.h_min = v_min - 0.0001;
    new.h_max = v_max + 0.0001;
    
    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginserthistogramparamsforservice_before', 'histogram_params_chains');

select f_create_trigger('trginserthistogramparamsforservice_before', 'before', 'insert or update', 'histogram_params_chains', 'inserthistogramparamsforservicebefore()');
