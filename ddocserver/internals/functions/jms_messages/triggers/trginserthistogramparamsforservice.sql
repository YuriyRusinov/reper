create or replace function insertHistogramParamsForService() returns trigger as
$BODY$
declare
    q varchar;
    --iCount int4;
begin

    if((new.h_min is not null and new.h_max is not null and new.h_min >= new.h_max) or new.h_count <= 0)  then
        raise exception 'Incorrect Params for histogram for service!';
        return NULL;
    end if;

    delete from histogram_graphics_chains where id_histogram_params_chains = new.id;
    insert into histogram_graphics_chains (id_histogram_params_chains, h_order, h_x, h_y) select new.id, d.h_order, d.h_x, d.h_y from getHistogramDataForService(new.id, new.h_count) d;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginserthistogramparamsforservice', 'histogram_params_chains');

select f_create_trigger('trginserthistogramparamsforservice', 'after', 'insert or update', 'histogram_params_chains', 'inserthistogramparamsforservice()');

