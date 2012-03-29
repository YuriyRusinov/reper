create or replace function templInsertCheck() returns trigger as
$BODY$
declare
begin
    
    if(new.code isnull or new.code = '') then
        new.code = 'TEMPLATE_' || new.id;
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgtemplinsert', 'io_templates');

select f_create_trigger('trgtemplinsert', 'before', 'insert or update', 'io_templates', 'templinsertcheck()');

/*
create trigger trgTemplInsert 
before insert or update
on io_templates
for each row 
execute procedure templInsertCheck();
*/
