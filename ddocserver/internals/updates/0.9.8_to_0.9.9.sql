\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');


select f_safe_drop_trigger('trgsyncrecords', 'organization');
select f_safe_drop_trigger('trgsyncrecords', 'organization_transport');
select f_safe_drop_trigger('trgsyncrecords', 'search_templates');
select f_safe_drop_trigger('trgsyncrecords', 'io_templates');
select f_safe_drop_trigger('trgsyncrecords', 'io_last_sync');
select f_safe_drop_trigger('trgsyncrecords', 'io_sync_archive');
select f_safe_drop_trigger('trgsyncrecords', 'io_sync_types');
select f_safe_drop_trigger('trgsyncrecords', 'jr_states');
select f_safe_drop_trigger('trgsyncrecords', 'io_templates');
select f_safe_drop_trigger('trgsyncrecords', 'mail_lists');
select f_safe_drop_trigger('trgsyncrecords', 'organization_type');
select f_safe_drop_trigger('trgsyncrecords', 'ranks');
select f_safe_drop_trigger('trgsyncrecords', 'tsd_control_journal');
select f_safe_drop_trigger('trgsyncrecords', 'urgency_levels');
select f_safe_drop_trigger('trgsyncrecords', 'url_extentions');
select f_safe_drop_trigger('trgsyncrecords', 'user_state');
select f_safe_drop_trigger('trgsyncrecords', 'work_mode');
select f_safe_drop_trigger('trgsyncrecords', 'work_mode_type');
select f_safe_drop_trigger('trgsyncrecords', 'command_journal');
select f_safe_drop_trigger('trgsyncrecords', 'message_journal');

insert into jr_states (id, name, description) values (7, 'Виртуальное состояние', NULL);

SELECT pg_catalog.setval('jr_states_id_seq', 7, true);


alter table alarm_events_q add column org_name varchar;
alter table alarm_events_q add column org_uid varchar;

alter table fault_devices_q add column org_name varchar;
alter table fault_devices_q add column org_uid varchar;

alter table dev_object_q add column org_name varchar;
alter table dev_object_q add column org_uid varchar;

alter table personal_q add column org_name varchar;
alter table personal_q add column org_uid varchar;

alter table access_cards_q add column org_name varchar;
alter table access_cards_q add column org_uid varchar;

alter table access_plan add column org_name varchar;
alter table access_plan add column org_uid varchar;


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-208', 208, 9, 'org_name', 'Наименование части', 'Наименование части', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-209', 209, 9, 'org_uid', 'UID части', 'UID части', NULL, NULL, 120, TRUE);

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (101, 208, NULL, false, false); --org_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (101, 209, NULL, false, false); --org_uid

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 208, NULL, false, false); --org_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 209, NULL, false, false); --org_uid

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 208, NULL, false, false); --org_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 209, NULL, false, false); --org_uid

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 208, NULL, false, false); --org_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 209, NULL, false, false); --org_uid

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 208, NULL, false, false); --org_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 209, NULL, false, false); --org_uid

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 208, NULL, false, false); --org_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 209, NULL, false, false); --org_uid

delete from io_views where id_io_category = 13 and id_io_template = 1 and id_io_attribute in(112, 111);


insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) 
values ('localorg-categories-151', 151, 10, NULL, false, 'Настройки системы ТСО', 'SYSCATEGORY_151', NULL::varchar, true, false, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) 
values ('localorg-categories-152', 152, 8, 151, true, 'Справочник справочник настроек системы ТСО', 'SYSCATEGORY_152', NULL::varchar, true, false, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-210', 210, 8, 'tso_max_level', 'Максимальный уровень тревоги', 'Максимальный уровень тревоги', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-211', 211, 9, 'tso_dl_from', 'UID должностного лица, отправителя распоряжений', 'UID должностного лица, отправителя распоряжений', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-212', 212, 9, 'tso_dl_exec', 'UID должностного лица, исполнителя распоряжений', 'UID должностного лица, исполнителя распоряжений', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-213', 213, 9, 'tso_dl_to', 'UID должностного лица, контроллера распоряжений', 'UID должностного лица, контроллера распоряжений', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-214', 214, 9, 'tso_category', 'UID категории доклада об устронении', 'UID категории доклада об устронении', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-215', 215, 5, 'tso_exectime', 'Время доклада об устронении', 'Время доклада об устронении', NULL, NULL, 120, TRUE);

