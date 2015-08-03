insert into criteria_types (id, name) values (1, 'принадлежит заданной категории');
insert into criteria_types (id, name) values (2, 'заданный автор');
insert into criteria_types (id, name) values (3, 'наличие прикрепленного файла заданного типа');
insert into criteria_types (id, name) values (4, 'наличие атрибута с заданным значением');
insert into criteria_types (id, name) values (5, 'наличие заданного атрибута с произвольным значением');
insert into criteria_types (id, name) values (6, 'наличие заданного атрибута с заданным значением');

SELECT pg_catalog.setval('public.criteria_types_id_seq', 6, true);

insert into operations (id, name, the_value) values (1, 'Равно', '=');
insert into operations (id, name, the_value) values (2, 'Больше', '>');
insert into operations (id, name, the_value) values (3, 'Меньше', '<');
insert into operations (id, name, the_value) values (4, 'Не меньше', '>=');
insert into operations (id, name, the_value) values (5, 'Не больше', '<=');
insert into operations (id, name, the_value) values (6, 'Включает в себя', 'in');
insert into operations (id, name, the_value) values (7, 'Не включает в себя', 'not in');
insert into operations (id, name, the_value) values (8, 'Между', 'between');
insert into operations (id, name, the_value) values (9, 'Подобие по шаблону (LIKE)', 'like');
insert into operations (id, name, the_value) values (10, 'Не равно', '<>');
insert into operations (id, name, the_value) values (11, 'Не имеет значения', 'is null');
insert into operations (id, name, the_value) values (12, 'Имеет значение', 'is not null');
insert into operations (id, name, the_value) values (13, 'Включает результат выборки', 'in (select ...)');
insert into operations (id, name, the_value) values (14, 'Содержит (LIKE)', 'like');
insert into operations (id, name, the_value) values (15, 'Начинается на (LIKE)', 'like');
insert into operations (id, name, the_value) values (16, 'Заканчивается на (LIKE)', 'like');

SELECT pg_catalog.setval('public.operations_id_seq', 16, true);


insert into search_template_types (id, name, description) values(1, 'Общие поисковые запросы', NULL);
SELECT pg_catalog.setval('search_template_types_id_seq', 1, true);
