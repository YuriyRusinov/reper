insert into io_states (id, name, description, is_system) values (1, 'Активный', NULL, true);
insert into io_states (id, name, description, is_system) values (2, 'Архивный', NULL, true);
insert into io_states (id, name, description, is_system) values (3, 'Осуществляется первоначальная синхронизация', NULL, true);
insert into io_states (id, name, description, is_system) values (4, 'Осуществляется синхронизация', NULL, true);
insert into io_states (id, name, description, is_system) values (5, 'Произошло изменение значений атрибутов', NULL, true);

--предусматриваем возможность в дальнейшем безболезненно добавлять новые системные состояния
SELECT pg_catalog.setval('io_states_id_seq', 40, true);


