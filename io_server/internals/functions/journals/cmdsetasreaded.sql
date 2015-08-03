create or replace function cmdSetAsReaded(int4) returns int4 as
$BODY$
declare
    idJournal alias for $1;
    r RECORD;
    idDl int4;
begin

    idDl := getCurrentDl();
    if (idDl is null) then
        return 0;
    end if;    

    for r in select * from command_journal where id = idJournal
    loop

        if(r.id_dl_to = idDl) then --controller

            if(r.read_datetime_c is not null) then
                return 1;
            end if;
            update command_journal set read_datetime_c = current_timestamp where id = idJournal;

        elsif(r.id_dl_executor = idDl) then --executor

            if(r.read_datetime_c is not null) then
                return 1;
            end if;

            update command_journal set read_datetime_e = current_timestamp where id = idJournal;

        else
            raise exception 'You cannot modify not yous commands!';
            return 0;
        end if;

    end loop;

    if(FOUND = FALSE) then 
        return 0;
    end if;

    
    return 1;
end
$BODY$
language 'plpgsql' security definer;
