\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

drop function if exists uSetMsgAsConfirmed(int4);
drop function if exists uSetMsgAsNotConfirmed(int4);

alter table message_journal disable trigger trgmsgjinsert;
alter table message_journal disable trigger trgsetuid;
alter table message_journal disable trigger zz_trg_out_message;

alter table message_journal drop column is_outed;
alter table message_journal add column is_outed int4;
alter table message_journal alter column is_outed set default 3;
alter table message_journal add constraint CKC_IS_OUTED_MESSAGE_ check (is_outed in (0,1,2,3));
update message_journal set is_outed = 3;
alter table message_journal alter column is_outed set not null;


alter table message_journal enable trigger trgmsgjinsert;
alter table message_journal enable trigger trgsetuid;
alter table message_journal enable trigger zz_trg_out_message;



--СЮДА ЗАПРОСЫ НА СОЗДАНИЕ ТАБЛИЦ СПРАВОЧНИКОВ (ЕСЛИ СОЗДАВАЛИСЬ ИО)
--ИХ ОБЯЗАТЕЛЬНО ПЕРЕД СОЗДАНИЕМ ФУНКЦИЙ!!!

alter table in_external_queue drop column if exists id_external;
alter table in_external_queue add column id_external INT8;
update in_external_queue set id_external = -1;
alter table in_external_queue alter column id_external set default -1;
alter table in_external_queue alter column id_external set not null;

drop function insertIncomeXML(int4, int4, int4, varchar);

drop table if exists notify_where;
create table notify_where (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_NOTIFY_WHERE primary key (id)
)
inherits (root_table);

comment on table notify_where is
'Когда квитанция должна генерироваться
1 - INSERT
2 - UPDATE
3 - DELETE
Используется в справочнике асинхронных квитанций в виде атрибута типа старые чекбоксы';

select setMacToNULL('notify_where');
select createTriggerUID('notify_where');

drop table if exists table_notifies_io_objects;
drop table if exists table_notifies;
/*==============================================================*/
/* Table: table_notifies                                        */
/*==============================================================*/
create table table_notifies (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   notify_where         int4[]               not null,
   description          VARCHAR              null,
   constraint PK_TABLE_NOTIFIES primary key (id)
)
inherits (root_table);

comment on table table_notifies is
'Справочник асинхронных квитанций, генерируемых сервером при создании, узменении и удалении тех или инх записей в справочниках (notify).
Название асинхронной квитанции должно быть уникальным, при этом допустимо генерировать квитанцию с одним именем для разных справочников.
Перечень справочников, для которых генерируется квитанция, задается в таблице table_notifies_io_objects (многие-ко-многим).
Асинхронная квитанция генерируется следующим образом
pg_notify(notify_name, payload)
payload формируется следующим образом:
tableName + ''~_~_~'' + idRecord + ''~_~_~'' + recordUniqueId + ''~_~_~'' +whatHappens
whatHappens формируется следующим образом:
1 - выполнена операция создания новой записи
2 - выполнена операция изменения записи
3 - выполнена операция удаления записи';

comment on column table_notifies.name is
'Название квитанции. Должно быть уникальным';

comment on column table_notifies.notify_where is
'Когда отправляется квитанция (старые чекбоксы)';

select setMacToNULL('table_notifies');
select createTriggerUID('table_notifies');

/*==============================================================*/
/* Index: i_u_table_notify_name                                 */
/*==============================================================*/
create unique index i_u_table_notify_name on table_notifies (
name
);

/*==============================================================*/
/* Table: table_notifies_io_objects                             */
/*==============================================================*/
create table table_notifies_io_objects (
   id_table_notifies      INT4                 not null,
   id_io_objects          INT4                 not null,
   constraint PK_TABLE_NOTIFIES_IO_OBJECTS primary key (id_table_notifies, id_io_objects)
);

alter table table_notifies_io_objects
   add constraint FK_TABLE_NO_REFERENCE_TABLE_NO foreign key (id_table_notifies)
      references table_notifies (id)
      on delete restrict on update restrict;

alter table table_notifies_io_objects
   add constraint FK_TABLE_NO_REFERENCE_IO_OBJEC foreign key (id_io_objects)
      references tbl_io_objects (id)
      on delete restrict on update restrict;

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

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-236', 236, 10, NULL, false, 'Моменты (условия) генерации асинхронных квитанций для справочников', 'SYSCATEGORY_236', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-237', 237, 8, 236, true, 'Справочник моментов (условий) генерации асинхронных квитанций для справочников', 'SYSCATEGORY_237', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-238', 238, 10, NULL, false, 'Асинхронные квитанции для справочников', 'SYSCATEGORY_238', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-239', 239, 8, 238, true, 'Справочник асинхронных квитанций для справочников', 'SYSCATEGORY_239', NULL::varchar, true, true, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-385', 385, 12, 'notify_where', 'Условия генерации квитанции', 'Условия генерации квитанции', 'notify_where', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-386', 386, 17, 'table_notifies_io_objects', 'Справочники, для которых генерируется квитанция', 'Справочники, для которых генерируется квитанция', 'io_objects', 'name', 200, TRUE);

--таблица моментов (условий) генерации асинхронных квитанций для справочников
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (701, 236, 1, NULL, true, true, 'ИД'); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (702, 236, 2, NULL, true, false, 'Название'); --name

--таблица асинхронных квитанций для справочников
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (703, 238, 1, NULL, true, true, 'ИД'); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (704, 238, 2, NULL, true, false, 'Название'); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (705, 238, 385, NULL, true, false, 'Условия генерации квитанции'); --notify_where
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (706, 238, 386, NULL, false, false, 'Справочники, для которых генерируется квитанция'); --table_notifies_io_objects
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (707, 238, 3, NULL, false, false, 'Описание');  --description

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacinsert', 'before', 'insert or update', 'attrs_categories', 'acinsertcheck()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
select f_safe_drop_trigger('trgsyncrecords', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-116', 116, 237, 1, 1, 'Справочник моментов (условий) генерации асинхронных квитанций для справочников', 'notify_where', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (116, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-117', 117, 239, 1, 1, 'Справочник асинхронных квитанций для справочников', 'table_notifies', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (117, -10, TRUE, TRUE, FALSE, FALSE);


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');
select f_create_trigger('trgsyncrecords', 'before', 'insert or update or delete', 'io_objects', 'syncrecords()');

select setAsLogging(1);
select setAsLogging(2);

----------
----------


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ

insert into notify_where(id, name) values (1, 'insert');
insert into notify_where(id, name) values (2, 'update');
insert into notify_where(id, name) values (3, 'delete');

SELECT pg_catalog.setval('notify_where_id_seq', 3, true);

insert into a_types (id, id_a_view, name, code) values (38, 14, 'Бинарные данные', 'BYTEA');
SELECT pg_catalog.setval('a_types_id_seq', 38, true);


--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
