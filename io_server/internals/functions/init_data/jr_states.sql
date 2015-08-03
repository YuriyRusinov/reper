insert into jr_states (id, name, description) values (1, 'Запланирован', NULL);
insert into jr_states (id, name, description) values (2, 'Выдано задание', NULL);
insert into jr_states (id, name, description) values (3, 'Исполняется', NULL);
insert into jr_states (id, name, description) values (4, 'Исполнен', NULL);
insert into jr_states (id, name, description) values (5, 'Просрочен', NULL);
insert into jr_states (id, name, description) values (6, 'Сохранен как черновик', NULL);
insert into jr_states (id, name, description) values (7, 'Виртуальное состояние', NULL);
--команда является виртуальной, когда у нее есть получатель, но пока еще нет отправителя.
--такое возможно при информационном обмене разных организаций, 
--когда была осуществлена автоматическая (неявная) отправка распоряжения от имени некоторого ДЛ

SELECT pg_catalog.setval('jr_states_id_seq', 7, true);


