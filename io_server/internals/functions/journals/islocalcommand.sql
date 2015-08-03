-- IN command is the command that result must be sended to sender
-- OUT command is the command th
create or replace function isLocalInCommand(int4) returns boolean as
$BODY$
declare
    idJournal alias for $1;

    bIsLocal bool;
begin

    select isLocalDl(id_dl_from) into bIsLocal from command_journal where id = idJournal;
    
    if(bIsLocal isnull) then
        bIsLocal = TRUE;
    end if;    

    return bIsLocal;

end
$BODY$
language 'plpgsql';

-- IN command is the command that result must be sended to sender
-- OUT command is the command th
create or replace function isLocalOutCommand(int4) returns boolean as
$BODY$
declare
    idJournal alias for $1;
    bIsLocal bool;
begin


    select isLocalDl(id_dl_executor) into bIsLocal from command_journal where id = idJournal;
    
    if(bIsLocal isnull) then
        bIsLocal = TRUE;
    end if;    

    return bIsLocal;

end
$BODY$
language 'plpgsql';
