--���������� ����� �������������� ��������

insert into io_category_types (id, name, r_name, description) values(1, '����������', '�����������', NULL);
insert into io_category_types (id, name, r_name, description) values(2, '������', '�������', NULL);
insert into io_category_types (id, name, r_name, description) values(3, '��������', '���������', NULL);
insert into io_category_types (id, name, r_name, description) values(4, '��������', '���������', NULL);
--insert into io_category_types (id, name, r_name, description) values(5, '���������', '���������', NULL);
--insert into io_category_types (id, name, r_name, description) values(6, '��������� � ��������� ������� 83�20', '��������� � ��������� ������� 83�20', NULL);
--insert into io_category_types (id, name, r_name, description) values(7, '������ ���������', '������� ����������', NULL);
insert into io_category_types (id, name, r_name, description) values(8, '��������� ����������', '��������� �����������', '������������ � �������� ���� ��� ���������, ������� �������� ����������. ��������, ���������� ���������; ���������� ���������; ���������� ����� ���������; ���������� ����� �������������� ��������.');
insert into io_category_types (id, name, r_name, description) values(9, '��������� ���������� ��������� ����������', '��������� ����������� ��������� ����������', '������������ � �������� ���� ��� ��������� �������������� ��������, ������� �������� ������������� ��������� �������������� ��������');
insert into io_category_types (id, name, r_name, description) values(10, '����������� ���������', '����������� ���������', '������������ � �������� ���� ��� ���������, ������� �������� ������������ (��������� �������)');
insert into io_category_types (id, name, r_name, description) values(11, '�������� � ����������������', '��������� � ����������������', '������������ � �������� ���� ��� ���������, ���������� ������� �������������� ���������� ���������� ������������ ����������������');
insert into io_category_types (id, name, r_name, description) values(12, '������ ���������� �������', '������ ���������� �������', '������������ � �������� ���� ��� ���������, ���������� ������� �������������� ��� ��������� �������');

SELECT pg_catalog.setval('io_category_types_id_seq', 12, true);

