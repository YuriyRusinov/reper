--Создается категория, описывающая простой поисковый запрос
--необходимо в нее добавить ряд доп. атрибутов
create or replace function catSimpleSearchInsertCheck() returns trigger as
$BODY$
declare
    ok bool;

    idTemplate int4;
    idGroup int4;
    idAttr int4;
    iOrder int4;
    r record; 
    idCatAttr int4;
begin

    if(new.is_completed = 0) then
        return new;
    end if;

    ok = false;

    if(old.is_completed = 0 and (new.is_completed = 1 or new.is_completed = 2 )) then
        ok = true;
    end if;

    if(ok <> true) then
        return new;
    end if;


    if(new.id_io_category_type <> 12) then
        return new;
    end if;

    --в БД создана категория ИО, являющегося шаблоном поискового запроса
    --теперь необходимо добавить в нее ряд атрибутов
    --и создать шаблон отображения

    select getNextSeq('io_templates', 'id') into idTemplate;
    insert into io_templates (id, id_io_category, name, code, description) 
    values (idTemplate, new.id, 'Просмотр поискового запроса ' || new.name, 'SEARCH_TEMPLATE_' || idTemplate, NULL);

    iOrder = 1;
    for r in 
        select 
            ac.id_io_category,
            ac.id_io_attribute,
            ac.def_value,
            ac.is_read_only,
            ac.is_mandatory,
            a.name
        from
            attrs_categories ac,
            attributes a
        where 
            ac.id_io_category = new.id
            and ac.id_io_attribute = a.id
    loop

        select getNextSeq('a_groups', 'id') into idGroup;
        insert into a_groups (id, id_io_template, id_parent, name, "order") 
        values (idGroup, idTemplate, NULL, r.name, iOrder);

        select acInsert1(new.id, 2, 'OPERATIONS_ATTR_' || iOrder, 'Операция', 'Операция', 'operations', 'name', 100, '1', r.is_mandatory, r.is_read_only, NULL, 'id', NULL) into idAttr;
        if(idAttr <= 0) then
            return NULL;
        end if;

        idCatAttr :=  acGetAttrCategoryId(r.id_io_category, r.id_io_attribute);

        insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value)
        values (idCatATtr, idTemplate, idGroup, r.is_read_only, 1, r.def_value);

        idCatAttr :=  acGetAttrCategoryId(r.id_io_category, idAttr);

        insert into io_views (id_attr_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value)
        values (idCatAttr, idTemplate, idGroup, r.is_read_only, 2, r.def_value);
        

        iOrder := iOrder + 1;
    
    end loop;
    
    


    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgcatsimplesearchinsert', 'io_categories');

create trigger trgCatSimpleSearchInsert 
before update
on io_categories
for each row 
execute procedure catSimpleSearchInsertCheck();

