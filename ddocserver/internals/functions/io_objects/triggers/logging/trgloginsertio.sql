create or replace function logInsertIO() returns trigger as
$BODY$
declare
    uname varchar;
    uid int4;
    is_log boolean;
begin

    select isLogging(1) into is_log;
    if(is_log = FALSE) then
        return new;
    end if;

    select session_user into uname;
    select getUser(uname) into uid;
    if(uid isnull) then
        return NULL;
    end if;
    
    insert into log (id_user, id_io_object, id_operation_type, datetime) 
    values(uid, new.id, 1, current_timestamp);
    
    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('zz_trgloginsertio', 'io_objects');

select f_create_trigger('zz_trgloginsertio', 'after', 'insert', 'io_objects', 'loginsertio()');

/*
create trigger zz_trgLogInsertIO
after insert
on io_objects 
for each row 
execute procedure logInsertIO();
*/
