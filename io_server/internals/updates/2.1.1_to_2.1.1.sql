\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

--���� ������� �� �������� ������ ������������ (���� ����������� ��)
--�� ����������� ����� ��������� �������!!!

alter table table_notifies drop column if exists id_position;
alter table table_notifies add column id_position int4;
alter table table_notifies
   add constraint FK_TABLE_NO_REFERENCE_POSITION foreign key (id_position)
      references "position" (id)
      on delete restrict on update restrict;

alter table table_notifies drop column if exists id_unit;
alter table table_notifies add column id_unit int4;
alter table table_notifies
   add constraint FK_TABLE_NO_REFERENCE_UNITS foreign key (id_unit)
      references units (id)
      on delete restrict on update restrict;

alter table table_notifies drop column if exists id_search_template;
alter table table_notifies add column id_search_template int4;
alter table table_notifies
   add constraint FK_TABLE_NO_REFERENCE_SEARCH_T foreign key (id_search_template)
      references search_templates (id)
      on delete restrict on update restrict;

alter table table_notifies drop column if exists is_accept;
alter table table_notifies add column is_accept bool;
alter table table_notifies alter column is_accept set default true;
update table_notifies set is_accept = true;
alter table table_notifies alter column is_accept set not null;

alter table table_notifies_io_objects
   drop constraint FK_TABLE_NO_REFERENCE_TABLE_NO;

alter table table_notifies_io_objects
   add constraint FK_TABLE_NO_REFERENCE_TABLE_NO foreign key (id_table_notifies)
      references table_notifies (id)
      on delete cascade on update cascade;

drop table if exists table_notifies_log;
/*==============================================================*/
/* Table: table_notifies_log                                    */
/*==============================================================*/
create table table_notifies_log (
   id                   BIGSERIAL            not null,
   notify_name          VARCHAR              not null,
   id_user              INT4                 not null,
   user_name            VARCHAR              not null,
   id_position          INT4                 not null,
   position_name        VARCHAR              not null,
   id_org               INT4                 not null,
   org_name             VARCHAR              not null,
   table_name           VARCHAR              not null,
   id_record            INT8                 not null,
   what_happens         INT4                 not null,
   invocation_datetime  TIMESTAMP            not null,
   constraint PK_TABLE_NOTIFIES_LOG primary key (id)
)
inherits (root_table);

comment on table table_notifies_log is
'������ ������� ������������� ����������� ���������
������ ���, ����� ������������ ����������� ���������, ���������� �� ���� ������������ � ������ ����������.
����� ���������� �� ��������� � �������� ��������������� ����� ��������, ������ ����������� ������� getTableNotify(int8)

���������: 
1) �������-�������������� ������������, ��������� , ���������� ���������� ������� ��� ��������� �����������, ����� �� �������� �� ��������� ������ � ��������������� ������������.
2) ���� ��������� �� ��������� ������ �� ������� table_notifies, ��������� ������������ ���� "�������� ���������". ��� ��������� ���������� � ������� table_notifies, ������� �� ������ ����� ���������������� ������ ��� ���������, ���� ��� �� ���� ������� �� table_notifies';

comment on column table_notifies_log.notify_name is
'�������� ��������������� ���������';

comment on column table_notifies_log.id_user is
'������������� ������������, �������� �������� ������� � ��������� ���������';

comment on column table_notifies_log.user_name is
'��� ����� ������������';

comment on column table_notifies_log.id_position is
'����������� ����, � ������ ���������� �������� ���������� ������ ������������';

comment on column table_notifies_log.position_name is
'�������� ����� ������������ ����';

comment on column table_notifies_log.id_org is
'������������� ����������, ������� ����������� ������������';

comment on column table_notifies_log.org_name is
'�������� ������ ����������';

comment on column table_notifies_log.table_name is
'�������� �������, �������� ��� ������� ������� � ��������� ���������';

comment on column table_notifies_log.id_record is
'������������� ������, ��� ������� ������������� ��������';

comment on column table_notifies_log.what_happens is
'��� �� �������� �������������
1 - INSERT
2 - UPDATE
3 - DELETE';

comment on column table_notifies_log.invocation_datetime is
'����� ���� ������������� ���������';

select setMacToNULL('table_notifies_log');
select createTriggerUID('table_notifies_log');


/*==============================================================*/
/* Table: notify_routing                                        */
/*==============================================================*/
drop table if exists notify_routing;
create table notify_routing (
   id                   SERIAL               not null,
   id_notify            INT4                 not null,
   id_subsystem         INT4                 not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_NOTIFY_ROUTING primary key (id)
)
inherits (root_table);

comment on table notify_routing is
'���������� ������������� ����������� ���������.
����������, � ����� ���������� ������ ���� �� ��� ���� ����������� ���������.
� ��������� � ����������� ��������� �� � �������� ��������� ������������ ����������� ������� ��������� ������ ���������� �������������� ������������';

comment on column notify_routing.id_notify is
'����� ������� (���������) ������ ���� ����������';

comment on column notify_routing.id_subsystem is
'� ����� ���������� ������ ���� ���������';

comment on column notify_routing.name is
'������������ ���������';

comment on column notify_routing.description is
'�����������';

select setMacToNULL('notify_routing');
select createTriggerUID('notify_routing');

/*==============================================================*/
/* Index: u_i_notify_routing                                    */
/*==============================================================*/
create unique index u_i_notify_routing on notify_routing (
id_notify,
id_subsystem
);

