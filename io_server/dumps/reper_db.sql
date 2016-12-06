INSERT INTO attrs_groups (unique_id, id, id_parent, name) VALUES ('localorg_prefix-attrs_groups-4', 4, NULL, 'Radio Image Attributes');

select setval ('attrs_groups_id_seq', 4, true);

INSERT INTO attributes (unique_id, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1001', 1001, 6, NULL, 4, 'length_min', 'length_min', 'Минимальная длина', NULL, NULL, NULL, 100, true, NULL);

INSERT INTO attributes (unique_id, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1002', 1002, 6, NULL, 4, 'length_max', 'length_max', 'Максимальная длина', NULL, NULL, NULL, 100, true, NULL);

INSERT INTO io_categories (unique_id, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-301', 301, 10, NULL, NULL, 1, NULL, false, 'Table of Ship types', 'CATEGORY_301', NULL, false, 1, false);
INSERT INTO io_categories (unique_id, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-302', 302, 1, 301, NULL, 1, NULL, true, 'Ship types', 'CATEGORY_302', NULL, false, 1, false);

select setval ('io_categories_id_seq', 302, true);

INSERT INTO attrs_categories (unique_id, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2120', 2120, 301, 1, 'Идентификатор', NULL, false, false, 1);
INSERT INTO attrs_categories (unique_id, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2121', 2121, 301, 2, 'Название', '', true, false, 2);
INSERT INTO attrs_categories (unique_id, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2122', 2122, 301, 3, 'Описание', NULL, false, false, 3);
INSERT INTO attrs_categories (unique_id, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2123', 2123, 301, 6, 'Код', '', true, false, 4);
INSERT INTO attrs_categories (unique_id, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2124', 2124, 301, 1001, 'Минимальная длина', '0', true, false, 5);
INSERT INTO attrs_categories (unique_id, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2125', 2125, 301, 1002, 'Максимальная длина', '0', true, false, 6);

select createtemptables();
select setcurrentdl(4);
select csetglobal (301);
select csetglobal (302);
select setval ('attrs_categories_id_seq', 2125, true);

INSERT INTO tbl_io_objects (unique_id, id, id_io_category, id_io_state, id_io_type, id_maclabel, author, id_sync_type, id_owner_org, name, table_name, description, information, is_system, is_completed, is_global, record_fill_color, record_text_color, id_search_template, ref_table_name, r_icon, uuid_t) VALUES ('localorg_prefix-io_objects-302', 302, 302, 1, 1, 1, 1, 1, 1, 'Типы надводных кораблей', 'type_ship', NULL, 'Администратор системы', false, 2, true, NULL, NULL, NULL, NULL, NULL, 'a98240ba-73c0-11e4-942f-938951a6ce63');

select setval ('tbl_io_objects_id_seq', 302, true);

INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-eio_table_302-1', '2014-11-24 13:00:16.640965', 1, 1, 'b0e1245c-73c0-11e4-bb01-f78297e7bcdc', 'Авианосец', NULL, NULL, NULL, 'Авианосец', NULL, 'CV', 182, 350);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-eio_table_302-2', '2014-11-24 13:01:28.528401', 2, 1, 'dbba388a-73c0-11e4-a0cb-ff6bc5f0ead2', 'Крейсер УРО', NULL, NULL, NULL, 'Крейсер УРО', NULL, 'CG', 172.5, 180);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-eio_table_302-3', '2014-11-24 13:02:31.002597', 3, 1, '00f70704-73c1-11e4-9295-d7482d8eb33a', 'Эскадренный миноносец УРО', NULL, NULL, NULL, 'Эскадренный миноносец УРО', NULL, 'DDG', 129, 172);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-4', '2014-11-24 13:03:07.954798', 4, 1, '16fd7c22-73c1-11e4-99da-5f3222d5a3c6', 'Эскадренный миноносец', NULL, NULL, NULL, 'Эскадренный миноносец', NULL, 'DD', 115, 172);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-5', '2014-11-24 13:03:39.51884', 5, 1, '29cdc6ae-73c1-11e4-82b7-8f417b783fee', 'Фрегат УРО', NULL, NULL, NULL, 'Фрегат УРО', NULL, 'FFG', 84, 147);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-6', '2014-11-24 13:18:28.484577', 6, 1, '3bab2ed2-73c3-11e4-8b3a-4b7bdc6a5363', 'Фрегат', NULL, NULL, NULL, 'Фрегат', NULL, 'FFL', 80, 148);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-7', '2014-11-24 13:19:45.764075', 7, 1, '69bb1940-73c3-11e4-be82-67f34dc9d824', 'Корвет', NULL, NULL, NULL, 'Корвет', NULL, 'FS', 54, 89);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-8', '2014-11-24 13:21:23.417355', 8, 1, 'a3efce58-73c3-11e4-b805-83dce9dbd022', 'Патрульный корабль', NULL, NULL, NULL, 'Патрульный корабль', NULL, 'PF', 54, 116);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-9', '2014-11-24 13:21:36.94618', 9, 1, 'ac00241c-73c3-11e4-9a5e-6346eff52ab7', 'Ракетный катер', NULL, NULL, NULL, 'Ракетный катер', NULL, 'PHM', 21, 68);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-10', '2014-11-24 13:22:13.220508', 10, 1, 'c19f2a3e-73c3-11e4-9eb1-efc0bb82ec59', 'Торпедный катер', NULL, NULL, NULL, 'Торпедный катер', NULL, 'PT', 21, 43);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-11', '2014-11-24 13:24:37.510214', 11, 1, '17a006ba-73c4-11e4-9e73-070865c4b954', 'Патрульный катер', NULL, NULL, NULL, 'Патрульный катер', NULL, 'PTF', 26, 59);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-12', '2014-11-24 13:24:56.373297', 12, 1, '22de4e24-73c4-11e4-9bf4-7310b91e7313', 'Десантный корабль штабной', NULL, NULL, NULL, 'Десантный корабль штабной', NULL, 'LCC', 158, 194);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-13', '2014-11-24 13:25:14.862269', 13, 1, '2de37f88-73c4-11e4-952a-9b56685f7f0b', 'Десантный корабль универсальный', NULL, NULL, NULL, 'Десантный корабль универсальный', NULL, 'LPA', 236, 260);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-14', '2014-11-24 13:25:31.551129', 14, 1, '37d6040c-73c4-11e4-bd0c-4bc828567afc', 'Десантный корабль вертолетоносец', NULL, NULL, NULL, 'Десантный корабль вертолетоносец', NULL, 'LPH', 182, 204);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-15', '2014-11-24 13:27:20.605217', 15, 1, '78d65952-73c4-11e4-993b-2be2fdfe1c12', 'Десантный корабль танкодесантный', NULL, NULL, NULL, 'Десантный корабль танкодесантный', NULL, 'LST', 50, 160);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-16', '2014-11-24 13:27:34.925985', 16, 1, '815f863e-73c4-11e4-8135-63d54b7c3de0', 'Десантный катер', NULL, NULL, NULL, 'Десантный катер', NULL, 'LCA', 13, 42);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-17', '2014-11-24 13:27:50.957948', 17, 1, '8aedcec2-73c4-11e4-8270-13ed0bd21b1a', 'Транспортное судно универсальное', NULL, NULL, NULL, 'Транспортное судно универсальное', NULL, 'AKS', 103, 242);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-18', '2014-11-24 13:28:03.615531', 18, 1, '92793384-73c4-11e4-b61e-170bfb68be12', 'Транспортное судно снабжения', NULL, NULL, NULL, 'Транспортное судно снабжения', NULL, 'AFS', 160, 184);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-19', '2014-11-24 13:28:40.042464', 19, 1, 'a82f82b4-73c4-11e4-9620-4b3eb820f185', 'Транспортное судно боеприпасы', NULL, NULL, NULL, 'Транспортное судно боеприпасы', NULL, 'AE', 105, 172);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-20', '2014-11-24 13:28:57.068625', 20, 1, 'b255818a-73c4-11e4-a59b-73940c590299', 'Танкер-заправщик', NULL, NULL, NULL, 'Танкер-заправщик', NULL, 'AO', 140, 207);
INSERT INTO tbl_type_ship (unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, name, description, code, length_min, length_max) VALUES ('localorg_prefix-type_ship-21', '2014-11-24 13:29:10.827327', 21, 1, 'ba88e874-73c4-11e4-aafe-ab9239a90e79', 'Минно-тральный корабль', NULL, NULL, NULL, 'Минно-тральный корабль', NULL, 'AM', 33, 94);