create table tso_params (
   id                   SERIAL               not null,
   tso_max_level        INT4                 not null,
   tso_dl_from          VARCHAR              not null,
   tso_dl_exec          VARCHAR              not null,
   tso_dl_to            VARCHAR              not null,
   tso_category         VARCHAR              not null,
   tso_exectime         VARCHAR              not null,
   constraint PK_TSO_PARAMS primary key (id)
)
inherits (root_table);

comment on table tso_params is
'Настройки системы ТСО';

select setMacToNULL('tso_params');
select createTriggerUID('tso_params');

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 210, NULL, true, true); --max_level
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 211, NULL, true, true); --tso_dl_from
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 212, NULL, true, true); --tso_dl_exec
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 213, NULL, true, true); --tso_dl_to
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 214, NULL, true, true); --tso_category
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 215, NULL, true, true); --tso_exectime

select cSetCompleted(151, 1);
select cSetCompleted(152, 1);
update io_categories set is_global = true where id in(151, 152);

select f_safe_drop_trigger('trgioinsert', 'io_objects');

\i ./functions/io_objects/checkioforowner.sql

insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-74', 74, 152, 1, 1, 'Справочник настроек системы ТСО', 'q_tso_params', 'tso_params', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (74, -10, TRUE, TRUE, FALSE, FALSE);

CREATE TRIGGER trgioinsert
  BEFORE INSERT OR UPDATE
  ON io_objects
  FOR EACH ROW
  EXECUTE PROCEDURE ioinsertcheck();


alter table io_objects add column id_search_template int4;
alter table io_objects add column ref_table_name varchar;

alter table io_objects
   add constraint FK_IO_OBJEC_REFERENCE_SEARCH_T foreign key (id_search_template)
      references search_templates (id)
      on delete restrict on update restrict;

alter table object_ref_tables add column id_search_template int4;
alter table object_ref_tables add column ref_table_name varchar;

alter table object_ref_tables
   add constraint FK_IO_OBJEC_REFERENCE_SEARCH_T foreign key (id_search_template)
      references search_templates (id)
      on delete restrict on update restrict;

alter table io_objects drop column code;
delete from attrs_categories where id_io_category = 13 and id_io_attribute = 6;

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-216', 216, 9, 'ref_table_name', 'Название базовой таблицы', 'Название базовой таблицы', NULL, NULL, 120, TRUE);

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, 110, NULL, false, false);--id_search_template
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, 216, NULL, false, false);--ref_table_name

--insert into a_groups (id_io_template, id_parent, name, "order") values(2, NULL, 'Основные атрибуты', 0);

/*
update io_views set id_a_group = 4 where id_io_category = 9 and id_io_template = 2;-- and id_a_group = 3;

delete from io_views where id_io_category = 13 and id_io_attribute = 117;
delete from a_groups where id_io_template = 1 and name = 'Синхронизация';
insert into io_views (id_io_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value) values(13, 117, 1, 2, true, 4, NULL);

insert into a_groups (id, id_io_template, id_parent, name, "order") values(nextval('a_groups_id_seq'), 1, NULL, 'Базовый справочник', 3);

insert into io_views (id_io_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value) values(13, 110, 1, currval('a_groups_id_seq'), true, 0, NULL);
insert into io_views (id_io_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value) values(13, 216, 1, currval('a_groups_id_seq'), true, 1, NULL);

*/

create trigger zz_trgZIOInserttableAfter
after insert
on io_objects
for each row
execute procedure ioInsertCheckTableAfter();

select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

commit;
