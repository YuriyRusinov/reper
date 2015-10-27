INSERT INTO attrs_groups (unique_id, id, id_parent, name) VALUES ('localorg_prefix-attrs_groups-4', 4, NULL, 'Radio Image Attributes');

select setval ('attrs_groups_id_seq', 4, true);


INSERT INTO io_categories (unique_id, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-301', 301, 10, NULL, NULL, 1, NULL, false, 'Table of Ship types', 'CATEGORY_301', NULL, false, 1, false);
INSERT INTO io_categories (unique_id, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-302', 302, 1, 301, NULL, 1, NULL, true, 'Ship types', 'CATEGORY_302', NULL, false, 1, false);

select setval ('io_categories_id_seq', 302, true);

INSERT INTO attrs_categories (id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES (301, 1, 'Идентификатор', NULL, false, false, 1);
INSERT INTO attrs_categories (id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES (301, 2, 'Название', '', true, false, 2);
INSERT INTO attrs_categories (id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES (301, 3, 'Описание', NULL, false, false, 3);
INSERT INTO attrs_categories (id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES (301, 6, 'Код', '', true, false, 4);

select createtemptables();
select setcurrentdl(4);
select csetglobal (301);
select csetglobal (302);
select setval ('attrs_categories_id_seq', 2127, true);

INSERT INTO tbl_io_objects (unique_id, id, id_io_category, id_io_state, id_io_type, id_maclabel, author, id_sync_type, id_owner_org, name, table_name, description, information, is_system, is_completed, is_global, record_fill_color, record_text_color, id_search_template, ref_table_name, r_icon, uuid_t) VALUES ('localorg_prefix-io_objects-302', 302, 302, 1, 1, 1, 1, 1, 1, 'Ship types', 'type_ship', NULL, 'Администратор системы', false, 2, true, NULL, NULL, NULL, NULL, NULL, 'a98240ba-73c0-11e4-942f-938951a6ce63');

select setval ('tbl_io_objects_id_seq', 302, true);

INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-eio_table_302-1', '2014-11-24 13:00:16.640965', 1, 1, 'b0e1245c-73c0-11e4-bb01-f78297e7bcdc', 'Авианосец', NULL, NULL, NULL, 'Авианосец', NULL, 'CV');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-eio_table_302-2', '2014-11-24 13:01:28.528401', 2, 1, 'dbba388a-73c0-11e4-a0cb-ff6bc5f0ead2', 'Крейсер УРО', NULL, NULL, NULL, 'Крейсер УРО', NULL, 'CG');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-eio_table_302-3', '2014-11-24 13:02:31.002597', 3, 1, '00f70704-73c1-11e4-9295-d7482d8eb33a', 'Эскадренный миноносец УРО', NULL, NULL, NULL, 'Эскадренный миноносец УРО', NULL, 'DDG');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-4', '2014-11-24 13:03:07.954798', 4, 1, '16fd7c22-73c1-11e4-99da-5f3222d5a3c6', 'Эскадренный миноносец', NULL, NULL, NULL, 'Эскадренный миноносец', NULL, 'DD');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-5', '2014-11-24 13:03:39.51884', 5, 1, '29cdc6ae-73c1-11e4-82b7-8f417b783fee', 'Фрегат УРО', NULL, NULL, NULL, 'Фрегат УРО', NULL, 'FFG');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-6', '2014-11-24 13:18:28.484577', 6, 1, '3bab2ed2-73c3-11e4-8b3a-4b7bdc6a5363', 'Фрегат', NULL, NULL, NULL, 'Фрегат', NULL, 'FFL');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-7', '2014-11-24 13:19:45.764075', 7, 1, '69bb1940-73c3-11e4-be82-67f34dc9d824', 'Корвет', NULL, NULL, NULL, 'Корвет', NULL, 'FS');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-8', '2014-11-24 13:21:23.417355', 8, 1, 'a3efce58-73c3-11e4-b805-83dce9dbd022', 'Патрульный корабль', NULL, NULL, NULL, 'Патрульный корабль', NULL, 'PF');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-9', '2014-11-24 13:21:36.94618', 9, 1, 'ac00241c-73c3-11e4-9a5e-6346eff52ab7', 'Ракетный катер', NULL, NULL, NULL, 'Ракетный катер', NULL, 'PHM');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-10', '2014-11-24 13:22:13.220508', 10, 1, 'c19f2a3e-73c3-11e4-9eb1-efc0bb82ec59', 'Торпедный катер', NULL, NULL, NULL, 'Торпедный катер', NULL, 'PT');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-11', '2014-11-24 13:24:37.510214', 11, 1, '17a006ba-73c4-11e4-9e73-070865c4b954', 'Патрульный катер', NULL, NULL, NULL, 'Патрульный катер', NULL, 'PTF');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-12', '2014-11-24 13:24:56.373297', 12, 1, '22de4e24-73c4-11e4-9bf4-7310b91e7313', 'Десантный корабль штабной', NULL, NULL, NULL, 'Десантный корабль штабной', NULL, 'LCC');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-13', '2014-11-24 13:25:14.862269', 13, 1, '2de37f88-73c4-11e4-952a-9b56685f7f0b', 'Десантный корабль универсальный', NULL, NULL, NULL, 'Десантный корабль универсальный', NULL, 'LPA');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-14', '2014-11-24 13:25:31.551129', 14, 1, '37d6040c-73c4-11e4-bd0c-4bc828567afc', 'Десантный корабль вертолетоносец', NULL, NULL, NULL, 'Десантный корабль вертолетоносец', NULL, 'LPH');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-15', '2014-11-24 13:27:20.605217', 15, 1, '78d65952-73c4-11e4-993b-2be2fdfe1c12', 'Десантный корабль танкодесантный', NULL, NULL, NULL, 'Десантный корабль танкодесантный', NULL, 'LST');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-16', '2014-11-24 13:27:34.925985', 16, 1, '815f863e-73c4-11e4-8135-63d54b7c3de0', 'Десантный катер', NULL, NULL, NULL, 'Десантный катер', NULL, 'LCA');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-17', '2014-11-24 13:27:50.957948', 17, 1, '8aedcec2-73c4-11e4-8270-13ed0bd21b1a', 'Транспортное судно универсальное', NULL, NULL, NULL, 'Транспортное судно универсальное', NULL, 'AKS');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-18', '2014-11-24 13:28:03.615531', 18, 1, '92793384-73c4-11e4-b61e-170bfb68be12', 'Транспортное судно снабжения', NULL, NULL, NULL, 'Транспортное судно снабжения', NULL, 'AFS');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-19', '2014-11-24 13:28:40.042464', 19, 1, 'a82f82b4-73c4-11e4-9620-4b3eb820f185', 'Транспортное судно боеприпасы', NULL, NULL, NULL, 'Транспортное судно боеприпасы', NULL, 'AE');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-20', '2014-11-24 13:28:57.068625', 20, 1, 'b255818a-73c4-11e4-a59b-73940c590299', 'Танкер-заправщик', NULL, NULL, NULL, 'Танкер-заправщик', NULL, 'AO');
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code) VALUES ('localorg_prefix-type_ship-21', '2014-11-24 13:29:10.827327', 21, 1, 'ba88e874-73c4-11e4-aafe-ab9239a90e79', 'Минно-тральный корабль', NULL, NULL, NULL, 'Минно-тральный корабль', NULL, 'AM');

