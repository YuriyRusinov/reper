\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

--СЮДА ЗАПРОСЫ НА СОЗДАНИЕ ТАБЛИЦ СПРАВОНИКОВ (ЕСЛИ СОЗДАВАЛИСЬ ИО)
--ИХ ОБЯЗАТЕЛЬНО ПЕРЕД СОЗДАНИЕМ ФУНКЦИЙ!!!
create table interaction_formats (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_INTERACTION_FORMATS primary key (id)
)
inherits (root_table);

comment on table interaction_formats is
'Форматы представления данных (информационных объектов, записей справочников), используемые для взаимодействия с внешними системами
Варианты:
1) unified XML format (унифицированный формат описания информационных объектов)
2) param-value representation (информационный объект, запись справочника предстают в виде набора атрибутов, описываемых в виде атрибут-строковое представление значения). Используется для простого базового взаимодействия. Проверка на соответствие структурам данных не осуществляется
3) shushun (формат представления данных ЦНИИ ЭИСУ в ОКР "Заря 22")
4) internal (внутренний формат представления данных при взаимодействии между системами DynamicDocs)
';

select setMacToNULL('interaction_formats');
select createTriggerUID('interaction_formats');


create table out_external_queue (
   id                   BIGSERIAL            not null,
   id_organization      INT4                 not null,
   id_format            INT4                 not null,
   id_io_object         int8                 not null,
   id_entity            int8                 null,
   interaction_type     int4                 not null,
   interaction_result   int4                 not null,
   constraint PK_OUT_EXTERNAL_QUEUE primary key (id)
)
inherits (root_table);

comment on table out_external_queue is
'Исходящая очередь для единоразовой отправки информационных объектов (записей справочников) во внешние взаимодействующие системы (не DynamicDocs)';

comment on column out_external_queue.id_organization is
'В какую организацию отправляем';

comment on column out_external_queue.id_format is
'При помощи какого формата представлять данные';

comment on column out_external_queue.id_io_object is
'Какой информационный рескрс передавать';

comment on column out_external_queue.id_entity is
'Если указано значение, то передается не сам информационный ресурс, а запись его справочника (для информационных ресурсов, являющихся справочниками)';

comment on column out_external_queue.interaction_type is
'Тип взаимодействия
1 - создать новую сущность на целевом объекте
2 - заменить сущность на данную на целевом объекте (если она не существует, создать новую)
3 - удалить сущность на целевом объекте
';

comment on column out_external_queue.interaction_result is
'Результаты информационного взаимодействия (передачи данных)
1 - ожидается передача (стоит в очереди на отправку). Это значение по умолчанию.
2 - передача осуществляется (информационный обмен еще не завершен)
3 - передача завершилась успешно
4 - произошла ошибка';

select setMacToNULL('out_external_queue');
select createTriggerUID('out_external_queue');

alter table out_external_queue
   add constraint FK_OUT_EXTE_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table out_external_queue
   add constraint FK_OUT_EXTE_REFERENCE_INTERACT foreign key (id_format)
      references interaction_formats (id)
      on delete restrict on update restrict;



create table in_external_queue (
   id                   BIGSERIAL            not null,
   id_organization      INT4                 not null,
   id_format            INT4                 not null,
   in_data              VARCHAR              not null,
   interaction_type     int4                 not null,
   interaction_result   int4                 not null,
   constraint PK_IN_EXTERNAL_QUEUE primary key (id)
)
inherits (root_table);

comment on table in_external_queue is
'Входящая очередь для единоразового получения информационных ресурсов (записей справочников) из внешних взаимодействующих систем (не DynamicDocs)';

comment on column in_external_queue.id_organization is
'Организация, из которой поступили данные';

comment on column in_external_queue.id_format is
'формат представления данных';

comment on column in_external_queue.in_data is
'Строкове представление входящих данных';

comment on column in_external_queue.interaction_type is
'Тип взаимодействия
1 - создать новую сущность на целевом объекте
2 - заменить сущность на данную на целевом объекте (если она не существует, создать новую)
3 - удалить сущность на целевом объекте
';

comment on column in_external_queue.interaction_result is
'Результаты сохранения информационного ресурса в БД
1 - Данные только поступили, их обработка еще не начиналась (значение по умолчанию).
2 - создание информационного ресурса завершилась успешно
3 - произошла ошибка при создании информационного ресурса в БД';

