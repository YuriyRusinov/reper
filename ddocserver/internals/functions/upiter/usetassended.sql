create or replace function uSetAsSended(int8, int4) returns int4 as
$BODY$
declare
    id alias for $1;
    itype alias for $2;
    
    res int4;
begin

    res := 0;

    if (itype = 0) then --command
        select uSetAsSended(id::int4) into res;
    elsif (itype = 1) then --document as response of command
        select uSetObjAsSended(id::int4) into res;
    elsif (itype = 2) then --message
        select uSetMsgAsSended(id::int4) into res;
    elsif (itype = 3) then --mail confirmation
        select uSetMsgAsConfirmed(id::int4) into res;
    elsif (itype = 4) then --cmd confirmation
        select uSetCmdAsConfirmed(id::int4) into res;
    elsif (itype = 5 or itype=11 or itype=15) then --record or org package or query for first sync (in out_sync_queue)
        select uSetRecordAsSended(id) into res;
    elsif (itype = 6 or itype = 14) then --record confirmation --or ping result
        select uSetReceptionAsSended (id) into res;
    elsif (itype = 8) then --organization (in out_sync_queue)
        select uSetRecordAsSended(id) into res;
    elsif (itype = 12) then --file (transferred in blocks)  (in out_sync_queue)
        select uSetRecordAsSended(id) into res;
    else
        res := 0;
    end if;

    return res;

end
$BODY$
language 'plpgsql';

create or replace function uSetAsSended(int4) returns int4 as
$BODY$
declare
    idCmdJournal alias for $1;
    idState int4;
begin

    select id_jr_state into idState from command_journal where id = idCmdJournal;
    if(idState = 7) then --virtual command
        update command_journal set id_jr_state = 1 where id = idCmdJournal;
    else    
        update command_journal set id_jr_state = 2 where id = idCmdJournal;
    end if;
    
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;
 
end
$BODY$
language 'plpgsql';

create or replace function dbgSetAsNotSended(int4) returns int4 as
$BODY$
declare
    idCmdJournal alias for $1;
begin

    update command_journal set id_jr_state = 1 where id = idCmdJournal;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;
 
end
$BODY$
language 'plpgsql';

create or replace function uSetObjAsSended(int4) returns int4 as
$BODY$
declare
    idControlJournal alias for $1;
begin

    update tsd_control_journal set is_outed = true where id = idControlJournal;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;
 
end
$BODY$
language 'plpgsql';

create or replace function uSetMsgAsSended(int4) returns int4 as
$BODY$
declare
    idMsgJournal alias for $1;
begin

    update message_journal set is_outed = true where id = idMsgJournal;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;
 
end
$BODY$
language 'plpgsql';

create or replace function uSetReceptionAsSended(int8) returns int4 as
$BODY$
declare
    idRecv alias for $1;
begin

    update queue_results set is_read = 2 where id = idRecv;
    if (not FOUND) then
        return 0;
    end if;

    return 1;
 
end
$BODY$
language 'plpgsql';