select setval ('q_base_table_id_seq', 21, true);

INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1001', '2014-11-24 13:31:37.557668', 1001, 2, NULL, 4, 'id_type_ship', 'Ship type', 'Ship type', 'type_ship', 'name', 'id', 100, true, 9);
INSERT INTO attributes (unique_id, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1002', 1002, 6, NULL, 4, 'azimuth', 'Azimuth', 'Azimuth', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1003', 1003, 6, NULL, 4, 'elevation_angle', 'Elevation', 'Elevation', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1004', '2014-11-24 13:35:28.814191', 1004, 38, NULL, 4, 'image_raw', 'image_raw', 'image_raw', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1005', '2014-11-24 13:35:47.571567', 1005, 15, NULL, 4, 'image_jpg', 'image_jpg', 'image_jpg', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1006', '2014-12-04 12:41:54.885399', 1006, 8, NULL, 4, 'depth', 'depth', 'depth', NULL, NULL, NULL, 100, true, NULL);
select setval ('attributes_id_seq', 1006, true);

INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-303', '2014-11-24 13:32:44.581934', 303, 10, NULL, NULL, 1, NULL, false, 'Table of RLI Image', 'CATEGORY_303', NULL, false, false, 1, false);
INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-304', '2014-11-24 13:32:44.610661', 304, 1, 303, NULL, 1, NULL, true, 'RLI Image', 'CATEGORY_304', NULL, false, false, 1, false);

select setval ('io_categories_id_seq', 304, true);
select csetglobal (303);
select csetglobal (304);


INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2129', '2014-11-24 13:36:10.593138', 2129, 303, 1, 'Идентификатор', NULL, false, false, 1);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2130', '2014-11-24 13:36:10.593138', 2130, 303, 1002, 'Azimuth', '0', true, false, 2);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2131', '2014-12-04 12:42:12.164466', 2131, 303, 1003, 'elevation_angle', '0', true, false, 3);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2132', '2014-11-24 13:36:10.593138', 2132, 303, 1004, 'image_raw', NULL, false, false, 4);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2133', '2014-11-24 13:36:10.593138', 2133, 303, 1005, 'image_jpg', NULL, false, false, 5);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2134', '2014-12-04 12:42:12.164466', 2134, 303, 1006, 'depth', NULL, false, false, 6);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2135', '2014-11-24 13:32:44.581934', 2135, 303, 1001, 'Ship type', NULL, true, false, 7);


select setval ('attrs_categories_id_seq', 2135, true);

INSERT INTO tbl_io_objects (unique_id, last_update, id, id_io_category, id_io_state, id_io_type, id_maclabel, author, id_sync_type, id_owner_org, name, table_name, description, information, is_system, insert_time, is_completed, is_global, record_fill_color, record_text_color, id_search_template, ref_table_name, r_icon, uuid_t) VALUES ('localorg_prefix-io_objects-303', '2014-12-04 12:50:05.647943', 303, 304, 1, 2, 1, 1, 1, 1, 'RLI Image raws', 'rli_image_raws', NULL, 'Администратор системы', false, '2014-12-04 04:12:20', 2, true, NULL, NULL, NULL, NULL, NULL, 'ecc82916-7b9a-11e4-af6b-9f445027ca5b');
select setval ('tbl_io_objects_id_seq', 303, true);


