
select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);


select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
select f_safe_drop_trigger('trg_acl_ioctrl', 'tbl_io_objects');



	--
--ZARYA22
--
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-152', 152, 10, NULL, false, '�������� ������ (����22)', 'SYSCATEGORY_152', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(152, -10, true, true, false, false, true);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-153', 153, 8, 142, true, '������ �������� ������ (����22)', 'SYSCATEGORY_153', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(153, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-154', 154, 10, NULL, false, '���������� ������� (����22)', 'SYSCATEGORY_154', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-155', 155, 8, 154, true, '������ ���������� �������� (����22)', 'SYSCATEGORY_155', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-156', 156, 10, NULL, false, '�������������� ������� ������� ����������� ���� (����22)', 'SYSCATEGORY_156', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-157', 157, 8, 156, true, '������ ������������� ������� ������� ����������� ���� (����22)', 'SYSCATEGORY_157', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-158', 158, 10, NULL, false, '��������������� ������� (����22)', 'SYSCATEGORY_158', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-159', 159, 8, 158, true, '������ ��������������� ������ (����22)', 'SYSCATEGORY_159', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-160', 160, 10, NULL, false, '����������� ������� (����22)', 'SYSCATEGORY_160', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-161', 161, 8, 160, true, '������ ����������� ������ (����22)', 'SYSCATEGORY_161', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-162', 162, 10, NULL, false, '��� (����22)', 'SYSCATEGORY_162', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-163', 163, 8, 162, true, '������ ��� (����22)', 'SYSCATEGORY_163', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-164', 164, 10, NULL, false, '����������� ���� (����22)', 'SYSCATEGORY_164', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-165', 165, 8, 164, true, '������ ������������ ��� (����22)', 'SYSCATEGORY_165', NULL::varchar, true, true, 1);

--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-166', 166, 10, NULL, false, '������ (����22)', 'SYSCATEGORY_166', NULL::varchar, true, true, 1);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-167', 167, 8, 166, true, '������ ������� (����22)', 'SYSCATEGORY_167', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-168', 168, 3, NULL, true, '������� �������� ��������� (����22)', 'SYSCATEGORY_168', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-169', 169, 3, NULL, true, '������� �������� ������� (����22)', 'SYSCATEGORY_169', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-170', 170, 3, NULL, true, '��������� � ������������ �������� (����22)', 'SYSCATEGORY_170', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-166', 166, 11, NULL, true, '��������� ���������� � �������� 7�1 (����22)', 'SYSCATEGORY_166', NULL::varchar, true, true, 1);





--
--ZARYA22
--
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-222', 222, 9, 'type', '���', '���', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-223', 223, 9, 'value', '��������', '��������', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-224', 224, 9, 'uri', 'URI', 'URI', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system, id_ref_attr_type) values('localorg-attributes-225', 225, 2, 'id_acs', '���', '���', 'shu_acs', 'name', 150, TRUE, 9);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system, id_ref_attr_type) values('localorg-attributes-226', 226, 2, 'id_org', '��������������� �������', '��������������� �������', 'shu_orgs', 'name', 150, TRUE, 9);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system, id_ref_attr_type) values('localorg-attributes-227', 227, 2, 'id_pos', '����������� �������', '����������� �������', 'shu_positions', 'name', 150, TRUE, 9);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-228', 228, 2, 'id_shu_dls', '����������� ���� (����22)', '����������� ���� (����22)', 'shu_dls', 'name', 150, TRUE);
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
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-266', 266, 17, 'shu_dls_position', '��������������� �� � ������� DynamicDocs', '��������������� �� � ������� DynamicDocs', 'position', 'name', 200, TRUE);

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


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-269', 269, 9, 'zarya_dl_from', 'UID ������������ ����, ����������� ������������', 'UID ������������ ����, ����������� ������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-287', 287, 9, 'zarya_dl_exec', 'UID ������������ ����, ����������� ������������', 'UID ������������ ����, ����������� ������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-288', 288, 9, 'zarya_dl_to', 'UID ������������ ����, ����������� ������������', 'UID ������������ ����, ����������� ������������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-289', 289, 9, 'zarya_category', 'UID ��������� ������� �� �������', 'UID ��������� ������� �� �������', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-290', 290, 21, 'zarya_exectime', '����� �������', '����� �������', NULL, NULL, 120, TRUE);



