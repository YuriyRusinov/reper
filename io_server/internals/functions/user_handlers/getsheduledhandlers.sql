select f_safe_drop_type('h_sheduled_handlers');
create type h_sheduled_handlers as(id_shedule int4,
                                   id_handler int4,
                                   name varchar,
                                   service varchar,
                                   h_host varchar,
                                   h_port int4,
                                   extra_params varchar,
                                   in_data varchar,
                                   is_external boolean);

create or replace function getSheduledHandlers() returns setof h_sheduled_handlers as
$BODY$
declare
    r h_sheduled_handlers%rowtype;
begin

    for r in 
        select 
            sh.id,
            hh.id,
            hh.name,
            hh.service,
            hh.h_host,
            hh.h_port,
            hh.extra_params,
            hh.handler_in_data,
            h.is_external
        from
            handler_params hh,
            handlers h,
            sheduled_handlers sh
        where
            hh.id = h.id_handler_params
            and h.id = sh.id_handler
            and 
                (    
                    sh.last_exec isnull
                    or (sh.last_exec + arrayToInterval(sh.exec_period) <= current_timestamp)
                )
    loop
        update sheduled_handlers set last_exec = clock_timestamp() where id = r.id_shedule;
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function arrayToInterval(int4[]) returns interval as
$BODY$
declare
    arr alias for $1;
    hInterval interval;
    what varchar;
    amount int4;
    val int4;
    q varchar;
    cnt int4;
begin

    cnt = array_upper(arr, 1);
    if(cnt <> 2) then
        return NULL;
    end if;

    amount = arr[1];
    val = arr[2];

    if(val = 1) then
        what = 'years';
    elsif(val = 2) then
        what = 'months';
        amount = amount * 6;
    elsif(val = 3) then
        what = 'months';
        amount = amount * 3;
    elsif(val = 4) then
        what = 'months';
    elsif(val = 5) then
        what = 'weeks';
    elsif(val = 6) then
        what = 'days';
    elsif(val = 7) then
        what = 'hours';
    elsif(val = 8) then
        what = 'minutes';
    elsif(val = 9) then
        what = 'seconds';
    else
        raise exception 'incorrect data in interval type!';
    end if;   

    q = 'select ' || quote_literal(amount || ' ' || what) || '::interval;';     
    execute q into hInterval;     

    return hInterval;
    

end
$BODY$
language 'plpgsql';
