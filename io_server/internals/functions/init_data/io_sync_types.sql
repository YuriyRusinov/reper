--���������� ����� ������������� �������������� ��������

insert into io_sync_types (id, name, description) values (1, '�� ����������������', '��');
insert into io_sync_types (id, name, description) values (2, '���������� ����������������� �������', '���');
insert into io_sync_types (id, name, description) values (3, '���������� ���������', '��');
insert into io_sync_types (id, name, description) values (4, '���������� ����������� �������, �������������', '����');
insert into io_sync_types (id, name, description) values (5, '���������� ����������� �������, ����������', '����');

SELECT pg_catalog.setval('io_sync_types_id_seq', 5, true);

