
create or replace function iInsertCheck() returns trigger as 
$BODY$
declare
    refTableName varchar;
    r record;
begin


    if(trim(new.table_name) = '') then
        new.table_name = NULL;
    end if;

/*
    if(trim(new.ref_column_name) = '') then
        new.ref_column_name = NULL;
    end if;
*/

    if(new.table_name isnull) then
        --new.ref_column_name = NULL;
        --для атрибута типа родитель-потомок мы не знаем название таблицы, поэтому на данном этапе не можем вычислить 
        -- тип атрибута, на который это все ссылается. В качестве компромиссной меры считаем этот тип строковым (9)
        if(new.column_name is not null and new.table_name is null) then
            new.id_ref_attr_type = 9;
/*
            if (new.ref_column_name isnull) then
                new.ref_column_name = 'id';
            end if;
*/
        end if;
        return new;
    end if;

    --здесь уже однозначно обрабатываем случай ссылочного атрибута
/*
    if (new.ref_column_name isnull) then
        new.ref_column_name = 'id';
    end if;
*/
    if(new.column_name is not null and new.table_name is not null) then
        new.id_ref_attr_type = (select id from atGetAttrType(new.column_name, new.table_name) limit 1);
    end if;


    -- проверка на то, что таблица, на которую (возможно) ссылается атрибут, унаследована от другой таблицы
    -- поскольку внутри цикла возможен выход из функции
    for r in 
        select io.ref_table_name,
               io.id_search_template
        from   f_sel_io_objects_tbl(new.table_name) io 
        where  io.table_name = new.table_name
    loop
        if(r.ref_table_name isnull) then
            return new;
        end if;

        new.table_name = r.ref_table_name;

/*
        if(new.id_search_template is not null) then
            new.id_search_template = aggSearchTemplates(new.id_search_template, r.id_search_template);
        else
            new.id_search_template = r.id_search_template;
        end if;
*/

    end loop;

    return new;
end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trgiinsert', 'indicator');

select f_create_trigger('trgiinsert', 'before', 'insert or update', 'indicator', 'iinsertcheck()');
