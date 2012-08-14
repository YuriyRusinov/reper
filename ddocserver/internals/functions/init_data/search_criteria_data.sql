insert into criteria_types (id, name) values (1, '����������� �������� ���������');
insert into criteria_types (id, name) values (2, '�������� �����');
insert into criteria_types (id, name) values (3, '������� �������������� ����� ��������� ����');
insert into criteria_types (id, name) values (4, '������� �������� � �������� ���������');
insert into criteria_types (id, name) values (5, '������� ��������� �������� � ������������ ���������');
insert into criteria_types (id, name) values (6, '������� ��������� �������� � �������� ���������');

SELECT pg_catalog.setval('public.criteria_types_id_seq', 6, true);

insert into operations (id, name, the_value) values (1, '�����', '=');
insert into operations (id, name, the_value) values (2, '������', '>');
insert into operations (id, name, the_value) values (3, '������', '<');
insert into operations (id, name, the_value) values (4, '�� ������', '>=');
insert into operations (id, name, the_value) values (5, '�� ������', '<=');
insert into operations (id, name, the_value) values (6, '�������� � ����', 'in');
insert into operations (id, name, the_value) values (7, '�� �������� � ����', 'not in');
insert into operations (id, name, the_value) values (8, '�����', 'between');
insert into operations (id, name, the_value) values (9, '������� �� ������� (LIKE)', 'like');
insert into operations (id, name, the_value) values (10, '�� �����', '<>');
insert into operations (id, name, the_value) values (11, '�� ����� ��������', 'is null');
insert into operations (id, name, the_value) values (12, '����� ��������', 'is not null');
insert into operations (id, name, the_value) values (13, '�������� ��������� �������', 'in (select ...)');
insert into operations (id, name, the_value) values (14, '�������� (LIKE)', 'like');
insert into operations (id, name, the_value) values (15, '���������� �� (LIKE)', 'like');
insert into operations (id, name, the_value) values (16, '������������� �� (LIKE)', 'like');

SELECT pg_catalog.setval('public.operations_id_seq', 16, true);


insert into search_template_types (id, name, description) values(1, '����� ��������� �������', NULL);
SELECT pg_catalog.setval('search_template_types_id_seq', 1, true);
