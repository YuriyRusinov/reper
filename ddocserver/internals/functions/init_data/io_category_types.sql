--���������� ����� �������������� ��������

insert into io_category_types (id, name, r_name, description, is_qualifier) values(1, '����������', '�����������', NULL, true);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(2, '������', '�������', NULL, true);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(3, '��������', '���������', NULL, false);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(4, '��������', '���������', NULL, false);
--insert into io_category_types (id, name, r_name, description) values(5, '���������', '���������', NULL);
--insert into io_category_types (id, name, r_name, description) values(6, '��������� � ��������� ������� 83�20', '��������� � ��������� ������� 83�20', NULL);
--insert into io_category_types (id, name, r_name, description) values(7, '������ ���������', '������� ����������', NULL);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(8, '��������� ����������', '��������� �����������', '������������ � �������� ���� ��� ���������, ������� �������� ����������. ��������, ���������� ���������; ���������� ���������; ���������� ����� ���������; ���������� ����� �������������� ��������.', true);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(9, '��������� ���������� ��������� ����������', '��������� ����������� ��������� ����������', '������������ � �������� ���� ��� ��������� �������������� ��������, ������� �������� ������������� ��������� �������������� ��������', true);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(10, '����������� ���������', '����������� ���������', '������������ � �������� ���� ��� ���������, ������� �������� ������������ (��������� �������)', false);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(11, '�������� � ����������������', '��������� � ����������������', '������������ � �������� ���� ��� ���������, ���������� ������� �������������� ���������� ���������� ������������ ����������������', false);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(12, '������ ���������� �������', '������ ���������� �������', '������������ � �������� ���� ��� ���������, ���������� ������� �������������� ��� ��������� �������', false);

SELECT pg_catalog.setval('io_category_types_id_seq', 12, true);

