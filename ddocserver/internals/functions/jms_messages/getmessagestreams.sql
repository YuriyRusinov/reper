create or replace function getMessageStreams () returns int4 as
$BODY$
declare
    r record;
    rr record;
    id_distrib int4;
    ctime timestamp;
    prev_time_step float8;
    time_step float8;
    tunit varchar;
    last_time timestamp;
    tquery varchar;
    tinterv interval;
    rand_cnt int4;
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
        loop
            select into ctime current_timestamp;
            if (r.start_time > ctime or ctime > r.stop_time) then
                continue;
            end if;
            id_distrib := r.id_partition_low;

            select into tunit name from time_units tu where tu.id=r.id_time_unit;

            for rr in
                select mser.time, mser.time_step 
                from message_series mser 
                where 
                    mser.id_message_stream = r.id 
                order by mser.time desc
                limit 1
                    --and mser.time = (select max(time) from message_series where id_message_stream = r.id)
            loop
                last_time := rr.time;
                prev_time_step := rr.time_step;
            end loop;

            if (last_time is null) then
                last_time := r.start_time;
            end if;

            if (prev_time_step is null) then
                time_step := generatetimestep(r.id); 
                if (time_step is null) then
                    raise warning 'Incorrect parameters';
                    return NULL;
                end if;
                prev_time_step := time_step;
                insert into message_series (id_message_stream, time, time_step) values (r.id, ctime, time_step);
                continue;
            end if;

            tquery := E'select ';
            tquery := tquery || E'interval \''|| to_char(prev_time_step, '0000000.99999999') || E' ' || tunit || E'\'';
            --raise exception '%', tquery;

            execute tquery into tinterv;
            --last_time := last_time + tinterv;
            raise warning 'last time is %, interval is %', last_time, tinterv;

            if (ctime < last_time+tinterv or ctime > r.stop_time) then
                continue;         
            end if;

            time_step := generatetimestep(r.id);
            if (time_step is null) then
                raise warning 'Incorrect parameters';
                return NULL;
            end if;

            insert into message_series (id_message_stream, time, time_step) values (r.id, ctime, time_step);

        end loop;

        return 1;
end
$BODY$
language 'plpgsql';
