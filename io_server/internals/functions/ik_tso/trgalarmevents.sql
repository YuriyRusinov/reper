create or replace function trgInsertAlarmEvent() returns trigger as
$BODY$
declare
    maxLevel int4;
    idCommand int4;
    theTable varchar;
begin
    maxLevel = getTSOMaxLevel();
    if(maxLevel isnull) then
        return new;
    end if;
    
    theTable = TG_RELNAME;
    if(substr(theTable, 1, 4) = 'tbl_') then
        theTable = substr(theTable, 5);
    end if;

    if(maxLevel is not null and new.alarm < maxLevel) then
        return new;
    end if;

    select getNextSeq('command_journal', 'id') into idCommand;
    insert into command_journal (id, 
                                 id_dl_from, 
                                 id_dl_executor, 
                                 id_dl_to, 
                                 id_io_category, 
                                 id_jr_state, 
                                 insert_time, 
                                 exec_datetime,
                                 exec_period,
                                 message_body,
                                 is_archived,
                                 id_urgency_level,
                                 extra_table,
                                 extra_table_id)
                        values
                                (idCommand,
                                 getTSOFromDl(),
                                 getTSOExecutorDl(),
                                 getTSOToDl(),
                                 getTSOCommandCategory(),
                                 7, --virtual
                                 current_timestamp,
                                 getTSOExecDatetime(),
                                 NULL,
                                 E'Появилось тревожное событие с высоким уровнем тревоги.\nИсточник: ' || new."source" || '. Событие: ' || new.message,
                                 false,
                                 1,
                                 theTable,
                                 new.id
                                );

    if(FOUND = FALSE) then
        raise exception 'Command does not inserted in command_journal!';
        return NULL;
    end if;

    perform setAlarmEventsState(2); --появилось тревожное событие

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgalarmevents', 'alarm_events_q');
create trigger trgAlarmEvents
after insert
on alarm_events_q
for each row
execute procedure trgInsertAlarmEvent();

