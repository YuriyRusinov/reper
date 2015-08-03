insert into ranks (id, name, short_name) values(1, 'Не задано', 'Не задано');
insert into ranks (id, name, short_name) values(2, 'Служащий РА', 'Сл. РА');
insert into ranks (id, name, short_name) values(3, 'Младший лейтенант', 'Мл. л-т');
insert into ranks (id, name, short_name) values(4, 'Лейтенант', 'Л-т');
insert into ranks (id, name, short_name) values(5, 'Старший лейтенант', 'Ст. л-т');
insert into ranks (id, name, short_name) values(6, 'Капитан', 'К-н');
insert into ranks (id, name, short_name) values(7, 'Майор', 'М-р');
insert into ranks (id, name, short_name, code) values(8, 'Подполковник', 'П/п-к', 'army.ppk');
insert into ranks (id, name, short_name, code) values(9, 'Полковник', 'П-к', 'army.pk');
insert into ranks (id, name, short_name) values(10, 'Генерал-майор', 'Г-л/м-р');
insert into ranks (id, name, short_name) values(11, 'Генерал-лейтенант', 'Г-л/л-т');
insert into ranks (id, name, short_name) values(12, 'Генерал-полковник', 'Г-л/п-к');
insert into ranks (id, name, short_name) values(13, 'Генерал армии', 'Г-л/армии');

SELECT pg_catalog.setval('ranks_id_seq', 13, true);
