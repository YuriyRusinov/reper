create or replace function msgSetAsReaded(int4) returns int4 as
$BODY$
declare
    idJournal alias for $1;
    r RECORD;
    idDl int4;
begin
    
    for r in select * from message_journal where id = idJournal
    loop
        if(r.read_datetime is not null) then
            return 1;
        end if;
        if(r.id_dl_receiver <> getCurrentDl()) then
            raise exception 'You cannot modify not yours input messages!';
            return -1;
        end if;
    end loop;

    if(FOUND = FALSE) then 
        return 0;
    end if;

    update message_journal set read_datetime = current_timestamp where id = idJournal;
    
    return 1;
end
$BODY$
language 'plpgsql' security definer;