select setval ('q_base_table_id_seq', 21, true);

INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1003', '2014-11-24 13:31:37.557668', 1003, 2, NULL, 4, 'id_type_ship', 'Ship type', 'Тип корабля', 'type_ship', 'name', 'id', 100, true, 9);
INSERT INTO attributes (unique_id, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1004', 1004, 6, NULL, 4, 'azimuth', 'Azimuth', 'Азимут', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1005', 1005, 6, NULL, 4, 'elevation_angle', 'Elevation', 'Угол возвышения', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1006', '2014-11-24 13:35:28.814191', 1006, 38, NULL, 4, 'image_raw', 'image_raw', 'image_raw', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1007', '2014-11-24 13:35:47.571567', 1007, 15, NULL, 4, 'image_jpg', 'image_jpg', 'image_jpg', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1008', '2014-12-04 12:41:54.885399', 1008, 8, NULL, 4, 'depth', 'depth', 'Глубина', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1009', '2014-12-04 12:41:54.885399', 1009, 6, NULL, 4, 'Goodness of fit', 'Goodness of fit', 'Goodness of fit', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1010', '2015-12-09 14:55:28.814191', 1010, 38, NULL, 4, 'search_object_raw', 'search_object_raw', 'search_object_raw', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1011', '2015-12-09 15:25:28.814191', 1011, 6, NULL, 4, 'resolution', 'resolution', 'Разрешение', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1012', '2015-12-09 15:25:28.814191', 1012, 6, NULL, 4, 'ship_length', 'ship_length', 'Длина корабля', NULL, NULL, NULL, 100, true, NULL);
INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1013', '2016-03-16 17:35:08.557668', 1013, 2, NULL, 4, 'id_sky', 'RLI parameters', 'Параметры объекта', 'rli_sky', 'id', 'id', 50, true, 9);

select setval ('attributes_id_seq', 1013, true);

INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-303', '2014-11-24 13:32:44.581934', 303, 10, NULL, NULL, 1, NULL, false, 'Table of RLI Image', 'CATEGORY_303', NULL, false, false, 1, false);
INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-304', '2014-11-24 13:32:44.610661', 304, 1, 303, NULL, 1, NULL, true, 'RLI Image', 'CATEGORY_304', NULL, false, false, 1, false);
INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-305', '2016-03-15 16:36:00.0', 305, 10, NULL, NULL, 1, NULL, false, 'Table of RLI Parameters', 'CATEGORY_305', NULL, false, false, 1, false);
INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-306', '2016-03-15 16:36:44.610661', 306, 1, 305, NULL, 1, NULL, true, 'RLI Parameters', 'CATEGORY_306', NULL, false, false, 1, false);

select setval ('io_categories_id_seq', 306, true);
select csetglobal (303);
select csetglobal (304);
select csetglobal (305);
select csetglobal (306);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2128', '2016-12-05 13:36:10.593138', 2128, 303, 1, 'Идентификатор', NULL, false, false, 1);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2129', '2014-11-24 13:36:10.593138', 2129, 303, 1004, 'Azimuth', '0', true, false, 2);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2130', '2014-12-04 12:42:12.164466', 2130, 303, 1005, 'elevation_angle', '0', true, false, 3);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2131', '2014-11-24 13:36:10.593138', 2131, 303, 1006, 'image_raw', NULL, false, false, 4);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2132', '2014-11-24 13:36:10.593138', 2132, 303, 1007, 'image_jpg', NULL, false, false, 5);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2133', '2014-12-04 12:42:12.164466', 2133, 303, 1008, 'depth', NULL, false, false, 6);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2134', '2014-11-24 13:32:44.581934', 2134, 303, 1012, 'Ship length', NULL, false, false, 7);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2135', '2014-11-24 13:32:44.581934', 2135, 303, 1003, 'Ship type', NULL, true, false, 8);


INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2136', '2014-11-24 13:32:44.581934', 2136, 303, 1009, 'Goodness of fit', '-1', true, false, 9);

select setval ('attrs_categories_id_seq', 2136, true);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2137', '2016-03-15 19:00:00.0', 2137, 305, 1, 'Идентификатор', NULL, false, false, 1);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2138', '2016-03-15 19:00:00.0', 2138, 305, 1011, 'resolution', NULL, false, false, 2);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2139', '2016-03-15 19:00:00.0', 2139, 305, 3, 'Описание', NULL, false, false, 3);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2140', '2016-03-15 19:00:00.0', 2140, 305, 6, 'Код', NULL, false, false, 4);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2141', '2016-03-15 19:00:00.0', 2141, 305, 1004, 'Azimuth', NULL, false, false, 5);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2142', '2016-03-15 19:00:00.0', 2142, 305, 1005, 'elevation_angle', NULL, false, false, 6);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2143', '2016-03-16 15:49:12.0', 2143, 303, 1011, 'resolution', NULL, false, false, 8);
select setval ('attrs_categories_id_seq', 2143, true);


INSERT INTO io_templates (unique_id, last_update, id, id_io_category, name, code, description) VALUES ('localorg_prefix-io_templates-54', '2015-10-28 20:25:23.622101', 54, 303, 'Специальный шаблон для Справочника радиоизображений', 'TEMPLATE_54', NULL);

