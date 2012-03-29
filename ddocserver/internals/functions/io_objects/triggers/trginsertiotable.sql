create or replace function ioInsertAddTable () returns trigger as
$BODY$
declare
    table_name varchar;
    id_io_category int4;
    idcattype int4;

    create_query varchar;
    alter_query varchar;
    query varchar;
    r RECORD;
    i int4;
begin
    id_io_category := new.id_io_category;
    select ct.id into idcattype from io_category_types ct inner join io_categories c on (ct.id=c.id_io_category_type and c.id=id_io_category);

    raise notice '%', idcattype;
    if (idcattype <> 10) then
        raise exception 'You cannot insert not table category';
        return null;
    end if;

    table_name = getRealTableName(new.table_name, new.id_io_category);--ИД ИО здесь нет, но можем попробовать при необходимость создать название таблицы с использованием ИД категории

    if(new.id_search_template is not null and new.ref_table_name is not null) then

        if(isApplicable(new.id_search_template, id_io_category) = FALSE) then
            raise exception 'Given search template is not applicable for tables created on the current table category! ID_SEARCH_TEMPLATE = %, ID_CHILD_CATEGORY = %', new.id_search_template, id_io_child_category;
            return NULL;
        end if;

        if (createIOView (table_name, id_io_category, new.id_search_template, new.ref_table_name) is not null) then
            return new;
        else
            return NULL;
        end if;
    else
        new.id_search_template = NULL;
        new.ref_table_name = NULL;

        if (createIOTable (table_name, id_io_category, TRUE) is not null) then
            return new;
        else
            return NULL;
        end if;
    end if;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgtableinsert', 'object_ref_tables');

select f_create_trigger('trgtableinsert', 'after', 'insert', 'object_ref_tables', 'ioinsertaddtable()');
