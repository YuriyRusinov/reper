insert into io_states (id, name, description, is_system) values (1, '��������', NULL, true);
insert into io_states (id, name, description, is_system) values (2, '��������', NULL, true);
insert into io_states (id, name, description, is_system) values (3, '�������������� �������������� �������������', NULL, true);
insert into io_states (id, name, description, is_system) values (4, '�������������� �������������', NULL, true);
insert into io_states (id, name, description, is_system) values (5, '��������� ��������� �������� ���������', NULL, true);

--��������������� ����������� � ���������� ������������� ��������� ����� ��������� ���������
SELECT pg_catalog.setval('io_states_id_seq', 40, true);


