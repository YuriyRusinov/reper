create or replace function cmdAnalyzeJournal() returns int4 as
$BODY$
declare
    r record;
    cDate timestamp;
    newMsgBody varchar;
    newExecDatetime timestamp;
    format text;
begin

    cDate = current_timestamp;

    format = 'DD.MM.YYYY HH24:MI';
    
    for r in 
        select 
            id, 
            id_dl_from,
            id_dl_executor,
            id_dl_to,
            id_io_category,
            id_jr_state,
            insert_time, 
            is_automated,
            exec_datetime, 
            exec_period, 
            message_body,
            id_tabel,
            last_exec_datetime
        from 
            command_journal 
        where
            is_archived = false
            and exec_period is not null
            and extra_id isnull --only local commands, not from remote object
            and last_exec_datetime < cDate
            and id_jr_state <> 7 --exclude virtual commands 
                                 --(виртуальной является команда, для которой есть запись в журнале получателя, 
                                 --но нет записи в журнале отправителя. 
                                 --Такое возможно при информационном обмене взаимодействующих организаций
                                 --при автоматической (неявной) выдаче команд)
    loop
        --insert new command without periodical

        newMsgBody := E'Периодическое распоряжение с периодом ' || getPeriodName(r.exec_period) || E', \nвыданное ' || to_char(r.insert_time, format) || E'.\n' || r.message_body;
        newExecDatetime := r.last_exec_datetime + asInterval(r.exec_period);
        
        insert into command_journal (id_dl_from,
                                     id_dl_executor,
                                     id_dl_to,
                                     id_io_category,
                                     id_jr_state,
                                     insert_time, 
                                     is_automated,
                                     exec_datetime, 
                                     exec_period, 
                                     message_body,
                                     id_tabel)
                             values( r.id_dl_from,
                                     r.id_dl_executor,
                                     r.id_dl_to,
                                     r.id_io_category,
                                     2, --job outed
                                     r.insert_time, 
                                     r.is_automated,
                                     newExecDatetime, 
                                     NULL, 
                                     newMsgBody,
                                     r.id_tabel);

        update command_journal set last_exec_datetime = newExecDatetime where id = r.id;
    end loop;

    --переводим просроченные распоряжения в состояние "просроченные"

    update command_journal set id_jr_state = 5 where exec_datetime <= cDate and id_jr_state in (1, 2, 3);

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function getPeriodName(int4[2]) returns varchar as
$BODY$
declare
    exec_period alias for $1;
    val varchar;
    amount int4;
    what varchar;
    iWhat int4;
begin

    amount := exec_period[1];
    iWhat = exec_period[2];

    what = 'дней';

    if(iWhat = 1) then
        what = 'лет';
    end if;

    if(iWhat = 2) then
        what = 'полугодий';
    end if;
    
    if(iWhat = 3) then
        what = 'кварталов';
    end if;

    if(iWhat = 4) then
        what = 'месяцев';
    end if;

    if(iWhat = 5) then
        what = 'недель';
    end if;

    if(iWhat = 6) then
        what = 'дней';
    end if;

    if(iWhat = 7) then
        what = 'часов';
    end if;

    val := '' || amount || ' ' || what;
    
    return val;
end
$BODY$
language 'plpgsql';


create or replace function asInterval(int4[2]) returns interval as
$BODY$
declare
    exec_period alias for $1;
    amount int4;
    what varchar;
    iWhat int4;
    query varchar;
    r record;

    theInterval interval;
begin

    amount = exec_period[1];
    iWhat = exec_period[2];

    what = 'day';
    
    if(iWhat = 1) then
        what = 'year';
    end if;

    if(iWhat = 2) then
        amount = amount * 6;
        what = 'month';
    end if;
    
    if(iWhat = 3) then
        amount = amount * 3;
        what = 'month';
    end if;

    if(iWhat = 4) then
        what = 'month';
    end if;

    if(iWhat = 5) then
        what = 'week';
    end if;

    if(iWhat = 6) then
        what = 'day';
    end if;

    if(iWhat = 7) then
        what = 'hour';
    end if;
    
    query = '' || amount || ' ' || what;
    query = 'select ' || quote_literal(query) || '::interval as res';
    for r in execute query 
    loop
        theInterval := r.res;
    end loop;
    
    return theInterval;

end
$BODY$
language 'plpgsql';

