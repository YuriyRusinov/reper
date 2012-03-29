create or replace function deletePublicDl(varchar) returns int4 as
$BODY$
declare
    uid alias for $1;
    
    idDl int4;
    idAnotherUnit int4;
    cnt int4;
    r record;
begin

    select id into idDl from "position" where unique_id = uid;
    if(idDl isnull) then
        return 0;
    end if;
    
    select count(*) into cnt from command_journal where id_dl_from = idDl or id_dl_to = idDl or id_dl_executor = idDl;
    if(cnt <> 0) then
        
        for r in select id from "position" where id_unit in (select id_unit from "position" where id = idDl) limit 1
        loop
            if(r.id isnull) then
                return 0;
            end if;
        
            update command_journal set id_dl_to = r.id where id_dl_to = idDl;
            update command_journal set id_dl_from = r.id where id_dl_from = idDl;
            update command_journal set id_dl_executor = r.id where id_dl_executor = idDl;
        end loop;
    end if;
    
    select count(*) into cnt from message_journal where id_dl_sender = idDl or id_dl_receiver = idDl;
    if(cnt <> 0) then
        for r in select id from "position" where id_unit in (select id_unit from "position" where id = idDl) limit 1
        loop
            if(r.id isnull) then
                return 0;
            end if;
        
            update message_journal set id_dl_sender = r.id where id_dl_sender = idDl;
            update message_journal set id_dl_receiver = r.id where id_dl_receiver = idDl;
        end loop;
    end if;
    
    delete from "position" where id = idDl;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';
