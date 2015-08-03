create or replace function ctrlJInsertCheck() returns trigger as
$BODY$
declare
    res int4;
    idDl int4;
    isLocal boolean;
    idUser int4;

    r record;
    idDlTo int4;
    idDlFrom int4;
    cnt int4;
begin
    
    perform clearLastError();

    idUser = getCurrentUser();
    if(new.id_io_object is not null) then
        if(idUser <> 1 and idUser <> 2) then  --admin and jupiter can do that
            if(getPrivilege(idUser, new.id_io_object, 5, true) = false) then
                raise exception 'You have insufficient privileges to send given result of command to another roles! ID_OBJECT = %', new.id_io_object;
                return NULL;
            end if;
        end if;
    end if;

    if(new.id_io_object is not null and new.id_journal is not null) then
        for r in select id_dl_to, id_dl_executor, id_dl_from from command_journal where id = new.id_journal
        loop
            idDlTo = r.id_dl_to;
            idDlFrom = r.id_dl_from;
        end loop;

        select count(*) into cnt from access_table where id_io_object = new.id_io_object and id_role = idDlTo;
        if(cnt = 0) then
            insert into access_table (id_io_object, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_send) values (new.id_io_object, idDlTo, true, true, true, true, true);
        else
            update access_table set allow_read = true, allow_readlist = true, allow_delete = true, allow_update = true, allow_send = true where id_io_object = new.id_io_object and id_role = idDlTo;
        end if;

        select count(*) into cnt from access_table where id_io_object = new.id_io_object and id_role = idDlFrom;
        if(cnt = 0) then
            insert into access_table (id_io_object, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_send) values (new.id_io_object, idDlFrom, true, true, true, true, true);
        else
            update access_table set allow_read = true, allow_readlist = true, allow_delete = true, allow_update = true, allow_send = true where id_io_object = new.id_io_object and id_role = idDlFrom;
        end if;
    end if;

        
    select getCurrentDl() into idDl;
    

    if(TG_OP = 'UPDATE') then
        --raise notice 'You cannot update control_journal. Only insert operation is allowed!';
        return new;
    end if;

    select isLocalInCommand(new.id_journal) into isLocal;
    if(isLocal = true) then
        new.is_outed = true;
    else
        new.is_outed = false;
    end if;

    return new;

end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trgctrljinsert', 'tsd_control_journal');

create trigger trgCtrlJInsert
before insert or update
on tsd_control_journal 
for each row 
execute procedure ctrlJInsertCheck();
