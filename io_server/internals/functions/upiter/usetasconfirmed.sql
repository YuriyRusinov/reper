create or replace function uSetCmdAsConfirmed(int4) returns int4 as
$BODY$
declare
    idCmd alias for $1;
begin
    delete from cmd_confirmations where id_cmd = idCmd;
    if(FOUND = FALSE) then
        return -1;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';

--эта функция вызывается на стороне получателя. Сообщает о том, что отправитель успешно получил квитанцию о том
--что получатель собщение получил или прочитал
--
create or replace function uSetMsgAsConfirmed(int4, bool) returns int4 as
$BODY$
declare
    idMsg alias for $1;
    bReadConfirmation alias for $2; --если true, то сообщается о том, что получатель сообщение прочитал. В противном случае - только получил

    r record;
    bSenderIsLocal bool;
    bReceiverIsLocal bool;
    isOuted int4;
begin
    
    for r in select id_dl_receiver, id_dl_sender, receive_datetime, read_datetime, is_outed from message_journal where id = idMsg
    loop
        bReceiverIsLocal = isLocalDl(r.id_dl_receiver);
        bSenderIsLocal = isLocalDl(r.id_dl_sender);

        if(bReceiverIsLocal = true and bSenderIsLocal = false) then 

            if(bReadConfirmation = true) then  --если пришла квитанция о прочтении, то помечаем сообщение как не требующее дальнейшей отправки (is_outed = 3)
                isOuted = 3;
            else
                if(r.read_datetime is not null) then
                    isOuted = 2; -- если имеем квитанцию только о получении и пользователь уже успел отметить сообщение как прочитанное, то никак не помечаем сообщение (фактически оставляем is_outed = 2)
                else
                    isOuted = 3; -- если имеем квитанцию только о получении и пользователь еще НЕ успел отметить сообщение как прочитанное, то надо его перевести в состояние "отправка не требуется".
                                 -- когда получатель пометит сообщение как прочитанное оно автоматически перейдет в состояние "требуется отправка квитанции и прочтении"
                end if;
            end if;

        else
            return 1;
        end if;
    end loop;

    update message_journal set is_outed = isOuted where id = idMsg;
    if(FOUND = FALSE) then
        return -1;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';


create or replace function uSetCmdAsNotConfirmed(int4) returns int4 as
$BODY$
declare
    idCmd alias for $1;
begin
    --delete from cmd_confirmations where id_cmd = idCmd;
    insert into cmd_confirmations (id_cmd) values (idCmd);
    if(FOUND = FALSE) then
        return -1;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function uSetMsgAsNotConfirmed(int4, bool) returns int4 as
$BODY$
declare
    idMsg alias for $1;
    bReadConfirmation alias for $2; --см. ф-ю uSetMsgAsConfirmed

    r record;
    bSenderIsLocal bool;
    bReceiverIsLocal bool;
    isOuted int4;
begin
    
    for r in select id_dl_receiver, id_dl_sender, receive_datetime, read_datetime from message_journal where id = idMsg
    loop
        bReceiverIsLocal = isLocalDl(r.id_dl_receiver);
        bSenderIsLocal = isLocalDl(r.id_dl_sender);

        if(bReceiverIsLocal = true and bSenderIsLocal = false) then

            if(bReadConfirmation = true) then
                isOuted = 2;
            else
                if(r.read_datetime is not null) then
                    isOuted = 2;
                else
                    isOuted = 1;
                end if;
            end if;

        else
            return 1;
        end if;
    end loop;

    update message_journal set is_outed = isOuted where id = idMsg;
    if(FOUND = FALSE) then
        return -1;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';
