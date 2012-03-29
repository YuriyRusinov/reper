--Справочник типов информационных объектов

insert into io_category_types (id, name, r_name, description) values(1, 'Справочник', 'Справочники', NULL);
insert into io_category_types (id, name, r_name, description) values(2, 'Журнал', 'Журналы', NULL);
insert into io_category_types (id, name, r_name, description) values(3, 'Документ', 'Документы', NULL);
insert into io_category_types (id, name, r_name, description) values(4, 'Формуляр', 'Формуляры', NULL);
--insert into io_category_types (id, name, r_name, description) values(5, 'Сообщение', 'Сообщения', NULL);
--insert into io_category_types (id, name, r_name, description) values(6, 'Сообщение в трафарете изделия 83т20', 'Сообщения в трафарете изделия 83т20', NULL);
--insert into io_category_types (id, name, r_name, description) values(7, 'Шаблон документа', 'Шаблоны документов', NULL);
insert into io_category_types (id, name, r_name, description) values(8, 'Системный справочник', 'Системные справочники', 'Используется в качестве типа для категорий, которые являются системными. Например, справочник состояний; справочник атрибутов; справочник типов атрибутов; справочник типов информационных объектов.');
insert into io_category_types (id, name, r_name, description) values(9, 'Системный справочник категорий документов', 'Системные справочники категорий документов', 'Используется в качестве типа для системных информационных объектов, которые являются справочниками категорий информационных объектов');
insert into io_category_types (id, name, r_name, description) values(10, 'Подчиненная категория', 'Подчиненные категории', 'Используется в качестве типа для категорий, которые являются подчиненными (описывают таблицы)');
insert into io_category_types (id, name, r_name, description) values(11, 'Документ в документообороте', 'Документы в документообороте', 'Используется в качестве типа для категорий, экземпляры которых обрабатываются средствами подсистемы электронного документооборота');
insert into io_category_types (id, name, r_name, description) values(12, 'Шаблон поискового запроса', 'Шаблон поискового запроса', 'Используется в качестве типа для категорий, экземпляры которых обрабатываются как поисковые запросы');

SELECT pg_catalog.setval('io_category_types_id_seq', 12, true);

