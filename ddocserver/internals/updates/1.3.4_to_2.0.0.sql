\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);


--���� ������� �� �������� ������ ����������� (���� ����������� ��)
--�� ����������� ����� ��������� �������!!!
create table interaction_formats (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_INTERACTION_FORMATS primary key (id)
)
inherits (root_table);

comment on table interaction_formats is
'������� ������������� ������ (�������������� ��������, ������� ������������), ������������ ��� �������������� � �������� ���������
��������:
1) unified XML format (��������������� ������ �������� �������������� ��������)
2) param-value representation (�������������� ������, ������ ����������� ��������� � ���� ������ ���������, ����������� � ���� �������-��������� ������������� ��������). ������������ ��� �������� �������� ��������������. �������� �� ������������ ���������� ������ �� ��������������
3) shushun (������ ������������� ������ ���� ���� � ��� "���� 22")
4) internal (���������� ������ ������������� ������ ��� �������������� ����� ��������� DynamicDocs)
';

select setMacToNULL('interaction_formats');
select createTriggerUID('interaction_formats');


create table out_external_queue (
   id                   BIGSERIAL            not null,
   id_organization      INT4                 not null,
   id_format            INT4                 not null,
   id_io_object         int8                 not null,
   id_entity            int8                 null,
   interaction_type     int4                 not null,
   interaction_result   int4                 not null,
   constraint PK_OUT_EXTERNAL_QUEUE primary key (id)
)
inherits (root_table);

comment on table out_external_queue is
'��������� ������� ��� ������������ �������� �������������� �������� (������� ������������) �� ������� ����������������� ������� (�� DynamicDocs)';

comment on column out_external_queue.id_organization is
'� ����� ����������� ����������';

comment on column out_external_queue.id_format is
'��� ������ ������ ������� ������������ ������';

comment on column out_external_queue.id_io_object is
'����� �������������� ������ ����������';

comment on column out_external_queue.id_entity is
'���� ������� ��������, �� ���������� �� ��� �������������� ������, � ������ ��� ����������� (��� �������������� ��������, ���������� �������������)';

comment on column out_external_queue.interaction_type is
'��� ��������������
1 - ������� ����� �������� �� ������� �������
2 - �������� �������� �� ������ �� ������� ������� (���� ��� �� ����������, ������� �����)
3 - ������� �������� �� ������� �������
';

comment on column out_external_queue.interaction_result is
'���������� ��������������� �������������� (�������� ������)
1 - ��������� �������� (����� � ������� �� ��������). ��� �������� �� ���������.
2 - �������� �������������� (�������������� ����� ��� �� ��������)
3 - �������� ����������� �������
4 - ��������� ������';

select setMacToNULL('out_external_queue');
select createTriggerUID('out_external_queue');

alter table out_external_queue
   add constraint FK_OUT_EXTE_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table out_external_queue
   add constraint FK_OUT_EXTE_REFERENCE_INTERACT foreign key (id_format)
      references interaction_formats (id)
      on delete restrict on update restrict;



create table in_external_queue (
   id                   BIGSERIAL            not null,
   id_organization      INT4                 not null,
   id_format            INT4                 not null,
   in_data              VARCHAR              not null,
   interaction_type     int4                 not null,
   interaction_result   int4                 not null,
   constraint PK_IN_EXTERNAL_QUEUE primary key (id)
)
inherits (root_table);

comment on table in_external_queue is
'�������� ������� ��� ������������� ��������� �������������� �������� (������� ������������) �� ������� ����������������� ������ (�� DynamicDocs)';

comment on column in_external_queue.id_organization is
'�����������, �� ������� ��������� ������';

comment on column in_external_queue.id_format is
'������ ������������� ������';

comment on column in_external_queue.in_data is
'�������� ������������� �������� ������';

comment on column in_external_queue.interaction_type is
'��� ��������������
1 - ������� ����� �������� �� ������� �������
2 - �������� �������� �� ������ �� ������� ������� (���� ��� �� ����������, ������� �����)
3 - ������� �������� �� ������� �������
';

comment on column in_external_queue.interaction_result is
'���������� ���������� ��������������� ������� � ��
1 - ������ ������ ���������, �� ��������� ��� �� ���������� (�������� �� ���������).
2 - �������� ��������������� ������� ����������� �������
3 - ��������� ������ ��� �������� ��������������� ������� � ��';

select setMacToNULL('in_external_queue');
select createTriggerUID('in_external_queue');

alter table in_external_queue
   add constraint FK_IN_EXTER_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table in_external_queue
   add constraint FK_IN_EXTER_REFERENCE_INTERACT foreign key (id_format)
      references interaction_formats (id)
      on delete restrict on update restrict;


insert into interaction_formats (id, name, description) values (1, '��������������� �������� XML-������ �������� �������������� �������� � ������� DynamicDocs', NULL);
insert into interaction_formats (id, name, description) values (2, '������� ������ ������������� �� ������ ������� PARAM=VALUE', '�������������� ������, ������ ����������� ��������� � ���� ������ ���������, ����������� � ���� �������-��������� ������������� ��������. ������������ ��� �������� �������� ��������������. �������� �� ������������ ���������� ������ �� ��������������');
insert into interaction_formats (id, name, description) values (3, 'XML-������ ��� "����-22"', '������ ������������� ������ ���� ���� � ��� "����-22"');
insert into interaction_formats (id, name, description) values (4, '���������� �������� ������ ������������� ������ ��� �������������� ����� ��������� DynamicDocs', NULL);

