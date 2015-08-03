\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

--СЮДА ЗАПРОСЫ НА СОЗДАНИЕ ТАБЛИЦ СПРАВОЧНИКОВ (ЕСЛИ СОЗДАВАЛИСЬ ИО)
--ИХ ОБЯЗАТЕЛЬНО ПЕРЕД СОЗДАНИЕМ ФУНКЦИЙ!!!

alter table attrs_categories drop column if exists directives;
alter table attrs_categories add column directives varchar;

alter table attrs_attrs drop column if exists directives;
alter table attrs_attrs add column directives varchar;

---------------------------
---------------------------


\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql
\i ./functions/general/createtriggeruidex.sql

\i ./functions/contribs/readd_contribs.sql

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/categories/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql


\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/readd_tasks_func.sql




--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');
select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');
select f_safe_drop_trigger('trgacinsert', 'attrs_categories');

create or replace function aaa() returns void as
$BODY$
declare
    idGroup int4;
    cnt int4;
begin
    select count(*) into cnt from attrs_groups where name = 'Служебные атрибуты';
    if(cnt = 0) then
        idGroup = getNextSeq('attrs_groups', 'id');
        insert into attrs_groups(id, name) values(idGroup, 'Служебные атрибуты');
    end if;

    select count(*) into cnt from a_types where id = 39;
    if(cnt = 0) then
        insert into a_types (id, id_a_view, name, code) values (39, 2, 'Ссылка на категорию (служебный атрибут)', 'INT4');
        perform pg_catalog.setval('a_types_id_seq', 39, true);
    end if;


    --!!! не использовать эти строки в качестве шаблона для других атрибутов!!!
    --здесь дополнительно устанавливается поле id_attr_group
    select count(*) into cnt from attributes where id = 402;
    if(cnt = 0) then
        insert into attributes (unique_id, id, id_a_type, id_attr_group, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-402', 402, 39, idGroup, 'id_table_category', 'Ссылка на категорию таблицы справочника', 'Ссылка на категорию таблицы справочника', 'io_categories', 'name', 300, TRUE);
    end if;
    select count(*) into cnt from attributes where id = 403;
    if(cnt = 0) then
        insert into attributes (unique_id, id, id_a_type, id_attr_group, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-403', 403, 8, idGroup, 'item_is_leaf', 'Иерархический признак записи', 'Иерархический признак записи', NULL, NULL, 300, TRUE);
    end if;

    select count(*) into cnt from attributes where id = 404;
    if(cnt = 0) then
        insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-404', 404, 8, 'order', 'Порядок следования атрибута', 'Порядок следования атрибута', NULL, NULL, 100, TRUE);
    end if;

    select count(*) into cnt from attributes where id = 405;
    if(cnt = 0) then
        insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-405', 405, 9, 'directives', 'Параметры по управлению поведением атрибута', 'Параметры по управлению поведением атрибута', NULL, NULL, 100, TRUE);
    end if;

    select count(*) into cnt from attrs_categories where id = 729;
    if(cnt = 0) then
        insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (729, 185, 404, '1', true, false, 'Порядок следования атрибута'); --order
    end if;

    select count(*) into cnt from attrs_categories where id = 730;
    if(cnt = 0) then
        insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (730, 185, 405, NULL, false, false, 'Параметры по управлению поведением атрибута'); --directives
    end if;

    select count(*) into cnt from attrs_categories where id = 731;
    if(cnt = 0) then
        insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (731, 189, 404, '1', true, false, 'Порядок следования атрибута'); --order
    end if;

    select count(*) into cnt from attrs_categories where id = 732;
    if(cnt = 0) then
        insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (732, 189, 405, NULL, false, false, 'Параметры по управлению поведением атрибута'); --directives
    end if;

    return;
end
$BODY$
language 'plpgsql';

select aaa();
drop function aaa();   


select f_safe_drop_type('h_attrs_categories1');
create type h_attrs_categories1 as(id int4, id_io_category int4, id_io_attribute int4, "order" int4, e_order int4);
create or replace function bbb() returns setof h_attrs_categories1 as
$BODY$
declare
    r h_attrs_categories1%rowtype;
    e_order int4;
    oldCat int4;
begin

    e_order = 0;
    oldCat = -1;
    for r in 
        select 
            distinct attrs_categories.id, 
            attrs_categories.id_io_category, 
            attrs_categories.id_io_attribute, 
            attrs_categories."order",        
            1 
        from 
            attrs_categories
        
        order by attrs_categories.id_io_category, attrs_categories."order", attrs_categories.id_io_attribute
    loop
        if(oldCat <> r.id_io_category) then
            e_order = 0;
        end if;
        oldCat = r.id_io_category;
        
        r.e_order = e_order+1;
        e_order = e_order + 1;
        return next r;
    end loop;

    return;
end
$BODY$
language plpgsql;

update attrs_categories 
    set "order" = a.e_order
from (select * from bbb()) as a
where 
    attrs_categories.id = a.id;

drop function bbb();
select f_safe_drop_type('h_attrs_categories1');


select f_safe_drop_type('h_attrs_attrs1');
create type h_attrs_attrs1 as(id int4, id_attr_parent int4, id_attr_child int4, "order" int4, e_order int4);
create or replace function ccc() returns setof h_attrs_attrs1 as
$BODY$
declare
    r h_attrs_attrs1%rowtype;
    e_order int4;
    oldAttr int4;
begin

    e_order = 0;
    oldAttr = -1;
    for r in 
        select 
            distinct attrs_attrs.id, 
            attrs_attrs.id_attr_parent, 
            attrs_attrs.id_attr_child, 
            attrs_attrs."order",        
            1 
        from 
            attrs_attrs
        
        order by attrs_attrs.id_attr_parent, attrs_attrs."order", attrs_attrs.id_attr_child
    loop
        if(oldAttr <> r.id_attr_parent) then
            e_order = 0;
        end if;
        oldAttr = r.id_attr_parent;
        
        r.e_order = e_order+1;
        e_order = e_order + 1;
        return next r;
    end loop;

    return;
end
$BODY$
language plpgsql;

update attrs_attrs
    set "order" = a.e_order
from (select * from ccc()) as a
where 
    attrs_attrs.id = a.id;

drop function ccc();
select f_safe_drop_type('h_attrs_attrs1');


select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacinsert', 'before', 'insert or update', 'attrs_categories', 'acinsertcheck()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
select f_safe_drop_trigger('trgsyncrecords', 'io_objects');



select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');
select f_create_trigger('trgsyncrecords', 'before', 'insert or update or delete', 'io_objects', 'syncrecords()');

select setAsLogging(1);
select setAsLogging(2);

----------
----------


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ


--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
