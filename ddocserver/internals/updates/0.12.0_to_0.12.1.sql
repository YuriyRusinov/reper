\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql


--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql



select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql
\i ./functions/security/readd_security.sql


insert into io_types (id, name, r_name, description) values(12, 'Шаблон поискового запроса', 'Шаблон поискового запроса', 'Используется в качестве типа для категорий, экземпляры которых обрабатываются как поисковые запросы');
SELECT pg_catalog.setval('io_types_id_seq', 12, true);


select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-176', 176, 10, NULL, false, 'Операторы сравнения', 'SYSCATEGORY_176', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(175, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-177', 177, 8, 176, true, 'Справочник операторов сравнения', 'SYSCATEGORY_177', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(176, -10, true, true, false, false, true);

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (176, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (176, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (177, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (177, 2, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-291', 291, 9, 'the_value', 'Значение', 'Значение', NULL, NULL, 150, TRUE);

--таблица операторов сравнения
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (176, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (176, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (176, 291, NULL, true, false); --the_value


alter table operations add column the_value varchar;
update operations set the_value = name;
alter table operations alter column the_value set not null;

alter table operations add column unique_id varchar, add column last_update timestamp;
update operations set unique_id = generateUID(id, 'operations'), last_update = current_timestamp;
alter table operations alter column unique_id set not null;
alter table operations alter column last_update set not null;
alter table operations alter column last_update set default current_timestamp;
alter table operations inherit root_table;
select createTriggerUID('operations');


select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-86', 86, 177, 1, 1, 'Справочник операторов сравнения', 'operations', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (86, -10, TRUE, TRUE, FALSE, FALSE);


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');



/*

select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-171', 171, 10, NULL, false, 'Группы атрибутов (пользовательские)', 'SYSCATEGORY_171', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-172', 172, 8, 171, true, 'Справочник групп атрибутов', 'SYSCATEGORY_172', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-173', 173, 10, NULL, false, 'Типы должностных лиц', 'SYSCATEGORY_173', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-174', 174, 8, 173, true, 'Справочник типов должностных лиц', 'SYSCATEGORY_174', NULL::varchar, true, true, 1);

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (171, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (171, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (172, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (172, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (173, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (173, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (174, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (174, 2, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-284', 284, 2, 'id_attr_group', 'Группа', 'Группа', 'attrs_groups', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-285', 285, 2, 'id_position_type', 'Тип ДЛ', 'Тип ДЛ', 'position_types', 'name', 150, TRUE);

--таблица групп атрибутов (пользовательских)
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (171, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (171, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (171, 27, NULL, false, false); --id_parent

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (9, 284, 2, true, false);--id_attr_group

--таблица типов должностей
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (173, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (173, 2, NULL, true, false); --name

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (51, 285, 1, true, false);--id_position_type

select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-83', 83, 172, 1, 1, 'Справочник групп атрибутов', 'attrs_groups', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (83, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-84', 84, 174, 1, 1, 'Справочник типов должностей', 'position_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (84, -10, TRUE, TRUE, FALSE, FALSE);

select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');
*/

commit;
