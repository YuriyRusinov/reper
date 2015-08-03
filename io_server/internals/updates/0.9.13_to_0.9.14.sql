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

\i ./functions/io_objects/checkioforowner.sql


alter table command_journal add column read_datetime_e timestamp;
alter table command_journal add column read_datetime_c timestamp;

drop table io_categories_organization;
delete from attrs_categories where id_io_category = 11 and id_io_attribute = 98;
delete from attributes where id = 98;


create table queue_results (
   id                   SERIAL               not null,
   id_transport         INT4                 not null,
   id_external_queue    INT4                 not null,
   sync_result          INT4                 not null default 4,
   address              varchar              not null,
   is_read              INT4                 not null default 1,
   constraint PK_QUEUE_RESULTS primary key (id)
);

alter table queue_results
   add constraint FK_QUEUE_RE_REFERENCE_TRANSPOR foreign key (id_transport)
      references transport (id)
      on delete restrict on update restrict;


create table access_categories_table (
   id_io_category       INT4                 not null,
   id_role              INT4                 not null,
   allow_read           BOOL                 not null default FALSE,
   allow_readlist       BOOL                 not null default FALSE,
   allow_delete         BOOL                 not null default FALSE,
   allow_update         BOOL                 not null default FALSE,
   allow_use            BOOL                 not null default FALSE,
   constraint PK_ACCESS_CATEGORIES_TABLE primary key (id_io_category, id_role)
);

comment on table access_categories_table is
'Данная таблица хранит информацию о правах доступа ролей к категориям';

alter table access_categories_table
   add constraint FK_ACCESS_C_REFERENCE_KKS_ROLE foreign key (id_role)
      references kks_roles (id)
      on delete restrict on update restrict;

alter table access_categories_table
   add constraint FK_ACCESS_C_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table access_table add column allow_send bool;
update access_table set allow_send = FALSE;
alter table access_table alter column allow_send set not null;
alter table access_table alter column allow_send set default false;

alter table acl_template_exceptions add column allow_use1 bool;
update acl_template_exceptions set allow_use1 = FALSE;
alter table acl_template_exceptions alter column allow_use1 set not null;
alter table acl_template_exceptions alter column allow_use1 set default false;

alter table acl_template_groups add column allow_use1 bool;
update acl_template_groups set allow_use1 = FALSE;
alter table acl_template_groups alter column allow_use1 set not null;
alter table acl_template_groups alter column allow_use1 set default false;

alter table acl_system_template_groups add column allow_use1 bool;
update acl_system_template_groups set allow_use1 = FALSE;
alter table acl_system_template_groups alter column allow_use1 set not null;
alter table acl_system_template_groups alter column allow_use1 set default false;

alter table command_journal add column extra_table VARCHAR;
alter table command_journal add column extra_table_id int4;


alter table alarm_events add column video_url varchar;
alter table alarm_events_q add column video_url varchar;
alter table alarm_events_q add column for_report bool;

select f_safe_drop_trigger('trgioupdate', 'io_objects');

alter table alarm_events_q add column is_deleted bool;
update alarm_events_q set is_deleted = false;
alter table alarm_events_q alter column is_deleted set default false;
alter table alarm_events_q alter column is_deleted set not null;

alter table fault_devices_q add column is_deleted bool;
update fault_devices_q set is_deleted = false;
alter table fault_devices_q alter column is_deleted set default false;
alter table fault_devices_q alter column is_deleted set not null;

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-217', 217, 9, 'video_url', 'Ссылка на видео-ролик', 'Ссылка на видео-ролик', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-218', 218, 1, 'for_report', 'Признак помещения в итоговый отчет', 'Признак помещения в итоговый отчет', NULL, NULL, 150, TRUE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (111, 217, NULL, false, false); --video_url
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 217, NULL, false, false); --video_url
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 218, NULL, false, false); --for_report

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-219', 219, 9, 'tso_state', 'Состояние ТСО', 'Состояние ТСО', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-220', 220, 18, 'tso_state_color', 'Состояние ТСО (цвет)', 'Состояние ТСО (цвет)', NULL, NULL, 150, TRUE);

--атрибуты для категорий интегральных справочников
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(134, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(134, 220, NULL, FALSE, FALSE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(136, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(136, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(138, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(138, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(140, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(140, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(142, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(142, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(102, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(102, 220, NULL, FALSE, FALSE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-221', 221, 1, 'is_deleted', 'Доклад об исполнении получен', 'Доклад об исполнении получен', NULL, NULL, 150, TRUE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 221, NULL, false, false); --is_deleted
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 221, NULL, false, false); --is_deleted


select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

select f_safe_drop_index('i_transport_name');

commit;
