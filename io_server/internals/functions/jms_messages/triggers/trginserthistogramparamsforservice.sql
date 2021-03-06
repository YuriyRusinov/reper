create or replace function insertHistogramParamsForServiceAfter() returns trigger as
$BODY$
declare
    q varchar;
    --iCount int4;
    m float8;
    q_hist_data varchar;
begin

    if((new.h_min is null or new.h_max is null or new.h_min > new.h_max) or new.h_count <= 0)  then
        drop table t_hist_filtered_data_s;
        raise exception 'Incorrect Params for histogram!';
        return NULL;
    end if;

    delete from histogram_graphics_chains where id_histogram_params_chains = new.id;

    q_hist_data = '
        insert into histogram_graphics_chains (id_histogram_params_chains, h_order, h_x, h_y) 
        select ' || new.id || ', r.r_index, r.step, h.bin 
        from 
            getRanges(' || asString(new.h_min, false) || ', ' || asString(new.h_max, false) || ', ' || asString(new.h_count, false) || ') r, 
            histogram(' || quote_literal('select time_step from t_hist_filtered_data_s;') || ', ' || asString(new.h_min, false) || ', ' || asString(new.h_max, false) || ', ' || asString(new.h_count, false) || ') h
        where
            h.inum = r.r_index';

    execute q_hist_data;
    --raise exception '%', q_hist_data;
                                                                         
    drop table t_hist_filtered_data_s;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginserthistogramparamsforservice', 'histogram_params_chains');
select f_safe_drop_trigger('trginserthistogramparamsforservice_after', 'histogram_params_chains');

select f_create_trigger('trginserthistogramparamsforservice_after', 'after', 'insert or update', 'histogram_params_chains', 'inserthistogramparamsforserviceafter()');
