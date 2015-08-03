\encoding win1251

begin;

insert into attributes (id, id_a_type, code, name, title, table_name, column_name, def_width) values(62, 1, 'with_inheritance', 'Наследовать права доступа', 'Наследовать права доступа', NULL, NULL, 60);
SELECT pg_catalog.setval('attributes_id_seq', 62, true);

delete from attrs_categories where id_io_category = 19 and id_io_attribute = 21;
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (19, 62, 'true', true, false);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (25, 62, 'true', true, false);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (27, 62, 'true', true, false);

insert into kks_roles(id, role_name, with_inheritance) values(-10, 'Остальные роли', FALSE);

insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (1, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (2, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (3, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (4, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (5, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (6, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (7, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (8, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (9, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (10, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (11, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (12, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (13, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (14, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (15, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (16, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (17, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (19, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (20, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (21, -10, TRUE, TRUE, FALSE, FALSE);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (22, -10, TRUE, TRUE, FALSE, FALSE);

--добавить создание индекса на access_table
--добавить оздание индекса на dls (id_unit)
--добавить оздание индекса на units (id_parent)


\i ./functions/readd_functions.sql

\i ./functions/init_data/acl_templates.sql

\i ./initprivs.sql

commit;
