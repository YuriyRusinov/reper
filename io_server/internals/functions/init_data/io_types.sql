--���������� ����� �������������� ��������

insert into io_types (id, name, description) values(1, '����� ���������', NULL);
insert into io_types (id, name, description) values(2, '����� �����������', NULL);
insert into io_types (id, name, description) values(3, '��������� �����������', NULL);

SELECT pg_catalog.setval('io_types_id_seq', 3, true);

