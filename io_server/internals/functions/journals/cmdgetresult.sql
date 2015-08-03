create or replace function cmdGetResult(int4) returns int4 as
$BODY$
declare
    idJournal alias for $1;
    idObject int4;
    r record;
    idDl int4;
begin
    idDl = getCurrentDl();

    for r in select id_dl_to, id_dl_from, id_dl_executor from command_journal where id = idJournal
    loop
        if(r.id_dl_to <> idDl and r.id_dl_from <> idDl and r.id_dl_executor <> idDl) then
            raise exception 'You cannot view results of command neither sender nor controller nor executor';
            return -1;
        end if;
    end loop;

    select id_io_object into idObject from tsd_control_journal where id_journal = idJournal order by real_exec limit 1;

    return idObject;
end
$BODY$
language 'plpgsql' security definer;
