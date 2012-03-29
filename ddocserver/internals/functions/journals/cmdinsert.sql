create or replace function cmdInsertOut(int4, int4, int4, timestamp, int4[2], varchar, int4, int4) returns int4 as
$BODY$
declare
    idDlTo alias for $1;
    idDlExecutor alias for $2;
    idCategory alias for $3;
    execDatetime alias for $4;
    execPeriod alias for $5;
    msgBody alias for $6;
    idUrgencyLevel alias for $7; --код срочности
    idObject alias for $8;

    idCmd int4;
    idDlFrom int4;
begin

    select getNextSeq('command_journal', 'id') into idCmd;

    select getCurrentDl() into idDlFrom;


    insert into command_journal(id, 
                                id_dl_from, 
                                id_dl_to, 
                                id_dl_executor,
                                id_io_category,
                                id_jr_state,
                                insert_time,
                                exec_datetime,
                                exec_period,
                                message_body,
                                id_urgency_level,
                                id_io_object_att,
                                is_automated,
                                is_archived)
    values
                                    
                               (idCmd, 
                                idDlFrom, 
                                idDlTo,
                                idDlExecutor,
                                idCategory,
                                1,--запланирован
                                current_timestamp,
                                execDatetime,
                                execPeriod, 
                                msgBody,
                                idUrgencyLevel,
                                idObject,
                                true,
                                false);

    return idCmd;

end
$BODY$
language 'plpgsql' security definer;
