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
        new_table_name := getRealTableName(new_table_name, new.id);
        if(new_table_name is null) then
            raise exception 'Cannot rename table to given name! -- %', new_table_name;
            return NULL;
        end if;

        --query := 'alter table ' || old_table_name || ' rename to ' || new_table_name;
        --execute query;
        perform renameDDocTable(old_table_name, new_table_name);
    end if;
    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgioupdate', 'io_objects');

select f_create_trigger('trgioupdate', 'after', 'update', 'io_objects', 'ioupdatechecktable()');

--вспомогательная функция для переименования набора, состоящего из таблицы и представлений, которые описывают справочник
create or replace function renameDDocTable(varchar, varchar) returns int4 as
$BODY$
declare
    old_Name alias for $1;
    new_Name alias for $2;

    oldName varchar;
    newName varchar;
    tCount int4;
    query varchar;
begin
    if(old_Name isnull or new_Name isnull) then
        raise exception 'Input parameters cannot be null!';
        return 0;
    end if;

    oldName := old_Name;
    newName := new_Name;

    select f_is_table_exist(oldName, NULL) into tCount;
    if(tCount > 0) then
        raise exception 'The relation % seems to be not a DynamicDocs table/view! Cannot rename that!', oldName;
        return 0;
    end if;

    select f_is_view_exist(oldName) into tCount;
    if(tCount = 0) then
        raise exception 'View with name % dows not exist in database!', oldName;
        return 0;
    end if;

    oldName = 'tbl_' || oldName;
    select f_is_table_exist(oldName, NULL) into tCount;
    if(tCount = 0) then
        raise exception 'The relation % seems to be not a DynamicDocs table/view! Cannot rename that!', oldName;
        return 0;
    end if;

    query := 'drop view "' || old_Name || '"';
    execute query;

    query := 'select f_drop_funcs_' || old_Name || '()';
    execute query;

    query := 'drop function f_drop_funcs_' || old_Name || '()';
    execute query;
    
    query := 'alter table ' || oldName || ' rename to ' || newName;
    execute query;

    perform acl_secureTable(newName);

    return 1;

end
$BODY$
language 'plpgsql' security definer;


