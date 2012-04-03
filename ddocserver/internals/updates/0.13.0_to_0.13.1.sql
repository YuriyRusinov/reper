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

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-189', 189, 10, NULL, false, 'Атрибуты в атрибутах', 'SYSCATEGORY_189', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-190', 190, 8, 189, true, 'Справочник атрибутов в атрибутах', 'SYSCATEGORY_190', NULL::varchar, true, true, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (189, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (189, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (190, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (190, 2, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-308', 308, 2, 'id_attr_attr', 'Атрибут', 'Атрибут', 'attrs_attrs', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-309', 309, 2, 'id_attr_value', 'Значение атрибута', 'Значение атрибута', 'attrs_values', 'value', 150, TRUE);

delete from attrs_categories where id_io_category = 187; --attrs_attrs_values
--таблица значений атрибутов в атрибутах
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (526, 187, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (527, 187, 309, NULL, true, false); --id_attr_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (528, 187, 308, NULL, true, false); --id_attr_attr
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (529, 187, 301, NULL, true, false); --value

--таблица атрибутов в атрибутах
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (519, 189, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (520, 189, 306, NULL, true, false); --id_attr_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (521, 189, 307, NULL, true, false); --id_attr_child
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (522, 189, 2, NULL, false, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (523, 189, 304, NULL, false, false); --def_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (524, 189, 128, 'false', true, false); --is_mandatory
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (525, 189, 305, 'false', true, false); --is_read_only

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-92', 92, 190, 1, 1, 'Справочник атрибутов в атрибутах', 'attrs_attrs', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (92, -10, TRUE, TRUE, FALSE, FALSE);

select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

drop table attrs_attrs_values;
create table attrs_attrs (
   id                   SERIAL               not null,
   id_attr_parent       INT4                 null,
   id_attr_child        INT4                 null,
   name                 VARCHAR              null,
   def_value            VARCHAR              null,
   is_mandatory         BOOL                 null,
   is_read_only         BOOL                 null,
   constraint PK_ATTRS_ATTRS primary key (id)
)
inherits (root_table);

select setMacToNULL('attrs_attrs');
select createTriggerUID('attrs_attrs');

create  index i_aav_parent on attrs_attrs (
id_attr_parent
);

create unique index i_aav_parent_child on attrs_attrs (
id_attr_parent,
id_attr_child
);

alter table attrs_attrs
   add constraint FK_ATTRS_ATTRS_PARENT foreign key (id_attr_parent)
      references attributes (id)
      on delete restrict on update restrict;

alter table attrs_attrs
   add constraint FK_ATTRS_ATTRS_CHILD foreign key (id_attr_child)
      references attributes (id)
      on delete restrict on update restrict;

create table attrs_attrs_values (
   id                   SERIAL               not null,
   id_attr_value        INT4                 not null,
   id_attr_attr         INT4                 not null,
   value                VARCHAR              not null,
   constraint PK_ATTRS_ATTRS_VALUES primary key (id)
);

alter table attrs_attrs_values
   add constraint FK_ATTRS_AT_REFERENCE_ATTRS_VA foreign key (id_attr_value)
      references tbl_attrs_values (id)
      on delete restrict on update restrict;

alter table attrs_attrs_values
   add constraint FK_ATTRS_AT_REFERENCE_ATTRS_AT foreign key (id_attr_attr)
      references attrs_attrs (id)
      on delete restrict on update restrict;


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ

--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
