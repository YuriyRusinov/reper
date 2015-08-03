create or replace function msgSetAsReceived(int4) returns int4 as
$BODY$
declare
    idJournal alias for $1;
    r RECORD;
begin
    
    for r in select * from message_journal where id = idJournal
    loop
        if(r.receive_datetime is not null) then
            return 1;
        end if;
    end loop;

    if(FOUND = FALSE) then 
        return 0;
    end if;

    update message_journal set receive_datetime = current_timestamp where id = idJournal;
    
    return 1;
end
$BODY$
language 'plpgsql';
