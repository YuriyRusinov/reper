\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);


create table time_units (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   constraint PK_TIME_UNITS primary key (id)
)
inherits (root_table);

comment on table time_units is
'Единицы измерения времени';

select setMacToNULL('time_units');
select createTriggerUID('time_units');




alter table message_streams add column id_time_unit int4 not null default 1;
alter table message_streams add constraint fk_time_units FOREIGN KEY (id_time_unit)
    REFERENCES time_units (id);

create table message_series (
   id                   SERIAL               not null,
   id_message_stream    INT4                 not null,
   "time"               TIMESTAMP            not null,
   time_step            FLOAT8               not null default 1.0,
   constraint PK_MESSAGE_SERIES primary key (id)
)
inherits (root_table);

select setMacToNULL('message_series');
select createTriggerUID('message_series');

alter table message_series
   add constraint FK_MESSAGE_SERIES__REF_MESSAGE_STREAMS foreign key (id_message_stream)
      references message_streams (id)
      on delete restrict on update restrict;



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

select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/zarya22/readd_zarya_func.sql


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-205', 205, 10, NULL, false, 'Единицы измерения времени', 'SYSCATEGORY_205', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-206', 206, 8, 205, true, 'Справочник единиц измерения времени', 'SYSCATEGORY_206', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-207', 207, 10, NULL, false, 'Серии потоков сообщений', 'SYSCATEGORY_207', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-208', 208, 8, 207, true, 'Справочник серий потоков сообщений', 'SYSCATEGORY_208', NULL::varchar, true, true, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-337', 337, 2, 'id_time_unit', 'Единицы измерения времени', 'Единицы измерения времени', 'time_units', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-338', 338, 5, 'time', 'Момент времени', 'Момент времени', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-339', 339, 6, 'time_step', 'Временной шаг', 'Временной шаг', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-340', 340, 2, 'id_message_stream', 'Поток сообщений', 'Поток сообщений', 'message_streams', 'name', 150, TRUE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (584, 193, 337, '1', true, false); --id_time_unit

--таблица единиц измерения времени
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (585, 205, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (586, 205, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (587, 205, 19, NULL, true, false); --short_name

--таблица серий потоков сообщений
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (588, 207, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (589, 207, 340, NULL, true, false); --id_message_stream
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (590, 207, 338, NULL, true, false); --time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (591, 207, 339, NULL, true, false); --time_step

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-100', 100, 206, 1, 1, 'Справочник единиц измерения времени', 'time_units', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (100, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-101', 101, 208, 1, 1, 'Справочник серий потоков сообщений', 'message_series', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (101, -10, TRUE, TRUE, FALSE, FALSE);


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--СЮДА ЗАПРОСЫ НА СОЗДАНИЕ ТАБЛИЦ СПРАВОНИКОВ (ЕСЛИ СОЗДАВАЛИСЬ ИО)


----------
----------


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ

insert into partition_lows (id, name, description) values (1, 'Нормальное распределение', 'Gaussian distribution');
insert into partition_lows (id, name, description) values (2, 'Экспоненциальное распределение', 'Exponential distribution');
insert into partition_lows (id, name, description) values (3, 'Равномерное распределение', 'Uniform distribution');

select setval('partition_lows_id_seq', 3, true);

insert into time_units (id, name, short_name) values (1, 'seconds', 's');
insert into time_units (id, name, short_name) values (2, 'minutes', 'min');
insert into time_units (id, name, short_name) values (3, 'hours', 'h');
insert into time_units (id, name, short_name) values (4, 'days', 'd');

select setval('time_units_id_seq', 4, true);


--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