alter table notify_routing
   add constraint FK_NOTIFY_R_REFERENCE_TABLE_NO foreign key (id_notify)
      references table_notifies (id)
      on delete restrict on update restrict;

alter table notify_routing
   add constraint FK_NOTIFY_R_REFERENCE_UNITS foreign key (id_subsystem)
      references units (id)
      on delete restrict on update restrict;


drop function if exists createNotify(varchar, varchar, int8, varchar, int4);
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




--���� (����� ��������� � ��������� ��������) �������� ������� �� �������� ��������� (��������� ������ � ACCESS_CATEGORIES_TABLE �� ����)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');
select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');
select f_safe_drop_trigger('trgacinsert', 'attrs_categories');

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-240', 240, 10, NULL, false, '����������� ��������� (�������)', 'SYSCATEGORY_240', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-241', 241, 8, 240, true, '������ ����������� ��������� (�������)', 'SYSCATEGORY_241', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-242', 242, 10, NULL, false, '������� ������������� ����������� ���������', 'SYSCATEGORY_242', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-243', 243, 8, 242, true, '���������� ������������� ����������� ���������', 'SYSCATEGORY_243', NULL::varchar, true, true, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-387', 387, 8, 'what_happens', '������� (INSERT, UPDATE, DELETE)', '������� (INSERT, UPDATE, DELETE)', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-388', 388, 8, 'id_org', '�� ����������', '�� ����������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-389', 389, 9, 'org_name', '�������� ����������', '�������� ����������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-390', 390, 9, 'position_name', '�������� ���������', '�������� ���������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-391', 391, 8, 'id_position', '�� ���������', '�� ���������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-392', 392, 9, 'user_name', '��� ������������', '��� ������������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-393', 393, 8, 'id_user', '�� ������������', '�� ������������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-394', 394, 9, 'notify_name', '�������� ���������', '�������� ���������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-395', 395, 37, 'invocation_datetime', '������ ��������� ���������', '������ ��������� ���������', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-396', 396, 2, 'id_notify', '��������� ��� ��������', '��������� ��� ��������', 'table_notifies', 'name', 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-397', 397, 2, 'id_subsystem', '� ����� ���������� ������ ����', '� ����� ���������� ������ ����', 'units', 'name', 300, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-398', 398, 2, 'id_position', '����������� ����', '����������� ����', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-399', 399, 2, 'id_unit', '���������� (�������������)', '���������� (�������������)', 'units', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-400', 400, 2, 'id_search_template', '�������� ������ (��������� ������)', '�������� ������ (��������� ������)', 'search_templates', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-401', 401, 1, 'is_accept', '������������ ��� ���������� �������', '������������ ��� ���������� �������', NULL, NULL, 100, TRUE);

--������� ������� ����������� ��������� (�������)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (708, 240, 1, NULL, true, true, '��'); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (709, 240, 394, NULL, true, true, '�������� ���������'); --notify_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (710, 240, 393, NULL, true, true, '�� ������������'); --id_user
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (711, 240, 392, NULL, true, true, '��� ������������'); --user_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (712, 240, 391, NULL, true, true, '�� ���������'); --id_position
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (713, 240, 390, NULL, true, true, '�������� ���������'); --position_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (714, 240, 388, NULL, true, true, '�� ����������'); --id_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (715, 240, 389, NULL, true, true, '�������� ����������'); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (716, 240, 8, NULL, true, true, '�������� ������� �����������'); --table_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (717, 240, 330, NULL, true, true, '�� ������ � �����������'); --id_record
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (718, 240, 387, NULL, true, true, '������� (1-INSERT, 2-UPDATE, 3-DELETE)'); --what_happens
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (719, 240, 395, NULL, true, true, '������ ��������� ���������'); --invocation_datetime

--������� ����������� ������������� ����������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (720, 242, 1, NULL, true, true, '��'); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (721, 242, 2, NULL, true, false, '��������'); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (722, 242, 396, NULL, true, false, '�� ��������� ��� ��������'); --id_notify
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (723, 242, 397, NULL, true, false, '� ����� ���������� ������ ����'); --id_subsystem
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (724, 242, 3, NULL, false, false, '��������'); --description


insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (725, 238, 398, NULL, false, false, '����������� ����'); --id_position
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (726, 238, 399, NULL, false, false, '���������� (�������������)'); --id_unit
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (727, 238, 400, NULL, false, false, '�������� ������ (��������� ������)'); --id_search_template
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, name) values (728, 238, 401, 'true', false, false, '������������ ��� ���������� �������'); --is_accept


select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacinsert', 'before', 'insert or update', 'attrs_categories', 'acinsertcheck()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--���� (����� ��������� � ��������� ���������) �������� ������� �� �������� �� (���� ��������� ������ � ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
select f_safe_drop_trigger('trgsyncrecords', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-118', 118, 241, 1, 1, '������ ����������� ��������� (�������)', 'table_notifies_log', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (118, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-119', 119, 243, 1, 1, '���������� ������������� ����������� ���������', 'notify_routing', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (119, -10, TRUE, TRUE, FALSE, FALSE);


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');
select f_create_trigger('trgsyncrecords', 'before', 'insert or update or delete', 'io_objects', 'syncrecords()');

select setAsLogging(1);
select setAsLogging(2);

----------
----------


--��� ��������� �����


--������-�� ������ ��� ��������� �� �����������.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
