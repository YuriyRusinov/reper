create or replace function ioUpdateCheckTable() returns trigger as
$BODY$
declare
    old_table_name varchar;
    new_table_name varchar;

    new_id_io_category int4;
    old_id_io_category int4;
    id_io_child_category int4;
    query varchar;
begin
    new_id_io_category := new.id_io_category;
    old_id_io_category := old.id_io_category;

    if (new_id_io_category <> old_id_io_category) then
        raise exception 'Category cannot been changed.';
        return NULL;
    end if;

    select into id_io_child_category id_child from io_categories c where c.id=new_id_io_category;
    if (id_io_child_category is null) then
        return new;
    end if;

    old_table_name := old.table_name;
    new_table_name := new.table_name;

    if (old_table_name is not null and new_table_name is not null and old_table_name <> new_table_name) then
        query := 'alter table ' || old_table_name || ' rename to ' || new_table_name;
        raise notice '%', query;
        execute query;
    end if;
    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgioupdate', 'io_objects');

select f_create_trigger('trgioupdate', 'after', 'update', 'io_objects', 'ioupdatechecktable()');

/*
create trigger trgIOUpdate 
after update 
on tbl_io_objects 
for each row 
execute procedure ioUpdateCheckTable();
*/