select setMacToNULL('in_external_queue');
select createTriggerUID('in_external_queue');

alter table in_external_queue
   add constraint FK_IN_EXTER_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table in_external_queue
   add constraint FK_IN_EXTER_REFERENCE_INTERACT foreign key (id_format)
      references interaction_formats (id)
      on delete restrict on update restrict;

insert into interaction_formats (id, name, description) values (1, 'Унифицированный обменный XML-формат описания информационных ресурсов в системе DynamicDocs', NULL);
insert into interaction_formats (id, name, description) values (2, 'Простой формат представления на основе шаблона PARAM=VALUE', 'Информационный ресурс, запись справочника предстают в виде набора атрибутов, описываемых в виде атрибут-строковое представление значения. Используется для простого базового взаимодействия. Проверка на соответствие структурам данных не осуществляется');
insert into interaction_formats (id, name, description) values (3, 'XML-Формат ОКР "Заря-22"', 'формат представления данных ЦНИИ ЭИСУ в ОКР "Заря-22"');
insert into interaction_formats (id, name, description) values (4, 'Внутренний бинарный формат представления данных при взаимодействии между системами DynamicDocs', NULL);

SELECT pg_catalog.setval('interaction_formats_id_seq', 4, true);

---------------------------
---------------------------

drop function if exists xGetRecordFields(int4);

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


select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/readd_tasks_func.sql


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');
select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-230', 230, 10, NULL, false, 'Форматы информационного взаимодействия с внешними системами ', 'SYSCATEGORY_230', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-231', 231, 8, 230, true, 'Справочник форматов информационного взаимодействия с внешними системами', 'SYSCATEGORY_231', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-232', 232, 10, NULL, false, 'Таблицы исходящей очереди информационного взаимодействия с внешними системами', 'SYSCATEGORY_232', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-233', 233, 8, 232, true, 'Исходящая очередь информационного взаимодействия с внешними системами', 'SYSCATEGORY_233', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-234', 234, 10, NULL, false, 'Таблицы входящей очереди информационного взаимодействия с внешними системами', 'SYSCATEGORY_234', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-235', 235, 8, 234, true, 'Входящая очередь информационного взаимодействия с внешними системами', 'SYSCATEGORY_235', NULL::varchar, true, true, 1);


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-379', 379, 2, 'id_format', 'Формат информационного взаимодействия', 'Формат взаимодействия', 'interaction_formats', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-380', 380, 30, 'id_entity', 'Идентификатор записи', 'Идентификатор записи', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-381', 381, 8, 'interaction_type', 'Тип взаимодействия', 'Тип взаимодействия', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-382', 382, 8, 'interaction_result', 'Результат взаимодействия', 'Результат взаимодействия', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-383', 383, 13, 'in_data', 'Пришедшие данные', 'Пришедшие данные', NULL, NULL, 100, TRUE);

--исходящая очередь информационного взаимодействия с внешними системами
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (683, 232, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (684, 232, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (685, 232, 379, NULL, true, false); --id_format
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (686, 232, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (687, 232, 380, NULL, false, false); --id_entity
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (688, 232, 381, NULL, true, false); --interaction_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (689, 232, 382, NULL, true, false); --interaction_result

--входящая очередь информационного взаимодействия с внешними системами
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (690, 234, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (691, 234, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (692, 234, 379, NULL, true, false); --id_format
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (693, 234, 383, NULL, true, false); --in_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (694, 234, 381, NULL, true, false); --interaction_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (695, 234, 382, NULL, true, false); --interaction_result

--таблица фопматов информационного взаимодействия с внешними системами
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (696, 230, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (697, 230, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (698, 230, 3, NULL, false, false);  --description

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-113', 113, 231, 1, 1, 'Справочник форматов информационного взаимодействия с внешними системами', 'interaction_formats', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (113, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-114', 114, 233, 1, 1, 'Исходящая очередь информационного взаимодействия с внешними системами', 'out_external_queue', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (114, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-115', 115, 235, 1, 1, 'Входящая очередь информационного взаимодействия с внешними системами', 'in_external_queue', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (115, -10, TRUE, TRUE, FALSE, FALSE);

select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');


----------
----------


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ

--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
