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

create or replace function uSetMsgAsConfirmed(int4) returns int4 as
$BODY$
declare
    idMsg alias for $1;
begin
    update message_journal set is_outed = true where id = idMsg;
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

create or replace function uSetMsgAsNotConfirmed(int4) returns int4 as
$BODY$
declare
    idMsg alias for $1;
begin
    update message_journal set is_outed = false where id = idMsg;
    if(FOUND = FALSE) then
        return -1;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';
