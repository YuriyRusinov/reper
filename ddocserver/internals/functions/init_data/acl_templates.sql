insert into user_acl_templates(name) values('������ 1');
insert into user_acl_templates(name) values('������ 2');
insert into user_acl_templates(name) values('������ 3');
insert into user_acl_templates(name) values('������ 4');

insert into acl_template_group_types (name) values ('����� ����������');
insert into acl_template_group_types (name) values ('����� �������������');
insert into acl_template_group_types (name) values ('����� ���������');

--������ 2
insert into acl_template_groups(id_group_type, id_template, allow_readlist, allow_read, allow_delete, allow_update)
values (1, 2, true, true, false, true);

--������ 3
insert into acl_template_groups(id_group_type, id_template, allow_readlist, allow_read, allow_delete, allow_update)
values (1, 3, true, true, false, true);
insert into acl_template_groups(id_group_type, id_template, allow_readlist, allow_read, allow_delete, allow_update)
values (2, 3, true, true, false, false);

--������ 4
insert into acl_template_groups(id_group_type, id_template, allow_readlist, allow_read, allow_delete, allow_update)
values (1, 4, true, true, false, true);
insert into acl_template_groups(id_group_type, id_template, allow_readlist, allow_read, allow_delete, allow_update)
values (2, 4, true, true, false, false);
insert into acl_template_groups(id_group_type, id_template, allow_readlist, allow_read, allow_delete, allow_update)
values (3, 4, true, true, false, false);
