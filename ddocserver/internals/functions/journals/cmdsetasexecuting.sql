create or replace function cmdSetAsExecuting(int4) returns int4 as
$BODY$
declare
    idJournal alias for $1;
    r RECORD;
    idDl int4;
begin
    
    for r in select * from command_journal where id = idJournal
    loop
        if(r.accepted_datetime is not null) then
            return 1;
        end if;
        if(r.id_dl_executor <> getCurrentDl()) then
            raise exception 'You cannot modify not yours input commands!';
            return -1;
        end if;
    end loop;

    if(FOUND = FALSE) then 
        return 0;
    end if;

    update command_journal set accepted_datetime = current_timestamp, id_jr_state = 3 where id = idJournal;
    
    return 1;
end
$BODY$
language 'plpgsql' security definer;
