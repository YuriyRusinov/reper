create or replace function logAddAttr() returns trigger as
$BODY$
declare
    uname varchar;
    uid int4;
    is_log boolean;
begin

    select isLogging(3) into is_log;
    if(is_log = FALSE) then
        return new;
    end if;

    select session_user into uname;
    select getUser(uname) into uid;
    if(uid isnull) then
        return NULL;
    end if;

    insert into log (id_user, id_io_object, id_operation_type, datetime) 
    values(uid, new.id_io_object, 3, current_timestamp);
    
    return new;
end
$BODY$
language 'plpgsql' security definer;


select f_safe_drop_trigger('zz_trglogaddattr', 'attrs_values');

select f_create_trigger('zz_trglogaddattr', 'before', 'insert', 'attrs_values', 'logaddattr()');

/*
create trigger zz_trgLogAddAttr
before insert
on attrs_values 
for each row 
execute procedure logAddAttr();
*/
