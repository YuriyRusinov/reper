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





\i ./functions/contribs/readd_contribs.sql

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/categories/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql

/*==============================================================*/
/* Table: histogram_params_streams                              */
/*==============================================================*/
create table histogram_params_streams (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   scenarios            INT4[]               null,
   variants             INT4[]               null,
   dl_froms             INT4[]               null,
   dl_tos               INT4[]               null,
   io_categories        INT4[]               null,
   io_objects           INT4[]               null,
   partition_lows       INT4[]               null,
   constraint PK_HISTOGRAM_PARAMS_STREAMS primary key (id)
)
inherits (root_table);

comment on table histogram_params_streams is
'������� �������� ��� ���������� ���������� �� ������� ���������';

select setMacToNULL('histogram_params_streams');
select createTriggerUID('histogram_params_streams');


/*==============================================================*/
/* Table: histogram_params_chains                               */
/*==============================================================*/
create table histogram_params_chains (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   scenarios            INT4[]               null,
   variants             INT4[]               null,
   life_cycles          INT4[]               null,
   io_objects           INT4[]               null,
   services             INT4[]               null,
   constraint PK_HISTOGRAM_PARAMS_CHAINS primary key (id)
)
inherits (root_table);

comment on table histogram_params_chains is
'������� �������� ��� ���������� ���������� �� ��������';

select setMacToNULL('histogram_params_chains');
select createTriggerUID('histogram_params_chains');

/*==============================================================*/
/* Table: histogram_graphics_streams                            */
/*==============================================================*/
create table histogram_graphics_streams (
   id                   SERIAL               not null,
   id_histogram_params_streams INT4                 not null,
   h_order              INT4                 not null,
   h_x                  float8               not null,
   h_y                  float8               not null,
   constraint PK_HISTOGRAM_GRAPHICS_STREAMS primary key (id)
)
inherits (root_table);

comment on table histogram_graphics_streams is
'������� �������� ����������';

select setMacToNULL('histogram_graphics_streams');
select createTriggerUID('histogram_graphics_streams');

alter table histogram_graphics_streams
   add constraint FK_HISTOGRAM_REF_HIST_STREAMS foreign key (id_histogram_params_streams)
      references histogram_params_streams (id)
      on delete restrict on update restrict;

/*==============================================================*/
/* Table: histogram_graphics_chains                             */
/*==============================================================*/
create table histogram_graphics_chains (
   id                   SERIAL               not null,
   id_histogram_params_chains INT4                 not null,
   h_order              INT4                 not null,
   h_x                  float8               not null,
   h_y                  float8               not null,
   constraint PK_HISTOGRAM_GRAPHICS_CHAINS primary key (id)
)
inherits (root_table);

comment on table histogram_graphics_chains is
'������� �������� ���������� ��� ��������';

select setMacToNULL('histogram_graphics_chains');
select createTriggerUID('histogram_graphics_chains');

alter table histogram_graphics_chains
   add constraint FK_HISTOGRA_REFERENCE_HISTOGRA foreign key (id_histogram_params_chains)
      references histogram_params_chains (id)
      on delete restrict on update restrict;



select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/readd_tasks_func.sql

