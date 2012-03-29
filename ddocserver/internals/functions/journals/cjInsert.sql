create or replace function cjInsert(int4, int4, boolean) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idJournal alias for $2;
    isDraft alias for $3;

    idControl int4;
    plannedExec timestamp;
    r record;
begin

    select getNextSeq('tsd_control_journal', 'id') into idControl;

    for r in select exec_datetime, id_dl_executor from command_journal where id = idJournal
    loop
        if(r.id_dl_executor <> getCurrentDl()) then
            raise exception 'You cannot create result for not yours command!';
            return -1;
        end if;
        if(r.exec_datetime isnull) then
            return 0;
        end if;
        plannedExec = r.exec_datetime;
    end loop;

    insert into tsd_control_journal (id, id_journal, id_io_object, insert_time, planned_exec, real_exec) values
                                    (idControl, idJournal, idObject, current_timestamp, plannedExec, current_timestamp); 

    if(isDraft = TRUE) then
        update command_journal set id_jr_state = 6 where id = idJournal;
    else
        update command_journal set id_jr_state = 4 where id = idJournal;
    end if;

    return idControl;

end
$BODY$
language 'plpgsql' security definer;

create or replace function cjInsertEx(int4, int4, timestamp) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idJournal alias for $2;
    realExec alias for $3;

    idControl int4;
    plannedExec timestamp;
begin

    select getNextSeq('tsd_control_journal', 'id') into idControl;

    select exec_datetime into plannedExec from command_journal where id = idJournal;
    if(plannedExec is null) then
        return 0;
    end if;

    insert into tsd_control_journal (id, id_journal, id_io_object, insert_time, planned_exec, real_exec) values
                                    (idControl, idJournal, idObject, current_timestamp, plannedExec, realExec); 

    update command_journal set id_jr_state = 4 where id = idJournal;

    return idControl;

end
$BODY$
language 'plpgsql';
