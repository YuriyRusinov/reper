create or replace function generatetimestep (int8) returns float8 as
$BODY$
declare
    id_message_stream alias for $1;

    r record;
    time_step float8;
    rand_cnt int8;
    id_distrib int8;
begin
    for r in
        select id,
               id_partition_low,
               id_io_object,
               lambda,
               sigma,
               moda,
               min_p,
               max_p,
               id_time_unit,
               start_time,
               stop_time
            from
                message_streams
            where
                id = id_message_stream
    loop
        id_distrib := r.id_partition_low;
        select count(*) into rand_cnt from rand_state;
        if (rand_cnt = 0) then
            perform initrand(0);
            perform saverand();
        end if;

        perform loadrand();

        if (id_distrib = 1) then
            select into time_step r.moda+gaussrand(r.sigma);
        elsif (id_distrib = 2) then
            select into time_step exprand (r.lambda);
        elsif (id_distrib = 3) then
            select into time_step r.min_p + (r.max_p-r.min_p)*unirand ();
        else
            raise warning 'Does not have any another distributions';
            select droprand();
            continue;
        end if;

        perform saverand();

        perform droprand();

        if (time_step is null) then
            raise warning 'Incorrect parameters';
            return NULL;
        end if;
    end loop;

    return time_step;
end
$BODY$
language 'plpgsql';
