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


create table attrs_groups (
   id                   SERIAL               not null,
   id_parent            INT4                 null,
   name                 VARCHAR              not null,
   constraint PK_ATTRS_GROUPS primary key (id)
)
inherits (root_table);


select setMacToNULL('attrs_groups');
select createTriggerUID('attrs_groups');


create table position_types(
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_POSITION_TYPES primary key (id)
)
inherits (root_table);


select setMacToNULL('position_types');
select createTriggerUID('position_types');

alter table attrs_groups
   add constraint FK_ATTRS_GR_REFERENCE_ATTRS_GR foreign key (id_parent)
      references attrs_groups (id)
      on delete restrict on update restrict;


insert into attrs_groups(name) values('Системные атрибуты'); --1
insert into attrs_groups(name) values('Прочие атрибуты');  --2

insert into position_types(name) values ('ДЛ в системе DynamicDocs');
insert into position_types(name) values ('ДЛ во внешней сопрягаемой системе');

alter table attributes add column id_attr_group int4;
alter table attributes
   add constraint FK_ATTRS_REFERENCE_GROUPS foreign key (id_attr_group)
      references attrs_groups(id)
      on delete restrict on update restrict;

alter table "position" add column id_position_type int4;
alter table "position"
   add constraint FK_POSITION_REFERENCE_TYPES foreign key (id_position_type)
      references position_types(id)
      on delete restrict on update restrict;

update attributes set id_attr_group = 1 where id < 300;
update attributes set id_attr_group = 2 where id >= 300;

alter table attributes alter column id_attr_group set not null;
alter table attributes alter column id_attr_group set default 2;

select f_safe_drop_trigger('trgdlinsert', 'position');
update "position" set id_position_type = 1;
select f_create_trigger('trgdlinsert', 'after', 'insert or update', 'position', 'dlinsertcheck()');


alter table "position" alter column id_position_type set not null;
alter table "position" alter column id_position_type set default 1;

commit;
