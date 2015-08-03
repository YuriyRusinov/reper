create or replace function orgInsertCheck() returns trigger as
$BODY$
declare
    r record;
    io_uid varchar;
begin
    
    --if(getCurrentUser() <> 2) then --jupiter
    --    return new;
    --end if;

    --if(new.id = getLocalOrgId()) then
    --    raise exception 'SOME STRANGE ERROR!';
    --    return NULL;
    --end if;

    if(TG_OP = 'INSERT') then
        insert into units (id_organization, 
                           id_parent, 
                           id_curr_mode, 
                           id_boss, 
                           name, 
                           short_name, 
                           code_name, 
                           tree_symbol, 
                           is_fgroup)
                    values(new.id,
                           NULL,
                           new.id_curr_mode,
                           NULL,
                           'Публичные должностные лица',
                           'Публичные должностные лица',
                           'Публичные должностные лица',
                           NULL,
                           false);

        if(FOUND = FALSE) then
            raise exception 'An error was occured during creation of system unit of remote organization!';
            return NULL;
        end if;
    end if;

/*
    if(new.is_created = 1) then --с такими организациями проверка связи завершилась успешно

        --для новой организации отправляем всех публичных должностных лиц текущей организации
        select unique_id into io_uid from io_objects where id = 26; --position qualifier
        for r in select id, unique_id from "position" where is_public = true
        loop
            perform addSyncRecord(new.id, r.id, r.unique_id, io_uid, 'position', 1, 10);
        end loop;

    end if;
*/

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgorginsert', 'organization');

create trigger trgOrgInsert
after insert or update
on organization
for each row 
execute procedure orgInsertCheck();