SELECT pg_catalog.setval('interaction_formats_id_seq', 4, true);

---------------------------
---------------------------

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql
\i ./functions/general/createtriggeruidex.sql


drop function recInsertRubric(int8, int8, varchar, varchar, varchar, varchar);
drop function recUpdateRubric(int8, varchar, varchar, int4, int4, int4, int8, varchar, varchar);
drop function recUpdateRubricEx(varchar, varchar, varchar, int8, int8, varchar);
drop function recUpdateRubricLocal(int8, varchar, varchar, varchar, varchar);

drop function if exists xGetRecordFields(int4);

alter table record_rubricator add column id_io_object int4;
alter table record_rubricator add column id_io_category int4;

alter table record_rubricator
   add constraint FK_RECORD_R_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references tbl_io_objects (id)
      on delete restrict on update restrict;

alter table record_rubricator
   add constraint FK_RECORD_R_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table q_base_table add column rr_name varchar;

create or replace function aaa() returns int4 as
$BODY$
declare
    r record;
    tName varchar;
    q varchar;
    i int4;
    bHasName bool;
begin

    for r in select table_name from io_objects where id > 300 and table_name is not null
    loop

        tName = r.table_name;

        
        select f_is_view_exist(tName) into i;
        if(i = 0) then
            continue;
        end if;
        
        q = 'drop view ' || tName || ' cascade';
        execute q;
        
        q = 'select f_drop_funcs_' || tName || '();';
        execute q;
        
        q = 'alter table tbl_' || tName || ' rename to ' || tName;
        execute q;

        perform acl_secureTable(tName);
      
        select f_is_column_exist('tbl_' || tName, 'name') into bHasName;
        if(bHasName = true) then
            q = 'update tbl_' || tName || ' set rr_name = name;';
        else
            q = 'update tbl_' || tName || ' set rr_name = ' || quote_literal('Record ') || ' || id';
        end if;

        execute q;
        
    end loop;

    return 1;
end
$BODY$
language 'plpgsql' security definer;

select aaa();
drop function aaa();


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

--���� (����� ��������� � ��������� ��������) �������� ������� �� �������� ��������� (��������� ������ � ACCESS_CATEGORIES_TABLE �� ����)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');
select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');

select f_safe_drop_trigger('trgainsert', 'attributes');
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-378', 378, 9, 'rr_name', '��������', '��������', NULL, NULL, 300, TRUE);
select f_create_trigger('trgainsert', 'before', 'insert or update', 'attributes', 'ainsertcheck()');

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-230', 230, 10, NULL, false, '������� ��������������� �������������� � �������� ��������� ', 'SYSCATEGORY_230', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-231', 231, 8, 230, true, '���������� �������� ��������������� �������������� � �������� ���������', 'SYSCATEGORY_231', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-232', 232, 10, NULL, false, '������� ��������� ������� ��������������� �������������� � �������� ���������', 'SYSCATEGORY_232', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-233', 233, 8, 232, true, '��������� ������� ��������������� �������������� � �������� ���������', 'SYSCATEGORY_233', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-234', 234, 10, NULL, false, '������� �������� ������� ��������������� �������������� � �������� ���������', 'SYSCATEGORY_234', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-235', 235, 8, 234, true, '�������� ������� ��������������� �������������� � �������� ���������', 'SYSCATEGORY_235', NULL::varchar, true, true, 1);


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-379', 379, 2, 'id_format', '������ ��������������� ��������������', '������ ��������������', 'interaction_formats', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-380', 380, 30, 'id_entity', '������������� ������', '������������� ������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-381', 381, 8, 'interaction_type', '��� ��������������', '��� ��������������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-382', 382, 8, 'interaction_result', '��������� ��������������', '��������� ��������������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-383', 383, 13, 'in_data', '��������� ������', '��������� ������', NULL, NULL, 100, TRUE);

--��������� ������� ��������������� �������������� � �������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (683, 232, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (684, 232, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (685, 232, 379, NULL, true, false); --id_format
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (686, 232, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (687, 232, 380, NULL, false, false); --id_entity
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (688, 232, 381, NULL, true, false); --interaction_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (689, 232, 382, NULL, true, false); --interaction_result

--�������� ������� ��������������� �������������� � �������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (690, 234, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (691, 234, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (692, 234, 379, NULL, true, false); --id_format
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (693, 234, 383, NULL, true, false); --in_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (694, 234, 381, NULL, true, false); --interaction_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (695, 234, 382, NULL, true, false); --interaction_result

--������� �������� ��������������� �������������� � �������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (696, 230, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (697, 230, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (698, 230, 3, NULL, false, false);  --description


select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--���� (����� ��������� � ��������� ���������) �������� ������� �� �������� �� (���� ��������� ������ � ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-113', 113, 231, 1, 1, '���������� �������� ��������������� �������������� � �������� ���������', 'interaction_formats', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (113, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-114', 114, 233, 1, 1, '��������� ������� ��������������� �������������� � �������� ���������', 'out_external_queue', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (114, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-115', 115, 235, 1, 1, '�������� ������� ��������������� �������������� � �������� ���������', 'in_external_queue', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (115, -10, TRUE, TRUE, FALSE, FALSE);

select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

----------
----------


--��� ��������� �����

update attributes set id_a_type = 30 where id = 1;

--������-�� ������ ��� ��������� �� �����������.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
