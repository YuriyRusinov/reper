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
        SELECT pg_catalog.setval('a_types_id_seq', 39, true);
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

    return;
end
$BODY$
language 'plpgsql';

select aaa();
drop function aaa();   

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