--���� (����� ��������� � ��������� ��������) �������� ������� �� �������� ��������� (��������� ������ � ACCESS_CATEGORIES_TABLE �� ����)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');
select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-220', 220, 10, NULL, false, '��������� ���������� ��� �������', 'SYSCATEGORY_220', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-221', 221, 8, 220, true, '���������� ���������� ���������� ��� �������', 'SYSCATEGORY_221', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-222', 222, 10, NULL, false, '��������� ���������� ��� ��������', 'SYSCATEGORY_222', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-223', 223, 8, 222, true, '���������� ���������� ���������� ��� ��������', 'SYSCATEGORY_223', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-224', 224, 10, NULL, false, '������� ���������� ��� �������', 'SYSCATEGORY_224', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-225', 225, 8, 224, true, '���������� �������� ���������� ��� �������', 'SYSCATEGORY_225', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-226', 226, 10, NULL, false, '������� ���������� ��� ��������', 'SYSCATEGORY_226', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-227', 227, 8, 226, true, '���������� �������� ���������� ��� ��������', 'SYSCATEGORY_227', NULL::varchar, true, true, 1);


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-360', 360, 2, 'id_histogram_params_streams', '��������� �����������', '��������� �����������', 'histogram_params_streams', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-361', 361, 2, 'id_histogram_params_chains', '��������� �����������', '��������� �����������', 'histogram_params_chains', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-362', 362, 8, 'h_order', '������� ����������', '������� ����������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-363', 363, 6, 'h_x', '��������', '��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-364', 364, 6, 'h_y', '��������', '��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-365', 365, 12, 'scenarios', '��������', '��������', 'processing_scenario', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-366', 366, 12, 'variants', '��������', '��������', 'processing_variant', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-367', 367, 12, 'dl_froms', '���������', '���������', 'position', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-368', 368, 12, 'dl_tos', '����������', '����������', 'position', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-369', 369, 12, 'io_categories', '���������', '���������', 'io_categories', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-370', 370, 12, 'io_objects', '��', '��', 'io_objects', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-371', 371, 12, 'partition_lows', '������ �������������', '������ �������������', 'partition_lows', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-372', 372, 12, 'life_cycles', '��', '��', 'life_cycle', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-373', 373, 12, 'services', '�������', '�������', 'handlers', 'name', 100, TRUE);


--������� ���������� ���������� ��� �������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (644, 220, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (645, 220, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (646, 220, 365, NULL, false, false); --scenarios
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (647, 220, 366, NULL, false, false); --variants
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (648, 220, 367, NULL, false, false); --dl_froms
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (649, 220, 368, NULL, false, false); --dl_tos
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (650, 220, 369, NULL, false, false); --io_categories
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (651, 220, 370, NULL, false, false); --io_objects
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (652, 220, 371, NULL, false, false); --partition_lows

--������� ���������� ���������� ��� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (653, 222, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (654, 222, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (655, 222, 365, NULL, false, false); --scenarios
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (656, 222, 366, NULL, false, false); --variants
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (657, 222, 372, NULL, false, false); --life_cycles
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (658, 222, 370, NULL, false, false); --io_objects
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (659, 222, 373, NULL, false, false); --services

--������� �������� ���������� ��� �������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (660, 224, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (661, 224, 360, NULL, true, false); --id_histogram_params_streams
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (662, 224, 362, NULL, true, false); --h_order
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (663, 224, 363, NULL, true, false); --h_x
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (664, 224, 364, NULL, true, false); --h_y

--������� �������� ���������� ��� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (665, 226, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (666, 226, 361, NULL, true, false); --id_histogram_params_chains
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (667, 226, 362, NULL, true, false); --h_order
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (668, 226, 363, NULL, true, false); --h_x
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (669, 226, 364, NULL, true, false); --h_y



select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--���� (����� ��������� � ��������� ���������) �������� ������� �� �������� �� (���� ��������� ������ � ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-108', 108, 221, 1, 1, '���������� ���������� ���������� ��� �������', 'histogram_params_streams', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (108, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-109', 109, 223, 1, 1, '���������� ���������� ���������� ��� ��������', 'histogram_params_chains', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (109, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-110', 110, 225, 1, 1, '���������� �������� ���������� ��� �������', 'histogram_graphics_streams', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (110, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-111', 111, 227, 1, 1, '���������� �������� ���������� ��� ��������', 'histogram_graphics_chains', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (111, -10, TRUE, TRUE, FALSE, FALSE);


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--���� ������� �� �������� ������ ����������� (���� ����������� ��)


----------
----------


--��� ��������� �����

--������-�� ������ ��� ��������� �� �����������.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
