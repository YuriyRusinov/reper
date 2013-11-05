create or replace function uGetReceiverEmailPrefix(int8, int4) returns varchar as
$BODY$
declare
    id alias for $1;
    itype alias for $2;
    
    res varchar;
begin

    if (itype = 0) then --command
        select uGetReceiverEmailPrefixCmd(id::int4) into res;
    elsif (itype = 1) then --document as response of command
        select uGetReceiverEmailPrefixObj(id::int4) into res;
    elsif (itype = 2) then --message
        select uGetReceiverEmailPrefixMsg(id::int4) into res;
    elsif (itype = 3) then --mail confirmation
        select uGetReceiverEmailPrefixMsg(id::int4) into res;
    elsif (itype = 4) then --cmd confirmation
        select uGetReceiverEmailPrefixCmd(id::int4) into res;
    elsif (itype = 5 or itype=11) then --record or org package (in out_sync_queue)
        select uGetReceiverEmailPrefixQueue(id) into res;
    elsif (itype = 6 or itype = 14) then --record confirmation --or ping result
        select uGetReceiverEmailPrefixQueueRes(id) into res;
    elsif (itype = 8) then --organization (in out_sync_queue)
        select uGetReceiverEmailPrefixQueue(id) into res;
    elsif (itype = 12) then --file (transferred in blocks)  (in out_sync_queue)
        select uGetReceiverEmailPrefixQueue(id) into res;
    else
        res := '';
    end if;

    return res;

end
$BODY$
language 'plpgsql';

create or replace function uGetReceiverEmailPrefixCmd(int4) returns varchar as
$BODY$
declare
    idCmdJournal alias for $1;
    ep varchar;
begin

    select o.email_prefix into ep 
    from 
        command_journal c
        inner join position p on (c.id_dl_executor = p.id)
        inner join units u on (p.id_unit = u.id)
        inner join organization o on (u.id_organization = o.id)
    where c.id = idCmdJournal;
    
    return ep;
 
end
$BODY$
language 'plpgsql';


create or replace function uGetReceiverEmailPrefixObj(int4) returns varchar as
$BODY$
declare
    idControlJournal alias for $1;
    ep varchar;
begin

    select o.email_prefix into ep 
    from 
        tsd_control_journal t
        inner join command_journal c on (t.id_journal = c.id)
        inner join position p on (c.id_dl_executor = p.id)
        inner join units u on (p.id_unit = u.id)
        inner join organization o on (u.id_organization = o.id)
    where t.id = idControlJournal;


    return ep;
 
end
$BODY$
language 'plpgsql';

create or replace function uGetReceiverEmailPrefixMsg(int4) returns varchar as
$BODY$
declare
    idMsgJournal alias for $1;
    ep varchar;
begin

    select o.email_prefix into ep 
    from 
        message_journal m
        inner join position p on (m.id_dl_receiver = p.id)
        inner join units u on (p.id_unit = u.id)
        inner join organization o on (u.id_organization = o.id)
    where m.id = idMsgJournal;

    return ep;
 
end
$BODY$
language 'plpgsql';

create or replace function uGetReceiverEmailPrefixQueueRes(int8) returns varchar as
$BODY$
declare
    idRecv alias for $1;
    ep varchar;
begin

    select org_uid into ep from queue_results where id = idRecv;

    return ep;
 
end
$BODY$
language 'plpgsql';

create or replace function uGetReceiverEmailPrefixQueue(int8) returns varchar as
$BODY$
declare
    idRecv alias for $1;
    ep varchar;
begin

    select o.email_prefix into ep 
    from 
        out_sync_queue q
        inner join organization o on (q.id_organization = o.id)
    where q.id = idRecv;

    return ep;
 
end
$BODY$
language 'plpgsql';
