create or replace function logUpdateIO() returns trigger as
$BODY$
declare
    uname varchar;
    uid int4;
    is_log boolean;
begin

    select isLogging(2) into is_log;
    if(is_log = FALSE) then
        return new;
    end if;

    select session_user into uname;
    select getUser(uname) into uid;
    if(uid isnull) then
        return NULL;
    end if;
    
    insert into log (id_user, id_io_object, id_operation_type, datetime) 
    values(uid, new.id, 2, current_timestamp);
    
    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('zz_trglogupdateio', 'io_objects');

select f_create_trigger('zz_trglogupdateio', 'before', 'update', 'io_objects', 'logupdateio()');

/*
create trigger zz_trgLogUpdateIO
before update
on io_objects 
for each row 
execute procedure logUpdateIO();
*/
