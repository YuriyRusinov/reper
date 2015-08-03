create or replace function msgInsertOut(int4, varchar, int4, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    msgBody alias for $2;
    idDlReceiver alias for $3;
    idUrgencyLevel alias for $4; --код срочности

    idMsg int4;
    idDlSender int4;
begin

    select getNextSeq('message_journal', 'id') into idMsg;

    select getCurrentDl() into idDlSender;


    insert into message_journal(id, 
                                id_dl_sender, 
                                id_dl_receiver, 
                                id_io_object, 
                                insert_time, 
                                sent_datetime, 
                                receive_datetime, 
                                read_datetime, 
                                message_body,
                                id_urgency_level) 
    values
                                    
                               (idMsg, 
                                idDlSender, 
                                idDlReceiver,
                                idObject, 
                                current_timestamp, 
                                NULL, 
                                NULL,
                                NULL,
                                msgBody,
                                idUrgencyLevel);

    return idMsg;

end
$BODY$
language 'plpgsql' security definer;

create or replace function msgInsertIn(int4, varchar, int4, int4, timestamp, int4, varchar, varchar, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    msgBody alias for $2;
    idDlReceiver alias for $3;
    idDlSender alias for $4;
    sentDatetime alias for $5;
    extraId alias for $6;
    uniqueID alias for $7;
    outputNumber alias for $8;
    idUrgencyLevel alias for $9; --код срочности

    idMsg int4;
begin

    --почему-то иногда при передаче сообщений пакеты множатся.
    --мы считаем, что сообщение дважды вставлено быть не может
    --поэтому если она есть на приемнике (т.е. с данным UID), 
    --то полагаем, что произошла эта самая ошибка и ничего не делаем
    select id into idMsg  from message_journal where unique_id = uniqueID;
    if(idMsg is not null) then
        return idMsg;
    end if;

    select getNextSeq('message_journal', 'id') into idMsg;

    insert into message_journal(id, 
                                id_dl_sender, 
                                id_dl_receiver, 
                                id_io_object, 
                                insert_time, 
                                sent_datetime, 
                                receive_datetime, 
                                read_datetime, 
                                message_body,
                                extra_id,
                                unique_id,
                                output_number,
                                id_urgency_level) 
    values
                                    
                               (idMsg, 
                                idDlSender, 
                                idDlReceiver,
                                idObject, 
                                current_timestamp, 
                                sentDatetime, 
                                current_timestamp,
                                NULL,
                                msgBody,
                                extraId,
                                uniqueID,
                                outputNumber,
                                idUrgencyLevel);

    return idMsg;

end
$BODY$
language 'plpgsql';
