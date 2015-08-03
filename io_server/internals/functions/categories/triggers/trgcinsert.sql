
create or replace function cInsertCheck() returns trigger as 
$BODY$
declare
    refTableName varchar;
    r record;
begin

    insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
    values(new.id, -10, true, true, false, false, true);

    insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
    values(new.id, getCurrentUser(), true, true, true, true, true);

    if(new.id_child is not null) then
        insert into attrs_categories (id_io_category, id_io_attribute, name, is_mandatory, is_read_only) values(new.id, 402, 'Ссылка на категорию таблицы справочника', false, true);
        --raise exception 'aaaa';
    end if;

    return new;

end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trgcinsert', 'io_categories');

select f_create_trigger('trgcinsert', 'after', 'insert', 'io_categories', 'cinsertcheck()');

/*create trigger trgCInsert
after insert 
on io_categories
for each row 
execute procedure cInsertCheck();
*/
