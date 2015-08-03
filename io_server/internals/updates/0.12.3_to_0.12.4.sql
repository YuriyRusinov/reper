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

\i ./functions/tasks/zarya22/readd_zarya_func.sql

--���� (����� ��������� � ��������� ��������) �������� ������� �� �������� ��������� (��������� ������ � ACCESS_CATEGORIES_TABLE �� ����)
--select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
--select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--���� (����� ��������� � ��������� ���������) �������� ������� �� �������� �� (���� ��������� ������ � ACCESS_TABLE)
--select f_safe_drop_trigger('trgioinsert', 'io_objects');
--select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

--select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
--select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');


--��� ��������� �����

--������-�� ������ ��� ��������� �� �����������.
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

alter table indicator add column table_name varchar;
alter table indicator add column column_name varchar;
alter table indicator add column id_ref_attr_type int4;
alter table indicator
   add constraint FK_INDICATOR_REF_A_TYPE foreign key (id_ref_attr_type)
      references a_types (id)
      on delete restrict on update restrict;

update indicator set id_ref_attr_type = (select id from atGetAttrType(column_name, table_name) limit 1) where id_ref_attr_type isnull and column_name is not null;
update indicator set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx();


commit;
