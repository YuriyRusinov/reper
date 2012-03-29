insert into a_views (id, name) values (1, '�������');
insert into a_views (id, name) values (2, '����� �� ������ (���������)');
insert into a_views (id, name) values (3, '����');
insert into a_views (id, name) values (4, '����-�����');
insert into a_views (id, name) values (5, '������ ������');
insert into a_views (id, name) values (6, '��������');
insert into a_views (id, name) values (7, '����� ���������');
insert into a_views (id, name) values (8, '���� ������');
insert into a_views (id, name) values (9, '������ ������ �������������� �����');
insert into a_views (id, name) values (10, '����������� JPG');
insert into a_views (id, name) values (11, '����� �����');
insert into a_views (id, name) values (12, '����� ���������������� �����');
insert into a_views (id, name) values (13, '������ SVG');
insert into a_views (id, name) values (14, '�����-�����');
insert into a_views (id, name) values (15, '�����');


insert into a_types (id, id_a_view, name, code) values (1, 1, '����������', 'BOOL');
insert into a_types (id, id_a_view, name, code) values (2, 2, '������� �����������', 'INT4');
insert into a_types (id, id_a_view, name, code) values (3, 2, '��������/�������', 'INT4');
insert into a_types (id, id_a_view, name, code) values (4, 3, '����', 'DATE');
insert into a_types (id, id_a_view, name, code) values (5, 3, '����-�����', 'TIMESTAMP');
insert into a_types (id, id_a_view, name, code) values (6, 5, '������� ��������', 'FLOAT8');
insert into a_types (id, id_a_view, name, code) values (7, 5, '������ �� �������������� ������', 'INT4');
insert into a_types (id, id_a_view, name, code) values (8, 5, '������������� ��������', 'INT4');
insert into a_types (id, id_a_view, name, code) values (9, 5, '������', 'VARCHAR');
insert into a_types (id, id_a_view, name, code) values (10, 6, '��������� �������� (���)', 'INT4[]');
insert into a_types (id, id_a_view, name, code) values (11, 5, '�����������', 'VARCHAR');
insert into a_types (id, id_a_view, name, code) values (12, 7, '����� ��������� ����������� (������ �������)', 'INT4[]');
insert into a_types (id, id_a_view, name, code) values (13, 8, '�����', 'TEXT');
insert into a_types (id, id_a_view, name, code) values (14, 9, '������ ������������� �����', 'VARCHAR');
insert into a_types (id, id_a_view, name, code) values (15, 10, '����������� JPG', 'VARCHAR');
insert into a_types (id, id_a_view, name, code) values (16, 5, '��������� �����', 'MACLABEL');

insert into a_types (id, id_a_view, name, code) values (17, 7, '����� ��������� �����������', 'INT4[]');
insert into a_types (id, id_a_view, name, code) values (18, 11, '���� ������ � ����������� (���)', 'INT8');
insert into a_types (id, id_a_view, name, code) values (19, 11, '������ �� ���� ������ (���)', 'INT4');
insert into a_types (id, id_a_view, name, code) values (20, 8, 'XML-��������', 'XML');
insert into a_types (id, id_a_view, name, code) values (21, 6, '��������� �������� (����)', 'INT4[3]');
insert into a_types (id, id_a_view, name, code) values (22, 12, '����� ���������������� �����', 'POINT[]');
insert into a_types (id, id_a_view, name, code) values (23, 13, '������ SVG', 'XML');
insert into a_types (id, id_a_view, name, code) values (24, 14, '�����-�����', 'BYTEA');
insert into a_types (id, id_a_view, name, code) values (25, 11, '���� ������ � ����������� (�����)', 'INT8');
insert into a_types (id, id_a_view, name, code) values (26, 11, '������ �� ���� ������ (�����)', 'INT4');
insert into a_types (id, id_a_view, name, code) values (27, 15, '�����', 'TIME');

SELECT pg_catalog.setval('a_views_id_seq', 15, true);
SELECT pg_catalog.setval('a_types_id_seq', 27, true);
