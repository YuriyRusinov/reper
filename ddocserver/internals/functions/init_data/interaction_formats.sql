insert into interaction_formats (id, name, description) values (1, '��������������� �������� XML-������ �������� �������������� �������� � ������� DynamicDocs', NULL);
insert into interaction_formats (id, name, description) values (2, '������� ������ ������������� �� ������ ������� PARAM=VALUE', '�������������� ������, ������ ����������� ��������� � ���� ������ ���������, ����������� � ���� �������-��������� ������������� ��������. ������������ ��� �������� �������� ��������������. �������� �� ������������ ���������� ������ �� ��������������');
insert into interaction_formats (id, name, description) values (3, 'XML-������ ��� "����-22"', '������ ������������� ������ ���� ���� � ��� "����-22"');
insert into interaction_formats (id, name, description) values (4, '���������� �������� ������ ������������� ������ ��� �������������� ����� ��������� DynamicDocs', NULL);

SELECT pg_catalog.setval('interaction_formats_id_seq', 4, true);