--
--ZARYA22
--
--�������� ������
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 1, NULL, true, true); --id
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 2, NULL, true, false); --name
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 6, NULL, true, false); --code
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 19, NULL, true, false); --short_name

--���������� �������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 6, NULL, true, false); --code

-- ������ chk sum
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 6, NULL, true, false); --code

-- ��� �������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 6, NULL, true, false); --code
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 224, NULL, true, false); --uri

-- ���� �������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 6, NULL, true, false); --code

-- ���
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 6, NULL, true, false); --code
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 224, NULL, true, false); --uri

-- dls
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 224, NULL, true, false); --uri
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 225, NULL, true, false); --id_acs
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 226, NULL, true, false); --id_org
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 227, NULL, true, false); --id_pos
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 266, NULL, false, false); --shu_dls_position

-- addressee
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 1, NULL, true, true); --id
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 2, NULL, true, false); --name
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 6, NULL, true, false); --code
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 224, NULL, true, false); --uri
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 225, NULL, true, false); --id_acs
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 222, NULL, true, false); --type
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 223, NULL, true, false); --value

--������� �������� ��������� (����22)
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 225, NULL, false, false); --id_acs
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 231, NULL, false, false); --shu_working_mode
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 226, NULL, false, false); --id_org
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 228, NULL, false, false); --id_dls
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 20, NULL, false, false); --id_rank
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 25, NULL, false, false); --fio
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 232, NULL, false, false); --shu_msg_type
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 233, NULL, false, false); --shu_creation_date
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 234, NULL, false, false); --shu_creation_time
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 235, NULL, false, false); --shu_maclabel
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 236, NULL, false, false); --shu_priority
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 230, NULL, false, false); --id_shu_domain
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 2, NULL, false, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 237, NULL, false, false); --shu_text

--������� �������� ������� (����22)
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 225, NULL, false, false); --id_acs
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 231, NULL, false, false); --shu_working_mode
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 226, NULL, false, false); --id_org
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 228, NULL, false, false); --id_dls
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 20, NULL, false, false); --id_rank
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 25, NULL, false, false); --fio
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 232, NULL, false, false); --shu_msg_type
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 233, NULL, false, false); --shu_creation_date
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 234, NULL, false, false); --shu_creation_time
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 235, NULL, false, false); --shu_maclabel
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 236, NULL, false, false); --shu_priority
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 230, NULL, false, false); --id_shu_domain
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 2, NULL, false, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 238, NULL, false, false); --shu_cmd_code
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 239, NULL, false, false); --shu_cmd_text

