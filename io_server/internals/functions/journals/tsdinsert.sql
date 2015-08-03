create or replace function tsdInsertOut(int4, int4, int4, int4, timestamp, int4[2], varchar) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    idDlTo alias for $2;
    idDlExecutor alias for $3;
    idDlController alias for $4;
    execDatetime alias for $5;
    execPeriod alias for $6;
    msgBody alias for $7;

    idTsd int4;
    idDlFrom int4;
begin

    select getNextSeq('tsd', 'id') into idTsd;

    select getCurrentDl() into idDlFrom;


    insert into tsd(id, 
                                id_dl_to, 
                                id_dl_executor,
                                id_dl_controller,
                                id_io_category,
                                id_jr_state,
                                insert_time,
                                exec_datetime,
                                exec_period,
                                osnovanie,
                                is_archived)
    values
                                    
                               (idTsd, 
                                idDlTo,
                                idDlExecutor,
                                idDlController,
                                idCategory,
                                1,--запланирован
                                current_timestamp,
                                execDatetime,
                                execPeriod, 
                                msgBody,
                                false);

    return idTsd;

end
$BODY$
language 'plpgsql' security definer;
