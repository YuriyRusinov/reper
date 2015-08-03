/*
��������� ������������
*/
select safeCreateUser('admin', 'admin', true);
insert into users (unique_id, id_state, id_rank, id_maclabel, role_name, lastname, firstname, sirname, fio, insert_time) 
values('localorg-users-1', 1, 1, 1, 'admin', '������������� �������', '', '', '������������� �������', current_timestamp);

select safeCreateUser('jupiter', 'jupiter', true);
insert into users (unique_id, id_state, id_rank, id_maclabel, role_name, lastname, firstname, sirname, fio, insert_time) 
values('localorg-users-2', 1, 1, 1, 'jupiter', '������������ ���������� ��� "������"', '', '', '������������ ����������� ��� "������"', current_timestamp);

--��������� ���� OTHERS_ROLE ������������ ��� ������� ������������� ���� "��������� �����"
insert into kks_roles(id, role_name, with_inheritance) values(-10, '��������� ����', FALSE); --id=-10

insert into units  (unique_id, id_organization, id_curr_mode, name, short_name, code_name) 
values ('localorg-units-3', NULL, 1, '�������������� ������� � �����������', '��������������', '���');--id=3

insert into "position" (unique_id, id_unit, id_user, is_io, name, short_name, id_maclabel, email_prefix) 
values ('localorg-position-4', 3, 1, false, '�������������', '���', 1, 'localorg_position_admin');--id=4

--SELECT pg_catalog.setval('kks_roles_id_seq', 1, true);