--
--��������� � ������������ �������� (����22)
--
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 225, NULL, false, false); --id_acs
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 231, NULL, false, false); --shu_working_mode
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 226, NULL, false, false); --id_org
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 228, NULL, false, false); --id_dls
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 20, NULL, false, false); --id_rank
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 25, NULL, false, false);  --fio
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 233, NULL, false, false); --shu_creation_date
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 234, NULL, false, false); --shu_creation_time
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 235, NULL, false, false); --shu_maclabel
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 236, NULL, false, false); --shu_priority
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 230, NULL, false, false); --id_shu_domain
--���������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 258, NULL, false, false);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 259, NULL, false, false);
--������������ ��������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 240, NULL, false, false);--shu_emerg_type
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 241, NULL, false, false);--shu_emerg_datetime
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 244, NULL, false, false);--shu_district
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 245, NULL, false, false);--shu_region
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 246, NULL, false, false);--shu_area
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 260, NULL, false, false);--shu_town
--�������� � ������������� ���������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 261, NULL, false, false);--shu_document_entry
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 242, NULL, false, false);--shu_obj_open_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 243, NULL, false, false);--shu_obj_close_name
--��������������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 262, NULL, false, false);--shu_block
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 263, NULL, false, false);--shu_country
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 264, NULL, false, false);--shu_department
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 265, NULL, false, false);--shu_company
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 266, NULL, false, false);--shu_block
--������������ ����� ��
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 257, NULL, false, false);--shu_emerg_pict
--������������ �����������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 267, NULL, false, false);--shu_authenticity
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 268, NULL, false, false);--shu_instrumentation_event_time
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 269, NULL, false, false);--shu_authenticity
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 270, NULL, false, false);--shu_cdomain1
--������� �������� ��
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 249, NULL, false, false);--shu_emerg_desc
--
--������������ �����������
--����������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 271, NULL, false, false);--shu_height_uom
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 247, NULL, false, false);--shu_emerg_lat
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 248, NULL, false, false);--shu_emerg_lon
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 272, NULL, false, false);--shu_emerg_height
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 273, NULL, false, false);--shu_coord_uom
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 276, NULL, false, false);--shu_cdomain2
--��������� ���������� �����
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 274, NULL, false, false);--shu_life_cycle_type
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 275, NULL, false, false);--shu_life_cycle_indicators
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 276, NULL, false, false);--shu_life_cycle_type
--������� ���
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 251, NULL, false, false);--shu_emerg_dang_area
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 277, NULL, false, false);--shu_emerg_dang_area_uom
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 253, NULL, false, false);--shu_emerg_dang_area1
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 278, NULL, false, false);--shu_emerg_dang_area1_uom
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 283, NULL, false, false);--shu_emerg_safe_area
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 279, NULL, false, false);--shu_emerg_safe_area_uom
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 250, NULL, false, false);--shu_emerg_rad_level
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 280, NULL, false, false);--shu_emerg_rad_level_uom
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 252, NULL, false, false);--shu_emerg_dang_area_rad_level
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 281, NULL, false, false);--shu_emerg_dang_area_rad_level_uom
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 254, NULL, false, false);--shu_emerg_dang_area_rad_level1
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 282, NULL, false, false);--shu_emerg_dang_area_rad_level1_uom
--����������
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 256, NULL, false, false);--shu_emerg_evac_act
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 255, NULL, false, false);--shu_emerg_lic_act
--end of emerg


insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 269, NULL, true, false); --zarya_dl_from
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 287, NULL, true, false); --zarya_dl_exec
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 288, NULL, true, false); --zarya_dl_to
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 289, NULL, true, false); --zarya_category
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 290, NULL, true, false); --zarya_exectime



--insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
--values ('localorg-io_objects-75', 75, 153, 1, 1, '������ �������� ������ (����22)', NULL, NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL);
--insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
--values (75, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-76', 76, 155, 1, 1, '������ ���������� �������� (����22)', 'shu_domains', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (76, -10, TRUE, TRUE, FALSE, FALSE);

select acl_secureTable('shu_domains');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-77', 77, 157, 1, 1, '������ ������� ������� ����������� ���� (����22)', 'shu_chksum', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (77, -10, TRUE, TRUE, FALSE, FALSE);

select acl_secureTable('shu_chksum');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-78', 78, 159, 1, 1, '������ ��������������� ������ (����22)', 'shu_orgs', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (78, -10, TRUE, TRUE, FALSE, FALSE);

select acl_secureTable('shu_orgs');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-79', 79, 161, 1, 1, '������ ����������� ������ (����22)', 'shu_positions', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (79, -10, TRUE, TRUE, FALSE, FALSE);

select acl_secureTable('shu_positions');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-80', 80, 163, 1, 1, '������ ��� (����22)', 'shu_acs', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (80, -10, TRUE, TRUE, FALSE, FALSE);

select acl_secureTable('shu_acs');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-81', 81, 165, 1, 1, '������ ����������� ��� (����22)', 'shu_dls', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (81, -10, TRUE, TRUE, FALSE, FALSE);

select acl_secureTable('shu_dls');

--insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
--values ('localorg-io_objects-82', 82, 167, 1, 1, '������ ������� (����22)', 'shu_addressee', NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL);
--insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
--values (82, -10, TRUE, TRUE, FALSE, FALSE);


insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-82', 82, 166, 1, 1, '��������� ���������� � �������� 7�1 (����22)', NULL, NULL, '��������� ������', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (82, -10, TRUE, TRUE, FALSE, FALSE);

revoke all on shu_dls_position from public; 
grant select, references on shu_dls_position to public;


create or replace function bbb(int4, int4) returns int4 as
$BODY$
declare
    idCat alias for $1;
    idAttr alias for $2;

    idCatAttr int4;
begin

    select id into idCatAttr from attrs_categories where id_io_category = idCat and id_io_attribute = idAttr;
    return idCatAttr;

end
$BODY$
language 'plpgsql';


create or replace function aaa() returns int4 as
$BODY$
declare
    idTemplate int4;
    idGroup int4;
begin
    select getNextSeq('io_templates', 'id') into idTemplate;
    select getNextSeq('a_groups', 'id') into idGroup;

    --�������� ������������ ������� ��� �������� ��������� ��������� (����22)
    insert into io_templates (id, id_io_category, name, code) values(idTemplate, 168, '����������� ������ ��� �������� ��������� ��������� (����22)', 'SYSTEMPLATE_' || idTemplate);
    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup, idTemplate, NULL, '��������� ���������', 0);

    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+1, idTemplate, idGroup, '���������� �� ��� �����������', 1);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 225), idTemplate, idGroup+1, false, 0, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 231), idTemplate, idGroup+1, false, 1, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 226), idTemplate, idGroup+1, false, 2, NULL);

    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+2, idTemplate, idGroup, '���������� �� �����������', 2);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 228), idTemplate, idGroup+2, false, 0, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 20), idTemplate, idGroup+2, false, 1, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 25), idTemplate, idGroup+2, false, 2, NULL);

    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+3, idTemplate, idGroup, '�������������� ���������', 3);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 232), idTemplate, idGroup+3, false, 0, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 233), idTemplate, idGroup+3, false, 1, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 234), idTemplate, idGroup+3, false, 2, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 235), idTemplate, idGroup+3, false, 3, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 236), idTemplate, idGroup+3, false, 4, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 230), idTemplate, idGroup+3, false, 5, NULL);

    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+4, idTemplate, NULL, '���������', 4);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 2), idTemplate, idGroup+4, false, 0, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(168, 237), idTemplate, idGroup+4, false, 1, NULL);

    --�������� ������������ ������� ��� ������� �������� ������� (����22)
    insert into io_templates (id, id_io_category, name, code) values(idTemplate+1, 169, '����������� ������ ��� ������� �������� ������� (����22)', 'SYSTEMPLATE_' || idTemplate+1);
    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+5, idTemplate+1, NULL, '��������� ���������', 0);
   
    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+6, idTemplate+1, idGroup+5, '���������� �� ��� �����������', 1);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 225), idTemplate+1, idGroup+6, false, 0, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 231), idTemplate+1, idGroup+6, false, 1, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 226), idTemplate+1, idGroup+6, false, 2, NULL);

    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+7, idTemplate+1, idGroup+5, '���������� �� �����������', 2);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 228), idTemplate+1, idGroup+7, false, 0, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 20), idTemplate+1, idGroup+7, false, 1, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 25), idTemplate+1, idGroup+7, false, 2, NULL);

    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+8, idTemplate+1, idGroup+5, '�������������� �������', 3);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 232), idTemplate+1, idGroup+8, false, 0, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 233), idTemplate+1, idGroup+8, false, 1, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 234), idTemplate+1, idGroup+8, false, 2, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 235), idTemplate+1, idGroup+8, false, 3, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 236), idTemplate+1, idGroup+8, false, 4, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 230), idTemplate+1, idGroup+8, false, 5, NULL);

    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+9, idTemplate+1, NULL, '�������', 4);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 2), idTemplate+1, idGroup+9, false, 0, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 238), idTemplate+1, idGroup+9, false, 1, NULL);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(bbb(169, 239), idTemplate+1, idGroup+9, false, 2, NULL);

    return 1;
end
$BODY$
language 'plpgsql';

select aaa();

drop function aaa();
drop function bbb(int4, int4);

select f_create_trigger('trg_acl_ioctrl', 'before', 'insert or update or delete', 'io_objects', 'acl_ioctrl()');
select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');


select setAsLogging(1);

