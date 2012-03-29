/*
insert into executors (name, code, address) values('����������� 1', '�1', (select uGetLocalAddress()));
insert into executors (name, code, address) values('����������� 2', '�2', (select uGetLocalAddress()));

select addGroup('otdel_1', true);
insert into units (id_executor, name, group_name, code) values(1, '����� 1', 'otdel_1', '���1');
select addGroup('otdel_2', true);
insert into units (id_executor, name, group_name, code) values(1, '����� 2', 'otdel_2', '���2');

select addGroup('otdel_3', true);
insert into units (id_executor, name, group_name, code) values(2, '����� 3', 'otdel_3', '���3');
select addGroup('otdel_4', true);
insert into units (id_executor, name, group_name, code) values(2, '����� 4', 'otdel_4', '���4');

select addUser('ivanov', 'otdel_1', '������', '����', '��������', '������ �.�.', 7, '111', true);
select addUser('petrov', 'otdel_1', '������', '����', '��������', '������ �.�.', 9, '111', true);
select addUser('karin', 'otdel_2', '�����', '������', '�������������', '����� �.�.', 7, '111', true);
select addUser('rusinov', 'otdel_2', '�������', '����', '��������', '������� �.�.', 2, '111', true);

insert into dls (id_position, id_unit, id_user, is_io, name) values(1, 3, 8, false, '��������� 1-�� ������');
insert into dls (id_position, id_unit, id_user, is_io, name) values(3, 3, 7, false, '�������� 1-�� ������');
insert into dls (id_position, id_unit, id_user, is_io, name) values(1, 4, 9, false, '��������� 2-�� ������');
insert into dls (id_position, id_unit, id_user, is_io, name) values(2, 4, 10, false, '������� �������� 2-�� ������');
insert into dls (id_position, id_unit, id_user, is_io, name) values(1, 5, 9, false, '��������� 3-�� ������');
insert into dls (id_position, id_unit, id_user, is_io, name) values(1, 6, 10, false, '��������� 4-�� ������');
*/

--insert into units  (id_executor, name, group_name, code) values(NULL, '������������� �������', 'admin', 'ADMINGROUP');--id=17
--insert into dls (id_position, id_unit, id_user, is_io, name) values(2, 17, 1, false, '�������������');--id=18

--select createtablecurrdl();
--select * from getUserDls();
--select * from users;
--select setCurrentDl(3);
--select * from dls;
--insert into dls (id_position, id_unit, id_user, is_io, name) values (2, 1, 3, true, '�� ��������� 1 ������')


--select * from io_objects;
--select * from command_journal;
--select * from tsd;
--select * from jr_states;
--select * from io_categories;
--insert into tsd (id_io_category, id_dl_to, id_dl_executor, id_dl_controller, id_jr_state, exec_datetime, exec_period, osnovanie)
--values( 1, 3, 4, 3, 1, current_timestamp + interval '5 days', interval '7 days', '�������� ������');

--select * from command_journal;
