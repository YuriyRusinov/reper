create or replace function messageCheck () returns trigger as
$BODY$
declare
    id_distrib int4;

    ctime timestamp;
    new_time_step float8;
    tunit varchar;
    last_time timestamp;
    r record;
    tquery varchar;
    tinterv interval;
    cnt int4;
begin

    id_distrib := new.id_partition_low;
    if (id_distrib is null ) then
        raise warning 'Incorrect distribution';
        return NULL;
    end if;

    --perform loadrand();

    if (id_distrib = 1) then
        if (new.moda - 3*new.sigma < 0.001) then
            raise EXCEPTION 'Incorrect values for gaussian distribution time step';
            return NULL;
        end if;
--        select into new_time_step new.moda+gaussrand(new.sigma);
    elsif (id_distrib = 2) then
        if (new.lambda < 0.0) then
            raise EXCEPTION 'Incorrect value for exponential distribution';
            return NULL;
        end if;
--        select into new_time_step exprand (new.lambda);
    elsif (id_distrib = 3) then
        if (new.min_p <= 0 or new.max_p <= 0 or new.max_p < new.min_p) then
            raise EXCEPTION 'Incorrect parameters for uniform distribution';
            return NULL;
        end if;
--        select into new_time_step new.min_p + (new.max_p-new.min_p)*unirand ();
    else
        raise warning 'Does not have any another distributions';
        select droprand();
        return NULL;
    end if;
    new_time_step := generatetimestep (new.id);

    --perform droprand();

    if (new_time_step is null) then
        raise warning 'Incorrect parameters';
        return NULL;
    end if;

    select into tunit name from time_units tu where tu.id=new.id_time_unit;
    select into last_time time from message_series mser where mser.id_message_stream=new.id and mser.time = (select max(time) from message_series where id_message_stream=new.id);

    if (last_time is null) then
        last_time := new.start_time;
    end if;

    tquery := E'select ';
    tquery := tquery || E'interval \''|| new_time_step || E' ' || tunit || E'\'';

    execute tquery into tinterv;
    --last_time := last_time + tinterv;
    raise warning 'last time is %', last_time;

    select into ctime current_timestamp;
    select into cnt count(*) from message_series mser where mser.id_message_stream=new.id;

    if (ctime >= new.start_time and cnt = 0 or
        ctime >= last_time-tinterv/2 and ctime <= last_time+tinterv/2) then
        insert into message_series (id_message_stream, time, time_step) values (new.id, new.start_time, new_time_step);
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger ('trgmessagecheck', 'message_streams');

create trigger trgmessagecheck
after insert or update
on message_streams
for each row
execute procedure messageCheck ();
