create or replace function uConfirmCmd(varchar, int4, timestamp, timestamp) returns int4 as
$BODY$
declare
    uidCmd alias for $1;
    idState alias for $2;
    acceptedDatetime alias for $3;
    receiveDatetime alias for $4;
begin

    if(idState = 2) then--выдано задание
        update command_journal set id_jr_state = idState, receive_datetime = receiveDatetime where unique_id = uidCmd;
    elsif(idState = 3) then -- исполняется
        update command_journal set id_jr_state = idState, accepted_datetime = acceptedDatetime where unique_id = uidCmd;
    else
        update command_journal set id_jr_state = idState where unique_id = uidCmd;
    end if;

    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return 1;

end
$BODY$
language 'plpgsql';
