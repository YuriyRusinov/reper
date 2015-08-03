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

--select createTriggerUIDEx('criteria');

\i ./functions/contribs/readd_contribs.sql

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/categories/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql


update a_views set name = '���-������' where id = 16;
update a_types set name = '���-������', code = 'GEOMETRY' where id = 28;

insert into a_views (id, name) values (19, '�����������');
insert into a_views (id, name) values (20, '���-������ (�����)');
insert into a_types (id, id_a_view, name, code) values (33, 19, '�����������', 'VARCHAR');
insert into a_types (id, id_a_view, name, code) values (34, 20, '���-������ (��������� ����)', 'VARCHAR');
insert into a_types (id, id_a_view, name, code) values (35, 20, '���-������ (��������� ����)', 'VARCHAR');
insert into a_types (id, id_a_view, name, code) values (36, 20, '���-������ (�����)', 'VARCHAR');
--insert into a_types (id, id_a_view, name, code) values (37, 4, '����-����� (� ��������������)', 'TIMESTAMP');

SELECT pg_catalog.setval('a_views_id_seq', 20, true);
SELECT pg_catalog.setval('a_types_id_seq', 36, true);


select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/readd_tasks_func.sql

--���� (����� ��������� � ��������� ��������) �������� ������� �� �������� ��������� (��������� ������ � ACCESS_CATEGORIES_TABLE �� ����)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--���� (����� ��������� � ��������� ���������) �������� ������� �� �������� �� (���� ��������� ������ � ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');


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
