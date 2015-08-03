create or replace function catInsertCheck() returns trigger as
$BODY$
declare
begin
    
    if(new.code isnull or new.code = '') then
        new.code = 'CATEGORY_' || new.id;
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgcatinsert', 'io_categories');

select f_create_trigger('trgcatinsert', 'before', 'insert or update', 'io_categories', 'catinsertcheck()');

/*
create trigger trgCatInsert 
before insert or update
on io_categories 
for each row 
execute procedure catInsertCheck();
*/
