--��������� ���������

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-1', 1, 10, NULL, false, '��������� �������������� ��������', 'SYSCATEGORY_1', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(1, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-2', 2, 8, 1, true, '���������� ��������� �������������� ��������', 'SYSCATEGORY_2', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(2, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-3', 3, 10, NULL, false, '���� ��������� �������������� ��������', 'SYSCATEGORY_3', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(3, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-4', 4, 8, 3, true, '���������� ����� ��������� �������������� ��������', 'SYSCATEGORY_4', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(4, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-5', 5, 10, NULL, false, '������������� ����� ���������', 'SYSCATEGORY_5', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(5, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-6', 6, 8, 5, true, '���������� ������������� ����� ���������', 'SYSCATEGORY_6', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(6, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-7', 7, 10, NULL, false, '���� ���������', 'SYSCATEGORY_7', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(7, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-8', 8, 8, 7, true, '���������� ����� ���������', 'SYSCATEGORY_8', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(8, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-9', 9, 10, NULL, false, '��������', 'SYSCATEGORY_9', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(9, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-10', 10, 8, 9, true, '���������� ���������', 'SYSCATEGORY_10', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(10, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-11', 11, 10, NULL, false, '���������', 'SYSCATEGORY_11', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(11, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-12', 12, 9, 11, true, '���������� ���������', 'SYSCATEGORY_12', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(12, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-13', 13, 10, NULL, false, '�������������� �������', 'SYSCATEGORY_13', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(13, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-14', 14, 8, 13, true, '���������� �������������� ��������', 'SYSCATEGORY_14', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(14, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-15', 15, 10, NULL, false, '������� �������������� ��������', 'SYSCATEGORY_15', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(15, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-16', 16, 8, 15, true, '���������� �������� �������������� ��������', 'SYSCATEGORY_16', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(16, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-17', 17, 10, NULL, false, '���� �������������� ��������', 'SYSCATEGORY_17', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(17, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-18', 18, 8, 17, true, '���������� ����� �������������� ��������', 'SYSCATEGORY_18', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(18, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-19', 19, 10, NULL, false, '���� ��������� ��������', 'SYSCATEGORY_19', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(19, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-20', 20, 8, 19, true, '���������� ����� ��������� ��������', 'SYSCATEGORY_20', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(20, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-29', 29, 10, NULL, false, '������ ������� ���������', 'SYSCATEGORY_29', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(29, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-30', 30, 2, 29, true, '������ ������� ���������', 'SYSCATEGORY_30', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(30, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-31', 31, 10, NULL, false, '��������� ������� ������� � ��������', 'SYSCATEGORY_31', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(31, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-32', 32, 8, 31, true, '���������� ��������� ������� ������� � ��������', 'SYSCATEGORY_32', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(32, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-33', 33, 10, NULL, false, '������������', 'SYSCATEGORY_33', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(33, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-34', 34, 2, 33, true, '������ �������� � ��������� ������������', 'SYSCATEGORY_34', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(34, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-37', 37, 10, NULL, false, '������ �������� ������� � �������� ������������', 'SYSCATEGORY_37', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(37, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-38', 38, 2, 37, true, '������ �������� ������� � �������� ������������', 'SYSCATEGORY_38', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(38, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-39', 39, 10, NULL, false, '���������', 'SYSCATEGORY_39', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(39, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-40', 40, 2, 39, true, '������ �������� � ��������� ��������� �������������', 'SYSCATEGORY_40', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(40, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-41', 41, 10, NULL, false, '���� ������������� ������', 'SYSCATEGORY_41', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(41, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-42', 42, 8, 41, true, '���������� ����� ������������� ������', 'SYSCATEGORY_42', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(42, -10, true, true, false, false, true);
                                                                                                                                                       
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-43', 43, 10, NULL, false, '������������������ ���������� ������������� ������', 'SYSCATEGORY_43', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(43, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-44', 44, 8, 43, true, '���������� ������������������ ���������� ������������� ������', 'SYSCATEGORY_44', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(44, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-45', 45, 10, NULL, false, '����� �����������', 'SYSCATEGORY_45', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(45, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-46', 46, 8, 45, true, '���������� ������ �����������', 'SYSCATEGORY_46', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(46, -10, true, true, false, false, true);




--����� ���������� ���������

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-47', 47, 10, NULL, false, '�������� ������', 'SYSCATEGORY_47', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(47, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-48', 48, 8, 47, true, '���������� �������� ������', 'SYSCATEGORY_48', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(48, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-49', 49, 10, NULL, false, '������������', 'SYSCATEGORY_49', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(49, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-50', 50, 8, 49, true, '���������� �������������', 'SYSCATEGORY_50', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(50, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-51', 51, 10, NULL, false, '���������', 'SYSCATEGORY_51', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(51, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-52', 52, 8, 51, true, '���������� ����������', 'SYSCATEGORY_52', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(52, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-53', 53, 10, NULL, false, '�����������', 'SYSCATEGORY_53', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(53, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-54', 54, 8, 53, true, '���������� �����������', 'SYSCATEGORY_54', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(54, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-55', 55, 10, NULL, false, '�������������', 'SYSCATEGORY_55', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(55, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-56', 56, 8, 55, true, '���������� �������������', 'SYSCATEGORY_56', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(56, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-59', 59, 10, NULL, false, '������ ����������������', 'SYSCATEGORY_59', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(59, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-60', 60, 8, 59, true, '���������� ������� ����������������', 'SYSCATEGORY_60', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(60, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-61', 61, 10, NULL, false, '��������� �������������', 'SYSCATEGORY_61', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(61, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-62', 62, 8, 61, true, '���������� ��������� �������������', 'SYSCATEGORY_62', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(62, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-63', 63, 10, NULL, false, '���� ������� ����������������', 'SYSCATEGORY_63', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(63, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-64', 64, 8, 63, true, '���������� ����� ������� ����������������', 'SYSCATEGORY_64', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(64, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-65', 65, 10, NULL, false, '���� �����������', 'SYSCATEGORY_65', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(65, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-66', 66, 8, 65, true, '���������� ����� �����������', 'SYSCATEGORY_66', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(66, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-67', 67, 10, NULL, false, '���������� �������������� ������', 'SYSCATEGORY_67', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(67, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-68', 68, 8, 67, true, '���������� ����������� �������������� ������', 'SYSCATEGORY_68', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(68, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-69', 69, 10, NULL, false, '������ �����������', 'SYSCATEGORY_69', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(69, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-70', 70, 8, 69, true, '���������� ������� �����������', 'SYSCATEGORY_70', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(70, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-71', 71, 10, NULL, false, '������� �������', 'SYSCATEGORY_71', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(71, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-72', 72, 8, 71, true, '���������� �������� �������', 'SYSCATEGORY_72', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(72, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-73', 73, 10, NULL, false, '��������� �������', 'SYSCATEGORY_73', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(73, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-74', 74, 8, 73, true, '���������� ��������� ��������', 'SYSCATEGORY_74', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(74, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-75', 75, 10, NULL, false, '���������������� �������', 'SYSCATEGORY_75', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(75, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-76', 76, 8, 75, true, '���������� ���������������� ��������', 'SYSCATEGORY_76', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(76, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-77', 77, 10, NULL, false, '������������� ����������', 'SYSCATEGORY_77', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(77, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-78', 78, 8, 77, true, '������������� ����������', 'SYSCATEGORY_78', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(78, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-79', 79, 10, NULL, false, '���� ������������� �������������� ��������', 'SYSCATEGORY_79', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(79, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-80', 80, 8, 79, true, '���������� ����� ������������� �������������� ��������', 'SYSCATEGORY_80', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(80, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-81', 81, 10, NULL, false, '����� (�������) ������������� �������������� ��������', 'SYSCATEGORY_81', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(81, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-82', 82, 8, 81, true, '����� (�������) ������������� �������������� ��������', 'SYSCATEGORY_82', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(82, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-83', 83, 10, NULL, false, '��������� ������������� �������������� ��������', 'SYSCATEGORY_83', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(83, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-84', 84, 8, 83, true, '���������� ���������� ������������� �������������� ��������', 'SYSCATEGORY_84', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(84, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-85', 85, 10, NULL, false, '���� XML-�������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_85', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(85, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-86', 86, 8, 85, true, '���������� ����� XML-�������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_86', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(86, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-87', 87, 10, NULL, false, 'XML-���� (�������) ������� �� ������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_87', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(87, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-88', 88, 8, 87, true, '���������� XML-����� (��������) ������� �� ������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_88', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(88, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-89', 89, 10, NULL, false, '���� XML-�������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_89', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(89, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-90', 90, 8, 89, true, '���������� ����� XML-�������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_90', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(90, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-91', 91, 10, NULL, false, 'XML-������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_91', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(91, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-92', 92, 8, 91, true, '���������� XML-�������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_92', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(92, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-93', 93, 10, NULL, false, '������ �� XML-������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_93', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(93, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-94', 94, 8, 93, true, '���������� ������� �� XML-������� ��� ������ ����������� � �������� ���������', 'SYSCATEGORY_94', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(94, -10, true, true, false, false, true);

---
---Internal qualifiers for IK TSO
---
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-95', 95, 10, NULL, false, '��������� (�� ���)', 'SYSCATEGORY_95', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(95, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-96', 96, 8, 95, true, '���������� ���������� (�� ���)', 'SYSCATEGORY_96', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(96, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-97', 97, 10, NULL, false, '���� ���� ������� (�� ���)', 'SYSCATEGORY_97', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(97, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-98', 98, 8, 97, true, '���������� ����� ���� ������� (�� ���)', 'SYSCATEGORY_98', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(98, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-99', 99, 10, NULL, false, '������� ���� ������� (�� ���)', 'SYSCATEGORY_99', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(99, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-100', 100, 8, 99, true, '���������� �������� ���� ������� (�� ���)', 'SYSCATEGORY_100', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(100, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-101', 101, 10, NULL, false, '����� ������� (�� ���)', 'SYSCATEGORY_101', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(101, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-102', 102, 8, 101, true, '���������� ������ ������� (�� ���)', 'SYSCATEGORY_102', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(102, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-103', 103, 10, NULL, false, '������ (�� ���)', 'SYSCATEGORY_103', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(103, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-104', 104, 8, 103, true, '���������� ������� (�� ���)', 'SYSCATEGORY_104', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(104, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-105', 105, 10, NULL, false, '���������� ������� (�� ���)', 'SYSCATEGORY_105', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(105, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-106', 106, 8, 105, true, '���������� ���������� �������� (�� ���)', 'SYSCATEGORY_106', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(106, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-107', 107, 10, NULL, false, 'MIME-����', 'SYSCATEGORY_107', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(107, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-108', 108, 8, 107, true, '���������� MIME-�����', 'SYSCATEGORY_108', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(108, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-109', 109, 10, NULL, false, '����������� ���������� (�� ���)', 'SYSCATEGORY_109', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(109, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-110', 110, 8, 109, true, '���������� ����������� ��������� (�� ���)', 'SYSCATEGORY_110', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(110, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-111', 111, 10, NULL, false, '������ �� ������� � ��������� �������� (�� ���)', 'SYSCATEGORY_111', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(111, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-112', 112, 8, 111, true, '���������� ������� �� ������� � ��������� �������� (�� ���)', 'SYSCATEGORY_112', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(112, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-113', 113, 10, NULL, false, '���������� � ��������� (�� ���)', 'SYSCATEGORY_113', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(113, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-114', 114, 8, 113, true, '���������� ��������� (�� ���)', 'SYSCATEGORY_114', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(114, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-115', 115, 10, NULL, false, '����. ����� (�� ���)', 'SYSCATEGORY_115', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(115, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-116', 116, 8, 115, true, '���������� ����. ������ (�� ���)', 'SYSCATEGORY_116', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(116, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-117', 117, 10, NULL, false, '���������� (��������) ����������� ������ (�� ���)', 'SYSCATEGORY_117', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(117, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-118', 118, 8, 117, true, '���������� ��������� (���������) ����������� ������ (�� ���)', 'SYSCATEGORY_118', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(118, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-119', 119, 10, NULL, false, '���� ����� ����������� ��������� (�� ���)', 'SYSCATEGORY_119', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(119, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-120', 120, 8, 119, true, '���������� ����� ����� ����������� ��������� (�� ���)', 'SYSCATEGORY_120', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(120, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-121', 121, 10, NULL, false, '���� �������� ��� ��������� (�� ���)', 'SYSCATEGORY_121', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(121, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-122', 122, 8, 121, true, '���������� ����� �������� ��� ��������� (�� ���)', 'SYSCATEGORY_122', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(122, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-123', 123, 10, NULL, false, '����� ����������� ����������� ���������� (�� ���)', 'SYSCATEGORY_123', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(123, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-124', 124, 8, 123, true, '���������� ������ ����������� ����������� ���������� (�� ���)', 'SYSCATEGORY_124', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(124, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-125', 125, 10, NULL, false, '������ ��� ����������� ��������� �� ����. ������ (�� ���)', 'SYSCATEGORY_125', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(125, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-126', 126, 8, 125, true, '���������� ����� ��� ����������� ��������� �� ����. ������ (�� ���)', 'SYSCATEGORY_126', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(126, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-127', 127, 10, NULL, false, '��������� ��������� �������� ����������� ��������� (�� ���)', 'SYSCATEGORY_127', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(127, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-128', 128, 8, 127, true, '���������� ���������� ��������� �������� ����������� ��������� (�� ���)', 'SYSCATEGORY_128', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(128, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-129', 129, 10, NULL, false, '������ ��������� ��������� (������������)', 'SYSCATEGORY_129', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(129, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-130', 130, 8, 129, true, '���������� ������� ��������� ��������� (������������)', 'SYSCATEGORY_130', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(130, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-131', 131, 10, NULL, false, '������ �������� ������������ � ���������', 'SYSCATEGORY_131', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(131, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-132', 132, 8, 131, true, '���������� ������� �������� ������������ � ���������', 'SYSCATEGORY_132', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(132, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-133', 133, 10, NULL, false, '������� �� ��� (������������)', 'SYSCATEGORY_133', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(133, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-134', 134, 8, 133, true, '������������ ���������� ������� �� ���', 'SYSCATEGORY_134', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(134, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-135', 135, 10, NULL, false, '����������� ���������� �� ��� (������������)', 'SYSCATEGORY_135', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(135, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-136', 136, 8, 135, true, '������������ ���������� ����������� ��������� �� ���', 'SYSCATEGORY_136', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(136, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-137', 137, 10, NULL, false, '���������� � ���������� ������� �� ��� (������������)', 'SYSCATEGORY_137', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(137, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-138', 138, 8, 137, true, '������������ ���������� ��������� � ���������� �������� �� ���', 'SYSCATEGORY_138', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(138, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-139', 139, 10, NULL, false, '�������� �������� �� ��� (������������)', 'SYSCATEGORY_139', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(139, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-140', 140, 8, 139, true, '������������ ���������� ��������� �������� �� ���', 'SYSCATEGORY_140', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(140, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-141', 141, 10, NULL, false, '����� ������� � �������� �� ��� (������������)', 'SYSCATEGORY_141', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(141, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-142', 142, 8, 141, true, '������������ ���������� ���� ������� � �������� �� ���', 'SYSCATEGORY_142', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(142, -10, true, true, false, false, true);

--������� �������� ���������
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-143', 143, 10, NULL, false, '���� ����������', 'SYSCATEGORY_143', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(143, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-144', 144, 8, 143, true, '���������� ����� ����������', 'SYSCATEGORY_144', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(144, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-145', 145, 10, NULL, false, '������� ����������', 'SYSCATEGORY_145', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(145, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-146', 146, 8, 145, true, '���������� �������� ����������', 'SYSCATEGORY_146', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(146, -10, true, true, false, false, true);

--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-147', 147, 10, NULL, false, '������� �������� ����������', 'SYSCATEGORY_147', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-148', 148, 11, NULL, true, '������� �������� ���������', 'SYSCATEGORY_148', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(148, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-149', 149, 10, NULL, false, '���� ����������', 'SYSCATEGORY_149', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(149, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-150', 150, 8, 149, true, '���������� ����� ����������', 'SYSCATEGORY_150', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(150, -10, true, true, false, false, true);

--TSO
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-151', 151, 11, NULL, true, '��������� ������� ���', 'SYSCATEGORY_151', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(151, -10, true, true, false, false, true);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-152', 152, 8, 151, true, '���������� ���������� �������� ������� ���', 'SYSCATEGORY_152', NULL::varchar, true, true, 1);

--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-151', 151, 10, NULL, false, '������� ������', 'SYSCATEGORY_151', NULL::varchar, true, true, 1);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-152', 152, 8, 151, true, '���������� ������� �������', 'SYSCATEGORY_152', NULL::varchar, true, true, 1);

--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-153', 153, 10, NULL, false, '��������� �����', 'SYSCATEGORY_153', NULL::varchar, true, true, 1);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-154', 154, 8, 153, true, '���������� ��������� ������', 'SYSCATEGORY_154', NULL::varchar, true, true, 1);

--
--ZARYA22
--
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-152', 152, 10, NULL, false, '�������� ������ (����22)', 'SYSCATEGORY_152', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(152, -10, true, true, false, false, true);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-153', 153, 8, 142, true, '������ �������� ������ (����22)', 'SYSCATEGORY_153', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(153, -10, true, true, false, false, true);
/*
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-154', 154, 10, NULL, false, '���������� ������� (����22)', 'SYSCATEGORY_154', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(154, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-155', 155, 8, 154, true, '������ ���������� �������� (����22)', 'SYSCATEGORY_155', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(155, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-156', 156, 10, NULL, false, '�������������� ������� ������� ����������� ���� (����22)', 'SYSCATEGORY_156', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(156, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-157', 157, 8, 156, true, '������ ������������� ������� ������� ����������� ���� (����22)', 'SYSCATEGORY_157', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(157, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-158', 158, 10, NULL, false, '��������������� ������� (����22)', 'SYSCATEGORY_158', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(158, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-159', 159, 8, 158, true, '������ ��������������� ������ (����22)', 'SYSCATEGORY_159', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(159, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-160', 160, 10, NULL, false, '����������� ������� (����22)', 'SYSCATEGORY_160', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(160, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-161', 161, 8, 160, true, '������ ����������� ������ (����22)', 'SYSCATEGORY_161', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(161, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-162', 162, 10, NULL, false, '��� (����22)', 'SYSCATEGORY_162', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(162, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-163', 163, 8, 162, true, '������ ��� (����22)', 'SYSCATEGORY_163', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(163, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-164', 164, 10, NULL, false, '����������� ���� (����22)', 'SYSCATEGORY_164', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(164, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-165', 165, 8, 164, true, '������ ������������ ��� (����22)', 'SYSCATEGORY_165', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(165, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-166', 166, 10, NULL, false, '������ (����22)', 'SYSCATEGORY_166', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(166, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-167', 167, 8, 166, true, '������ ������� (����22)', 'SYSCATEGORY_167', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(167, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-168', 168, 3, NULL, true, '������� �������� ��������� (����22)', 'SYSCATEGORY_168', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(168, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-169', 169, 3, NULL, true, '������� �������� ������� (����22)', 'SYSCATEGORY_169', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(169, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-170', 170, 3, NULL, true, '��������� � ������������ �������� (����22)', 'SYSCATEGORY_170', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(170, -10, true, true, false, false, true);
*/

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-171', 171, 10, NULL, false, '������ ��������� (����������������)', 'SYSCATEGORY_171', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(171, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-172', 172, 8, 171, true, '���������� ����� ���������', 'SYSCATEGORY_172', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(172, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-173', 173, 10, NULL, false, '���� ����������� ���', 'SYSCATEGORY_173', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(173, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-174', 174, 8, 173, true, '���������� ����� ����������� ���', 'SYSCATEGORY_174', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(174, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-175', 175, 10, NULL, false, '��������� ���������', 'SYSCATEGORY_175', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(175, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-176', 176, 8, 175, true, '���������� ���������� ���������', 'SYSCATEGORY_176', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(176, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-177', 177, 10, NULL, false, '���� �����������', 'SYSCATEGORY_177', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(177, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-178', 178, 8, 177, true, '���������� ����� �����������', 'SYSCATEGORY_178', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(178, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-179', 179, 10, NULL, false, '����������', 'SYSCATEGORY_179', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(179, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-180', 180, 8, 179, true, '���������� �����������', 'SYSCATEGORY_180', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(180, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-181', 181, 10, NULL, false, '�������� �����������', 'SYSCATEGORY_181', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(181, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-182', 182, 8, 181, true, '���������� �������� �����������', 'SYSCATEGORY_182', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(182, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-183', 183, 10, NULL, false, '�������� ��������� � �������������� ��������', 'SYSCATEGORY_183', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(183, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-184', 184, 8, 183, true, '���������� �������� ��������� � �������������� ��������', 'SYSCATEGORY_184', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(184, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-185', 185, 10, NULL, false, '�������� � ����������', 'SYSCATEGORY_185', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(185, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-186', 186, 8, 185, true, '���������� ��������� � ����������', 'SYSCATEGORY_186', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(186, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-187', 187, 10, NULL, false, '�������� ��������� � ���������', 'SYSCATEGORY_187', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(187, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-188', 188, 8, 187, true, '���������� �������� ��������� � ���������', 'SYSCATEGORY_188', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(188, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-189', 189, 10, NULL, false, '�������� � ���������', 'SYSCATEGORY_189', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(189, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-190', 190, 8, 189, true, '���������� ��������� � ���������', 'SYSCATEGORY_190', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(190, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-191', 191, 10, NULL, false, '������ �������������', 'SYSCATEGORY_191', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(191, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-192', 192, 8, 191, true, '���������� ������� �������������', 'SYSCATEGORY_192', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(192, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-193', 193, 10, NULL, false, '������ ���������', 'SYSCATEGORY_193', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(193, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-194', 194, 8, 193, true, '���������� ������� ���������', 'SYSCATEGORY_194', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(194, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-195', 195, 10, NULL, false, '��������� ����� ���������', 'SYSCATEGORY_195', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(195, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-196', 196, 8, 195, true, '���������� ��������� ������ ���������', 'SYSCATEGORY_196', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(196, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-197', 197, 10, NULL, false, '�������� ��������� � ��������� ������', 'SYSCATEGORY_197', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(197, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-198', 198, 8, 197, true, '���������� ��������� ��������� � ��������� ������', 'SYSCATEGORY_198', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(198, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-199', 199, 10, NULL, false, '�������-����������� ��������', 'SYSCATEGORY_199', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(199, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-200', 200, 8, 199, true, '���������� ��������-������������ ��������', 'SYSCATEGORY_200', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(200, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-201', 201, 10, NULL, false, '�������� �������� ���������', 'SYSCATEGORY_201', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(201, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-202', 202, 8, 201, true, '���������� �������� �������� ���������', 'SYSCATEGORY_202', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(202, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-203', 203, 10, NULL, false, '������� ��������� (������ ��� ���������)', 'SYSCATEGORY_203', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(203, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-204', 204, 8, 203, true, '���������� �������� ��������� (������ ��� ���������)', 'SYSCATEGORY_204', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(204, -10, true, true, false, false, true);


SELECT pg_catalog.setval('io_categories_id_seq', 300, true); --��� ���������������� ��������� ����� ���������� � ������ 301
                                                             --��� ������� ��� ����, ����� �������� ������ ��� ��������� ��������� 

update io_categories set is_completed = 1;

--�������� "�������� ���������"
--�����!!! ��� ��������� ��������� ����� �� ��������������� ��� ��������, �� ������� ��� ���������. ��� �������� ��������� ��������� � ����� ������� �������
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-1', 1, 8, 'id', '�������������', '��', NULL, NULL, 50, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-2', 2, 9, 'name', '��������', '��������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-3', 3, 9, 'description', '��������', '��������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-4', 4, 2, 'id_a_view', '���������� ������� ����������', '������', 'a_views', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-5', 5, 2, 'id_a_type', '��� ��������', '��� ��������', 'a_types', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-6', 6, 9, 'code', '���', '���', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-7', 7, 9, 'title', '���������� �������', '�������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-8', 8, 9, 'table_name', '�������� �������', '�������� �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-9', 9, 9, 'column_name', '�������� ������� �������', '������� �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-10', 10, 2, 'id_io_category_type', '��� ��������� ��������������� �������', '��� ���������', 'io_category_types', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-11', 11, 3, 'id_child', '����������� ���������', '����������� ���������', 'io_categories', 'name', 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-12', 12, 2, 'id_io_category', '��������� ��������������� �������', '��������� ��', 'io_categories', 'name', 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-13', 13, 2, 'author', '����� (���������)', '�����', 'users', 'fio', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-14', 14, 13, 'information', '���������� �� ������ ��', '���������� �� ������ ��', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-15', 15, 1, 'is_system', '���������', '���������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-16', 16, 5, 'insert_time', '���� � ����� ��������', '������( ������ � ��)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-17', 17, 1, 'is_main', '�������� ���������', '�������� ���������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-18', 18, 2, 'id_io_state', '���������', '���������', 'io_states', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-19', 19, 9, 'short_name', '�������� ��������', '�������� ��������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-20', 20, 2, 'id_rank', '�������� ������', '������', 'ranks', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-21', 21, 9, 'role_name', '��������� ������������', '��������� ������������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-22', 22, 9, 'lastname', '�������', '�������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-23', 23, 9, 'firstname', '���', '���', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-24', 24, 9, 'sirname', '��������', '��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-25', 25, 9, 'fio', '���', '���', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-26', 26, 1, 'is_boss', '�������� �������������', '������������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-27', 27, 3, 'id_parent', '��������', '������ �', NULL, 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-28', 28, 9, 'address', '����� ����������', '����� ����������', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-29', 29, 9, 'ref_column_name', '�������� ��������� �������', '��������� �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-30', 30, 2, 'id_mimetype', 'MIME-���', 'MIME-���', 'mimetypes', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-31', 31, 2, 'id_unit', '�������������', '�������������', 'units', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-32', 32, 2, 'id_user', '������������', '������������', 'users', 'fio', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-33', 33, 1, 'is_io', '�������� ����������� �����������', '����', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-34', 34, 2, 'id_dl_to', '���� �����������', '���� ����������� ������ �� ����������', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-35', 35, 2, 'id_dl_executor', '������������� �����������', '������������� �����������', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-36', 36, 2, 'id_dl_controller', '��������� ����������', '��������� ����������', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-37', 37, 5, 'exec_datetime', '����-����� ����������', '��������� �', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-38', 38, 10, 'exec_period', '������������� ����������', '�������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-39', 39, 13, 'osnovanie', '���������', '���������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-40', 40, 1, 'is_archived', '� ������', '� ������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-41', 41, 2, 'id_jr_state', '��������� ������ �������', '���������', 'jr_states', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-42', 42, 2, 'id_dl_from', '�����������', '�����������', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-43', 43, 2, 'id_io_type', '��� ��������������� �������', '��� ��', 'io_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-44', 44, 1, 'is_automated', '������ ��������� (�����������������/�������������������)', '������� �����������������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-45', 45, 13, 'message_body', '���� ���������', '���� ���������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-46', 46, 9, 'type', '���', '���', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-47', 47, 2, 'id_journal', '������ ����� ������������ � ������', '������', 'command_journal', 'id', 50, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-48', 48, 2, 'id_io_object', '�������������� ������ (��������)', '��������', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-49', 49, 5, 'planned_exec', '��������������� ����� ����������', '��������������� ����� ����������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-50', 50, 5, 'real_exec', '�������� ����� ����������', '�������� ����� ����������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-51', 51, 2, 'id_dl_sender', '�����������', '�����������', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-52', 52, 2, 'id_dl_receiver', '����������', '����������', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-53', 53, 2, 'id_search_template_type', '��� ���������� �������', '��� ���������� �������', 'search_template_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-54', 54, 5, 'sent_datetime', '����� ��������', '����� ��������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-55', 55, 5, 'receive_datetime', '����� ���������', '����� ���������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-56', 56, 5, 'read_datetime', '����� ���������', '����� ���������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-57', 57, 8, 'def_width', '���������� ������ ������� � �������', '������ �������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-58', 58, 9, 'group_name', '��������� ������', '��������� ������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-59', 59, 9, 'win_app', '���������� ��� ������� ����� � ����� Windows', 'Windows-����������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-60', 60, 9, 'lin_app', '���������� ��� ������� ����� � ����� ����', '����-����������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-61', 61, 12, 'extentions', '����� ��������������� � ����� ����� ����������', '��������������� ���������� ������', 'url_extentions', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-62', 62, 1, 'with_inheritance', '����������� ����� �������', '����������� ����� �������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-63', 63, 2, 'id_maclabel', '���� �����������', '���� �����������', 'maclabels', 'name', 50, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-64', 64, 16, 'mac_value', '��������� �����', '��������� �����', NULL, NULL, 50, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-65', 65, 2, 'id_state', '���������', '��������� ������������', 'user_state', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-66', 66, 9, 'email', '����� ��. �����', '����� ����������� �����', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-67', 67, 9, 'acc_right_num', '����� �������', '����� �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-68', 68, 9, 'acc_right_date', '���� ������ �������', '���� ������ �������', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-1', 66, 1, 'is_connected', 'is_connected', 'is_connected', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-69', 69, 9, 'subtype', '������', '������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-70', 70, 2, 'id_user_vrio', '����', '������������ ����', 'users', 'fio', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-71', 71, 17, 'position_work_mode', '������ �-� ���������', '������, ��� ������� ��������� �������������', 'work_mode', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-72', 72, 2, 'id_mode_type', '��� ������', '��� ������ ����������������', 'work_mode_type', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-73', 73, 15, 'symbol', '�������� ����', '�������� ���� ������', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-1', XX, XX, 'audio_file', '�����-����', '�����-���� ������ ����������������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-74', 74, 2, 'id_organization', '�����������', '�����������', 'organization', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-75', 75, 2, 'id_curr_mode', '������� �����', '������� ����� ����������������', 'work_mode', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-76', 76, 2, 'id_boss', '������������', '������������', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-77', 77, 9, 'code_name', '������� ��������', '������� ��������', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-78', 78, 15, 'tree_symbol', '�������� ����', '�������� ����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-79', 79, 1, 'is_fgroup', '�������� �������������� �������', '�������� �������������� �������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-80', 80, 5, 'time_elapsed', '����� ������� ������������� �������������� ������', '����� ������� ������������� ��', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-81', 81, 17, 'units_work_mode', '������ �-� �������������', '������, ��� ������� ������������� �������������', 'work_mode', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-82', 82, 17, 'organization_work_mode', '������ �-� �����������', '������, ��� ������� ����������� �������������', 'work_mode', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-83', 83, 2, 'id_type_org', '��� �����������', '��� �����������', 'organization_type', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-84', 84, 3, 'id_parent1', '������ � (�� ������ ������)', '������ � (�� ������ ������)', NULL, 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-85', 85, 3, 'id_substitutor', '��������� ����������� ��� ������ �� �����', '��������� ����������� ��� ������ �� �����', NULL, 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-86', 86, 2, 'id_prev_mode', '���������� �����', '���������� ����� ����������������', 'work_mode', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-87', 87, 9, 'email_prefix', '������� ��. �����', '������� ��. �����', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-88', 88, 6, 'latitude', '������', '������', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-89', 89, 6, 'longitude', '�������', '�������', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-90', 90, 15, 'map_symbol', '�������� ���� �� �����', '�������� ���� �� �����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-91', 91, 5, 'dt_prev_mode', '����� �������� � ���������� �����', '����� �������� � ���������� �����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-92', 92, 5, 'dt_curr_mode', '����� �������� � ������� �����', '����� �������� � ������� �����', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-93', 93, 1, 'is_active', '�������', '��������� �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-94', 94, 2, 'id_transport', '���������', '��������� �������������� ������', 'transport', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-95', 95, 9, 'local_address', '��������� �����', '��������� ����� ����������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-96', 96, 5, 'time_start', '������ ������� ������������� �������������� ������', '������ ������� ������������� ��', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-97', 97, 17, 'io_objects_organization', '���������������� �����������', '���������������� �����������', 'organization', 'name', 200, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-98', 98, 17, 'io_categories_organization', '���������������� �����������', '���������������� �����������', 'organization', 'name', 200, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-99', 99, 9, 'input_number', '�������� �����', '�������� �����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-100', 100, 9, 'output_number', '��������� �����', '��������� �����', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-101', 101, 9, 'report_name', '�������� �������', '�������� ������� ������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-102', 102, 9, 'report_descrip', '�������� �������', '�������� ������� ������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-103', 103, 8, 'report_grade', '������� �������', '������� ������� ������', NULL, NULL, 70, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-104', 104, 2, 'shb', '������ ������', '������ ������', 'report', 'report_name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-105', 105, 9, 'p_value', '�������� ���������', '�������� ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-106', 106, 17, 'report_organization', '���������������� �����������', '���������������� �����������', 'organization', 'name', 200, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-107', 107, 17, 'user_chains_organization', '���������������� �����������', '���������������� �����������', 'organization', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-108', 108, 8, 'trig_level', '���������� �������', '���������� �������', NULL, NULL, 80, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-109', 109, 9, 'proc_name', '�������� ��������� ��', '��������� �������� ��������� ��', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-110', 110, 2, 'id_search_template', '��������� ������', '����������� ������', 'search_templates', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-111', 111, 1, 'is_global', '�������� ����������', '����������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-112', 112, 2, 'id_sync_type', '��� �������������', '��� �������������', 'io_sync_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-113', 113, 8, 'sync_result', '��������� �������������', '��������� �������������', NULL, NULL, 80, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-114', 114, 10, 'min_period', '����������� ������ �������������', '����������� ������ �������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-115', 115, 5, 'last_sync', '���� � ����� ��������� �������������', '���� � ����� ��������� �������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-116', 116, 5, 'sync_datetime', '���� � ����� �������������', '���� � ����� �������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-117', 117, 2, 'id_owner_org', '�����������-��������', '�����������-��������', 'organization', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-118', 118, 1, 'is_public', '�������� ���������', '�������� ���������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-119', 119, 9, 'phone', '��������� �������', '��������� �������', NULL, NULL, 150, TRUE);

--����� ����������� � �������� ��������� (��������, �� ���)
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-120', 120, 2, 'id_xml_query_type', '��� XML-������� � ������� �������', '��� XML-�������', 'xml_query_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-121', 121, 9, 'xpath', '���� � ���� � ������ XML-������', '���� � ���� � ������ XML-������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-122', 122, 9, 'xcode', '��� (��������) XML-����', '��� (��������) XML-����', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-123', 123, 9, 'ref_table', '������� ��, ���������� ������, ����������� XML-�����', '������� ��, ���������� ������, ����������� XML-�����', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-124', 124, 2, 'id_xml_query_kind', '��� XML-������� � ������� �������', '��� XML-�������', 'xml_query_kinds', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-125', 125, 20, 'xml_data', 'XML-��������', 'XML-��������', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-126', 126, 21, 'exec_period_h', '������������� ����������', '������������� ����������', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-127', 127, 2, 'id_xml_query', 'XML-������ � ������� �������', 'XML-������', 'xml_queries', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-128', 128, 1, 'is_mandatory', '�������� ������������', '�������� ������������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-129', 129, 5, 'exec_time', '����-����� ���������� �������', '����-����� ���������� �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-130', 130, 9, 'data_type', '��� ������', '��� ������', NULL, NULL, 120, TRUE);

--�� ���
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-131', 131, 5, 'guard_datetime', '��������� ��� ������', '��������� ��� ������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-132', 132, 8, 'fault_code', '��� ������', '��� ������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-133', 133, 9, 'fault_string', '�������� ������', '�������� ������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-134', 134, 5, 'reg_datetime', '������������� �������������', '������������� �������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-135', 135, 9, 'fault_state_name', '�������� ��������� ����������', '�������� ��������� ����������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-136', 136, 9, 'fault_state_value', '��������� ����������', '��������� ����������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-137', 137, 2, 'id_device', '����������', '����������', 'devices', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-138', 138, 17, 'guard_objects_devices', '����������, �������� � ������ ������', '����������, �������� � ������ ������', 'devices', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-139', 139, 2, 'id_person', '����������� ����', '����������� ����', 'persons', 'lastname', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-140', 140, 5, 'event_date', '������� ���������', '������� ���������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-141', 141, 9, 'message', '�������� �������', '�������� �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-142', 142, 9, 'source', '�������� �������', '�������� �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-143', 143, 8, 'alarm', '������� ������� �������', '������� ������� �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-144', 144, 9, 'card_code', '��� ����� �������', '��� ����� �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-145', 145, 1, 'pass', '���� �������', '���� �������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-146', 146, 9, 'direction', '����������� �������', '����������� �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-147', 147, 24, 'video', '�����-�����', '�����-�����', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-148', 148, 2, 'id_card_type', '��� ����� �������', '��� ����� �������', 'card_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-149', 149, 2, 'id_card_status', '������ ����� �������', '������ ����� �������', 'card_statuses', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-150', 150, 2, 'id_access_plan', '���� �������', '���� �������', 'access_plan', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-151', 151, 2, 'id_tso_position', '���������', '���������', 'tso_positions', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-152', 152, 2, 'id_tso_unit', '�������������', '�������������', 'tso_units', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-153', 153, 9, 'comment', '�����������', '�����������', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-154', 154, 9, 'bg_image', '����������� ��������', '����������� ��������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-155', 155, 23, 'svg_image', '����������� SVG', '����������� SVG', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-156', 156, 2, 'id_graph_plan', '����. ����', '����. ����', 'graph_plans', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-157', 157, 2, 'id_shape_type', '��� ������', '��� ������', 'shape_types', 'name', 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-158', 158, 2, 'id_element', '����������', '����������', 'graph_plan_elements', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-159', 159, 2, 'id_graph_style', '����� �����������', '����� �����������', 'graph_styles', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-160', 160, 2, 'id_segment_type', '��� ��������', '��� ��������', 'segment_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-161', 161, 2, 'id_element_shape', '������', '������', 'element_shapes', 'id', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-162', 162, 22, 'points', '����������', '����������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-163', 163, 8, 'p_order', '���������� ����� ����������', '���������� ����� ����������', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-164', 164, 15, 'urgency_symbol', '�������� ���� ������ ���������', '�������� ���� ������ ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-165', 165, 2, 'id_urgency_level', '������� ���������', '������� ���������', 'urgency_levels', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-166', 166, 17, 'mail_lists_position', '�������� ����������� ��� � ������ ��������', '�������� ����������� ��� � ������ ��������', 'position', 'name', 200, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-167', 167, 2, 'id_io_object_att', '������� ����������', '������� ����������', 'io_objects', 'name', 150, TRUE);

--�� ���
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-168', 168, 8, 'id_person_p', '����������� ���� (��)', '����������� ���� (��)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-169', 169, 8, 'id_device_p', '���������� (��)', '���������� (��)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-170', 170, 8, 'id_unit_p', '������������� (��)', '������������� (��)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-171', 171, 9, 'unit_name', '�������������', '�������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-172', 172, 8, 'id_dolzh_p', '��������� (��)', '��������� (��)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-173', 173, 9, 'dolzh_name', '���������', '���������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-174', 174, 9, 'dev_name', '����������', '����������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-175', 175, 8, 'id_guard_obj_p', '���������� ������ (��)', '���������� ������ (��)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-176', 176, 9, 'guard_obj_name', '���������� ������', '���������� ������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-177', 177, 23, 'device_svg', '����-���� ����������', '����. ���� ����������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-178', 178, 23, 'plan_svg', '����-����', '����. ����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-179', 179, 1, 'is_ok', '���������� ��������', '���������� ��������', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-180', 180, 5, 'ok_datetime', '����������������� ������������� (��������)', '����������������� ������������� (��������)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-181', 181, 12, 'access_cards', '����� �������', '����� �������', 'access_cards_q', 'code', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-182', 182, 8, 'id_card_type_p', '��� ����� (��)', '��� ����� (��)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-183', 183, 9, 'card_type_name', '��� �����', '��� �����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-184', 184, 8, 'id_card_status_p', '������ ����� (��)', '������ ����� (��)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-185', 185, 9, 'card_status_name', '������ �����', '������ �����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-186', 186, 12, 'access_plans', '����� �������', '����� �������', 'access_plan', 'name', 100, TRUE);

--������� �������� ����������
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-187', 187, 9, 'r_name', '�������� (��.�.)', '�������� (��.�.)', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-188', 188, 5, 'accepted_datetime', '����-����� �������� � ����������', '������� � ����������', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-189', 189, 2, 'id_document_type', '��� ���������', '��� ���������', 'document_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-190', 190, 2, 'id_document_subtype', '������ ���������', '������ ���������', 'document_subtypes', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-191', 191, 2, 'id_exec_org', '�����������-�����������', '�����������-�����������', 'organization', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-192', 192, 2, 'id_document_kind', '��� ���������', '��� ���������', 'document_kinds', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-193', 193, 5, 'regout_datetime', '����-����� ����������� (���.)', '����-����� ����������� (���.)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-194', 194, 5, 'regin_datetime', '����-����� ����������� (��.)', '����-����� ����������� (��.)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-195', 195, 9, 'doc_header', '���������', '���������', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-196', 196, 8, 'page_count', '���-�� ������', '���-�� ������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-197', 197, 9, 'ekz_number', '���������', '���������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-198', 198, 9, 'book_number', '����', '����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-199', 199, 9, 'book_subnumber', '������ � ����', '������ � ����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-200', 200, 9, 'class_code', '����������������� ���', '����������������� ���', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-201', 201, 9, 'inv_number', '����������� �����', '����������� �����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-202', 202, 9, 'registrator_info', '���������������', '���������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-203', 203, 13, 'annotation', '���������', '���������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-204', 204, 13, 'command_body', '���������', '���������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-205', 205, 5, 'create_datetime', '����-����� ��������', '����-����� ��������', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-206', 206, 18, 'record_fill_color', '���� ���� ��� ����������� � �������', '���� ���� ��� ����������� � �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-207', 207, 25, 'record_text_color', '���� ������ ��� ����������� � �������', '���� ������ ��� ����������� � �������', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-208', 208, 9, 'org_name', '������������ �����', '������������ �����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-209', 209, 9, 'org_uid', 'UID �����', 'UID �����', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-210', 210, 8, 'tso_max_level', '������������ ������� �������', '������������ ������� �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-211', 211, 9, 'tso_dl_from', 'UID ������������ ����, ����������� ������������', 'UID ������������ ����, ����������� ������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-212', 212, 9, 'tso_dl_exec', 'UID ������������ ����, ����������� ������������', 'UID ������������ ����, ����������� ������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-213', 213, 9, 'tso_dl_to', 'UID ������������ ����, ����������� ������������', 'UID ������������ ����, ����������� ������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-214', 214, 9, 'tso_category', 'UID ��������� ������� �� ����������', 'UID ��������� ������� �� ����������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-215', 215, 21, 'tso_exectime', '����� ������� �� ����������', '����� ������� �� ����������', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-216', 216, 9, 'ref_table_name', '�������� ������� �������', '�������� ������� �������', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-217', 217, 9, 'video_url', '������ �� �����-�����', '������ �� �����-�����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-218', 218, 1, 'for_report', '������� ��������� � �������� �����', '������� ��������� � �������� �����', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-219', 219, 9, 'tso_state', '��������� ���', '��������� ���', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-220', 220, 18, 'tso_state_color', '��������� ��� (����)', '��������� ��� (����)', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-221', 221, 1, 'is_deleted', '������ �� ���������� �������', '������ �� ���������� �������', NULL, NULL, 150, TRUE);

--
--ZARYA22
--
/*
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-222', 222, 9, 'type', '���', '���', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-223', 223, 9, 'value', '��������', '��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-224', 224, 9, 'uri', 'URI', 'URI', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-225', 225, 2, 'id_acs', '���', '���', 'shu_acs', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-226', 226, 2, 'id_org', '��������������� �������', '��������������� �������', 'shu_orgs', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-227', 227, 2, 'id_pos', '����������� �������', '����������� �������', 'shu_positions', 'name', 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-228', 228, 2, 'id_shu_dls', '����������� ����', '����������� ����', 'shu_dls', 'name', 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-229', 229, 2, 'id_shu_rank', '�������� ������', '�������� ������', 'shu_ranks', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-230', 230, 2, 'id_shu_domain', '���������� �������', '���������� �������', 'shu_domains', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-231', 231, 9, 'shu_working_mode', '����� ����������������', '����� ����������������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-232', 232, 9, 'shu_msg_type', '��� ���������', '��� ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-233', 233, 9, 'shu_creation_date', '���� ��������', '���� ��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-234', 234, 9, 'shu_creation_time', '����� �������� ', '����� ��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-235', 235, 9, 'shu_maclabel', '������� �������', '������� �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-236', 236, 9, 'shu_priority', '���������', '���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-237', 237, 13, 'shu_text', '����� ���������', '����� ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-238', 238, 9, 'shu_cmd_code', '��� �������', '��� �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-239', 239, 13, 'shu_cmd_text', '����� �������', '����� �������', NULL, NULL, 150, TRUE);

--���������
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-258', 258, 9, 'shu_echelon', '������', '������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-259', 259, 9, 'shu_cdomain', '����� ���������� �������', '����� ���������� �������', NULL, NULL, 150, TRUE);

--������������ ��������
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-240', 240, 9, 'shu_emerg_type', '��� ������������ ��������', '��� ������������ ��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-241', 241, 5, 'shu_emerg_datetime', '���� � ����� ������������� ��', '���� � ����� ������������� ��', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-244', 244, 9, 'shu_district', '������� �����', '������� �����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-245', 245, 9, 'shu_region', '�������', '�������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-246', 246, 9, 'shu_area', '�����', '�����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-260', 260, 9, 'shu_town', '�����', '�����', NULL, NULL, 150, TRUE);

--�������� � ������������� ���������
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-261', 261, 9, 'shu_document_entry', '�������� ���������', '�������� ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-242', 242, 9, 'shu_obj_open_name', '������ �������� ������������ �������', '������ �������� ������������ �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-243', 243, 9, 'shu_obj_close_name', '������ �������� ������������ �������', '������ �������� ������������ �������', NULL, NULL, 150, TRUE);

--��������������
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-262', 262, 9, 'shu_block', '������������/������� ����', '������������/������� ����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-263', 263, 9, 'shu_country', '������', '������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-264', 264, 9, 'shu_department', '���������', '���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-265', 265, 9, 'shu_company', '�����', '�����', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-266', 266, 9, 'shu_block', '������������/������� ����', '������������/������� ����', NULL, NULL, 150, TRUE);

--������������ ����� ��
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-257', 257, 15, 'shu_emerg_pict', '������������ ����� ��', '������������ ����� ��', NULL, NULL, 150, TRUE);

--������������ �����������
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-267', 267, 9, 'shu_authenticity', '�������������', '�������������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-268', 268, 5, 'shu_instrumentation_event_time', '����� ����� ����������', '����� ����� ����������', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-269', 269, 9, 'shu_authenticity', '�������������', '�������������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-270', 270, 9, 'shu_cdomain1', '����� ���������� �������', '����� ���������� �������', NULL, NULL, 150, TRUE);

--������� �������� ��
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-249', 249, 9, 'shu_emerg_desc', '������� �������� ������������ ��������', '������� �������� ������������ ��������', NULL, NULL, 150, TRUE);

--
--������������ �����������
--����������
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-271', 271, 9, 'shu_height_uom', '������� ��������� ������', '������� ��������� ������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-247', 247, 9, 'shu_emerg_lat', '���������� �� (������)', '���������� �� (������)', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-248', 248, 9, 'shu_emerg_lon', '���������� �� (�������)', '���������� �� (�������)', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-272', 272, 9, 'shu_emerg_height', '���������� �� (������)', '���������� �� (������)', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-273', 273, 9, 'shu_coord_uom', '������� ��������� ���������', '������� ��������� ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-276', 276, 9, 'shu_cdomain2', '����� ���������� �������', '����� ���������� �������', NULL, NULL, 150, TRUE);

--��������� ���������� �����
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-274', 274, 9, 'shu_life_cycle_type', '��� ���������� �����', '��� ���������� �����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-275', 275, 9, 'shu_life_cycle_indicators', '����������', '����������', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-276', 276, 9, 'shu_life_cycle_type', '��� ���������� �����', '��� ���������� �����', NULL, NULL, 150, TRUE);

--������� ���
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-251', 251, 9, 'shu_emerg_dang_area', '������ ������� ����', '������ ������� ����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-277', 277, 9, 'shu_emerg_dang_area_uom', '������� ��������� �������', '������� ��������� �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-253', 253, 9, 'shu_emerg_dang_area1', '������ ���� ������������ ���������', '������ ���� ������������ ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-278', 278, 9, 'shu_emerg_dang_area1_uom', '������� ��������� �������', '������� ��������� �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-283', 283, 9, 'shu_emerg_safe_area', '������ ���������� ����', '������ ���������� ����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-279', 279, 9, 'shu_emerg_safe_area_uom', '������� ��������� �������', '������� ��������� �������', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-250', 250, 9, 'shu_emerg_rad_level', '������� �������� ������ ���������', '������� �������� ������ ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-280', 280, 9, 'shu_emerg_rad_level_uom', '������� ���������', '������� ���������', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-252', 252, 9, 'shu_emerg_dang_area_rad_level', '������� �������� � ������� ����', '������� �������� � ������� ����', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-281', 281, 9, 'shu_emerg_dang_area_rad_level_uom', '������� ���������', '������� ���������', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-254', 254, 9, 'shu_emerg_dang_area_rad_level1', '������� �������� � ���� ������������ ���������', '������� �������� � ���� ������������ ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-282', 282, 9, 'shu_emerg_dang_area_rad_level1_uom', '������� ���������', '������� ���������', NULL, NULL, 150, TRUE);

--����������
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-256', 256, 9, 'shu_emerg_evac_act', '����������� ����������� �� ���������', '����������� ����������� �� ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-255', 255, 9, 'shu_emerg_lic_act', '����������� ����������� �� ���������� ����������� ��', '����������� ����������� �� ���������� ����������� ��', NULL, NULL, 150, TRUE);
*/

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-284', 284, 2, 'id_attr_group', '������', '������', 'attrs_groups', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-285', 285, 2, 'id_position_type', '��� ��', '��� ��', 'position_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-286', 286, 9, 'the_value', '��������', '��������', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-291', 291, 2, 'id_indicator_type', '��� ����������', '��� ����������', 'indicator_type', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-292', 292, 2, 'id_io_object', '������', '������', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-293', 293, 2, 'id_io_object_src', '���������� �������', '���������� �������', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-294', 294, 2, 'id_io_object_src1', '�� ���� �������', '�� ���� �������', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-295', 295, 5, 'meas_time', '����� ���������', '����� ���������', NULL, NULL, 120, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-295', 295, 5, 'meas_time', '����� ���������', '����� ���������', NULL, NULL, 120, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-296', 296, 9, 'value', '��������', '��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-297', 297, 5, 'start_time', '�������� �', '�������� �', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-298', 298, 5, 'stop_time', '�������� ��', '�������� ��', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-299', 299, 2, 'id_indicator', '���������', '���������', 'indicator', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-300', 300, 2, 'id_attr_category', '������� � ���������', '������� � ���������', 'attrs_categories', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-301', 301, 9, 'value', '�������� ��������', '�������� ��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-302', 302, 1, 'is_actual', '�������� �������� ���������', '�������� �������� ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-303', 303, 2, 'id_io_attribute', '�������', '�������', 'attributes', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-304', 304, 9, 'def_value', '�������� �� ���������', '�������� �� ���������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-305', 305, 1, 'is_read_only', '�������� �������� �� ������������', '�������� �������� �� ������������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-306', 306, 2, 'id_attr_parent', '����������� �������', '����������� �������', 'attributes', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-307', 307, 2, 'id_attr_child', '����������� �������', '����������� �������', 'attributes', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-308', 308, 2, 'id_attr_attr', '�������', '�������', 'attrs_attrs', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-309', 309, 2, 'id_attr_value', '�������� ��������', '�������� ��������', 'attrs_values', 'value', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-310', 310, 1, 'is_qualifier', '�������� ������������', '�������� ������������', NULL, NULL, 50, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-311', 311, 3, 'id_child2', '����������� ��������� �����������', '����������� ��������� �����������', 'io_categories', 'name', 300, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-312', 312, 15, 'r_icon', '�������� ���� ��������', '�������� ���� ��������', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-313', 313, 5, 'start_time', '������ ������', '������ ������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-314', 314, 5, 'stop_time', '��������� ������', '��������� ������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-315', 315, 2, 'id_partition_low', '����� �������������', '����� �������������', 'partition_lows', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-316', 316, 6, 'lambda', '������', '������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-317', 317, 6, 'sigma', '�����', '�����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-318', 318, 6, 'moda', '����', '����', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-319', 319, 6, 'min_p', '���.', '���.', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-320', 320, 6, 'max_p', '����.', '����.', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-321', 321, 2, 'id_start_state', '��������� ���������', '��������� ���������', 'io_states', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-322', 322, 17, 'life_cycle_io_states', '�������� ��������� ���������� �����', '�������� ��������� ���������� �����', 'io_states', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-323', 323, 2, 'id_life_cycle', '��������� ����', '��������� ����', 'life_cycle', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-324', 324, 2, 'id_state_src', '�������� ���������', '�������� ���������', 'io_states', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-325', 325, 2, 'id_state_dest', '��������� ���������', '��������� ���������', 'io_states', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-326', 326, 9, 'service', '��������� (���������) �������', '��������� (���������) �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-327', 327, 9, 'extra_params', '���. ��������� ��� �������', '���. ��������� ��� �������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-328', 328, 2, 'id_handler', '������-����������', '������-����������', 'handlers', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-329', 329, 2, 'id_chain', '������� ���������', '������� ���������', 'chains', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-330', 330, 30, 'id_record', '������������� ������ �����������', '������������� ������ �����������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-331', 331, 8, 'is_handled', '������� ���������', '������� ���������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-332', 332, 5, 'handled_time', '������ ��������� ������ ���������', '������ ��������� ������ ���������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-333', 333, 5, 'start_service_time', '������ ��������� ������ ��������', '������ ��������� ������ ��������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-334', 334, 5, 'end_service_time', '����� ��������� ������ ��������', '����� ��������� ������ ��������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-335', 335, 8, 'return_code', '��� �������� �������', '��� �������� �������', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-336', 336, 1, 'is_external', '������� �������� �������', '������� �������� �������', NULL, NULL, 100, TRUE);



SELECT pg_catalog.setval('attributes_id_seq', 1000, true); --��� ���������������� �������� ����� ���������� � ������ 1001
                                                          --��� ������� ��� ����, ����� �������� ������ ��� ��������� ���������

/*
��������� ��� ��������, ������� ������ ��� ������ ���������, ��������� � �������������� ��������
��������� ������ ����� ��������� ������� �������, �� ��� ���������� ����� ������� ���� �������, 
������� �� �������� ���� ������ �������� - ������� ������������� ���� name ������� attrs_categories
����� �� ���� ������� ������, � ������ ���� � ����� ������ ��������� �������
*/

create or replace function acInsertCheck() returns trigger as 
$BODY$
declare
    aName varchar;
begin
    select name into aName from attributes where id = new.id_io_attribute;
    
    new.name := aName;

    return new;
end
$BODY$ 
language 'plpgsql';


select f_create_trigger('trgacinsert', 'before', 'insert or update', 'attrs_categories', 'acinsertcheck()');

                                    --���������� ��������� � �����������
--������� ��������� ��
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (1, 1, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (2, 1, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (3, 1, 3, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (581, 1, 15, 'false', true, false);

--������� ����� ��������� ��
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (4, 3, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (5, 3, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (6, 3, 3, NULL, false, false);--desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (7, 3, 187, NULL, true, false);--r_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (530, 3, 310, 'FALSE', true, false);--is_qualifier


--������� ������������� ����� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (8, 5, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (9, 5, 2, NULL, true, false);

--������� ����� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (10, 7, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (11, 7, 4, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (12, 7, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, 7, 6, NULL, true, false);

--������� ����� ��������� (����������������)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (14, 171, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (15, 171, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (16, 171, 27, NULL, false, false); --id_parent

--������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (17, 9, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (18, 9, 5, NULL, true, false); --id_a_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (19, 9, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (20, 9, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (21, 9, 7, NULL, true, false);  --title
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (22, 9, 8, NULL, false, false); --table_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (23, 9, 9, NULL, false, false); --column_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (24, 9, 57, '100', true, false);--def_width
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (25, 9, 15, 'false', true, false);--is_system
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (26, 9, 110, NULL, false, false);--id_search_template
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (27, 9, 284, 2, true, false);--id_attr_group
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (28, 9, 29, NULL, false, false);--ref_column_name

--������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (29, 11, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (30, 11, 10, NULL, true, false); --id_io_category_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (31, 11, 11, NULL, false, false);--id_child
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (531, 11, 311, NULL, false, false);--id_child2
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (32, 11, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (33, 11, 3, NULL, false, false); --description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (34, 11, 6, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (35, 11, 17, 'true', true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (36, 11, 18, '1', true, false);--id_io_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (582, 11, 323, NULL, false, false);--id_life_cycle
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (37, 11, 111, 'false', true, false);--is_global
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (532, 11, 40, 'false', true, true);--is_archived (������������� ������)


--������� �������������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (38, 13, 1, '-1', true, true);--id !!! -1 ��������� ����������� !!!
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (39, 13, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (40, 13, 3, NULL, false, false);-- desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (41, 13, 43, '1', true, false);--id_io_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (42, 13, 12, NULL, true, true); --id_io_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (43, 13, 13, NULL, false, true); --author
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (44, 13, 8, NULL, false, false); --table_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (45, 13, 14, NULL, false, true);--information
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (46, 13, 15, 'false', true, true);--is_system
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (47, 13, 16, 'current_timestamp', true, true);--insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (48, 13, 18, 1, true, false); --id_io_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (49, 13, 63, NULL, true, true); --id_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (50, 13, 97, NULL, false, false);--io_objects_organization (separate table)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (51, 13, 112, '1', true, false);--id_sync_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (52, 13, 117, NULL, true, true);--id_owner_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (53, 13, 111, 'false', true, false);--is_global
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (54, 13, 206, NULL, false, false);--record_fill_color
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (55, 13, 207, NULL, false, false);--record_text_color
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (56, 13, 110, NULL, false, false);--id_search_template
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (57, 13, 216, NULL, false, false);--ref_table_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (534, 13, 312, NULL, false, false);--r_icon




--������� �������� �������������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (58, 15, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (59, 15, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (60, 15, 3, NULL, false, false);  --description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (61, 15, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (62, 15, 12, NULL, true, false); --id_io_category

--������� ����� �������������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (63, 17, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (64, 17, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (65, 17, 3, NULL, false, false);  --description

--������� ����� ��������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (66, 19, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (67, 19, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (68, 19, 27, NULL, false, false); --id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (69, 19, 3, NULL, false, false);  --description

--������� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (70, 29, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (71, 29, 12, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (72, 29, 34, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (73, 29, 35, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (74, 29, 36, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (75, 29, 16, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (76, 29, 37, NULL, true, false);--exec_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (77, 29, 38, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (78, 29, 39, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (79, 29, 40, 'false', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (80, 29, 41, '1', true, false);



--������� ��������� ������� ������� � ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (81, 31, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (82, 31, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (83, 31, 3, NULL, false, false);

--������� ������� �������� � ��������� ������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (84, 33, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (85, 33, 12, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (86, 33, 99, NULL, true, true); --input_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (87, 33, 100, NULL, true, true); --output_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (88, 33, 34, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (89, 33, 35, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (90, 33, 42, NULL, true, false);--id_dl_from
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (91, 33, 16, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (92, 33, 37, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (93, 33, 38, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (94, 33, 40, 'false', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (95, 33, 41, '1', true, false);
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (96, 33, 43, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (97, 33, 44, 'true', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (98, 33, 45, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (99, 33, 165, '1', true, false);--id_urgency_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (100, 33, 167, NULL, false, false);--id_io_object_att
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (101, 33, 55, NULL, false, false);--receive_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (102, 33, 188, NULL, false, false);--accepted_datetime
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (103, 33, 46, NULL, false, true);

--������� ������� �������� ���������� ������� � �������� ������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (104, 37, 1, NULL, true, true);
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (105, 37, 46, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (106, 37, 47, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (107, 37, 48, NULL, false, false);--id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (108, 37, 16, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (109, 37, 49, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (110, 37, 50, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (111, 37, 40, 'false', true, false);

--������� ������� �������� � ��������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (112, 39, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 39, 51, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (114, 39, 99, NULL, true, true); --input_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (115, 39, 100, NULL, true, true); --output_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (116, 39, 52, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (117, 39, 48, NULL, false, false);
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (118, 39, 53, '1', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (119, 39, 54, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (120, 39, 55, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (121, 39, 56, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (122, 39, 45, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (123, 39, 16, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (124, 39, 165, '1', true, false); --id_urgency_level

--������� ����������� ����� ������������� ������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (125, 41, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (126, 41, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (127, 41, 59, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (128, 41, 60, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (129, 41, 61, NULL, true, false);

--������� ����������� ��������� ���������� ������������� ������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (130, 43, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (131, 43, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (132, 43, 30, '1', true, false);  --id_mimetype

--������� ������ �����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 45, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (134, 45, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 45, 64, NULL, true, false);


----------------------------
----------------------------
-----����� ���--------------

--������� �������� ������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (136, 47, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 47, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (138, 47, 6, NULL, false, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 47, 19, NULL, true, false); --short_name

--������� �������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (140, 49, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 49, 20, NULL, true, false);--id_rank
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (142, 49, 21, NULL, true, false);--role_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (143, 49, 65, NULL, true, false);--id_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (144, 49, 22, NULL, true, false);--lastname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (145, 49, 23, NULL, true, false);--firstname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (146, 49, 24, NULL, true, false);--sirname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (147, 49, 25, NULL, true, false);--fio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 49, 16, 'current_timestamp', true, true);--insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (149, 49, 62, 'true', true, false);--with_inheritance
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (150, 49, 66, NULL, false, false);--email
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 49, 63, NULL, false, false);--id_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 49, 67, NULL, false, false);--acc_right_num
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (153, 49, 68, NULL, false, false);--acc_right_date

--������� ����� ����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 173, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (155, 173, 2, NULL, true, false); --name

--������� ���������� (��� ������ ���)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 51, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (157, 51, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 51, 19, NULL, true, false);--short_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (159, 51, 31, NULL, true, false);--id_unit
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 51, 32, NULL, false, false);--id_user
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (161, 51, 70, NULL, false, false);--id_user_vrio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 51, 33, 'false', true, false);--is_io
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (163, 51, 63, NULL, true, false);--id_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 51, 62, 'true', true, false);--with_inheritance
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (165, 51, 71, NULL, false, false);--position_work_mode (separate table)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 51, 87, NULL, true, false);--email_prefix
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (167, 51, 119, NULL, false, false);--phone
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 51, 118, 'false', true, false);--is_public
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 51, 285, 1, true, false);--id_position_type


--������� �����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 53, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (171, 53, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (172, 53, 83, '1', true, false);--id_type_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (173, 53, 27, NULL, false, false);--id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (174, 53, 84, NULL, false, false);--id_parent1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (175, 53, 85, NULL, false, false);--id_substitutor
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (176, 53, 75, '1', true, false);--id_curr_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (177, 53, 86, '1', true, false);--id_prev_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (178, 53, 76, NULL, false, false);--id_boss
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 53, 19, NULL, true, false);--short_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (180, 53, 77, NULL, true, false);--code_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 53, 87, NULL, true, false);--email_prefix
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (182, 53, 88, NULL, true, false);--latitude
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (183, 53, 89, NULL, true, false);--longitude
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (184, 53, 90, NULL, true, false);--map_symbol (����� ��� � ��������� ������ ������ ��������� ��������)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (185, 53, 78, NULL, true, false);--tree_symbol (��. ���� org_symbol.sql)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (186, 53, 91, NULL, true, false);--dt_prev_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (187, 53, 92, NULL, true, false);--dt_curr_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (188, 53, 82, NULL, false, false);--organization_work_mode (separate table)

--������� �������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (189, 55, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (190, 55, 74, NULL, false, false);--id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (191, 55, 27, NULL, false, false);--id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (192, 55, 75, '1', true, false);--id_curr_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (193, 55, 76, NULL, false, false);--id_boss
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (194, 55, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (195, 55, 19, NULL, true, false);--short_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (196, 55, 77, NULL, true, false);--code_name
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (197, 55, 28, NULL, true, false);--address
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (198, 55, 78, NULL, false, false);--tree_symbol
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (199, 55, 79, 'false', true, false);--is_fgroup
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (200, 55, 96, NULL, false, false);--time_start
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (201, 55, 80, NULL, false, false);--time_elapsed
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (202, 55, 62, 'true', true, false);--with_inheritance
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (203, 55, 81, NULL, false, false);--units_work_mode

--������� ������� ����������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (204, 59, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (205, 59, 72, NULL, true, false); --id_mode_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (206, 59, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (207, 59, 3, NULL, false, false);--description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (208, 59, 73, NULL, false, false);--symbol
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (209, 59, 74, NULL, false, false);--audio_file

--������� ��������� �������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (210, 61, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (211, 61, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (212, 61, 19, NULL, true, false);--short_name

--������� ����� ������� ����������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (213, 63, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (214, 63, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (215, 63, 19, NULL, true, false);--short_name

--������� ����� �����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (216, 65, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (217, 65, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (218, 65, 19, NULL, true, false); --short_name

--������� �����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (219, 67, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (220, 67, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (221, 67, 95, NULL, true, false); --local_address
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (222, 67, 93, 'true', true, false); --is_active

--������� ������� �����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (223, 69, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (224, 69, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (225, 69, 94, NULL, true, false); --id_transport
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (226, 69, 28, NULL, true, false); --address
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (227, 69, 93, 'true', true, false); --is_active

--������� �������� �������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (228, 71, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (229, 71, 101, NULL, true, false); --report_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (230, 71, 102, NULL, false, false); --report_descrip
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (231, 71, 103, NULL, true, false); --report_grade
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (232, 71, 106, NULL, false, false);--report_organization (separate table)

--������� ��������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (233, 73, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (234, 73, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (235, 73, 53, '1', true, false); --id_search_template_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (236, 73, 13, NULL, true, false); --author
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (237, 73, 3, NULL, false, false); --desc

--������� ���������������� ������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (238, 75, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (239, 75, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (240, 75, 3, NULL, false, false);--desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (241, 75, 108, '1', true, false);--trig_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (242, 75, 109, NULL, true, false);--proc_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (243, 75, 107, NULL, false, false);--user_chains_organization (separate table)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (244, 75, 110, NULL, false, false);--id_search_template

--������������� ����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (245, 77, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (246, 77, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (247, 77, 3, NULL, false, false);--desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (248, 77, 6, NULL, false, false);--code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (249, 77, 27, NULL, false, false);--id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (250, 77, 48, NULL, false, true);--id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (251, 77, 110, NULL, false, false);--id_search_template
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (252, 77, 12, NULL, false, false);--id_io_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (533, 77, 312, NULL, false, false);--r_icon

--
--������������� �������������� ��������
--

--������� ����� ������������� ��
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (253, 79, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (254, 79, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (255, 79, 3, NULL, true, false); --description

--������� ������ ������������� �������������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (256, 81, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (257, 81, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (258, 81, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (259, 81, 116, NULL, true, false); --sync_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (260, 81, 113, NULL, true, false); --sync_result

--������� ���������� ������������� �������������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (261, 83, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (262, 83, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (263, 83, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (264, 83, 114, NULL, true, false); --min_period
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (265, 83, 115, NULL, true, false); --last_sync

--
--�������������� ����� � �������� ��������� (� �������, �� ���)
--

--������� ����� XML-��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (266, 85, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (267, 85, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (268, 85, 6, NULL, true, false); --code

--������� ����� XML-��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (269, 89, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (270, 89, 2, NULL, true, false); --name

--������� XML-����� (��������)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (271, 87, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (272, 87, 120, NULL, true, false); --id_xml_query_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (273, 87, 121, NULL, true, false); --xpath
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (274, 87, 122, NULL, true, false); --xcode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (275, 87, 123, NULL, false, false); --ref_table
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (276, 87, 128, false, true, false); --is_mandatory
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (277, 87, 130, NULL, true, false); --data_type

--������� XML-��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (278, 91, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (279, 91, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (280, 91, 120, NULL, true, false); --id_xml_query_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (281, 91, 124, NULL, true, false); --id_xml_query_kind
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (282, 91, 125, NULL, true, false); --xml_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (283, 91, 16, NULL, true, false); --insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (284, 91, 126, NULL, false, false); --exec_period_h
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (285, 91, 3, NULL, false, false); --description

--������� XML-������� �� �������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (286, 93, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (287, 93, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (288, 93, 127, NULL, true, false); --id_xml_query
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (289, 93, 125, NULL, true, false); --xml_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (290, 93, 129, NULL, true, false); --exec_time

---
---����������� �� ���
---

--������� ���������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (291, 95, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (292, 95, 2, NULL, true, false); --name

--������� ����� ���� ������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (293, 97, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (294, 97, 2, NULL, true, false); --name

--������� �������� ���� ������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (295, 99, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (296, 99, 2, NULL, true, false); --name

--������� ������ ������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (297, 101, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (298, 101, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (299, 101, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (300, 101, 209, NULL, false, false); --org_uid

--������� ������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (301, 103, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (302, 103, 2, NULL, true, false); --name

--������� ���������� �������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (303, 105, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (304, 105, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (305, 105, 131, NULL, true, false); --guard_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (306, 105, 138, NULL, false, false); --guard_objects_devices

--������� MIME-�����
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (307, 107, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (308, 107, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (309, 107, 46, NULL, true, false); --type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (310, 107, 69, NULL, true, false); --subtype

--������� ����������� ��������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (311, 109, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (109, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (109, 133, NULL, false, false); --fault_string
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (312, 109, 134, NULL, true, false); --reg_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (313, 109, 135, NULL, true, false); --fault_state_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (314, 109, 136, NULL, true, false); --fault_state_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (315, 109, 137, NULL, true, false); --id_device

--������� ������� �� ������� � ��������� �������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (316, 111, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (111, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (111, 133, NULL,false, false); --fault_string
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (317, 111, 137, NULL, false, false); --id_device
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (318, 111, 139, NULL, false, false); --id_person
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (319, 111, 140, NULL, true, false); --event_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (320, 111, 141, NULL, true, false); --message
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (321, 111, 142, NULL, true, false); --source
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (322, 111, 143, NULL, true, false); --alarm
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (323, 111, 144, NULL, false, false); --card_code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (324, 111, 145, NULL, false, false); --pass
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (325, 111, 146, NULL, false, false); --direction
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (326, 111, 147, NULL, false, false); --video
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (327, 111, 217, NULL, false, false); --video_url

--������� � ����������� � ��������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (328, 113, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 133, NULL, false, false); --fault_string
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (329, 113, 151, NULL, false, false); --id_tso_position
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (330, 113, 152, NULL, false, false); --id_tso_unit
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (331, 113, 22, NULL, true, false); --lastname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (332, 113, 23, NULL, false, false); --firstname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (333, 113, 24, NULL, false, false); --sirname

--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 144, NULL, false, false); --card_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 148, NULL, false, false); --id_card_type
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 149, NULL, false, false); --id_card_status
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 150, NULL, false, false); --id_access_plan
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 153, NULL, false, false); --comment

--������� ����. ������ �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (334, 115, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (335, 115, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (336, 115, 27, NULL, false, false); --id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (337, 115, 154, NULL, false, false); --bg_image
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (338, 115, 155, NULL, false, false); --svg_image
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (115, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (115, 133, NULL, false, false); --fault_string

--������� ��������� (���������) ����. ������ �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (339, 117, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (340, 117, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (341, 117, 156, NULL, true, false); --id_graph_plan
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (342, 117, 155, NULL, false, false); --svg_image
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (117, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (117, 133, NULL, false, false); --fault_string

--������� ����� ����� ����. ��������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (343, 119, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (344, 119, 2, NULL, true, false); --name

--������� ����� �������� ��� ��������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (345, 121, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (346, 121, 2, NULL, true, false); --name

--������� ������ ����������� ����������� ���������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (347, 123, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (348, 123, 2, NULL, true, false); --name

--������� ����� ��� ����������� ��������� �� ����. ������ �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (349, 125, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (125, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (125, 133, NULL, false, false); --fault_string
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (350, 125, 157, NULL, true, false); --id_shape_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (351, 125, 137, NULL, true, false); --id_device
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (352, 125, 159, NULL, false, false); --id_graph_style

--������� ���������� ��������� �������� ����������� ��������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (353, 127, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (354, 127, 160, NULL, true, false); --id_segment_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (355, 127, 161, NULL, true, false); --id_element_shape
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (356, 127, 162, NULL, true, false); --points
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (357, 127, 163, '0', true, false); --p_order
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (127, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (127, 133, NULL, false, false); --fault_string

--������� ������� ��������� ��������� (������������)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (358, 129, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (359, 129, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (360, 129, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (361, 129, 164, NULL, true, false); --urgency_symbol

--������� ������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (362, 131, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (363, 131, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (364, 131, 3, NULL, false, false); --desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (365, 131, 166, NULL, true, false); --mail_lists_position


--�� ��� (�����������)

--�������� ��� ��������� ������������ ������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(366, 134, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(367, 134, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(368, 136, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(369, 136, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(370, 138, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(371, 138, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(372, 140, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(373, 140, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(374, 142, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(375, 142, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(376, 102, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(377, 102, 220, NULL, FALSE, FALSE);


--������������ ���������� �������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (378, 133, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (379, 133, 168, NULL, false, false); --id_person_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (380, 133, 140, NULL, false, false); --event_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (381, 133, 141, NULL, false, false); --message
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (382, 133, 142, NULL, false, false); --source
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (383, 133, 143, NULL, false, false); --alarm
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (384, 133, 144, NULL, false, false); --card_code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (385, 133, 145, NULL, false, false); --pass
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (386, 133, 146, NULL, false, false); --direction
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (387, 133, 22, NULL, false, false); --lastname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (388, 133, 23, NULL, false, false); --firstname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (389, 133, 24, NULL, false, false); --sirname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (390, 133, 169, NULL, false, false); --id_device_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (391, 133, 174, NULL, false, false); --dev_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (392, 133, 170, NULL, false, false); --id_unit_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (393, 133, 171, NULL, false, false); --unit_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (394, 133, 172, NULL, false, false); --id_dolzh_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (395, 133, 173, NULL, false, false); --dolzh_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (396, 133, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (397, 133, 209, NULL, false, false); --org_uid
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (398, 133, 217, NULL, false, false); --video_url
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (399, 133, 218, NULL, false, false); --for_report
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (400, 133, 221, NULL, false, false); --is_deleted


--������������ ���������� ����������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (401, 135, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (402, 135, 169, NULL, false, false); --id_device_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (403, 135, 174, NULL, false, false); --dev_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (404, 135, 134, NULL, false, false); --reg_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (405, 135, 135, NULL, false, false); --fault_state_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (406, 135, 136, NULL, false, false); --fault_state_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (407, 135, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (408, 135, 209, NULL, false, false); --org_uid
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (409, 135, 221, NULL, false, false); --is_deleted

--������������ ���������� ��������� � ���������� �������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (410, 137, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (411, 137, 169, NULL, false, false); --id_device_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (412, 137, 174, NULL, false, false); --dev_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (413, 137, 175, NULL, false, false); --id_guard_obj_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (414, 137, 176, NULL, false, false); --guard_obj_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (415, 137, 177, NULL, false, false); --device_svg
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (416, 137, 178, NULL, false, false); --plan_svg
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (417, 137, 179, NULL, false, false); --is_ok
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (418, 137, 180, NULL, false, false); --ok_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (419, 137, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (420, 137, 209, NULL, false, false); --org_uid

--������������ ���������� ��������� �������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (421, 139, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (422, 139, 22, NULL, false, false); --lastname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (423, 139, 23, NULL, false, false); --firstname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (424, 139, 24, NULL, false, false); --sirname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (425, 139, 181, NULL, false, false); --access_cards
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (426, 139, 172, NULL, false, false); --id_dolzh_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (427, 139, 173, NULL, false, false); --dolzh_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (428, 139, 170, NULL, false, false); --id_unit_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (429, 139, 171, NULL, false, false); --unit_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (430, 139, 153, NULL, false, false); --comment
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (431, 139, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (432, 139, 209, NULL, false, false); --org_uid

--������������ ���������� ���� ������� � �������� �� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (433, 141, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (434, 141, 6, NULL, true, true); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (435, 141, 153, NULL, false, false); --comment
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (436, 141, 182, NULL, false, false); --id_card_type_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (437, 141, 183, NULL, false, false); --card_type_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (438, 141, 184, NULL, false, false); --id_card_status_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (439, 141, 185, NULL, false, false); --card_status_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (440, 141, 186, NULL, false, false); --access_plans
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (441, 141, 3, NULL, false, false); --desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (442, 141, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (443, 141, 209, NULL, false, false); --org_uid

--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (444, 151, 210, NULL, true, false); --max_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (445, 151, 211, NULL, true, false); --tso_dl_from
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (446, 151, 212, NULL, true, false); --tso_dl_exec
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (447, 151, 213, NULL, true, false); --tso_dl_to
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (448, 151, 214, NULL, true, false); --tso_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (449, 151, 215, NULL, true, false); --tso_exectime





--������� �������� ���������
--���������� ����� ����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (450, 143, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (451, 143, 2, NULL, true, false); --name

--���������� �������� ����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (452, 145, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (453, 145, 2, NULL, true, false); --name

--���������� ����� ����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (454, 149, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (455, 149, 2, NULL, true, false); --name

--������� �������� (��� �� �������� ������������)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (456, 148, 191, NULL, true, false); --id_exec_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (457, 148, 35, NULL, true, false); --id_dl_executor
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (458, 148, 42, NULL, true, false); --id_dl_from
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (459, 148, 205, NULL, true, false); --create_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (460, 148, 100, NULL, true, false); --output_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (461, 148, 193, NULL, true, false); --regout_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (462, 148, 99, NULL, true, false); --input_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (463, 148, 194, NULL, true, false); --regin_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (464, 148, 189, NULL, true, false); --id_document_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (465, 148, 190, NULL, true, false); --id_document_subtype
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (466, 148, 192, NULL, true, false); --id_document_kind
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (467, 148, 195, NULL, true, false); --doc_header
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (468, 148, 196, NULL, true, false); --page_count
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (469, 148, 197, NULL, true, false); --ekz_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (470, 148, 198, NULL, true, false); --book_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (471, 148, 199, NULL, true, false); --book_subnumber
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (472, 148, 200, NULL, true, false); --class_code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (473, 148, 201, NULL, true, false); --inv_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (474, 148, 202, NULL, true, false); --registrator_info
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (475, 148, 3, NULL, true, false); --desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (476, 148, 203, NULL, true, false); --annotation
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (477, 148, 204, NULL, true, false); --command_body


/*
--
--ZARYA22
--
--�������� ������
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 2, NULL, true, false); --name
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 6, NULL, true, false); --code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 19, NULL, true, false); --short_name

--���������� �������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 6, NULL, true, false); --code

-- ������ chk sum
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 6, NULL, true, false); --code

-- ��� �������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 224, NULL, true, false); --uri

-- ���� �������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 6, NULL, true, false); --code

-- ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 224, NULL, true, false); --uri

-- dls
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 224, NULL, true, false); --uri
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 225, NULL, true, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 226, NULL, true, false); --id_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 227, NULL, true, false); --id_pos

-- addressee
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 224, NULL, true, false); --uri
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 225, NULL, true, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 222, NULL, true, false); --type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 223, NULL, true, false); --value

--������� �������� ��������� (����22)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 225, NULL, false, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 231, NULL, false, false); --shu_working_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 226, NULL, false, false); --id_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 228, NULL, false, false); --id_dls
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 20, NULL, false, false); --id_rank
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 25, NULL, false, false); --fio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 232, NULL, false, false); --shu_msg_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 233, NULL, false, false); --shu_creation_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 234, NULL, false, false); --shu_creation_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 235, NULL, false, false); --shu_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 236, NULL, false, false); --shu_priority
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 230, NULL, false, false); --id_shu_domain
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 2, NULL, false, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 237, NULL, false, false); --shu_text

--������� �������� ������� (����22)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 225, NULL, false, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 231, NULL, false, false); --shu_working_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 226, NULL, false, false); --id_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 228, NULL, false, false); --id_dls
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 20, NULL, false, false); --id_rank
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 25, NULL, false, false); --fio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 232, NULL, false, false); --shu_msg_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 233, NULL, false, false); --shu_creation_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 234, NULL, false, false); --shu_creation_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 235, NULL, false, false); --shu_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 236, NULL, false, false); --shu_priority
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 230, NULL, false, false); --id_shu_domain
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 2, NULL, false, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 238, NULL, false, false); --shu_cmd_code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 239, NULL, false, false); --shu_cmd_text

--
--��������� � ������������ �������� (����22)
--
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 225, NULL, false, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 231, NULL, false, false); --shu_working_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 226, NULL, false, false); --id_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 228, NULL, false, false); --id_dls
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 20, NULL, false, false); --id_rank
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 25, NULL, false, false);  --fio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 233, NULL, false, false); --shu_creation_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 234, NULL, false, false); --shu_creation_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 235, NULL, false, false); --shu_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 236, NULL, false, false); --shu_priority
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 230, NULL, false, false); --id_shu_domain
--���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 258, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 259, NULL, false, false);
--������������ ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 240, NULL, false, false);--shu_emerg_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 241, NULL, false, false);--shu_emerg_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 244, NULL, false, false);--shu_district
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 245, NULL, false, false);--shu_region
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 246, NULL, false, false);--shu_area
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 260, NULL, false, false);--shu_town
--�������� � ������������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 261, NULL, false, false);--shu_document_entry
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 242, NULL, false, false);--shu_obj_open_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 243, NULL, false, false);--shu_obj_close_name
--��������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 262, NULL, false, false);--shu_block
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 263, NULL, false, false);--shu_country
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 264, NULL, false, false);--shu_department
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 265, NULL, false, false);--shu_company
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 266, NULL, false, false);--shu_block
--������������ ����� ��
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 257, NULL, false, false);--shu_emerg_pict
--������������ �����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 267, NULL, false, false);--shu_authenticity
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 268, NULL, false, false);--shu_instrumentation_event_time
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 269, NULL, false, false);--shu_authenticity
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 270, NULL, false, false);--shu_cdomain1
--������� �������� ��
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 249, NULL, false, false);--shu_emerg_desc
--
--������������ �����������
--����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 271, NULL, false, false);--shu_height_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 247, NULL, false, false);--shu_emerg_lat
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 248, NULL, false, false);--shu_emerg_lon
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 272, NULL, false, false);--shu_emerg_height
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 273, NULL, false, false);--shu_coord_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 276, NULL, false, false);--shu_cdomain2
--��������� ���������� �����
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 274, NULL, false, false);--shu_life_cycle_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 275, NULL, false, false);--shu_life_cycle_indicators
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 276, NULL, false, false);--shu_life_cycle_type
--������� ���
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 251, NULL, false, false);--shu_emerg_dang_area
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 277, NULL, false, false);--shu_emerg_dang_area_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 253, NULL, false, false);--shu_emerg_dang_area1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 278, NULL, false, false);--shu_emerg_dang_area1_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 283, NULL, false, false);--shu_emerg_safe_area
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 279, NULL, false, false);--shu_emerg_safe_area_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 250, NULL, false, false);--shu_emerg_rad_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 280, NULL, false, false);--shu_emerg_rad_level_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 252, NULL, false, false);--shu_emerg_dang_area_rad_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 281, NULL, false, false);--shu_emerg_dang_area_rad_level_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 254, NULL, false, false);--shu_emerg_dang_area_rad_level1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 282, NULL, false, false);--shu_emerg_dang_area_rad_level1_uom
--����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 256, NULL, false, false);--shu_emerg_evac_act
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 255, NULL, false, false);--shu_emerg_lic_act
--end of emerg
*/


--������� ���������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (478, 175, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (479, 175, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (480, 175, 286, NULL, true, false); --the_value

--������� ����� �����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (481, 177, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (482, 177, 2, NULL, true, false); --name

--������� �����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (483, 179, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (484, 179, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (485, 179, 3, NULL, false, false); --desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (486, 179, 291, NULL, true, false); --id_indicator_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (487, 179, 27, NULL, false, false); --id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (488, 179, 5, NULL, true, false); --id_a_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (489, 179, 8, NULL, false, false); --table_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (490, 179, 9, NULL, false, false); --column_name

--������� �������� �����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (491, 181, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (492, 181, 286, NULL, true, false); --the_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (493, 181, 292, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (494, 181, 293, NULL, false, false); --id_io_object_src
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (495, 181, 294, NULL, false, false); --id_io_object_src1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (496, 181, 295, NULL, true, false); --meas_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (497, 181, 16, NULL, true, false); --insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (498, 181, 297, NULL, true, false); --start_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (499, 181, 298, NULL, false, false); --stop_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (500, 181, 299, NULL, true, false); --id_indicator

--������� �������� ��������� � �������������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (501, 183, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (502, 183, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (503, 183, 300, NULL, true, false); --id_attr_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (504, 183, 301, NULL, true, false); --value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (505, 183, 297, 'current_timestamp', true, false); --start_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (506, 183, 298, NULL, false, false); --stop_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (507, 183, 295, 'current_timestamp', true, false); --meas_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (508, 183, 16, 'current_timestamp', true, false); --insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (509, 183, 293, NULL, false, false); --id_io_object_src
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (510, 183, 294, NULL, false, false); --id_io_object_src1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (511, 183, 302, 'true', true, true); --is_actual

--������� ��������� � ����������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (512, 185, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (513, 185, 12, NULL, true, false); --id_io_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (514, 185, 303, NULL, true, false); --id_io_attribute
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (515, 185, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (516, 185, 304, NULL, false, false); --def_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (517, 185, 128, 'false', true, false); --is_mandatory
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (518, 185, 305, 'false', true, false); --is_read_only

--������� �������� ��������� � ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (526, 187, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (527, 187, 309, NULL, true, false); --id_attr_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (528, 187, 308, NULL, true, false); --id_attr_attr
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (529, 187, 301, NULL, true, false); --value

--������� ��������� � ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (519, 189, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (520, 189, 306, NULL, true, false); --id_attr_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (521, 189, 307, NULL, true, false); --id_attr_child
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (522, 189, 2, NULL, false, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (523, 189, 304, NULL, false, false); --def_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (524, 189, 128, 'false', true, false); --is_mandatory
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (525, 189, 305, 'false', true, false); --is_read_only
--530 ����� ��� �������� is_qualifier � ����������� ����� ���������
--531 ����� ��� �������� id_child2 � �����������  ���������
--532 ����� ��� �������� is_archived � ����������� ���������
--533 ����� ��� �������� r_icon � ����������� �����������
--534 ����� ��� �������� r_icon � ����������� �������������� ��������

--������� ������� �������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (535, 191, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (536, 191, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (537, 191, 3, NULL, false, false);  --description

--������� ������� ���������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (538, 193, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (539, 193, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (540, 193, 315, NULL, true, false); --id_partition_low
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (541, 193, 52, NULL, true, false); --id_dl_receiver
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (542, 193, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (543, 193, 316, '0', false, false); --lambda
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (544, 193, 317, '0', false, false); --sigma
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (545, 193, 318, '0', false, false); --moda
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (546, 193, 319, '0', false, false); --min_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (547, 193, 320, '0', false, false); --max_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (548, 193, 313, NULL, true, false); --start_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (549, 193, 314, NULL, true, false); --end_time

--������� ��������� ������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (550, 195, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (551, 195, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (552, 195, 321, NULL, false, false);  --id_start_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (553, 195, 322, NULL, false, false);  --life_cycle_io_states
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (554, 195, 3, NULL, false, false);  --description

--������� ��������� ��������� � ��������� ������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (555, 197, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (556, 197, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (557, 197, 323, NULL, true, false);  --id_life_cycle
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (558, 197, 324, NULL, true, false);  --id_state_src
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (559, 197, 325, NULL, true, false);  --id_state_dest


--������� ��������-������������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (560, 199, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (561, 199, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (562, 199, 3, NULL, false, false);  --description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (563, 199, 326, NULL, true, false);  --service
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (564, 199, 327, NULL, false, false);  --extra_params
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (583, 199, 336, 'FALSE', true, false);  --is_external


--������� �������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (565, 201, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (566, 201, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (567, 201, 3, NULL, false, false);  --description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (568, 201, 328, NULL, true, false);  --id_handler
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (569, 201, 18, NULL, true, false);  --id_io_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (570, 201, 12, NULL, true, false);  --id_io_category


--������� ��������
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (571, 203, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (572, 203, 329, NULL, true, false); --id_chain
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (573, 203, 330, NULL, false, false);  --id_record
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (574, 203, 48, NULL, false, false);  --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (575, 203, 16, NULL, true, true);  --insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (576, 203, 331, '0', true, false);  --is_handled
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (577, 203, 332, NULL, false, false);  --handled_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (578, 203, 333, NULL, false, false);  --start_service_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (579, 203, 334, NULL, false, false);  --end_service_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (580, 203, 335, NULL, false, false);  --return_code
--581 ������������ � ����������� ��������� ��
--582 ������������ � ����������� ��������� ��
--583 ������������ � ����������� ��������

SELECT pg_catalog.setval('attrs_categories_id_seq', 2000, true); --��� ���������������� �������� � ���������� ����� ���������� � ������ 2001
                                                                 --��� ������� ��� ����, ����� �������� ������ ��� ��������� ���������

select f_safe_drop_trigger('trgacinsert', 'attrs_categories');


create temp table notlogging_oper_table (id int4 not null);
select setAsNotLogging(1);


select f_safe_drop_trigger('trg_acl_ioctrl', 'tbl_io_objects');

--��������� �������������� �������
--� ��� ����� ������ �� ��� ����� ��������� ���� OTHERS_ROLE ��� (TRUE, TRUE, FALSE, FALSE)
insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-1', 1, 2, 1, 1, '���������� ��������� �������������� ��������', 'io_states', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (1, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-2', 2, 4, 1, 1, '���������� ����� ��������� �������������� ��������', 'io_category_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (2, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-3', 3, 6, 1, 1, '���������� ������������� ����� ���������', 'a_views', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (3, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-4', 4, 8, 1, 1, '���������� ����� ���������', 'a_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (4, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-5', 5, 10, 1, 1, '���������� ���������', 'attributes', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (5, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-6', 6, 12, 1, 1, '���������� ���������', 'io_categories', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (6, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-7', 7, 14, 1, 1, '���������� �������������� ��������', 'io_objects', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (7, -10, TRUE, TRUE, FALSE, TRUE); --������ ������������ ������ ����� ����� �������������� ������� ����������� �������������� ��������. �.�. ���������, ������� � �������� �������������� �������

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-8', 8, 16, 1, 1, '���������� �������� �������������� ��������', 'io_templates', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (8, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-9', 9, 18, 1, 1, '���������� ����� �������������� ��������', 'io_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (9, -10, TRUE, TRUE, FALSE, TRUE); --������ ������������ ������ ����� ����� �������������� ������� ����������� �������������� ��������. �.�. ���������, ������� � �������� �������������� �������

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-10', 10, 20, 1, 1, '���������� ����� ��������� ��������', 'search_template_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (10, -10, TRUE, TRUE, FALSE, FALSE);

--old ���
 
insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-15', 15, 30, 1, 1, '������ ������� ���������', 'tsd', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (15, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-16', 16, 32, 1, 1, '���������� ��������� ������� ������� � ��������', 'jr_states', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (16, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-17', 17, 34, 1, 1, '������ �������� � ��������� ������������', 'command_journal', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (17, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-19', 19, 38, 1, 1, '������ �������� ���������� ������� � �������� ������������', 'tsd_control_journal', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (19, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-20', 20, 40, 1, 1, '������ ���������', 'message_journal', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (20, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-21', 21, 42, 1, 1, '���������� ����� ������������� ������', 'url_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (21, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-22', 22, 44, 1, 1, '���������� ��������� ���������� ������������� ������', 'url_extentions', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (22, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-23', 23, 46, 1, 1, '���������� ������ �����������', 'maclabels', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (23, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-24', 24, 48, 1, 1, '���������� �������� ������', 'ranks', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (24, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-25', 25, 50, 1, 1, '���������� �������������', 'users', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (25, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-26', 26, 52, 1, 1, '���������� ����������', 'position', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (26, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-27', 27, 54, 1, 1, '���������� �����������', 'organization', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (27, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-28', 28, 56, 1, 1, '���������� �������������', 'units', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (28, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-29', 29, 60, 1, 1, '���������� ������� ����������������', 'work_mode', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (29, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-30', 30, 62, 1, 1, '���������� ��������� �������������', 'user_state', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (30, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-31', 31, 64, 1, 1, '���������� ����� ������� ����������������', 'work_mode_type', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (31, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-32', 32, 66, 1, 1, '���������� ����� �����������', 'organization_type', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (32, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-33', 33, 68, 1, 1, '���������� ����������� �������������� ������', 'transport', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (33, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-34', 34, 70, 1, 1, '���������� ������� �����������', 'organization_transport', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (34, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-35', 35, 72, 1, 1, '���������� �������� �������', 'report', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (35, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-36', 36, 74, 1, 1, '���������� ��������� �������� (��������)', 'search_templates', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (36, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-37', 37, 76, 1, 1, '���������� ���������������� ��������', 'user_chains', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (37, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-38', 38, 78, 1, 1, '������������� ����������', 'rubricator', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (38, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-39', 39, 80, 1, 1, '���� ������������� �������������� ��������', 'io_sync_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (39, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-40', 40, 82, 1, 1, '����� (�������) ������������� �������������� ��������', 'io_sync_archive', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (40, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-41', 41, 84, 1, 1, '��������� ������������� �������������� ��������', 'io_last_sync', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (41, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-42', 42, 86, 1, 1, '���������� ����� XML-�������� ��� ������ ����������� � �������� ���������', 'xml_query_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (42, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-43', 43, 88, 1, 1, '���������� XML-����� (��������) ������� �� ������� ��� ������ ����������� � �������� ���������', 'xml_paths', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (43, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-44', 44, 90, 1, 1, '���������� ����� XML-�������� ��� ������ ����������� � �������� ���������', 'xml_query_kinds', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (44, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-45', 45, 92, 1, 1, '���������� XML-�������� ��� ������ ����������� � �������� ���������', 'xml_queries', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (45, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-46', 46, 94, 1, 1, '���������� ������� �� XML-������� ��� ������ ����������� � �������� ���������', 'xml_query_responses', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (46, -10, TRUE, TRUE, FALSE, FALSE);

---
---�� ���
---
insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-47', 47, 96, 1, 1, '���������� ���������� �� ���', 'tso_positions', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (47, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-48', 48, 98, 1, 1, '���������� ����� ���� ������� �� ���', 'card_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (48, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-49', 49, 100, 1, 1, '���������� �������� ���� ������� �� ���', 'card_statuses', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (49, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-50', 50, 102, 1, 1, '���������� ������ ������� �� ���', 'access_plan', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (50, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-51', 51, 104, 1, 1, '���������� ������� �� ���', 'tso_units', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (51, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-52', 52, 106, 1, 1, '���������� ���������� �������� �� ���', 'guard_objects', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (52, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-53', 53, 108, 1, 1, '���������� MIME-�����', 'mimetypes', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (53, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-54', 54, 110, 1, 1, '���������� ����������� ��������� �� ���', 'fault_devices', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (54, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-55', 55, 112, 1, 1, '���������� ������� �� ������� � ��������� �������� �� ���', 'alarm_events', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (55, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-56', 56, 114, 1, 1, '���������� ��������� �� ���', 'persons', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (56, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-57', 57, 116, 1, 1, '���������� ����. ������ �� ���', 'graph_plans', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (57, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-58', 58, 118, 1, 1, '���������� ��������� (���������) ����. ������ �� ���', 'devices', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (58, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-59', 59, 120, 1, 1, '���������� ����� ����� ��������� ����. ������ �� ���', 'shape_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (59, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-60', 60, 122, 1, 1, '���������� ����� ��������� ��������� ����. ������ �� ���', 'segment_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (60, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-61', 61, 124, 1, 1, '���������� ������ ����������� ���������� �� ���', 'graph_styles', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (61, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-62', 62, 126, 1, 1, '���������� ����� ��������� ����. ������ �� ���', 'element_shapes', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (62, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-63', 63, 128, 1, 1, '���������� ���������� ��������� �������� ����������� ��������� �� ���', 'shape_segments', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (63, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-64', 64, 130, 1, 1, '���������� ������� ��������� ��������� (������������)', 'urgency_levels', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (64, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-65', 65, 132, 1, 1, '���������� ������� �������� ��������� � ������������', 'mail_lists', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (65, -10, TRUE, TRUE, FALSE, FALSE);

--�� ���
insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-66', 66, 134, 1, 1, '������������ ���������� ������� �� ���', 'alarm_events_q', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (66, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-67', 67, 138, 1, 1, '������������ ���������� ��������� � ���������� �������� �� ���', 'dev_object_q', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (67, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-68', 68, 136, 1, 1, '������������ ���������� ����������� ��������� �� ���', 'fault_devices_q', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (68, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-69', 69, 140, 1, 1, '������������ ���������� ��������� �������� �� ���', 'personal_q', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (69, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-70', 70, 142, 1, 1, '������������ ���������� ���� ������� � �������� �� ���', 'access_cards_q', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (70, -10, TRUE, TRUE, FALSE, FALSE);


insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-71', 71, 144, 1, 1, '���������� ����� ����������', 'document_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (71, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-72', 72, 146, 1, 1, '���������� �������� ����������', 'document_subtypes', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (72, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-73', 73, 150, 1, 1, '���������� ����� ����������', 'document_kinds', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (73, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-74', 74, 151, 1, 1, '��������� ������� ���', NULL, NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (74, -10, TRUE, TRUE, FALSE, FALSE);

/*
--insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
--values ('localorg-io_objects-75', 75, 153, 1, 1, '������ �������� ������ (����22)', NULL, NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
--insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
--values (75, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-76', 76, 155, 1, 1, '������ ���������� �������� (����22)', 'shu_domains', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (76, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-77', 77, 157, 1, 1, '������ ������� ������� ����������� ���� (����22)', 'shu_chksum', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (77, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-78', 78, 159, 1, 1, '������ ��������������� ������ (����22)', 'shu_orgs', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (78, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-79', 79, 161, 1, 1, '������ ����������� ������ (����22)', 'shu_positions', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (79, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-80', 80, 163, 1, 1, '������ ��� (����22)', 'shu_acs', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (80, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-81', 81, 165, 1, 1, '������ ����������� ��� (����22)', 'shu_dls', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (81, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-82', 82, 167, 1, 1, '������ ������� (����22)', 'shu_addressee', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (82, -10, TRUE, TRUE, FALSE, FALSE);
*/

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-83', 83, 172, 1, 1, '���������� ����� ���������', 'attrs_groups', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (83, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-84', 84, 174, 1, 1, '���������� ����� ����������', 'position_types', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (84, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-85', 85, 176, 1, 1, '���������� ���������� ���������', 'operations', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (85, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-86', 86, 178, 1, 1, '���������� ����� �����������', 'indicator_type', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (86, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-87', 87, 180, 1, 1, '���������� �����������', 'indicator', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (87, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-88', 88, 182, 1, 1, '���������� �������� �����������', 'indicators_values', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (88, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-89', 89, 184, 1, 1, '���������� �������� ��������� � �������������� ��������', 'attrs_values', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (89, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-90', 90, 186, 1, 1, '���������� ��������� � ����������', 'attrs_categories', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (90, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-91', 91, 188, 1, 1, '���������� �������� ��������� � ���������', 'attrs_attrs_values', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (91, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-92', 92, 190, 1, 1, '���������� ��������� � ���������', 'attrs_attrs', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (92, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-93', 93, 192, 1, 1, '���������� ������� �������������', 'partition_lows', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (93, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-94', 94, 194, 1, 1, '���������� ������� ���������', 'message_streams', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (94, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-95', 95, 196, 1, 1, '���������� ��������� ������', 'life_cycle', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (95, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-96', 96, 198, 1, 1, '���������� ��������� ��������� � ��������� ������', 'state_crosses', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (96, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-97', 97, 200, 1, 1, '���������� ��������-������������ ��������', 'handlers', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (97, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-98', 98, 202, 1, 1, '���������� �������� �������� ���������', 'chains', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (98, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-99', 99, 204, 1, 1, '���������� �������� ��������� (������ ��� ���������)', 'chains_data', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (99, -10, TRUE, TRUE, FALSE, FALSE);


SELECT pg_catalog.setval('tbl_io_objects_id_seq', 300, true); --��� ���������������� �������������� ������� ����� ���������� � ������ 301
                                                          --��� ������� ��� ����, ����� �������� ������ ��� ��������� ��


select f_create_trigger('trg_acl_ioctrl', 'before', 'insert or update or delete', 'io_objects', 'acl_ioctrl()');

--�������� ������� ��� ����������� �������������� ��������
insert into io_templates (id_io_category, name, code) values(13, '�������� ������ ��� ����������� �������������� ��������', 'SYSTEMPLATE_1');

insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, '��������', 0);
insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, '�� ������', 1);
insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, '��������� � ����������', 2);
--insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, '�������������', 2);
insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, '������������', 3);
insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, '������� ����������', 4);


insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(49, 1, 1, true, 0, NULL);--id_maclabel
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(39, 1, 1, false, 1, NULL);--name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(40, 1, 1, false, 2, NULL);--description
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(41, 1, 1, false, 3, '1');--id_io_type
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(48, 1, 1, false, 4, '1');--id_io_state

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(45, 1, 2, false, 1, NULL); --information
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(47, 1, 2, true, 2, 'current_timestamp'); --insert_time
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(50, 1, 2, true, 3, NULL);--id_object_organization
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(52, 1, 2, true, 4, NULL);--owner_org

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(44, 1, 3, false, 0, NULL);--table_name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(42, 1, 3, true, 1, NULL);--id_io_category

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(54, 1, 4, true, 0, NULL);--colors
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(55, 1, 4, true, 1, NULL);
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(534, 1, 4, false, 2, NULL);

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(56, 1, 5, true, 0, NULL);--id_search_template
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(57, 1, 5, true, 1, NULL);--ref_table_name

--�������� ������������ ������� ��� ����������� ���������
insert into io_templates (id_io_category, name, code) values(9, '����������� ������ ��� ����������� ���������', 'SYSTEMPLATE_2');
insert into a_groups (id_io_template, id_parent, name, "order") values(2, NULL, '�������� ��������', 0); --6
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(21, 2, 6, false, 0, NULL);
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(18, 2, 6, false, 1, NULL);

--�������� ������������ ������� ��� ����������� �����������
insert into io_templates (id_io_category, name, code) values(53, '����������� ������ ��� ����������� �����������', 'SYSTEMPLATE_3');--3
insert into a_groups (id_io_template, id_parent, name, "order") values(3, NULL, '�������� �����������', 0); --7
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(170, 3, 7, false, 0, NULL); --id
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(171, 3, 7, false, 1, NULL); --name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(179, 3, 7, false, 2, NULL);--short_name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(180, 3, 7, false, 3, NULL);--code_name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(181, 3, 7, false, 4, NULL);--email_prefix

insert into a_groups (id_io_template, id_parent, name, "order") values(3, NULL, '������ ����������������', 1); --8
insert into a_groups (id_io_template, id_parent, name, "order") values(3, 7, '������� �����', 0); --9
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(176, 3, 9, false, 0, NULL);--id_curr_mode
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(187, 3, 9, false, 1, NULL);--dt_curr_mode

insert into a_groups (id_io_template, id_parent, name, "order") values(3, 7, '���������� �����', 1); --10
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(177, 3, 10, false, 0, NULL);--id_prev_mode
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(186, 3, 10, false, 1, NULL);--dt_prev_mode

insert into a_groups (id_io_template, id_parent, name, "order") values(3, NULL, '�������������', 2); --11
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(178, 3, 11, false, 0, NULL);--id_boss
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(173, 3, 11, false, 1, NULL);--id_parent
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(174, 3, 11, false, 2, NULL);--id_parent1
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(175, 3, 11, false, 3, NULL);--id_substitutor

insert into a_groups (id_io_template, id_parent, name, "order") values(3, NULL, '���, ���������� � �������� �����', 3); --12
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(172, 3, 12, false, 0, NULL);--id_type_org
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(182, 3, 12, false, 1, NULL);--lat
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(183, 3, 12, false, 2, NULL);--lon
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(185, 3, 12, false, 3, NULL);--tree_symbol
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(184, 3, 12, false, 4, NULL);--map_symbol
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(188, 3, 12, false, 5, NULL);--work_modes (separate table)

--�������� ������������ ������� ��� ����������� ������� ���������
insert into io_templates (id_io_category, name, code) values(193, '����������� ������ ��� ����������� ������� ���������', 'SYSTEMPLATE_4');
insert into a_groups (id_io_template, id_parent, name, "order") values(4, NULL, '�������� ��������', 0); --13
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(538, 4, 13, false, 0, NULL); --id
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(539, 4, 13, false, 1, NULL); --name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(541, 4, 13, false, 2, NULL); --id_dl_receiver
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(542, 4, 13, false, 3, NULL); --id_io_object

insert into a_groups (id_io_template, id_parent, name, "order") values(4, NULL, '��������� ������ �������������', 1); --14
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(540, 4, 14, false, 0, NULL); --id_partition_low
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(543, 4, 14, false, 1, '0'); --lambda
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(544, 4, 14, false, 2, '0'); --sigma
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(545, 4, 14, false, 3, '0'); --moda
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(546, 4, 14, false, 4, '0'); --min_p
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(547, 4, 14, false, 5, '0'); --max_p

insert into a_groups (id_io_template, id_parent, name, "order") values(4, NULL, '�������� ��������� ������', 2); --15
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(548, 4, 15, false, 0, NULL);--start_time
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(549, 4, 15, false, 1, NULL);--end_time

SELECT pg_catalog.setval('io_templates_id_seq', 50, true); --��� ���������������� ������� ����� ���������� � ������ 51
                                                           --��� ������� ��� ����, ����� �������� ������ ��� ��������� 
                                                           --������� ��� ����22 ��������� � ����. �������, ������� ���������� ��������� ��������� ��
                                                           --������� �� ��� ��� ������ ����� �� ������    

SELECT pg_catalog.setval('a_groups_id_seq', 100, true);    --������� ��� ����22 ��������� � ����. �������, ������� ���������� ��������� ��������� ��
                                                           --������� �� ��� ��� ������ ����� �� ������    

SELECT pg_catalog.setval('io_views_id_seq', 300, true);    --������� ��� ����22 ��������� � ����. �������, ������� ���������� ��������� ��������� ��
                                                           --������� �� ��� ��� ������ ����� �� ������    

update attributes set id_ref_attr_type = 9 where column_name is not null;

select setAsLogging(1);
