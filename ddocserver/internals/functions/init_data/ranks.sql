insert into ranks (id, name, short_name) values(1, '�� ������', '�� ������');
insert into ranks (id, name, short_name) values(2, '�������� ��', '��. ��');
insert into ranks (id, name, short_name) values(3, '������� ���������', '��. �-�');
insert into ranks (id, name, short_name) values(4, '���������', '�-�');
insert into ranks (id, name, short_name) values(5, '������� ���������', '��. �-�');
insert into ranks (id, name, short_name) values(6, '�������', '�-�');
insert into ranks (id, name, short_name) values(7, '�����', '�-�');
insert into ranks (id, name, short_name, code) values(8, '������������', '�/�-�', 'army.ppk');
insert into ranks (id, name, short_name, code) values(9, '���������', '�-�', 'army.pk');
insert into ranks (id, name, short_name) values(10, '�������-�����', '�-�/�-�');
insert into ranks (id, name, short_name) values(11, '�������-���������', '�-�/�-�');
insert into ranks (id, name, short_name) values(12, '�������-���������', '�-�/�-�');
insert into ranks (id, name, short_name) values(13, '������� �����', '�-�/�����');

SELECT pg_catalog.setval('ranks_id_seq', 13, true);
