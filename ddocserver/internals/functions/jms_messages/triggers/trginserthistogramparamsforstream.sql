create or replace function insertHistogramParamsForStream() returns trigger as
$BODY$
declare
    q varchar;
    --iCount int4;
    m float8;
begin

    if((new.h_min is not null and new.h_max is not null and new.h_min >= new.h_max) or new.h_count <= 0)  then
        raise exception 'Incorrect Params for histogram!';
        return NULL;
    end if;

    delete from histogram_graphics_streams where id_histogram_params_streams = new.id;
    insert into histogram_graphics_streams (id_histogram_params_streams, h_order, h_x, h_y) select new.id, d.h_order, d.h_x, d.h_y from getHistogramData(new.id, new.h_count) d;
    if(new.h_min isnull) then
        select min(g.h_x) into m from histogram_graphics_streams g where g.id_histogram_params_streams = new.id;
        update histogram_params_streams set h_min = m where id = new.id;
    end if;
    if(new.h_max isnull) then
        select max(g.h_x) into m from histogram_graphics_streams g where g.id_histogram_params_streams = new.id;
        update histogram_params_streams set h_max = m where id = new.id;
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginserthistogramparamsforstream', 'histogram_params_streams');

select f_create_trigger('trginserthistogramparamsforstream', 'after', 'insert or update', 'histogram_params_streams', 'inserthistogramparamsforstream()');

