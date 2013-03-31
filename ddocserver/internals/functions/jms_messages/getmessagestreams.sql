create or replace function getmessagestreams () returns int4 as
$BODY$
declare
    r record;
    id_distrib int4;
    ctime timestamp;
    time_step float8;
    tunit varchar;
    last_time timestamp;
    tquery varchar;
    tinterv interval;
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
            perform initrand();
            if (id_distrib = 1) then
                select into time_step r.moda+gaussrand(r.sigma);
            elsif (id_distrib = 2) then
                select into time_step exprand (r.lambda);
            elsif (id_distrib = 3) then
                select into time_step new.min_p + (r.max_p-r.min_p)*unirand ();
            else
                raise warning 'Does not have any another distributions';
                select droprand();
                continue;
            end if;

            perform droprand();
            if (time_step is null) then
                raise warning 'Incorrect parameters';
                return NULL;
            end if;

            select into tunit name from time_units tu where tu.id=r.id_time_unit;
            select into last_time time from message_series mser where mser.id_message_stream=new.id and mser.time = (select max(time) from message_series where id_message_stream=r.id);
            if (last_time is null) then
                last_time := new.start_time;
            end if;
            tquery := E'select ';
            tquery := tquery || E'interval \''|| time_step || E' ' || tunit || E'\'';
            execute tquery into tinterv;
            last_time := last_time + tinterv;
            raise warning 'last time is %', last_time;
            insert into message_series (id_message_stream, time) values (r.id, last_time);

        end loop;
end
$BODY$
language 'plpgsql';
