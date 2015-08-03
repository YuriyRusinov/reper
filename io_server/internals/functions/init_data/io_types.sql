--Справочник типов информационных объектов

insert into io_types (id, name, description) values(1, 'Общие документы', NULL);
insert into io_types (id, name, description) values(2, 'Общие справочники', NULL);
insert into io_types (id, name, description) values(3, 'Системные справочники', NULL);

SELECT pg_catalog.setval('io_types_id_seq', 3, true);

