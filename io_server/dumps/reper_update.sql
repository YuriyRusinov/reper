INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-305', '2016-03-15 16:36:00.0', 305, 10, NULL, NULL, 1, NULL, false, 'Table of RLI Parameters', 'CATEGORY_305', NULL, false, false, 1, false);
INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-306', '2016-03-15 16:36:44.610661', 306, 1, 305, NULL, 1, NULL, true, 'RLI Parameters', 'CATEGORY_306', NULL, false, false, 1, false);

select setval ('io_categories_id_seq', 306, true);
select csetglobal (305);
select csetglobal (306);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2138', '2016-03-15 19:00:00.0', 2138, 305, 1, 'Идентификатор', NULL, false, false, 1);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2139', '2016-03-15 19:00:00.0', 2139, 305, 1009, 'resolution', NULL, false, false, 2);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2140', '2016-03-15 19:00:00.0', 2140, 305, 3, 'Описание', NULL, false, false, 3);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2141', '2016-03-15 19:00:00.0', 2141, 305, 6, 'Код', NULL, false, false, 4);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2142', '2016-03-15 19:00:00.0', 2142, 305, 1002, 'Azimuth', NULL, false, false, 5);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2143', '2016-03-15 19:00:00.0', 2143, 305, 1003, 'elevation_angle', NULL, false, false, 6);
select setval ('attrs_categories_id_seq', 2143, true);

INSERT INTO tbl_io_objects (unique_id, last_update, id, id_io_category, id_io_state, id_io_type, id_maclabel, author, id_sync_type, id_owner_org, name, table_name, description, information, is_system, insert_time, is_completed, is_global, record_fill_color, record_text_color, id_search_template, ref_table_name, r_icon, uuid_t) VALUES ('localorg_prefix-io_objects-304', '2016-03-15 21:20:05.647943', 304, 306, 1, 2, 1, 1, 1, 1, 'RLI Parameters', 'rli_sky', NULL, 'Администратор системы', false, '2016-03-15 21:20:20', 2, true, NULL, NULL, NULL, NULL, NULL, '0e9b6c6e-eadb-11e5-81b2-7b6df462f3b9');


