/*функция меняет состояние ИО с "Осуществляется первоначальная синхронизация" на "Активный", анализируя таблицу исходящей очереди синхронизации*/
create or replace function setIOState() returns trigger as
$BODY$
declare
    tName varchar;
    cnt int4;
begin

    if(old.sync_result = new.sync_result) then
        return new;
    end if;

    if(new.entity_type <> 2 and new.entity_type <> 3 and new.entity_type <> 4) then -- IO, EIO, additional table EIO
        return new;
    end if;

    if(new.sync_result <> 3) then  --success result
        return new;
    end if;

    if(new.sync_type <> 1) then --при первоначальной синхронизации sync_type всегда = 1
        return new;
    end if;

    if(new.entity_type = 2) then --IO
        select table_name into tName from f_sel_io_objects(new.id_entity::int4) where id = new.id_entity::int4;
        if(tName isnull) then --IO is not qualifier, so just change its state
            update io_objects set id_io_state = 1 where id = new.id_entity::int4 and id_io_state = 3;--first sync
        end if;

        return new;
    end if;

    select count(*) into cnt from f_sel_io_objects(new.entity_io_uid) where unique_id = new.entity_io_uid and id_io_state = 3; --исплючаем ситуацию, когда первоначальная синхронизация уже завершилась и просто идут обновления
    if(cnt = 0) then
        return new;
    end if;

    select count(*) into cnt from out_sync_queue where entity_io_uid = new.entity_io_uid and sync_result <> 3;
    if(cnt = 0) then --all records are syncronized
        update io_objects set id_io_state = 1 where unique_id = new.entity_io_uid;
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgsetiostate', 'out_sync_queue');

select f_create_trigger('trgsetiostate', 'after', 'update', 'out_sync_queue', 'setiostate()');

/*
create trigger trgSetIOState
after update
on out_sync_queue
for each row 
execute procedure setIOState();
*/