SELECT pg_catalog.setval('io_templates_id_seq', 54, true);

INSERT INTO a_groups (unique_id, last_update, id, id_io_template, id_parent, name, "order") VALUES ('localorg_prefix-a_groups-112', '2015-10-28 20:25:23.622101', 112, 54, NULL, 'Group1', 0);


SELECT pg_catalog.setval('a_groups_id_seq', 112, true);

INSERT INTO io_views (unique_id, last_update, id, id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) VALUES ('localorg_prefix-io_views-330', '2015-10-28 20:25:23.622101', 330, 2135, 54, 112, false, 6, NULL);
INSERT INTO io_views (unique_id, last_update, id, id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) VALUES ('localorg_prefix-io_views-331', '2015-10-28 20:25:23.622101', 331, 2130, 54, 112, false, 2, '0');
INSERT INTO io_views (unique_id, last_update, id, id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) VALUES ('localorg_prefix-io_views-332', '2015-10-30 15:31:10.048484', 332, 2129, 54, 112, false, 1, '0');
INSERT INTO io_views (unique_id, last_update, id, id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) VALUES ('localorg_prefix-io_views-333', '2015-10-30 15:31:10.048484', 333, 2131, 54, 112, false, 0, NULL);
INSERT INTO io_views (unique_id, last_update, id, id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) VALUES ('localorg_prefix-io_views-334', '2015-10-30 15:31:10.048484', 334, 2136, 54, 112, false, 5, NULL);
INSERT INTO io_views (unique_id, last_update, id, id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) VALUES ('localorg_prefix-io_views-335', '2016-03-16 16:31:10.048484', 335, 2143, 54, 112, false, 3, NULL);
INSERT INTO io_views (unique_id, last_update, id, id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) VALUES ('localorg_prefix-io_views-336', '2016-03-16 16:31:10.048484', 336, 2134, 54, 112, false, 4, NULL);
SELECT pg_catalog.setval('io_views_id_seq', 336, true);

INSERT INTO user_templates (id_user, id_template, id_io_object, type) VALUES (1, 1, 7, 0);
INSERT INTO user_templates (id_user, id_template, id_io_object, type) VALUES (1, 54, 303, 0);

INSERT INTO tbl_io_objects (unique_id, last_update, id, id_io_category, id_io_state, id_io_type, id_maclabel, author, id_sync_type, id_owner_org, name, table_name, description, information, is_system, insert_time, is_completed, is_global, record_fill_color, record_text_color, id_search_template, ref_table_name, r_icon, uuid_t) VALUES ('localorg_prefix-io_objects-303', '2016-03-15 21:20:05.647943', 303, 306, 1, 2, 1, 1, 1, 1, 'КРСА параметры', 'rli_sky', NULL, 'Администратор системы', false, '2016-03-15 21:20:20', 2, true, NULL, NULL, NULL, NULL, NULL, '0e9b6c6e-eadb-11e5-81b2-7b6df462f3b9');
select setval ('tbl_io_objects_id_seq', 303, true);

INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-307', '2016-03-16 18:02:00.0', 307, 10, NULL, NULL, 1, NULL, false, 'Table of Object Passports', 'CATEGORY_307', NULL, false, false, 1, false);
INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-308', '2016-03-16 18:03:44.610661', 308, 1, 307, NULL, 1, NULL, true, 'Object Passports', 'CATEGORY_308', NULL, false, false, 1, false);

select setval ('io_categories_id_seq', 308, true);
select csetglobal (307);
select csetglobal (308);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2148', '2016-03-16 18:32:12.0', 2148, 307, 1, 'Идентификатор', NULL, false, false, 1);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2149', '2016-03-16 18:32:12.0', 2149, 307, 2, 'Название', NULL, true, false, 2);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2150', '2016-03-16 18:32:12.0', 2150, 307, 3, 'description', NULL, false, false, 3);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2151', '2016-03-16 18:32:44.581934', 2151, 307, 1003, 'Ship type', NULL, true, false, 4);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2152', '2016-03-16 18:32:44.581934', 2152, 307, 1013, 'RLI Parameters', NULL, false, false, 5);
select setval ('attrs_categories_id_seq', 2152, true);

INSERT INTO tbl_io_objects (unique_id, last_update, id, id_io_category, id_io_state, id_io_type, id_maclabel, author, id_sync_type, id_owner_org, name, table_name, description, information, is_system, insert_time, is_completed, is_global, record_fill_color, record_text_color, id_search_template, ref_table_name, r_icon, uuid_t) VALUES ('localorg_prefix-io_objects-304', '2016-03-16 19:20:05.647943', 304, 308, 1, 2, 1, 1, 1, 1, 'Экспериментальные изображения объектов', 'object_passports', NULL, 'Администратор системы', false, '2016-03-16 19:20:20', 2, true, NULL, NULL, NULL, NULL, NULL, '83b3ffc8-eb91-11e5-988e-b3422cd447de');
select setval ('tbl_io_objects_id_seq', 305, true);

INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1014', '2016-03-16 17:35:08.557668', 1014, 2, NULL, 4, 'id_rli_object', 'object_passport', 'Параметры объекта', 'object_passports', 'name', 'id', 100, true, 9);
select setval ('attributes_id_seq', 1014, true);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2153', '2016-03-18 13:47:44.581934', 2153, 303, 1014, 'id_rli_object', NULL, false, false, 10);
select setval ('attrs_categories_id_seq', 2153, true);

INSERT INTO tbl_io_objects (unique_id, last_update, id, id_io_category, id_io_state, id_io_type, id_maclabel, author, id_sync_type, id_owner_org, name, table_name, description, information, is_system, insert_time, is_completed, is_global, record_fill_color, record_text_color, id_search_template, ref_table_name, r_icon, uuid_t) VALUES ('localorg_prefix-io_objects-305', '2014-12-04 12:50:05.647943', 305, 304, 1, 2, 1, 1, 1, 1, 'Бинарные RLI', 'rli_image_raws', NULL, 'Администратор системы', false, '2014-12-04 04:12:20', 2, true, NULL, NULL, NULL, NULL, NULL, 'ecc82916-7b9a-11e4-af6b-9f445027ca5b');
select setval ('tbl_io_objects_id_seq', 305, true);

INSERT INTO user_templates (id_user, id_template, id_io_object, type) VALUES (1, 54, 305, 0);

