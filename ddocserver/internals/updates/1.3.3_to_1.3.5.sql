\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql
\i ./functions/general/createtriggeruidex.sql


/*==============================================================*/
/* Table: sheduled_handlers                                     */
/*==============================================================*/
create table sheduled_handlers (
   id                   SERIAL               not null,
   id_handler           INT4                 not null,
   name                 VARCHAR              not null,
   exec_period          INT4[2]              not null,
   last_exec            TIMESTAMP            null,
   constraint PK_SHEDULED_HANDLERS primary key (id)
)
inherits (root_table);

comment on table sheduled_handlers is
'Перечень сервисов, запускаемых периодически';

comment on column sheduled_handlers.id_handler is
'Запускаемый сервис';

comment on column sheduled_handlers.name is
'Название запускаемого сервиса (если не задано - при создании по умолчанию возьмется из справочника сервисов)';

comment on column sheduled_handlers.exec_period is
'периодичность выполнения данного сервиса
';

comment on column sheduled_handlers.last_exec is
'Дата и время последнего вызова';

select setMacToNULL('sheduled_handlers');
select createTriggerUID('sheduled_handlers');

alter table sheduled_handlers
   add constraint FK_SHEDULED_REFERENCE_HANDLERS foreign key (id_handler)
      references handlers (id)
      on delete restrict on update restrict;



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

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-228', 228, 10, NULL, false, 'Расписание периодического запуска сервисов', 'SYSCATEGORY_228', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-229', 229, 8, 228, true, 'Расписание периодического запуска сервисов', 'SYSCATEGORY_229', NULL::varchar, true, true, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-377', 377, 37, 'last_exec', 'Дата и время последнего запуска сервиса', 'Дата и время последнего запуска', NULL, NULL, 120, TRUE);

--таблица расписания периодического запуска сервисов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (678, 228, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (679, 228, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (680, 228, 328, NULL, true, false); --id_handler
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (681, 228, 38, NULL, true, false); --exec_period
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (682, 228, 377, NULL, false, true); --last_exec


select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-112', 112, 229, 1, 1, 'Расписание периодического запуска сервисов', 'sheduled_handlers', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (112, -10, TRUE, TRUE, FALSE, FALSE);


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--СЮДА ЗАПРОСЫ НА СОЗДАНИЕ ТАБЛИЦ СПРАВОНИКОВ (ЕСЛИ СОЗДАВАЛИСЬ ИО)


----------
----------


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ

--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
