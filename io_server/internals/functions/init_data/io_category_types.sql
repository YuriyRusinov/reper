--Справочник типов информационных объектов

insert into io_category_types (id, name, r_name, description, is_qualifier) values(1, 'Справочник', 'Справочники', NULL, true);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(2, 'Журнал', 'Журналы', NULL, true);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(3, 'Документ', 'Документы', NULL, false);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(4, 'Формуляр', 'Формуляры', NULL, false);
--insert into io_category_types (id, name, r_name, description) values(5, 'Сообщение', 'Сообщения', NULL);
--insert into io_category_types (id, name, r_name, description) values(6, 'Сообщение в трафарете изделия 83т20', 'Сообщения в трафарете изделия 83т20', NULL);
--insert into io_category_types (id, name, r_name, description) values(7, 'Шаблон документа', 'Шаблоны документов', NULL);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(8, 'Системный справочник', 'Системные справочники', 'Используется в качестве типа для категорий, которые являются системными. Например, справочник состояний; справочник атрибутов; справочник типов атрибутов; справочник типов информационных объектов.', true);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(9, 'Системный справочник категорий документов', 'Системные справочники категорий документов', 'Используется в качестве типа для системных информационных объектов, которые являются справочниками категорий информационных объектов', true);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(10, 'Подчиненная категория', 'Подчиненные категории', 'Используется в качестве типа для категорий, которые являются подчиненными (описывают таблицы)', false);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(11, 'Документ в документообороте', 'Документы в документообороте', 'Используется в качестве типа для категорий, экземпляры которых обрабатываются средствами подсистемы электронного документооборота', false);
insert into io_category_types (id, name, r_name, description, is_qualifier) values(12, 'Шаблон поискового запроса', 'Шаблон поискового запроса', 'Используется в качестве типа для категорий, экземпляры которых обрабатываются как поисковые запросы', false);

SELECT pg_catalog.setval('io_category_types_id_seq', 12, true);

