create or replace function uSetAsNotSended(int8, int4) returns int4 as
$BODY$
declare
    id alias for $1;
    itype alias for $2;
    
    res int4;
begin

    res := 0;

    if (itype = 0) then --command
        select uSetAsNotSended(id::int4) into res;
    elsif (itype = 1) then --document as response of command
        select uSetObjAsNotSended(id::int4) into res;
    elsif (itype = 2) then --message
        select uSetMsgAsNotSended(id::int4) into res;
    elsif (itype = 3 or itype = 17) then --mail receive confirmation or mail read confirmation
        if(itype = 3) then
            select uSetMsgAsNotConfirmed(id::int4, false) into res;
        else
            select uSetMsgAsNotConfirmed(id::int4, true) into res;
        end if;
    elsif (itype = 4) then --cmd confirmation
        select uSetCmdAsNotConfirmed(id::int4) into res;
    elsif (itype = 5 or itype=11 or itype=15) then --record or org package or query for first sync (in out_sync_queue)
        select uSetRecordAsNotSended(id) into res;
    elsif (itype = 6) then --record confirmation
        select uSetReceptionAsNotSended (id) into res;
    elsif (itype = 8) then --organization (in out_sync_queue)
        select uSetRecordAsNotSended(id) into res;
    elsif (itype = 12) then --file (transferred in blocks)  (in out_sync_queue)
        select uSetRecordAsNotSended(id) into res;
    else
        res := 0;
    end if;

    return res;

end
$BODY$
language 'plpgsql';

create or replace function uSetAsNotSended(int4) returns int4 as
$BODY$
declare
    idCmdJournal alias for $1;
    idState int4;
begin

    select id_jr_state into idState from command_journal where id = idCmdJournal;
    if(idState = 1) then --assumed, that if we invoke this function on command with id_jr_state = 1, then it is virtual command
        update command_journal set id_jr_state = 7 where id = idCmdJournal;
    else    
        update command_journal set id_jr_state = 1 where id = idCmdJournal;
    end if;
    
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;
 
end
$BODY$
language 'plpgsql';

create or replace function uSetObjAsNotSended(int4) returns int4 as
$BODY$
declare
    idControlJournal alias for $1;
begin

    update tsd_control_journal set is_outed = false where id = idControlJournal;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;
 
end
$BODY$
language 'plpgsql';


create or replace function uSetMsgAsNotSended(int4) returns int4 as
$BODY$
declare
    idMsgJournal alias for $1;
begin

    update message_journal set is_outed = 0 where id = idMsgJournal;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;
 
end
$BODY$
language 'plpgsql';


create or replace function uSetReceptionAsNotSended(int8) returns int4 as
$BODY$
declare
    idRecv alias for $1;
begin

    update queue_results set is_read = 1 where id = idRecv;
    if (not FOUND) then
        return 0;
    end if;

    return 1;
 
end
$BODY$
language 'plpgsql';
