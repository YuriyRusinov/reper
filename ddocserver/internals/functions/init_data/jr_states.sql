insert into jr_states (id, name, description) values (1, '������������', NULL);
insert into jr_states (id, name, description) values (2, '������ �������', NULL);
insert into jr_states (id, name, description) values (3, '�����������', NULL);
insert into jr_states (id, name, description) values (4, '��������', NULL);
insert into jr_states (id, name, description) values (5, '���������', NULL);
insert into jr_states (id, name, description) values (6, '�������� ��� ��������', NULL);
insert into jr_states (id, name, description) values (7, '����������� ���������', NULL);
--������� �������� �����������, ����� � ��� ���� ����������, �� ���� ��� ��� �����������.
--����� �������� ��� �������������� ������ ������ �����������, 
--����� ���� ������������ �������������� (�������) �������� ������������ �� ����� ���������� ��

SELECT pg_catalog.setval('jr_states_id_seq', 7, true);


