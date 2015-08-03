insert into notify_where(id, name) values (1, 'insert');
insert into notify_where(id, name) values (2, 'update');
insert into notify_where(id, name) values (3, 'delete');

SELECT pg_catalog.setval('notify_where_id_seq', 3, true);
