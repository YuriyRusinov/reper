create or replace function logRemoveAttr() returns trigger as
$BODY$
declare
    uname varchar;
    uid int4;
    is_log boolean;
begin

    select isLogging(4) into is_log;
    if(is_log = FALSE) then
        return old;
    end if;

    select session_user into uname;
    select getUser(uname) into uid;
    if(uid isnull) then
        return NULL;
    end if;
   
    insert into log (id_user, id_io_object, id_operation_type, datetime) 
    values(uid, old.id_io_object, 4, current_timestamp);
    
    return old;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('zz_trglogremoveattr', 'attrs_values');

select f_create_trigger('zz_trglogremoveattr', 'before', 'delete', 'attrs_values', 'logremoveattr()');

/*
create trigger zz_trgLogRemoveAttr
before delete
on attrs_values 
for each row 
execute procedure logRemoveAttr();
*/
