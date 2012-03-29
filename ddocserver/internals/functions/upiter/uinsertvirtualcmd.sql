--создать виртуальню команду на стороне отправителя (считатся, что на стороне получателя такая команда уже создана)
create or replace function uInsertVirtualOutCmd(int4, int4, int4, int4, timestamp, varchar, varchar, int4, int4) returns int4 as
$BODY$
declare
    dl_from_id alias for $1; --отправитель (ИД)
    dl_executor_id alias for $2; --исполнитель (ИД)
    dl_to_id alias for $3; --контроллер (ИД)
    id_category alias for $4;--категория, по которой представить ответ
    execDatetime alias for $5; --
    messageBody alias for $6;
    uniqueID alias for $7;
    idUrgencyLevel alias for $8;
    idObject alias for $9;

    idCommand int4;
begin


    select id into idCommand from command_journal where unique_id = uniqueID;
    if(idCommand is not null) then
        return idCommand;
    end if;

    select getNextSeq('command_journal', 'id') into idCommand;

    insert into command_journal (id, 
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
                                 is_archived,
                                 extra_id,
                                 unique_id,
                                 output_number,
                                 id_urgency_level,
                                 id_io_object_att)
                        values
                                (idCommand,
                                 dl_from_id,
                                 dl_executor_id,
                                 dl_to_id,
                                 id_category,
                                 1,
                                 current_timestamp,
                                 true,
                                 execDatetime,
                                 NULL,
                                 messageBody,
                                 false,
                                 NULL,
                                 uniqueID,
                                 NULL,
                                 idUrgencyLevel,
                                 idObject
                                );

    if(FOUND = FALSE) then
        raise notice 'Command does not inserted in command_journal!';
        return -1;
    end if;

    return idCommand;
end
$BODY$
language 'plpgsql';
