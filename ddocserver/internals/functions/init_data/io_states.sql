insert into io_states (id, name, description) values (1, 'Активный', NULL);
insert into io_states (id, name, description) values (2, 'Архивный', NULL);
insert into io_states (id, name, description) values (3, 'Осуществляется первоначальная синхронизация', NULL);
insert into io_states (id, name, description) values (4, 'Осуществляется синхронизация', NULL);
--insert into io_states (id, name, description) values (5, 'На исполнении', NULL);
--insert into io_states (id, name, description) values (6, 'Исполнен', NULL);
--insert into io_states (id, name, description) values (7, 'Отправлен', NULL);

SELECT pg_catalog.setval('io_states_id_seq', 4, true);


