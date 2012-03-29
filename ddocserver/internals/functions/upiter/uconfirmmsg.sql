create or replace function uConfirmMsg(int4, timestamp, timestamp) returns int4 as
$BODY$
declare
    idMsg alias for $1;
    readDatetime alias for $2;
    receiveDatetime alias for $3;
begin

    update message_journal set read_datetime = readDatetime, receive_datetime = receiveDatetime where id = idMsg;
    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return 1;

end
$BODY$
language 'plpgsql';