/*
alter table tbl_rli_image_raws add id_rli_object bigint;

alter table tbl_rli_image_raws
   add constraint FK_TBL_RLI__REFERENCE_TBL_OBJE foreign key (id_rli_object)
      references tbl_object_passports (id)
      on delete restrict on update restrict;

drop view rli_image_raws ;

create or replace function f_sel_rli_image_raws () returns setof "tbl_rli_image_raws" as
$BODY$
declare
    r "tbl_rli_image_raws"%rowtype;
begin
    for r 
        in select * from "tbl_rli_image_raws"
    loop
        if (getPrivilege(getCurrentUser(), ioGetObjectIDByTableName('rli_image_raws'), 2, true) = false) then
            raise exception 'You have insufficient permissions to do the operation!';
            return;
        end if;
        return next r;
    end loop;
    return;
end
$BODY$
language'plpgsql' security definer;

DROP FUNCTION  if exists f_ins_rli_image_raws(character varying, timestamp without time zone, bigint, integer, uuid, character varying, character varying, bigint, bigint, double precision, bigint, double precision, double precision, bytea, character varying, integer, double precision, double precision) ;

create or replace function f_ins_rli_image_raws (character varying, timestamp without time zone, bigint, integer, uuid, character varying, character varying, bigint, bigint, bigint, double precision, double precision, double precision, bytea, character varying, integer, double precision, double precision, bigint) returns int4 as
$BODY$
declare
"ii_unique_id" alias for $1;
"ii_last_update" alias for $2;
"ii_id" alias for $3;
"ii_id_io_state" alias for $4;                                                                                            
"ii_uuid_t" alias for $5;                                                                                                 
"ii_rr_name" alias for $6;                                                                                                
"ii_r_icon" alias for $7;
"ii_record_fill_color" alias for $8;
"ii_record_text_color" alias for $9;
"ii_id_type_ship" alias for $10;
"ii_azimuth" alias for $11;
"ii_elevation_angle" alias for $12;                                                                                                    
"ii_image_raw" alias for $13;                                                                                                          
"ii_image_jpg" alias for $14;                                                                                                          
"ii_depth" alias for $15;                                                                                                              
"ii_Goodness of fit" alias for $16;
"ii_resolution" alias for $17;
"ii_ship_length" alias for $18;
"ii_id_rli_object" alias for $19;
begin
    if(getPrivilege(getCurrentUser(), ioGetObjectIDByTableName('rli_image_raws'), 4, true) = false) then 
        raise exception 'You have insufficient permissions to do the operation!';
        return 0;
    end if;
    insert into "tbl_rli_image_raws" ("unique_id", "last_update", "id", "id_io_state", "uuid_t", "rr_name", "r_icon", "record_fill_color", "record_text_color", "id_type_ship", "azimuth", "elevation_angle", "image_raw", "image_jpg", "depth", "Goodness of fit", "resolution", "ship_length", "id_rli_object") values ("ii_unique_id", "ii_last_update", "ii_id", "ii_id_io_state", "ii_uuid_t", "ii_rr_name", "ii_r_icon", "ii_record_fill_color", "ii_record_text_color", "ii_id_type_ship", "ii_azimuth", "ii_elevation_angle", "ii_image_raw", "ii_image_jpg", "ii_depth", "ii_Goodness of fit", "ii_resolution", "ii_ship_length", "ii_id_rli_object");
    return 1;
end
$BODY$
language'plpgsql' security definer;

drop function f_upd_rli_image_raws(character varying, timestamp without time zone, bigint, integer, uuid, character varying, character varying, bigint, bigint, double precision, bigint, double precision, double precision, bytea, character varying, integer, double precision, double precision);

-- character varying, timestamp without time zone, bigint, integer, uuid, character varying, character varying, bigint, bigint, bigint, double precision,  double precision, bytea, character varying, integer, double precision, double precision, double precision, bigint

create or replace function f_upd_rli_image_raws (character varying, timestamp without time zone, bigint, integer, uuid, character varying, character varying, bigint, bigint, bigint, double precision, double precision, double precision, bytea, character varying, integer, double precision, double precision, bigint) returns int4 as
$BODY$
declare
"ii_unique_id" alias for $1;
"ii_last_update" alias for $2;
"ii_id" alias for $3;
"ii_id_io_state" alias for $4;                                                                                            
"ii_uuid_t" alias for $5;                                                                                                 
"ii_rr_name" alias for $6;                                                                                                
"ii_r_icon" alias for $7;
"ii_record_fill_color" alias for $8;
"ii_record_text_color" alias for $9;
"ii_azimuth" alias for $10;
"ii_id_type_ship" alias for $11;
"ii_elevation_angle" alias for $12;                                                                                                    
"ii_image_raw" alias for $13;                                                                                                          
"ii_image_jpg" alias for $14;                                                                                                          
"ii_depth" alias for $15;                                                                                                              
"ii_Goodness of fit" alias for $16;
"ii_resolution" alias for $17;
"ii_ship_length" alias for $18;
"ii_id_rli_object" alias for $19;
begin
    if(getPrivilege(getCurrentUser(), ioGetObjectIDByTableName('rli_image_raws'), 4, true) == false) then 
        raise exception 'You have insufficient permissions to do the operation!';
        return 0;
    end if;
    update "tbl_rli_image_raws" set "unique_id" = "ii_unique_id", "last_update" = "ii_last_update", "id_io_state" = "ii_id_io_state", "uuid_t" = "ii_uuid_t", "rr_name" = "ii_rr_name", "r_icon" = "ii_r_icon", "record_fill_color" = "ii_record_fill_color",  "record_text_color" = "ii_record_text_color", "id_type_ship" = "ii_id_type_ship", "azimuth" = "ii_azimuth", "elevation_angle" = "ii_elevation_angle", "image_raw" = "ii_image_raw",  "image_jpg" = "ii_image_jpg", "depth" = "ii_depth", "Goodness of fit" = "ii_Goodness of fit", "resolution" = "ii_resolution", "ship_length" = "ii_ship_length", "id_rli_object" = "ii_id_rli_object" where id = ii_id;
    return 1;
end
$BODY$
language'plpgsql' security definer;

create view rli_image_raws as SELECT f_sel_rli_image_raws.unique_id, f_sel_rli_image_raws.last_update, 
    f_sel_rli_image_raws.id, f_sel_rli_image_raws.id_io_state, 
    f_sel_rli_image_raws.uuid_t, f_sel_rli_image_raws.rr_name, 
    f_sel_rli_image_raws.r_icon, f_sel_rli_image_raws.record_fill_color, 
    f_sel_rli_image_raws.record_text_color, f_sel_rli_image_raws.id_type_ship, 
    f_sel_rli_image_raws.azimuth, f_sel_rli_image_raws.elevation_angle, 
    f_sel_rli_image_raws.image_raw, f_sel_rli_image_raws.image_jpg, 
    f_sel_rli_image_raws.depth, f_sel_rli_image_raws.resolution, f_sel_rli_image_raws.ship_length, f_sel_rli_image_raws.id_rli_object, f_sel_rli_image_raws."Goodness of fit" FROM f_sel_rli_image_raws() f_sel_rli_image_raws(unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, id_type_ship, azimuth, elevation_angle, image_raw, image_jpg, depth, "Goodness of fit", resolution, ship_length, id_rli_object);

create or replace rule r_ins_rli_image_raws AS ON INSERT TO rli_image_raws DO INSTEAD SELECT f_ins_rli_image_raws(new.unique_id, new.last_update, new.id, new.id_io_state, new.uuid_t, new.rr_name, new.r_icon, new.record_fill_color, new.record_text_color, new.azimuth, new.id_type_ship,  new.elevation_angle, new.image_raw, new.image_jpg, new.depth, new."Goodness of fit", new.resolution, new.ship_length, new.id_rli_object) AS f_ins_rli_image_raws;

create or replace rule r_upd_rli_image_raws AS ON UPDATE TO rli_image_raws DO INSTEAD SELECT f_upd_rli_image_raws(new.unique_id, new.last_update, new.id, new.id_io_state, new.uuid_t, new.rr_name, new.r_icon, new.record_fill_color, new.record_text_color, new.azimuth, new.id_type_ship, new.elevation_angle, new.image_raw, new.image_jpg, new.depth, new."Goodness of fit", new.resolution, new.ship_length, new.id_rli_object) AS f_upd_rli_image_raws;

create or replace rule r_del_rli_image_raws AS ON DELETE TO rli_image_raws DO INSTEAD SELECT f_del_rli_image_raws(old.id) AS f_del_rli_image_raws;
*/
