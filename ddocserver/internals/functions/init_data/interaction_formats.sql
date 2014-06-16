insert into interaction_formats (id, name, description) values (1, 'Унифицированный обменный XML-формат описания информационных ресурсов в системе DynamicDocs', NULL);
insert into interaction_formats (id, name, description) values (2, 'Простой формат представления на основе шаблона PARAM=VALUE', 'Информационный ресурс, запись справочника предстают в виде набора атрибутов, описываемых в виде атрибут-строковое представление значения. Используется для простого базового взаимодействия. Проверка на соответствие структурам данных не осуществляется');
insert into interaction_formats (id, name, description) values (3, 'XML-Формат ОКР "Заря-22"', 'формат представления данных ЦНИИ ЭИСУ в ОКР "Заря-22"');
insert into interaction_formats (id, name, description) values (4, 'Внутренний бинарный формат представления данных при взаимодействии между системами DynamicDocs', NULL);

SELECT pg_catalog.setval('interaction_formats_id_seq', 4, true);
