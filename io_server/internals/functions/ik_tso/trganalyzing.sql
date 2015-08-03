--используется для проверки наличия необработанных собщений о тревожных ситуациях
create or replace function trgTSOAnalyzeCmdCheck() returns trigger as
$BODY$
declare

    cnt int4;

begin

    if( not (old.id_jr_state <> 4 and new.id_jr_state = 4) ) then
        return new;
    end if;

    if(new.extra_table is null or new.extra_table <> 'alarm_events_q') then
        return new;
    end if;

    select count(*) into cnt
    from command_journal cmd
    where
        cmd.id_jr_state <> 4 
        and cmd.extra_table is not null
        and cmd.extra_table = 'alarm_events_q'
        and cmd.extra_table_id is not null;

    update alarm_events_q set is_deleted = true where id = new.extra_table_id;

    if(cnt <> 0) then
        return new;
    end if;

    perform setAlarmEventsState(1);



    return new;
end
$BODY$
language 'plpgsql';


select f_safe_drop_trigger('trgtsoanalyzecmd', 'command_journal');

select f_create_trigger('trgtsoanalyzecmd', 'after', 'update', 'command_journal', 'trgtsoanalyzecmdcheck()');

/*
create trigger trgTSOAnalyzeCmd
after update
on command_journal
for each row
execute procedure trgTSOAnalyzeCmdCheck();
*/

--используется для проверки наличия необработанных собщений о неисправных устройствах
create or replace function trgTSOAnalyzeCmdCheckFault() returns trigger as
$BODY$
declare

    cnt int4;

begin

    if( not (old.id_jr_state <> 4 and new.id_jr_state = 4) ) then
        return new;
    end if;

    if(new.extra_table is null or new.extra_table <> 'fault_devices_q') then
        return new;
    end if;

    select count(*) into cnt
    from command_journal cmd
    where
        cmd.id_jr_state <> 4 
        and cmd.extra_table is not null
        and cmd.extra_table = 'fault_devices_q'
        and cmd.extra_table_id is not null;

    update fault_devices_q set is_deleted = true where id = new.extra_table_id;


    if(cnt <> 0) then
        return new;
    end if;

    perform setFaultDevicesState(1);

    return new;
end
$BODY$
language 'plpgsql';


select f_safe_drop_trigger('trgtsoanalyzecmdfault', 'command_journal');
select f_create_trigger('trgtsoanalyzecmdfault', 'after', 'update', 'command_journal', 'trgtsoanalyzecmdcheckfault()');

/*
create trigger trgTSOAnalyzeCmdFault
after update
on command_journal
for each row
execute procedure trgTSOAnalyzeCmdCheckFault();
*/

