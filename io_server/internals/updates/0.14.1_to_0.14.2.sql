\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql

\i ./functions/contribs/readd_contribs.sql

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/categories/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql

drop function getIDByUID(varchar, varchar);
drop function eioInsertIndicator(int4, int4, varchar, timestamp, timestamp, timestamp, int4, int4, varchar);
drop function eioUpdateIndicator(int4, int4, varchar, timestamp, timestamp, timestamp, int4, int4, varchar);
drop function eioCatIndExist(int4, int4);
drop function putRecIntoRubric (int4, int4);
drop function recDeleteRubrics(int4);
drop function recDeleteIncludes(int4, int4[]);



select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/zarya22/readd_zarya_func.sql

--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
--select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
--сюда
--select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
--сюда
select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ
insert into a_views (id, name) values (16, 'ГИС-объект (базовый)');
insert into a_views (id, name) values (17, 'ГИС-объект (стандартный полигон)');
insert into a_types (id, id_a_view, name, code) values (28, 16, 'ГИС-объект (базовый)', 'GEOMETRY');
insert into a_types (id, id_a_view, name, code) values (29, 17, 'ГИС-объект (стандартный полигон)', 'GEOMETRY');
insert into a_types (id, id_a_view, name, code) values (30, 5, 'Целочисленное значение (64 бита)', 'INT8');
insert into a_types (id, id_a_view, name, code) values (31, 5, 'Уникальный идентификатор (UUID)', 'UUID');

SELECT pg_catalog.setval('a_views_id_seq', 17, true);
SELECT pg_catalog.setval('a_types_id_seq', 31, true);

delete from a_types where id in (7, 11, 16, 22);
update a_types set name = 'Изображение' where id = 15;
update a_types set name = 'Целочисленное значение (32 бита)' where id = 8;
update a_types set code = 'INT8' where id in (2, 3, 19, 26);
update a_types set code = 'INT8[]' where id in (12, 17);

alter table io_urls add column uploaded bool;
update io_urls set uploaded = true;
alter table io_urls alter column uploaded set default false;
alter table io_urls alter column uploaded set not null;

drop index i_url;
create unique index i_url on io_urls using BTREE (
url
)
where url <> '' and url <> 'not assigned';


--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
