create or replace function cjSetAsResult(int4, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idJournal alias for $2;

    idControl int4;
    idExecutor int4;
begin

    --select getNextSeq('tsd_control_journal', 'id') into idControl;
    select id_dl_executor into idExecutor from command_journal where id = idJournal;
    if(idExecutor <> getCurrentDl()) then
        raise exception 'You cannot create result for not yours command!';
        return -1;
    end if;


    select id into idControl 
    from tsd_control_journal 
    where 
        id_io_object = idObject and 
        id_journal = idJournal  
    limit 1;

    if(idControl is null) then
        return 0;
    end if;

    update command_journal set id_jr_state = 4 where id = idJournal;

    return 1;

end
$BODY$
language 'plpgsql' security definer;
