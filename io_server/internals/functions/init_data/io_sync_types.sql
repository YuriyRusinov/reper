--Справочник типов синхронизации информационных объектов

insert into io_sync_types (id, name, description) values (1, 'Не синхронизируется', 'НС');
insert into io_sync_types (id, name, description) values (2, 'Глобальные централизованного ведения', 'ГЦВ');
insert into io_sync_types (id, name, description) values (3, 'Глобальные формуляры', 'ГФ');
insert into io_sync_types (id, name, description) values (4, 'Глобальные объектового ведения, интегрируемые', 'ГОВИ');
insert into io_sync_types (id, name, description) values (5, 'Глобальные объектового ведения, автономные', 'ГОВА');

SELECT pg_catalog.setval('io_sync_types_id_seq', 5, true);

