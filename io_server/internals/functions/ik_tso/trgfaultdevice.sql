create or replace function trgInsertFaultDevice() returns trigger as
$BODY$
declare
    idCommand int4;
    commandCategory int4;
    cnt int4;
begin
    if(TG_OP = 'UPDATE') then
        if(new.is_deleted = true or new.is_deleted = old.is_deleted) then
            return new;
        end if;
    end if;


    commandCategory = getTSOCommandCategory();
    if(commandCategory isnull) then
        return new;
    end if;


    select count(*) into cnt from command_journal where extra_table = TG_RELNAME and extra_table_id = new.id and id_jr_state <> 4;
    if(cnt <> 0) then
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
                                 E'Обнаружено неисправное устройство.\n' || new.dev_name || '. Состояние: ' || new.fault_state_value,
                                 false,
                                 1,
                                 TG_RELNAME,
                                 new.id
                                );

    if(FOUND = FALSE) then
        raise exception 'Command does not inserted in command_journal!';
        return NULL;
    end if;

    perform setFaultDevicesState(2);

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgfaultdevice', 'fault_devices_q');
create trigger trgFaultDevice
after insert or update
on fault_devices_q
for each row
execute procedure trgInsertFaultDevice();

