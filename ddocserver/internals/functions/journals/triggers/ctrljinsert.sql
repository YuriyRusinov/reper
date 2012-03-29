create or replace function ctrlJInsertCheck() returns trigger as
$BODY$
declare
    res int4;
    idDl int4;
    isLocal boolean;
    idUser int4;
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
language 'plpgsql';

select f_safe_drop_trigger('trgctrljinsert', 'tsd_control_journal');

create trigger trgCtrlJInsert
before insert or update
on tsd_control_journal 
for each row 
execute procedure ctrlJInsertCheck();
