--Системные категории

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-1', 1, 10, NULL, false, 'Состояния информационных объектов', 'SYSCATEGORY_1', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(1, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-2', 2, 8, 1, true, 'Справочник состояний информационных объектов', 'SYSCATEGORY_2', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(2, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-3', 3, 10, NULL, false, 'Типы категорий информационных объектов', 'SYSCATEGORY_3', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(3, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-4', 4, 8, 3, true, 'Справочник типов категорий информационных объектов', 'SYSCATEGORY_4', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(4, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-5', 5, 10, NULL, false, 'Представления типов атрибутов', 'SYSCATEGORY_5', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(5, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-6', 6, 8, 5, true, 'Справочник представлений типов атрибутов', 'SYSCATEGORY_6', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(6, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-7', 7, 10, NULL, false, 'Типы атрибутов', 'SYSCATEGORY_7', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(7, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-8', 8, 8, 7, true, 'Справочник типов атрибутов', 'SYSCATEGORY_8', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(8, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-9', 9, 10, NULL, false, 'Атрибуты', 'SYSCATEGORY_9', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(9, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-10', 10, 8, 9, true, 'Справочник атрибутов', 'SYSCATEGORY_10', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(10, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-11', 11, 10, NULL, false, 'Категории', 'SYSCATEGORY_11', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(11, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-12', 12, 9, 11, true, 'Справочник категорий', 'SYSCATEGORY_12', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(12, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-13', 13, 10, NULL, false, 'Информационные объекты', 'SYSCATEGORY_13', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(13, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-14', 14, 8, 13, true, 'Справочник информационных объектов', 'SYSCATEGORY_14', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(14, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-15', 15, 10, NULL, false, 'Шаблоны информационных объектов', 'SYSCATEGORY_15', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(15, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-16', 16, 8, 15, true, 'Справочник шаблонов информационных объектов', 'SYSCATEGORY_16', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(16, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-17', 17, 10, NULL, false, 'Типы информационных объектов', 'SYSCATEGORY_17', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(17, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-18', 18, 8, 17, true, 'Справочник типов информационных объектов', 'SYSCATEGORY_18', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(18, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-19', 19, 10, NULL, false, 'Типы поисковых запросов', 'SYSCATEGORY_19', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(19, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-20', 20, 8, 19, true, 'Справочник типов поисковых запросов', 'SYSCATEGORY_20', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(20, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-29', 29, 10, NULL, false, 'Табель срочных донесений', 'SYSCATEGORY_29', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(29, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-30', 30, 2, 29, true, 'Табель срочных донесений', 'SYSCATEGORY_30', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(30, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-31', 31, 10, NULL, false, 'Состояния пунктов табелей и журналов', 'SYSCATEGORY_31', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(31, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-32', 32, 8, 31, true, 'Справочник состояний пунктов табелей и журналов', 'SYSCATEGORY_32', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(32, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-33', 33, 10, NULL, false, 'Распоряжения', 'SYSCATEGORY_33', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(33, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-34', 34, 2, 33, true, 'Журнал входящих и исходящих распоряжений', 'SYSCATEGORY_34', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(34, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-37', 37, 10, NULL, false, 'Журнал контроля табелей и журналов распоряжений', 'SYSCATEGORY_37', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(37, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-38', 38, 2, 37, true, 'Журнал контроля табелей и журналов распоряжений', 'SYSCATEGORY_38', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(38, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-39', 39, 10, NULL, false, 'Сообщения', 'SYSCATEGORY_39', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(39, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-40', 40, 2, 39, true, 'Журнал входящих и исходящих сообщений пользователей', 'SYSCATEGORY_40', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(40, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-41', 41, 10, NULL, false, 'Типы прикрепленных файлов', 'SYSCATEGORY_41', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(41, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-42', 42, 8, 41, true, 'Справочник типов прикрепленных файлов', 'SYSCATEGORY_42', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(42, -10, true, true, false, false, true);
                                                                                                                                                       
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-43', 43, 10, NULL, false, 'Зарегистрированные расширения прикрепляемых файлов', 'SYSCATEGORY_43', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(43, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-44', 44, 8, 43, true, 'Справочник зарегистрированных расширений прикрепляемых файлов', 'SYSCATEGORY_44', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(44, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-45', 45, 10, NULL, false, 'Грифы секретности', 'SYSCATEGORY_45', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(45, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-46', 46, 8, 45, true, 'Справочник грифов секретности', 'SYSCATEGORY_46', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(46, -10, true, true, false, false, true);




--новая оргштатная структура

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-47', 47, 10, NULL, false, 'Воинские звания', 'SYSCATEGORY_47', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(47, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-48', 48, 8, 47, true, 'Справочник воинских званий', 'SYSCATEGORY_48', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(48, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-49', 49, 10, NULL, false, 'Пользователи', 'SYSCATEGORY_49', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(49, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-50', 50, 8, 49, true, 'Справочник пользователей', 'SYSCATEGORY_50', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(50, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-51', 51, 10, NULL, false, 'Должности', 'SYSCATEGORY_51', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(51, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-52', 52, 8, 51, true, 'Справочник должностей', 'SYSCATEGORY_52', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(52, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-53', 53, 10, NULL, false, 'Организации', 'SYSCATEGORY_53', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(53, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-54', 54, 8, 53, true, 'Справочник организаций', 'SYSCATEGORY_54', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(54, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-55', 55, 10, NULL, false, 'Подразделения', 'SYSCATEGORY_55', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(55, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-56', 56, 8, 55, true, 'Справочник подразделений', 'SYSCATEGORY_56', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(56, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-59', 59, 10, NULL, false, 'Режимы функционирования', 'SYSCATEGORY_59', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(59, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-60', 60, 8, 59, true, 'Справочник режимов функционирования', 'SYSCATEGORY_60', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(60, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-61', 61, 10, NULL, false, 'Состояния пользователей', 'SYSCATEGORY_61', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(61, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-62', 62, 8, 61, true, 'Справочник состояний пользователей', 'SYSCATEGORY_62', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(62, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-63', 63, 10, NULL, false, 'Типы режимов функционирования', 'SYSCATEGORY_63', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(63, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-64', 64, 8, 63, true, 'Справочник типов режимов функционирования', 'SYSCATEGORY_64', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(64, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-65', 65, 10, NULL, false, 'Типы организаций', 'SYSCATEGORY_65', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(65, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-66', 66, 8, 65, true, 'Справочник типов организаций', 'SYSCATEGORY_66', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(66, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-67', 67, 10, NULL, false, 'Транспорты межобъектового обмена', 'SYSCATEGORY_67', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(67, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-68', 68, 8, 67, true, 'Справочник транспортов межобъектового обмена', 'SYSCATEGORY_68', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(68, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-69', 69, 10, NULL, false, 'Адреса организаций', 'SYSCATEGORY_69', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(69, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-70', 70, 8, 69, true, 'Справочник адресов организаций', 'SYSCATEGORY_70', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(70, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-71', 71, 10, NULL, false, 'Шаблоны отчетов', 'SYSCATEGORY_71', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(71, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-72', 72, 8, 71, true, 'Справочник шаблонов отчетов', 'SYSCATEGORY_72', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(72, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-73', 73, 10, NULL, false, 'Поисковые запросы', 'SYSCATEGORY_73', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(73, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-74', 74, 8, 73, true, 'Справочник поисковых запросов', 'SYSCATEGORY_74', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(74, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-75', 75, 10, NULL, false, 'Пользовательские очереди', 'SYSCATEGORY_75', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(75, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-76', 76, 8, 75, true, 'Справочник пользовательских очередей', 'SYSCATEGORY_76', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(76, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-77', 77, 10, NULL, false, 'Общесистемный рубрикатор', 'SYSCATEGORY_77', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(77, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-78', 78, 8, 77, true, 'Общесистемный рубрикатор', 'SYSCATEGORY_78', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(78, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-79', 79, 10, NULL, false, 'Типы синхронизации информационных объектов', 'SYSCATEGORY_79', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(79, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-80', 80, 8, 79, true, 'Справочник типов синхронизации информационных объектов', 'SYSCATEGORY_80', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(80, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-81', 81, 10, NULL, false, 'Архив (история) синхронизации информационных объектов', 'SYSCATEGORY_81', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(81, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-82', 82, 8, 81, true, 'Архив (история) синхронизации информационных объектов', 'SYSCATEGORY_82', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(82, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-83', 83, 10, NULL, false, 'Параметры синхронизации информационных объектов', 'SYSCATEGORY_83', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(83, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-84', 84, 8, 83, true, 'Справочник параметров синхронизации информационных объектов', 'SYSCATEGORY_84', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(84, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-85', 85, 10, NULL, false, 'Типы XML-запросов для обмена информацией с внешними системами', 'SYSCATEGORY_85', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(85, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-86', 86, 8, 85, true, 'Справочник типов XML-запросов для обмена информацией с внешними системами', 'SYSCATEGORY_86', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(86, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-87', 87, 10, NULL, false, 'XML-тэги (форматы) ответов на запросы при обмене информацией с внешними системами', 'SYSCATEGORY_87', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(87, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-88', 88, 8, 87, true, 'Справочник XML-тэгов (форматов) ответов на запросы при обмене информацией с внешними системами', 'SYSCATEGORY_88', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(88, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-89', 89, 10, NULL, false, 'Виды XML-запросов для обмена информацией с внешними системами', 'SYSCATEGORY_89', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(89, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-90', 90, 8, 89, true, 'Справочник видов XML-запросов при обмене информацией с внешними системами', 'SYSCATEGORY_90', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(90, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-91', 91, 10, NULL, false, 'XML-запросы для обмена информацией с внешними системами', 'SYSCATEGORY_91', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(91, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-92', 92, 8, 91, true, 'Справочник XML-запросов при обмене информацией с внешними системами', 'SYSCATEGORY_92', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(92, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-93', 93, 10, NULL, false, 'Ответы на XML-запросы при обмене информацией с внешними системами', 'SYSCATEGORY_93', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(93, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-94', 94, 8, 93, true, 'Справочник ответов на XML-запросы при обмене информацией с внешними системами', 'SYSCATEGORY_94', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(94, -10, true, true, false, false, true);

---
---Internal qualifiers for IK TSO
---
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-95', 95, 10, NULL, false, 'Должности (ИК ТСО)', 'SYSCATEGORY_95', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(95, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-96', 96, 8, 95, true, 'Справочник должностей (ИК ТСО)', 'SYSCATEGORY_96', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(96, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-97', 97, 10, NULL, false, 'Типы карт доступа (ИК ТСО)', 'SYSCATEGORY_97', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(97, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-98', 98, 8, 97, true, 'Справочник типов карт доступа (ИК ТСО)', 'SYSCATEGORY_98', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(98, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-99', 99, 10, NULL, false, 'Статусы карт доступа (ИК ТСО)', 'SYSCATEGORY_99', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(99, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-100', 100, 8, 99, true, 'Справочник статусов карт доступа (ИК ТСО)', 'SYSCATEGORY_100', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(100, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-101', 101, 10, NULL, false, 'Планы доступа (ИК ТСО)', 'SYSCATEGORY_101', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(101, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-102', 102, 8, 101, true, 'Справочник планов доступа (ИК ТСО)', 'SYSCATEGORY_102', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(102, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-103', 103, 10, NULL, false, 'Отделы (ИК ТСО)', 'SYSCATEGORY_103', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(103, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-104', 104, 8, 103, true, 'Справочник отделов (ИК ТСО)', 'SYSCATEGORY_104', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(104, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-105', 105, 10, NULL, false, 'Охраняемые объекты (ИК ТСО)', 'SYSCATEGORY_105', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(105, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-106', 106, 8, 105, true, 'Справочник охраняемых объектов (ИК ТСО)', 'SYSCATEGORY_106', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(106, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-107', 107, 10, NULL, false, 'MIME-типы', 'SYSCATEGORY_107', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(107, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-108', 108, 8, 107, true, 'Справочник MIME-типов', 'SYSCATEGORY_108', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(108, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-109', 109, 10, NULL, false, 'Неисправные устройства (ИК ТСО)', 'SYSCATEGORY_109', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(109, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-110', 110, 8, 109, true, 'Справочник неисправных устройств (ИК ТСО)', 'SYSCATEGORY_110', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(110, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-111', 111, 10, NULL, false, 'Ответы на запросы о тревожных событиях (ИК ТСО)', 'SYSCATEGORY_111', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(111, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-112', 112, 8, 111, true, 'Справочник ответов на запросы о тревожных событиях (ИК ТСО)', 'SYSCATEGORY_112', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(112, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-113', 113, 10, NULL, false, 'Информация о персонале (ИК ТСО)', 'SYSCATEGORY_113', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(113, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-114', 114, 8, 113, true, 'Справочник персонала (ИК ТСО)', 'SYSCATEGORY_114', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(114, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-115', 115, 10, NULL, false, 'Граф. планы (ИК ТСО)', 'SYSCATEGORY_115', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(115, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-116', 116, 8, 115, true, 'Справочник граф. планов (ИК ТСО)', 'SYSCATEGORY_116', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(116, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-117', 117, 10, NULL, false, 'Устройства (элементы) графических планов (ИК ТСО)', 'SYSCATEGORY_117', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(117, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-118', 118, 8, 117, true, 'Справочник устройств (элементов) графических планов (ИК ТСО)', 'SYSCATEGORY_118', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(118, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-119', 119, 10, NULL, false, 'Типы фигур графических элементов (ИК ТСО)', 'SYSCATEGORY_119', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(119, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-120', 120, 8, 119, true, 'Справочник типов фигур графических элементов (ИК ТСО)', 'SYSCATEGORY_120', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(120, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-121', 121, 10, NULL, false, 'Типы отрезков для сегментов (ИК ТСО)', 'SYSCATEGORY_121', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(121, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-122', 122, 8, 121, true, 'Справочник типов отрезков для сегментов (ИК ТСО)', 'SYSCATEGORY_122', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(122, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-123', 123, 10, NULL, false, 'Стили отображения графических примитивов (ИК ТСО)', 'SYSCATEGORY_123', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(123, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-124', 124, 8, 123, true, 'Справочник стилей отображения графических примитивов (ИК ТСО)', 'SYSCATEGORY_124', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(124, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-125', 125, 10, NULL, false, 'Фигуры для отображения устройств на граф. планах (ИК ТСО)', 'SYSCATEGORY_125', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(125, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-126', 126, 8, 125, true, 'Справочник фигур для отображения устройств на граф. планах (ИК ТСО)', 'SYSCATEGORY_126', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(126, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-127', 127, 10, NULL, false, 'Параметры отдельных отрезков графических элементов (ИК ТСО)', 'SYSCATEGORY_127', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(127, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-128', 128, 8, 127, true, 'Справочник параметров отдельных отрезков графических элементов (ИК ТСО)', 'SYSCATEGORY_128', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(128, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-129', 129, 10, NULL, false, 'Уровни срочности сообщений (распоряжений)', 'SYSCATEGORY_129', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(129, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-130', 130, 8, 129, true, 'Справочник уровней срочности сообщений (распоряжений)', 'SYSCATEGORY_130', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(130, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-131', 131, 10, NULL, false, 'Списки рассылки распоряжений и сообщений', 'SYSCATEGORY_131', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(131, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-132', 132, 8, 131, true, 'Справочник списков рассылки распоряжений и сообщений', 'SYSCATEGORY_132', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(132, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-133', 133, 10, NULL, false, 'События ИК ТСО (интегральный)', 'SYSCATEGORY_133', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(133, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-134', 134, 8, 133, true, 'Интегральный справочник событий ИК ТСО', 'SYSCATEGORY_134', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(134, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-135', 135, 10, NULL, false, 'Неисправные устройства ИК ТСО (интегральный)', 'SYSCATEGORY_135', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(135, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-136', 136, 8, 135, true, 'Интегральный справочник неисправных устройств ИК ТСО', 'SYSCATEGORY_136', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(136, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-137', 137, 10, NULL, false, 'Устройства и охраняемые объекты ИК ТСО (интегральный)', 'SYSCATEGORY_137', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(137, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-138', 138, 8, 137, true, 'Интегральный справочник устройств и охраняемых объектов ИК ТСО', 'SYSCATEGORY_138', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(138, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-139', 139, 10, NULL, false, 'Персонал объектов ИК ТСО (интегральный)', 'SYSCATEGORY_139', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(139, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-140', 140, 8, 139, true, 'Интегральный справочник персонала объектов ИК ТСО', 'SYSCATEGORY_140', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(140, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-141', 141, 10, NULL, false, 'Карты доступа к объектам ИК ТСО (интегральный)', 'SYSCATEGORY_141', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(141, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-142', 142, 8, 141, true, 'Интегральный справочник карт доступа к объектам ИК ТСО', 'SYSCATEGORY_142', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(142, -10, true, true, false, false, true);

--УЧЕТНАЯ КАРТОЧКА ДОКУМЕНТА
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-143', 143, 10, NULL, false, 'Типы документов', 'SYSCATEGORY_143', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(143, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-144', 144, 8, 143, true, 'Справочник типов документов', 'SYSCATEGORY_144', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(144, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-145', 145, 10, NULL, false, 'Подтипы документов', 'SYSCATEGORY_145', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(145, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-146', 146, 8, 145, true, 'Справочник подтипов документов', 'SYSCATEGORY_146', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(146, -10, true, true, false, false, true);

--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-147', 147, 10, NULL, false, 'Учетные карточки документов', 'SYSCATEGORY_147', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-148', 148, 11, NULL, true, 'Учетная карточка документа', 'SYSCATEGORY_148', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(148, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-149', 149, 10, NULL, false, 'Виды документов', 'SYSCATEGORY_149', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(149, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-150', 150, 8, 149, true, 'Справочник видов документов', 'SYSCATEGORY_150', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(150, -10, true, true, false, false, true);

--TSO
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-151', 151, 11, NULL, true, 'Настройки системы ТСО', 'SYSCATEGORY_151', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(151, -10, true, true, false, false, true);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-152', 152, 8, 151, true, 'Справочник справочник настроек системы ТСО', 'SYSCATEGORY_152', NULL::varchar, true, true, 1);

--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-151', 151, 10, NULL, false, 'Военные округа', 'SYSCATEGORY_151', NULL::varchar, true, true, 1);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-152', 152, 8, 151, true, 'Справочник военных округов', 'SYSCATEGORY_152', NULL::varchar, true, true, 1);

--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-153', 153, 10, NULL, false, 'Войсковые части', 'SYSCATEGORY_153', NULL::varchar, true, true, 1);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-154', 154, 8, 153, true, 'Справочник войсковых частей', 'SYSCATEGORY_154', NULL::varchar, true, true, 1);

--
--ZARYA22
--
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-152', 152, 10, NULL, false, 'Воинские звания (Заря22)', 'SYSCATEGORY_152', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(152, -10, true, true, false, false, true);
--insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-153', 153, 8, 142, true, 'Реестр воинских званий (Заря22)', 'SYSCATEGORY_153', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(153, -10, true, true, false, false, true);
/*
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-154', 154, 10, NULL, false, 'Предметные области (Заря22)', 'SYSCATEGORY_154', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(154, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-155', 155, 8, 154, true, 'Реестр предметных областей (Заря22)', 'SYSCATEGORY_155', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(155, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-156', 156, 10, NULL, false, 'Характеристики методов расчета контрольных сумм (Заря22)', 'SYSCATEGORY_156', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(156, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-157', 157, 8, 156, true, 'Реестр характеристик методов расчета контрольных сумм (Заря22)', 'SYSCATEGORY_157', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(157, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-158', 158, 10, NULL, false, 'Организационные единицы (Заря22)', 'SYSCATEGORY_158', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(158, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-159', 159, 8, 158, true, 'Реестр организационных единиц (Заря22)', 'SYSCATEGORY_159', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(159, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-160', 160, 10, NULL, false, 'Должностные единицы (Заря22)', 'SYSCATEGORY_160', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(160, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-161', 161, 8, 160, true, 'Реестр должностных единиц (Заря22)', 'SYSCATEGORY_161', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(161, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-162', 162, 10, NULL, false, 'АСУ (Заря22)', 'SYSCATEGORY_162', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(162, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-163', 163, 8, 162, true, 'Реестр АСУ (Заря22)', 'SYSCATEGORY_163', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(163, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-164', 164, 10, NULL, false, 'Должностные лица (Заря22)', 'SYSCATEGORY_164', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(164, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-165', 165, 8, 164, true, 'Реестр должэностных лиц (Заря22)', 'SYSCATEGORY_165', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(165, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-166', 166, 10, NULL, false, 'Адреса (Заря22)', 'SYSCATEGORY_166', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(166, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-167', 167, 8, 166, true, 'Реестр адресов (Заря22)', 'SYSCATEGORY_167', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(167, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-168', 168, 3, NULL, true, 'Простое входящее сообщение (Заря22)', 'SYSCATEGORY_168', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(168, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-169', 169, 3, NULL, true, 'Простая входящая команда (Заря22)', 'SYSCATEGORY_169', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(169, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-170', 170, 3, NULL, true, 'Донесение о чрезвычайной ситуации (Заря22)', 'SYSCATEGORY_170', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(170, -10, true, true, false, false, true);
*/

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-171', 171, 10, NULL, false, 'Группы атрибутов (пользовательские)', 'SYSCATEGORY_171', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(171, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-172', 172, 8, 171, true, 'Справочник групп атрибутов', 'SYSCATEGORY_172', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(172, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-173', 173, 10, NULL, false, 'Типы должностных лиц', 'SYSCATEGORY_173', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(173, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-174', 174, 8, 173, true, 'Справочник типов должностных лиц', 'SYSCATEGORY_174', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(174, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-175', 175, 10, NULL, false, 'Операторы сравнения', 'SYSCATEGORY_175', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(175, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-176', 176, 8, 175, true, 'Справочник операторов сравнения', 'SYSCATEGORY_176', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(176, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-177', 177, 10, NULL, false, 'Типы показателей', 'SYSCATEGORY_177', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(177, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-178', 178, 8, 177, true, 'Справочник типов показателей', 'SYSCATEGORY_178', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(178, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-179', 179, 10, NULL, false, 'Показатели', 'SYSCATEGORY_179', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(179, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-180', 180, 8, 179, true, 'Справочник показателей', 'SYSCATEGORY_180', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(180, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-181', 181, 10, NULL, false, 'Значения показателей', 'SYSCATEGORY_181', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(181, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-182', 182, 8, 181, true, 'Справочник значений показателей', 'SYSCATEGORY_182', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(182, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-183', 183, 10, NULL, false, 'Значения атрибутов в информационных объектах', 'SYSCATEGORY_183', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(183, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-184', 184, 8, 183, true, 'Справочник значений атрибутов в информационных объектах', 'SYSCATEGORY_184', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(184, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-185', 185, 10, NULL, false, 'Атрибуты в категориях', 'SYSCATEGORY_185', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(185, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-186', 186, 8, 185, true, 'Справочник атрибутов в категориях', 'SYSCATEGORY_186', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(186, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-187', 187, 10, NULL, false, 'Значения атрибутов в атрибутах', 'SYSCATEGORY_187', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(187, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-188', 188, 8, 187, true, 'Справочник значений атрибутов в атрибутах', 'SYSCATEGORY_188', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(188, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-189', 189, 10, NULL, false, 'Атрибуты в атрибутах', 'SYSCATEGORY_189', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(189, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-190', 190, 8, 189, true, 'Справочник атрибутов в атрибутах', 'SYSCATEGORY_190', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(190, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-191', 191, 10, NULL, false, 'Законы распределения', 'SYSCATEGORY_191', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(191, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-192', 192, 8, 191, true, 'Справочник законов распределения', 'SYSCATEGORY_192', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(192, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-193', 193, 10, NULL, false, 'Потоки сообщений', 'SYSCATEGORY_193', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(193, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-194', 194, 8, 193, true, 'Справочник потоков сообщений', 'SYSCATEGORY_194', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(194, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-195', 195, 10, NULL, false, 'Жизненные циклы категорий', 'SYSCATEGORY_195', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(195, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-196', 196, 8, 195, true, 'Справочник жизненных циклов категорий', 'SYSCATEGORY_196', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(196, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-197', 197, 10, NULL, false, 'Переходы состояний в жизненных циклах', 'SYSCATEGORY_197', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(197, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-198', 198, 8, 197, true, 'Справочник переходов состояний в жизненных циклах', 'SYSCATEGORY_198', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(198, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-199', 199, 10, NULL, false, 'Сервисы-обработчики очередей', 'SYSCATEGORY_199', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(199, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-200', 200, 8, 199, true, 'Справочник сервисов-обработчиков очередей', 'SYSCATEGORY_200', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(200, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-201', 201, 10, NULL, false, 'Описания очередей обработки', 'SYSCATEGORY_201', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(201, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-202', 202, 8, 201, true, 'Справочник описаний очередей обработки', 'SYSCATEGORY_202', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(202, -10, true, true, false, false, true);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-203', 203, 10, NULL, false, 'Очереди обработки (данные для обработки)', 'SYSCATEGORY_203', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(203, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-204', 204, 8, 203, true, 'Справочник очередей обработки (данные для обработки)', 'SYSCATEGORY_204', NULL::varchar, true, true, 1);
insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
values(204, -10, true, true, false, false, true);


SELECT pg_catalog.setval('io_categories_id_seq', 300, true); --все пользовательские категории будут начинаться с номера 301
                                                             --это сделано для того, чтобы оставить резерв для системных категорий 

update io_categories set is_completed = 1;

--атрибуты "дочерних категорий"
--ВАЖНО!!! Для атрибутов ссылочных здесь не устанавливается тип атрибута, на который они ссылаются. Это делается отдельной операцией в конце данного скрипта
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-1', 1, 8, 'id', 'Идентификатор', 'ИД', NULL, NULL, 50, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-2', 2, 9, 'name', 'Название', 'Название', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-3', 3, 9, 'description', 'Описание', 'Описание', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-4', 4, 2, 'id_a_view', 'Визуальный элемент управления', 'Виджет', 'a_views', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-5', 5, 2, 'id_a_type', 'Тип атрибута', 'Тип атрибута', 'a_types', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-6', 6, 9, 'code', 'Код', 'Код', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-7', 7, 9, 'title', 'Визуальная надпись', 'Надпись', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-8', 8, 9, 'table_name', 'Название таблицы', 'Название таблицы', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-9', 9, 9, 'column_name', 'Название колонки таблицы', 'Колонка таблицы', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-10', 10, 2, 'id_io_category_type', 'Тип категории информационного объекта', 'Тип категории', 'io_category_types', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-11', 11, 3, 'id_child', 'Подчиненная категория', 'Подчиненная категория', 'io_categories', 'name', 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-12', 12, 2, 'id_io_category', 'Категория информационного объекта', 'Категория ИО', 'io_categories', 'name', 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-13', 13, 2, 'author', 'Автор (создатель)', 'Автор', 'users', 'fio', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-14', 14, 13, 'information', 'Информация об авторе ИО', 'Информация об авторе ИО', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-15', 15, 1, 'is_system', 'Системный', 'Системный', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-16', 16, 5, 'insert_time', 'Дата и время создания', 'Создан( внесен в БД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-17', 17, 1, 'is_main', 'Основная категория', 'Основная категория', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-18', 18, 2, 'id_io_state', 'Состояние', 'Состояние', 'io_states', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-19', 19, 9, 'short_name', 'Короткое название', 'Короткое название', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-20', 20, 2, 'id_rank', 'Воинское звание', 'Звание', 'ranks', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-21', 21, 9, 'role_name', 'Системный пользователь', 'Системный пользователь', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-22', 22, 9, 'lastname', 'Фамилия', 'Фамилия', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-23', 23, 9, 'firstname', 'Имя', 'Имя', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-24', 24, 9, 'sirname', 'Отчество', 'Отчество', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-25', 25, 9, 'fio', 'ФИО', 'ФИО', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-26', 26, 1, 'is_boss', 'Является руководителем', 'Руководитель', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-27', 27, 3, 'id_parent', 'Родитель', 'Входит в', NULL, 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-28', 28, 9, 'address', 'Адрес транспорта', 'Адрес транспорта', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-29', 29, 9, 'ref_column_name', 'Название ссылочной колонки', 'Ссылочная колонка', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-30', 30, 2, 'id_mimetype', 'MIME-тип', 'MIME-тип', 'mimetypes', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-31', 31, 2, 'id_unit', 'Подразделение', 'Подразделение', 'units', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-32', 32, 2, 'id_user', 'Пользователь', 'Пользователь', 'users', 'fio', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-33', 33, 1, 'is_io', 'Является исполняющим обязанности', 'ВРИО', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-34', 34, 2, 'id_dl_to', 'Кому представить', 'Кому представить доклад об исполнении', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-35', 35, 2, 'id_dl_executor', 'Ответственный исполнитель', 'Ответственный исполнитель', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-36', 36, 2, 'id_dl_controller', 'Контролер исполнения', 'Контролер исполнения', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-37', 37, 5, 'exec_datetime', 'Дата-время исполнения', 'Исполнить к', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-38', 38, 10, 'exec_period', 'Периодичность исполнения', 'Периодичность', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-39', 39, 13, 'osnovanie', 'Основание', 'Основание', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-40', 40, 1, 'is_archived', 'В архиве', 'В архиве', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-41', 41, 2, 'id_jr_state', 'Состояние пункта журнала', 'Состояние', 'jr_states', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-42', 42, 2, 'id_dl_from', 'Отправитель', 'Отправитель', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-43', 43, 2, 'id_io_type', 'Тип информационного объекта', 'Тип ИО', 'io_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-44', 44, 1, 'is_automated', 'Способ получения (автоматизированно/неавтоматизированно)', 'Получен автоматизированно', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-45', 45, 13, 'message_body', 'Тело сообщения', 'Тело сообщения', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-46', 46, 9, 'type', 'Тип', 'Тип', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-47', 47, 2, 'id_journal', 'Журнал учета распоряжений и команд', 'Журнал', 'command_journal', 'id', 50, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-48', 48, 2, 'id_io_object', 'Информационный объект (документ)', 'Документ', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-49', 49, 5, 'planned_exec', 'Запланированное время исполнения', 'Запланированное время исполнения', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-50', 50, 5, 'real_exec', 'Реальное время исполнения', 'Реальное время исполнения', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-51', 51, 2, 'id_dl_sender', 'Отправитель', 'Отправитель', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-52', 52, 2, 'id_dl_receiver', 'Получатель', 'Получатель', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-53', 53, 2, 'id_search_template_type', 'Тип поискового запроса', 'Тип поискового запроса', 'search_template_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-54', 54, 5, 'sent_datetime', 'Время отправки', 'Время отправки', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-55', 55, 5, 'receive_datetime', 'Время получения', 'Время получения', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-56', 56, 5, 'read_datetime', 'Время прочтения', 'Время прочтения', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-57', 57, 8, 'def_width', 'Визуальный размер колонки в таблице', 'Размер колонки', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-58', 58, 9, 'group_name', 'Системная группа', 'Системная группа', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-59', 59, 9, 'win_app', 'Приложение для запуска файла в среде Windows', 'Windows-приложение', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-60', 60, 9, 'lin_app', 'Приложение для запуска файла в среде МСВС', 'МСВС-приложение', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-61', 61, 12, 'extentions', 'Набор ассоциированных с типом файла расширений', 'Ассоциированные расширения файлов', 'url_extentions', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-62', 62, 1, 'with_inheritance', 'Наследовать права доступа', 'Наследовать права доступа', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-63', 63, 2, 'id_maclabel', 'Гриф секретности', 'Гриф секретности', 'maclabels', 'name', 50, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-64', 64, 16, 'mac_value', 'Мандатная метка', 'Мандатная метка', NULL, NULL, 50, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-65', 65, 2, 'id_state', 'Состояние', 'Состояние пользователя', 'user_state', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-66', 66, 9, 'email', 'Адрес эл. почты', 'Адрес электронной почты', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-67', 67, 9, 'acc_right_num', 'Номер допуска', 'Номер допуска', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-68', 68, 9, 'acc_right_date', 'Дата выдачи допуска', 'Дата выдачи допуска', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-1', 66, 1, 'is_connected', 'is_connected', 'is_connected', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-69', 69, 9, 'subtype', 'Подтип', 'Подтип', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-70', 70, 2, 'id_user_vrio', 'ВрИО', 'Пользователь ВрИО', 'users', 'fio', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-71', 71, 17, 'position_work_mode', 'Режимы ф-я должности', 'Режимы, при которых должность функционирует', 'work_mode', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-72', 72, 2, 'id_mode_type', 'Тип режима', 'Тип режима функционирования', 'work_mode_type', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-73', 73, 15, 'symbol', 'Условный знак', 'Условный знак режима', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-1', XX, XX, 'audio_file', 'Аудио-файл', 'Аудио-файл режима функционирования', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-74', 74, 2, 'id_organization', 'Организация', 'Организация', 'organization', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-75', 75, 2, 'id_curr_mode', 'Текущий режим', 'Текущий режим функционирования', 'work_mode', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-76', 76, 2, 'id_boss', 'Руководитель', 'Руководитель', 'position', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-77', 77, 9, 'code_name', 'Кодовое название', 'Кодовое название', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-78', 78, 15, 'tree_symbol', 'Условный знак', 'Условный знак', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-79', 79, 1, 'is_fgroup', 'Является функциональной группой', 'Является функциональной группой', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-80', 80, 5, 'time_elapsed', 'Конец периода существования функциональной группы', 'Конец периода существования ФГ', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-81', 81, 17, 'units_work_mode', 'Режимы ф-я подразделения', 'Режимы, при которых подразделение функционирует', 'work_mode', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-82', 82, 17, 'organization_work_mode', 'Режимы ф-я организации', 'Режимы, при которых организация функционирует', 'work_mode', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-83', 83, 2, 'id_type_org', 'Тип организации', 'Тип организации', 'organization_type', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-84', 84, 3, 'id_parent1', 'Входит в (на особый период)', 'Входит в (на особый период)', NULL, 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-85', 85, 3, 'id_substitutor', 'Исполняет обязанности при выходе из строя', 'Исполняет обязанности при выходе из строя', NULL, 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-86', 86, 2, 'id_prev_mode', 'Предыдущий режим', 'Предыдущий режим функционирования', 'work_mode', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-87', 87, 9, 'email_prefix', 'Префикс эл. почты', 'Префикс эл. почты', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-88', 88, 6, 'latitude', 'Широта', 'Широта', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-89', 89, 6, 'longitude', 'Долгота', 'Долгота', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-90', 90, 15, 'map_symbol', 'Условный знак на карте', 'Условный знак на карте', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-91', 91, 5, 'dt_prev_mode', 'Время перехода в предыдущий режим', 'Время перехода в предыдущий режим', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-92', 92, 5, 'dt_curr_mode', 'Время перехода в текущий режим', 'Время перехода в текущий режим', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-93', 93, 1, 'is_active', 'Активен', 'Транспорт активен', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-94', 94, 2, 'id_transport', 'Транспорт', 'Транспорт межобъектового обмена', 'transport', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-95', 95, 9, 'local_address', 'Локальный адрес', 'Локальный адрес транспорта', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-96', 96, 5, 'time_start', 'Начало периода существования функциональной группы', 'Начало периода существования ФГ', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-97', 97, 17, 'io_objects_organization', 'Синхронизируемые организации', 'Синхронизируемые организации', 'organization', 'name', 200, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-98', 98, 17, 'io_categories_organization', 'Синхронизируемые организации', 'Синхронизируемые организации', 'organization', 'name', 200, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-99', 99, 9, 'input_number', 'Входящий номер', 'Входящий номер', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-100', 100, 9, 'output_number', 'Исходящий номер', 'Исходящий номер', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-101', 101, 9, 'report_name', 'Название шаблона', 'Название шаблона отчета', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-102', 102, 9, 'report_descrip', 'Описание шаблона', 'Описание шаблона отчета', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-103', 103, 8, 'report_grade', 'Уровень шаблона', 'Уровень шаблона отчета', NULL, NULL, 70, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-104', 104, 2, 'shb', 'Шаблон отчета', 'Шаблон отчета', 'report', 'report_name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-105', 105, 9, 'p_value', 'Значение параметра', 'Значение параметра', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-106', 106, 17, 'report_organization', 'Синхронизируемые организации', 'Синхронизируемые организации', 'organization', 'name', 200, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-107', 107, 17, 'user_chains_organization', 'Синхронизируемые организации', 'Синхронизируемые организации', 'organization', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-108', 108, 8, 'trig_level', 'Триггерный уровень', 'Триггерный уровень', NULL, NULL, 80, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-109', 109, 9, 'proc_name', 'Хранимая проыедура БД', 'Сигнатура хранимой процедуры БД', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-110', 110, 2, 'id_search_template', 'Поисковый запрос', 'Применяемый фильтр', 'search_templates', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-111', 111, 1, 'is_global', 'Является глобальным', 'Глобальный', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-112', 112, 2, 'id_sync_type', 'Тип синхронизации', 'Тип синхронизации', 'io_sync_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-113', 113, 8, 'sync_result', 'Результат синхронизации', 'Результат синхронизации', NULL, NULL, 80, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-114', 114, 10, 'min_period', 'Минимальный период синхронизации', 'Минимальный период синхронизации', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-115', 115, 5, 'last_sync', 'Дата и время последней синхронизации', 'Дата и время последней синхронизации', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-116', 116, 5, 'sync_datetime', 'Дата и время синхронизации', 'Дата и время синхронизации', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-117', 117, 2, 'id_owner_org', 'Организация-владелец', 'Организация-владелец', 'organization', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-118', 118, 1, 'is_public', 'Является публичным', 'Является публичным', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-119', 119, 9, 'phone', 'Служебный телефон', 'Служебный телефон', NULL, NULL, 150, TRUE);

--обмен информацией с внешними системами (например, ИК ТСО)
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-120', 120, 2, 'id_xml_query_type', 'Тип XML-запроса к внешней системе', 'Тип XML-запроса', 'xml_query_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-121', 121, 9, 'xpath', 'Путь к тэгу в дереве XML-ответа', 'Путь к тэгу в дереве XML-ответа', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-122', 122, 9, 'xcode', 'Код (название) XML-тэга', 'Код (название) XML-тэга', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-123', 123, 9, 'ref_table', 'Таблица БД, содержащая записи, описываемые XML-тэгом', 'Таблица БД, содержащая записи, описываемые XML-тэгом', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-124', 124, 2, 'id_xml_query_kind', 'Вид XML-запроса к внешней системе', 'Вид XML-запроса', 'xml_query_kinds', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-125', 125, 20, 'xml_data', 'XML-документ', 'XML-документ', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-126', 126, 21, 'exec_period_h', 'Периодичность выполнения', 'Периодичность выполнения', NULL, NULL, 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-127', 127, 2, 'id_xml_query', 'XML-запрос к внешней системе', 'XML-запрос', 'xml_queries', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-128', 128, 1, 'is_mandatory', 'Является обязательным', 'Является обязательным', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-129', 129, 5, 'exec_time', 'Дата-время выполнения запроса', 'Дата-время выполнения запроса', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-130', 130, 9, 'data_type', 'Тип данных', 'Тип данных', NULL, NULL, 120, TRUE);

--ИК ТСО
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-131', 131, 5, 'guard_datetime', 'Поставлен под охрану', 'Поставлен под охрану', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-132', 132, 8, 'fault_code', 'Код ошибки', 'Код ошибки', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-133', 133, 9, 'fault_string', 'Описание ошибки', 'Описание ошибки', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-134', 134, 5, 'reg_datetime', 'Неисправность зафиксирована', 'Неисправность зафиксирована', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-135', 135, 9, 'fault_state_name', 'Описание состояния устройства', 'Описание состояния устройства', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-136', 136, 9, 'fault_state_value', 'Состояние устройства', 'Состояние устройства', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-137', 137, 2, 'id_device', 'Устройство', 'Устройство', 'devices', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-138', 138, 17, 'guard_objects_devices', 'Устройства, входящие в объект охраны', 'Устройства, входящие в объект охраны', 'devices', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-139', 139, 2, 'id_person', 'Должностное лицо', 'Должностное лицо', 'persons', 'lastname', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-140', 140, 5, 'event_date', 'Событие произошло', 'Событие произошло', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-141', 141, 9, 'message', 'Описание события', 'Описание события', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-142', 142, 9, 'source', 'Источник события', 'Источник события', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-143', 143, 8, 'alarm', 'Уровень тревоги события', 'Уровень тревоги события', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-144', 144, 9, 'card_code', 'Код карты доступа', 'Код карты доступа', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-145', 145, 1, 'pass', 'Факт прохода', 'Факт прохода', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-146', 146, 9, 'direction', 'Направление прохода', 'Направление прохода', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-147', 147, 24, 'video', 'Видео-ролик', 'Видео-ролик', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-148', 148, 2, 'id_card_type', 'Тип карты доступа', 'Тип карты доступа', 'card_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-149', 149, 2, 'id_card_status', 'Статус карты доступа', 'Статус карты доступа', 'card_statuses', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-150', 150, 2, 'id_access_plan', 'План доступа', 'План доступа', 'access_plan', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-151', 151, 2, 'id_tso_position', 'Должность', 'Должность', 'tso_positions', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-152', 152, 2, 'id_tso_unit', 'Подразделение', 'Подразделение', 'tso_units', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-153', 153, 9, 'comment', 'Комментарий', 'Комментарий', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-154', 154, 9, 'bg_image', 'Графическая подложка', 'Графическая подложка', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-155', 155, 23, 'svg_image', 'Изображение SVG', 'Изображение SVG', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-156', 156, 2, 'id_graph_plan', 'Граф. план', 'Граф. план', 'graph_plans', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-157', 157, 2, 'id_shape_type', 'Тип фигуры', 'Тип фигуры', 'shape_types', 'name', 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-158', 158, 2, 'id_element', 'Устройство', 'Устройство', 'graph_plan_elements', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-159', 159, 2, 'id_graph_style', 'Стиль отображения', 'Стиль отображения', 'graph_styles', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-160', 160, 2, 'id_segment_type', 'Тип сегмента', 'Тип сегмента', 'segment_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-161', 161, 2, 'id_element_shape', 'Фигура', 'Фигура', 'element_shapes', 'id', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-162', 162, 22, 'points', 'Координаты', 'Координаты', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-163', 163, 8, 'p_order', 'порядковый номер координаты', 'Порядковый номер координаты', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-164', 164, 15, 'urgency_symbol', 'Условный знак уровня срочности', 'Условный знак уровня срочности', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-165', 165, 2, 'id_urgency_level', 'Уровень срочности', 'Уровень срочности', 'urgency_levels', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-166', 166, 17, 'mail_lists_position', 'Перечень должностных лиц в списке рассылки', 'Перечень должностных лиц в списке рассылки', 'position', 'name', 200, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-167', 167, 2, 'id_io_object_att', 'Образец исполнения', 'Образец исполнения', 'io_objects', 'name', 150, TRUE);

--ИК ТСО
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-168', 168, 8, 'id_person_p', 'Должностное лицо (ИД)', 'Должностное лицо (ИД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-169', 169, 8, 'id_device_p', 'Устройство (ИД)', 'Устройство (ИД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-170', 170, 8, 'id_unit_p', 'Подразделение (ИД)', 'Подразделение (ИД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-171', 171, 9, 'unit_name', 'Подразделение', 'Подразделение', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-172', 172, 8, 'id_dolzh_p', 'Должность (ИД)', 'Должность (ИД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-173', 173, 9, 'dolzh_name', 'Должность', 'Должность', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-174', 174, 9, 'dev_name', 'Устройство', 'Устройство', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-175', 175, 8, 'id_guard_obj_p', 'Охраняемый объект (ИД)', 'Охраняемый объект (ИД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-176', 176, 9, 'guard_obj_name', 'Охраняемый объект', 'Охраняемый объект', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-177', 177, 23, 'device_svg', 'Граф-план устройства', 'Граф. план устройства', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-178', 178, 23, 'plan_svg', 'Граф-план', 'Граф. план', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-179', 179, 1, 'is_ok', 'Устройство исправно', 'Устройство исправно', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-180', 180, 5, 'ok_datetime', 'Работоспособность восстановлена (нарушена)', 'Работоспособность восстановлена (нарушена)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-181', 181, 12, 'access_cards', 'Карты доступа', 'Карты доступа', 'access_cards_q', 'code', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-182', 182, 8, 'id_card_type_p', 'Тип карты (ИД)', 'Тип карты (ИД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-183', 183, 9, 'card_type_name', 'Тип карты', 'Тип карты', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-184', 184, 8, 'id_card_status_p', 'Статус карты (ИД)', 'Статус карты (ИД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-185', 185, 9, 'card_status_name', 'Статус карты', 'Статус карты', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-186', 186, 12, 'access_plans', 'Планы доступа', 'Планы доступа', 'access_plan', 'name', 100, TRUE);

--УЧЕТНЫЕ КАРТОЧКИ ДОКУМЕНТОВ
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-187', 187, 9, 'r_name', 'Название (мн.ч.)', 'Название (мн.ч.)', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-188', 188, 5, 'accepted_datetime', 'Дата-время принятия к исполнению', 'Принято к исполнению', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-189', 189, 2, 'id_document_type', 'Тип документа', 'Тип документа', 'document_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-190', 190, 2, 'id_document_subtype', 'Подтип документа', 'Подтип документа', 'document_subtypes', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-191', 191, 2, 'id_exec_org', 'Организация-исполнитель', 'Организация-исполнитель', 'organization', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-192', 192, 2, 'id_document_kind', 'Вид документа', 'Вид документа', 'document_kinds', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-193', 193, 5, 'regout_datetime', 'Дата-время регистрации (исх.)', 'Дата-время регистрации (исх.)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-194', 194, 5, 'regin_datetime', 'Дата-время регистрации (вх.)', 'Дата-время регистрации (вх.)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-195', 195, 9, 'doc_header', 'Заголовок', 'Заголовок', NULL, NULL, 300, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-196', 196, 8, 'page_count', 'Кол-во листов', 'Кол-во листов', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-197', 197, 9, 'ekz_number', 'Экземпляр', 'Экземпляр', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-198', 198, 9, 'book_number', 'Дело', 'Дело', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-199', 199, 9, 'book_subnumber', 'Раздел в деле', 'Раздел в деле', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-200', 200, 9, 'class_code', 'Классификационный код', 'Классификационный код', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-201', 201, 9, 'inv_number', 'Инвентарный номер', 'Инвентарный номер', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-202', 202, 9, 'registrator_info', 'Зарегистрировал', 'Зарегистрировал', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-203', 203, 13, 'annotation', 'Аннотация', 'Аннотация', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-204', 204, 13, 'command_body', 'Поручение', 'Поручение', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-205', 205, 5, 'create_datetime', 'Дата-время создания', 'Дата-время создания', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-206', 206, 18, 'record_fill_color', 'Цвет фона при отображении в таблице', 'Цвет фона при отображении в таблице', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-207', 207, 25, 'record_text_color', 'Цвет текста при отображении в таблице', 'Цвет текста при отображении в таблице', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-208', 208, 9, 'org_name', 'Наименование части', 'Наименование части', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-209', 209, 9, 'org_uid', 'UID части', 'UID части', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-210', 210, 8, 'tso_max_level', 'Максимальный уровень тревоги', 'Максимальный уровень тревоги', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-211', 211, 9, 'tso_dl_from', 'UID должностного лица, отправителя распоряжений', 'UID должностного лица, отправителя распоряжений', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-212', 212, 9, 'tso_dl_exec', 'UID должностного лица, исполнителя распоряжений', 'UID должностного лица, исполнителя распоряжений', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-213', 213, 9, 'tso_dl_to', 'UID должностного лица, контроллера распоряжений', 'UID должностного лица, контроллера распоряжений', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-214', 214, 9, 'tso_category', 'UID категории доклада об устранении', 'UID категории доклада об устранении', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-215', 215, 21, 'tso_exectime', 'Время доклада об устранении', 'Время доклада об устранении', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-216', 216, 9, 'ref_table_name', 'Название базовой таблицы', 'Название базовой таблицы', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-217', 217, 9, 'video_url', 'Ссылка на видео-ролик', 'Ссылка на видео-ролик', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-218', 218, 1, 'for_report', 'Признак помещения в итоговый отчет', 'Признак помещения в итоговый отчет', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-219', 219, 9, 'tso_state', 'Состояние ТСО', 'Состояние ТСО', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-220', 220, 18, 'tso_state_color', 'Состояние ТСО (цвет)', 'Состояние ТСО (цвет)', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-221', 221, 1, 'is_deleted', 'Доклад об исполнении получен', 'Доклад об исполнении получен', NULL, NULL, 150, TRUE);

--
--ZARYA22
--
/*
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-222', 222, 9, 'type', 'Тип', 'Тип', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-223', 223, 9, 'value', 'Значение', 'Значение', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-224', 224, 9, 'uri', 'URI', 'URI', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-225', 225, 2, 'id_acs', 'АСУ', 'АСУ', 'shu_acs', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-226', 226, 2, 'id_org', 'Организационная единица', 'Организационная единица', 'shu_orgs', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-227', 227, 2, 'id_pos', 'Должностная единица', 'Должностная единица', 'shu_positions', 'name', 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-228', 228, 2, 'id_shu_dls', 'Должностное лицо', 'Должностное лицо', 'shu_dls', 'name', 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-229', 229, 2, 'id_shu_rank', 'Воинское звание', 'Воинское звание', 'shu_ranks', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-230', 230, 2, 'id_shu_domain', 'Предметная область', 'Предметная область', 'shu_domains', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-231', 231, 9, 'shu_working_mode', 'Режим функционирования', 'Режим функционирования', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-232', 232, 9, 'shu_msg_type', 'Вид сообщения', 'Вид сообщения', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-233', 233, 9, 'shu_creation_date', 'Дата создания', 'Дата создания', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-234', 234, 9, 'shu_creation_time', 'Время создания ', 'Время создания', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-235', 235, 9, 'shu_maclabel', 'Уровень доступа', 'Уровень доступа', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-236', 236, 9, 'shu_priority', 'Приоритет', 'Приоритет', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-237', 237, 13, 'shu_text', 'Текст сообщения', 'Текст сообщения', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-238', 238, 9, 'shu_cmd_code', 'Код команды', 'Код команды', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-239', 239, 13, 'shu_cmd_text', 'Текст команды', 'Текст команды', NULL, NULL, 150, TRUE);

--заголовок
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-258', 258, 9, 'shu_echelon', 'Эшелон', 'Эшелон', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-259', 259, 9, 'shu_cdomain', 'Домен предметной области', 'Домен предметной области', NULL, NULL, 150, TRUE);

--Чрезвычайная ситуация
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-240', 240, 9, 'shu_emerg_type', 'Тип чрезвычайной ситуации', 'Тип чрезвычайной ситуации', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-241', 241, 5, 'shu_emerg_datetime', 'Дата и время возникновения ЧС', 'Дата и время возникновения ЧС', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-244', 244, 9, 'shu_district', 'Военный округ', 'Военный округ', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-245', 245, 9, 'shu_region', 'Область', 'Область', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-246', 246, 9, 'shu_area', 'Район', 'Район', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-260', 260, 9, 'shu_town', 'Город', 'Город', NULL, NULL, 150, TRUE);

--Документ о радиоактивном заражении
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-261', 261, 9, 'shu_document_entry', 'Описание документа', 'Описание документа', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-242', 242, 9, 'shu_obj_open_name', 'Полное открытое наименование объекта', 'Полное открытое наименование объекта', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-243', 243, 9, 'shu_obj_close_name', 'Полное закрытое наименование объекта', 'Полное закрытое наименование объекта', NULL, NULL, 150, TRUE);

--Принадлежность
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-262', 262, 9, 'shu_block', 'Политический/военный блок', 'Политический/военный блок', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-263', 263, 9, 'shu_country', 'Страна', 'Страна', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-264', 264, 9, 'shu_department', 'Ведомство', 'Ведомство', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-265', 265, 9, 'shu_company', 'Фирма', 'Фирма', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-266', 266, 9, 'shu_block', 'Политический/военный блок', 'Политический/военный блок', NULL, NULL, 150, TRUE);

--Ситуационная схема ЧС
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-257', 257, 15, 'shu_emerg_pict', 'Ситуационная схема ЧС', 'Ситуационная схема ЧС', NULL, NULL, 150, TRUE);

--Радиационное загрязнение
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-267', 267, 9, 'shu_authenticity', 'Достоверность', 'Достоверность', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-268', 268, 5, 'shu_instrumentation_event_time', 'Время съема информации', 'Время съема информации', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-269', 269, 9, 'shu_authenticity', 'Достоверность', 'Достоверность', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-270', 270, 9, 'shu_cdomain1', 'Домен предметной области', 'Домен предметной области', NULL, NULL, 150, TRUE);

--краткое описание ЧС
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-249', 249, 9, 'shu_emerg_desc', 'Краткое описание чрезвычайной ситуации', 'Краткое описание чрезвычайной ситуации', NULL, NULL, 150, TRUE);

--
--радиационное загрязнение
--координаты
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-271', 271, 9, 'shu_height_uom', 'Единицы измерения высоты', 'Единицы измерения высоты', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-247', 247, 9, 'shu_emerg_lat', 'Координаты ЧС (широта)', 'Координаты ЧС (широта)', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-248', 248, 9, 'shu_emerg_lon', 'Координаты ЧС (долгота)', 'Координаты ЧС (долгота)', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-272', 272, 9, 'shu_emerg_height', 'Координаты ЧС (высота)', 'Координаты ЧС (высота)', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-273', 273, 9, 'shu_coord_uom', 'Единицы измерения координат', 'Единицы измерения координат', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-276', 276, 9, 'shu_cdomain2', 'Домен предметной области', 'Домен предметной области', NULL, NULL, 150, TRUE);

--состояние жизненного цикла
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-274', 274, 9, 'shu_life_cycle_type', 'Тип жизненного цикла', 'Тип жизненного цикла', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-275', 275, 9, 'shu_life_cycle_indicators', 'Показатели', 'Показатели', NULL, NULL, 150, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-276', 276, 9, 'shu_life_cycle_type', 'Тип жизненного цикла', 'Тип жизненного цикла', NULL, NULL, 150, TRUE);

--радиусы зон
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-251', 251, 9, 'shu_emerg_dang_area', 'Радиус опасной зоны', 'Радиус опасной зоны', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-277', 277, 9, 'shu_emerg_dang_area_uom', 'Единицы измерения радиуса', 'Единицы измерения радиуса', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-253', 253, 9, 'shu_emerg_dang_area1', 'Радиус зоны ограниченной опасности', 'Радиус зоны ограниченной опасности', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-278', 278, 9, 'shu_emerg_dang_area1_uom', 'Единицы измерения радиуса', 'Единицы измерения радиуса', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-283', 283, 9, 'shu_emerg_safe_area', 'Радиус безопасной зоны', 'Радиус безопасной зоны', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-279', 279, 9, 'shu_emerg_safe_area_uom', 'Единицы измерения радиуса', 'Единицы измерения радиуса', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-250', 250, 9, 'shu_emerg_rad_level', 'Уровень радиации вблизи источника', 'Уровень радиации вблизи источника', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-280', 280, 9, 'shu_emerg_rad_level_uom', 'Единицы измерения', 'Единицы измерения', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-252', 252, 9, 'shu_emerg_dang_area_rad_level', 'Уровень радиации в опасной зоне', 'Уровень радиации в опасной зоне', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-281', 281, 9, 'shu_emerg_dang_area_rad_level_uom', 'Единицы измерения', 'Единицы измерения', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-254', 254, 9, 'shu_emerg_dang_area_rad_level1', 'Уровень радиации в зоне ограниченной опасности', 'Уровень радиации в зоне ограниченной опасности', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-282', 282, 9, 'shu_emerg_dang_area_rad_level1_uom', 'Единицы измерения', 'Единицы измерения', NULL, NULL, 150, TRUE);

--приложение
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-256', 256, 9, 'shu_emerg_evac_act', 'Проведенные мероприятия по эвакуации', 'Проведенные мероприятия по эвакуации', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-255', 255, 9, 'shu_emerg_lic_act', 'Проведенные мероприятия по устранению последствий ЧС', 'Проведенные мероприятия по устранению последствий ЧС', NULL, NULL, 150, TRUE);
*/

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-284', 284, 2, 'id_attr_group', 'Группа', 'Группа', 'attrs_groups', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-285', 285, 2, 'id_position_type', 'Тип ДЛ', 'Тип ДЛ', 'position_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-286', 286, 9, 'the_value', 'Значение', 'Значение', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-291', 291, 2, 'id_indicator_type', 'Тип показателя', 'Тип показателя', 'indicator_type', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-292', 292, 2, 'id_io_object', 'Объект', 'Объект', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-293', 293, 2, 'id_io_object_src', 'Информацию передал', 'Информацию передал', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-294', 294, 2, 'id_io_object_src1', 'За кого передал', 'За кого передал', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-295', 295, 5, 'meas_time', 'Время измерения', 'Время измерения', NULL, NULL, 120, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-295', 295, 5, 'meas_time', 'Время измерения', 'Время измерения', NULL, NULL, 120, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-296', 296, 9, 'value', 'Значение', 'Значение', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-297', 297, 5, 'start_time', 'Актуален с', 'Актуален с', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-298', 298, 5, 'stop_time', 'Актуален по', 'Актуален по', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-299', 299, 2, 'id_indicator', 'Индикатор', 'Индикатор', 'indicator', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-300', 300, 2, 'id_attr_category', 'Атрибут в категории', 'Атрибут в категории', 'attrs_categories', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-301', 301, 9, 'value', 'Значение атрибута', 'Значение атрибута', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-302', 302, 1, 'is_actual', 'Значение атрибута актуально', 'Значение атрибута актуально', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-303', 303, 2, 'id_io_attribute', 'Атрибут', 'Атрибут', 'attributes', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-304', 304, 9, 'def_value', 'Значение по умолчанию', 'Значение по умолчанию', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-305', 305, 1, 'is_read_only', 'Значение атрибута не редактируемо', 'Значение атрибута не редактируемо', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-306', 306, 2, 'id_attr_parent', 'Описываемый атрибут', 'Описываемый атрибут', 'attributes', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-307', 307, 2, 'id_attr_child', 'Описывающий атрибут', 'Описывающий атрибут', 'attributes', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-308', 308, 2, 'id_attr_attr', 'Атрибут', 'Атрибут', 'attrs_attrs', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-309', 309, 2, 'id_attr_value', 'Значение атрибута', 'Значение атрибута', 'attrs_values', 'value', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-310', 310, 1, 'is_qualifier', 'Является справочником', 'Является справочником', NULL, NULL, 50, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-311', 311, 3, 'id_child2', 'Подчиненная категория показателей', 'Подчиненная категория показателей', 'io_categories', 'name', 300, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-312', 312, 15, 'r_icon', 'Условный знак элемента', 'Условный знак элемента', NULL, NULL, 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-313', 313, 5, 'start_time', 'Начало потока', 'Начало потока', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-314', 314, 5, 'stop_time', 'Окончание потока', 'Окончание потока', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-315', 315, 2, 'id_partition_low', 'Закон распределения', 'Закон распределения', 'partition_lows', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-316', 316, 6, 'lambda', 'Лямбда', 'Лямбда', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-317', 317, 6, 'sigma', 'Сигма', 'Сигма', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-318', 318, 6, 'moda', 'Мода', 'Мода', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-319', 319, 6, 'min_p', 'Мин.', 'Мин.', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-320', 320, 6, 'max_p', 'Макс.', 'Макс.', NULL, NULL, 120, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-321', 321, 2, 'id_start_state', 'Начальное состояние', 'Начальное состояние', 'io_states', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-322', 322, 17, 'life_cycle_io_states', 'Перечень состояний жизненного цикла', 'Перечень состояний жизненного цикла', 'io_states', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-323', 323, 2, 'id_life_cycle', 'Жизненный цикл', 'Жизненный цикл', 'life_cycle', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-324', 324, 2, 'id_state_src', 'Исходное состояние', 'Исходное состояние', 'io_states', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-325', 325, 2, 'id_state_dest', 'Следующее состояние', 'Следующее состояние', 'io_states', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-326', 326, 9, 'service', 'Параметры (сигнатура) сервиса', 'Параметры (сигнатура) сервиса', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-327', 327, 9, 'extra_params', 'Доп. параметры для запуска', 'Доп. параметры для запуска', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-328', 328, 2, 'id_handler', 'Сервис-обработчик', 'Сервис-обработчик', 'handlers', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-329', 329, 2, 'id_chain', 'Очередь обработки', 'Очередь обработки', 'chains', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-330', 330, 30, 'id_record', 'Идентификатор записи справочника', 'Идентификатор записи справочника', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-331', 331, 8, 'is_handled', 'Признак обработки', 'Признак обработки', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-332', 332, 5, 'handled_time', 'Момент обработки записи триггером', 'Момент обработки записи триггером', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-333', 333, 5, 'start_service_time', 'Начало обработки записи сервисом', 'Начало обработки записи сервисом', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-334', 334, 5, 'end_service_time', 'Конец обработки записи сервисом', 'Конец обработки записи сервисом', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-335', 335, 8, 'return_code', 'Код возврата сервиса', 'Код возврата сервиса', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-336', 336, 1, 'is_external', 'Признак внешнего запуска', 'Признак внешнего запуска', NULL, NULL, 100, TRUE);



SELECT pg_catalog.setval('attributes_id_seq', 1000, true); --все пользовательские атрибуты будут начинаться с номера 1001
                                                          --это сделано для того, чтобы оставить резерв для системных атрибутов

/*
Поскольку все триггеры, которые заданы для таблиц атрибутов, категорий и информационных объектов
создаются только после отработки данного скрипта, то нам необходимо здесь создать один триггер, 
который бы выполнял одно только действие - задавал автоматически поле name таблицы attrs_categories
Потом мы этот триггер удалим, а вместо него в конце концов создастся штатный
*/

create or replace function acInsertCheck() returns trigger as 
$BODY$
declare
    aName varchar;
begin
    select name into aName from attributes where id = new.id_io_attribute;
    
    new.name := aName;

    return new;
end
$BODY$ 
language 'plpgsql';


select f_create_trigger('trgacinsert', 'before', 'insert or update', 'attrs_categories', 'acinsertcheck()');

                                    --связывание атрибутов с категориями
--таблица состояний ИО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (1, 1, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (2, 1, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (3, 1, 3, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (581, 1, 15, 'false', true, false);

--таблица типов категорий ИО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (4, 3, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (5, 3, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (6, 3, 3, NULL, false, false);--desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (7, 3, 187, NULL, true, false);--r_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (530, 3, 310, 'FALSE', true, false);--is_qualifier


--таблица представлений типов атрибутов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (8, 5, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (9, 5, 2, NULL, true, false);

--таблица типов атрибутов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (10, 7, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (11, 7, 4, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (12, 7, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, 7, 6, NULL, true, false);

--таблица групп атрибутов (пользовательских)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (14, 171, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (15, 171, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (16, 171, 27, NULL, false, false); --id_parent

--таблица атрибутов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (17, 9, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (18, 9, 5, NULL, true, false); --id_a_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (19, 9, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (20, 9, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (21, 9, 7, NULL, true, false);  --title
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (22, 9, 8, NULL, false, false); --table_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (23, 9, 9, NULL, false, false); --column_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (24, 9, 57, '100', true, false);--def_width
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (25, 9, 15, 'false', true, false);--is_system
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (26, 9, 110, NULL, false, false);--id_search_template
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (27, 9, 284, 2, true, false);--id_attr_group
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (28, 9, 29, NULL, false, false);--ref_column_name

--таблица категорий
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (29, 11, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (30, 11, 10, NULL, true, false); --id_io_category_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (31, 11, 11, NULL, false, false);--id_child
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (531, 11, 311, NULL, false, false);--id_child2
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (32, 11, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (33, 11, 3, NULL, false, false); --description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (34, 11, 6, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (35, 11, 17, 'true', true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (36, 11, 18, '1', true, false);--id_io_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (582, 11, 323, NULL, false, false);--id_life_cycle
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (37, 11, 111, 'false', true, false);--is_global
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (532, 11, 40, 'false', true, true);--is_archived (редактировать нельзя)


--таблица информационных объектов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (38, 13, 1, '-1', true, true);--id !!! -1 оставлять обязательно !!!
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (39, 13, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (40, 13, 3, NULL, false, false);-- desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (41, 13, 43, '1', true, false);--id_io_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (42, 13, 12, NULL, true, true); --id_io_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (43, 13, 13, NULL, false, true); --author
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (44, 13, 8, NULL, false, false); --table_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (45, 13, 14, NULL, false, true);--information
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (46, 13, 15, 'false', true, true);--is_system
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (47, 13, 16, 'current_timestamp', true, true);--insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (48, 13, 18, 1, true, false); --id_io_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (49, 13, 63, NULL, true, true); --id_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (50, 13, 97, NULL, false, false);--io_objects_organization (separate table)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (51, 13, 112, '1', true, false);--id_sync_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (52, 13, 117, NULL, true, true);--id_owner_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (53, 13, 111, 'false', true, false);--is_global
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (54, 13, 206, NULL, false, false);--record_fill_color
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (55, 13, 207, NULL, false, false);--record_text_color
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (56, 13, 110, NULL, false, false);--id_search_template
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (57, 13, 216, NULL, false, false);--ref_table_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (534, 13, 312, NULL, false, false);--r_icon




--таблица шаблонов информационных объектов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (58, 15, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (59, 15, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (60, 15, 3, NULL, false, false);  --description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (61, 15, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (62, 15, 12, NULL, true, false); --id_io_category

--таблица типов информационных объектов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (63, 17, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (64, 17, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (65, 17, 3, NULL, false, false);  --description

--таблица типов поисковых запросов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (66, 19, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (67, 19, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (68, 19, 27, NULL, false, false); --id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (69, 19, 3, NULL, false, false);  --description

--таблица ТСД
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (70, 29, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (71, 29, 12, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (72, 29, 34, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (73, 29, 35, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (74, 29, 36, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (75, 29, 16, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (76, 29, 37, NULL, true, false);--exec_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (77, 29, 38, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (78, 29, 39, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (79, 29, 40, 'false', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (80, 29, 41, '1', true, false);



--таблица состояний пунктов табелей и журналов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (81, 31, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (82, 31, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (83, 31, 3, NULL, false, false);

--таблица журнала входящих и исходящих распоряжений
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (84, 33, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (85, 33, 12, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (86, 33, 99, NULL, true, true); --input_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (87, 33, 100, NULL, true, true); --output_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (88, 33, 34, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (89, 33, 35, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (90, 33, 42, NULL, true, false);--id_dl_from
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (91, 33, 16, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (92, 33, 37, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (93, 33, 38, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (94, 33, 40, 'false', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (95, 33, 41, '1', true, false);
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (96, 33, 43, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (97, 33, 44, 'true', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (98, 33, 45, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (99, 33, 165, '1', true, false);--id_urgency_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (100, 33, 167, NULL, false, false);--id_io_object_att
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (101, 33, 55, NULL, false, false);--receive_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (102, 33, 188, NULL, false, false);--accepted_datetime
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (103, 33, 46, NULL, false, true);

--таблица журнала контроля исполнения табелей и журналов команд
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (104, 37, 1, NULL, true, true);
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (105, 37, 46, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (106, 37, 47, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (107, 37, 48, NULL, false, false);--id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (108, 37, 16, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (109, 37, 49, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (110, 37, 50, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (111, 37, 40, 'false', true, false);

--таблица журнала входящих и исходящих сообщений
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (112, 39, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 39, 51, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (114, 39, 99, NULL, true, true); --input_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (115, 39, 100, NULL, true, true); --output_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (116, 39, 52, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (117, 39, 48, NULL, false, false);
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (118, 39, 53, '1', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (119, 39, 54, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (120, 39, 55, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (121, 39, 56, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (122, 39, 45, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (123, 39, 16, 'current_timestamp', true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (124, 39, 165, '1', true, false); --id_urgency_level

--таблица справочника типов прикрепляемых файлов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (125, 41, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (126, 41, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (127, 41, 59, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (128, 41, 60, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (129, 41, 61, NULL, true, false);

--таблица справочника известных расширений прикрепляемых файлов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (130, 43, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (131, 43, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (132, 43, 30, '1', true, false);  --id_mimetype

--таблица грифов секретности
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 45, 1, NULL, true, true);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (134, 45, 2, NULL, true, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 45, 64, NULL, true, false);


----------------------------
----------------------------
-----новая ОШС--------------

--таблица воинских званий
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (136, 47, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 47, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (138, 47, 6, NULL, false, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 47, 19, NULL, true, false); --short_name

--таблица пользователей
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (140, 49, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 49, 20, NULL, true, false);--id_rank
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (142, 49, 21, NULL, true, false);--role_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (143, 49, 65, NULL, true, false);--id_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (144, 49, 22, NULL, true, false);--lastname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (145, 49, 23, NULL, true, false);--firstname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (146, 49, 24, NULL, true, false);--sirname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (147, 49, 25, NULL, true, false);--fio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 49, 16, 'current_timestamp', true, true);--insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (149, 49, 62, 'true', true, false);--with_inheritance
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (150, 49, 66, NULL, false, false);--email
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 49, 63, NULL, false, false);--id_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 49, 67, NULL, false, false);--acc_right_num
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (153, 49, 68, NULL, false, false);--acc_right_date

--таблица типов должностей
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 173, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (155, 173, 2, NULL, true, false); --name

--таблица должностей (как клеток ОШС)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 51, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (157, 51, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 51, 19, NULL, true, false);--short_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (159, 51, 31, NULL, true, false);--id_unit
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 51, 32, NULL, false, false);--id_user
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (161, 51, 70, NULL, false, false);--id_user_vrio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 51, 33, 'false', true, false);--is_io
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (163, 51, 63, NULL, true, false);--id_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 51, 62, 'true', true, false);--with_inheritance
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (165, 51, 71, NULL, false, false);--position_work_mode (separate table)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 51, 87, NULL, true, false);--email_prefix
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (167, 51, 119, NULL, false, false);--phone
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 51, 118, 'false', true, false);--is_public
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 51, 285, 1, true, false);--id_position_type


--таблица организаций
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 53, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (171, 53, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (172, 53, 83, '1', true, false);--id_type_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (173, 53, 27, NULL, false, false);--id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (174, 53, 84, NULL, false, false);--id_parent1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (175, 53, 85, NULL, false, false);--id_substitutor
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (176, 53, 75, '1', true, false);--id_curr_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (177, 53, 86, '1', true, false);--id_prev_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (178, 53, 76, NULL, false, false);--id_boss
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 53, 19, NULL, true, false);--short_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (180, 53, 77, NULL, true, false);--code_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 53, 87, NULL, true, false);--email_prefix
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (182, 53, 88, NULL, true, false);--latitude
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (183, 53, 89, NULL, true, false);--longitude
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (184, 53, 90, NULL, true, false);--map_symbol (потом эта и следующая запись меняют дефолтное значение)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (185, 53, 78, NULL, true, false);--tree_symbol (см. файл org_symbol.sql)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (186, 53, 91, NULL, true, false);--dt_prev_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (187, 53, 92, NULL, true, false);--dt_curr_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (188, 53, 82, NULL, false, false);--organization_work_mode (separate table)

--таблица подразделений
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (189, 55, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (190, 55, 74, NULL, false, false);--id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (191, 55, 27, NULL, false, false);--id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (192, 55, 75, '1', true, false);--id_curr_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (193, 55, 76, NULL, false, false);--id_boss
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (194, 55, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (195, 55, 19, NULL, true, false);--short_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (196, 55, 77, NULL, true, false);--code_name
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (197, 55, 28, NULL, true, false);--address
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (198, 55, 78, NULL, false, false);--tree_symbol
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (199, 55, 79, 'false', true, false);--is_fgroup
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (200, 55, 96, NULL, false, false);--time_start
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (201, 55, 80, NULL, false, false);--time_elapsed
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (202, 55, 62, 'true', true, false);--with_inheritance
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (203, 55, 81, NULL, false, false);--units_work_mode

--таблица режимов функционирования
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (204, 59, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (205, 59, 72, NULL, true, false); --id_mode_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (206, 59, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (207, 59, 3, NULL, false, false);--description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (208, 59, 73, NULL, false, false);--symbol
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (209, 59, 74, NULL, false, false);--audio_file

--таблица состояний пользователей
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (210, 61, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (211, 61, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (212, 61, 19, NULL, true, false);--short_name

--таблица типов режимов функционирования
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (213, 63, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (214, 63, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (215, 63, 19, NULL, true, false);--short_name

--таблица типов организаций
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (216, 65, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (217, 65, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (218, 65, 19, NULL, true, false); --short_name

--таблица транспортов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (219, 67, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (220, 67, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (221, 67, 95, NULL, true, false); --local_address
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (222, 67, 93, 'true', true, false); --is_active

--таблица адресов организаций
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (223, 69, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (224, 69, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (225, 69, 94, NULL, true, false); --id_transport
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (226, 69, 28, NULL, true, false); --address
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (227, 69, 93, 'true', true, false); --is_active

--таблица шаблонов отчетов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (228, 71, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (229, 71, 101, NULL, true, false); --report_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (230, 71, 102, NULL, false, false); --report_descrip
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (231, 71, 103, NULL, true, false); --report_grade
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (232, 71, 106, NULL, false, false);--report_organization (separate table)

--таблица поисковых запросов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (233, 73, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (234, 73, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (235, 73, 53, '1', true, false); --id_search_template_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (236, 73, 13, NULL, true, false); --author
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (237, 73, 3, NULL, false, false); --desc

--таблица пользовательских обработчиков
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (238, 75, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (239, 75, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (240, 75, 3, NULL, false, false);--desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (241, 75, 108, '1', true, false);--trig_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (242, 75, 109, NULL, true, false);--proc_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (243, 75, 107, NULL, false, false);--user_chains_organization (separate table)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (244, 75, 110, NULL, false, false);--id_search_template

--общесистемный рубрикатор
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (245, 77, 1, NULL, true, true);--id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (246, 77, 2, NULL, true, false);--name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (247, 77, 3, NULL, false, false);--desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (248, 77, 6, NULL, false, false);--code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (249, 77, 27, NULL, false, false);--id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (250, 77, 48, NULL, false, true);--id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (251, 77, 110, NULL, false, false);--id_search_template
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (252, 77, 12, NULL, false, false);--id_io_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (533, 77, 312, NULL, false, false);--r_icon

--
--синхронизация информационных объектов
--

--таблица типов синхронизации ИО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (253, 79, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (254, 79, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (255, 79, 3, NULL, true, false); --description

--таблица архива синхронизации информационных объектов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (256, 81, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (257, 81, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (258, 81, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (259, 81, 116, NULL, true, false); --sync_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (260, 81, 113, NULL, true, false); --sync_result

--таблица параметров синхронизации информационных объектов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (261, 83, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (262, 83, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (263, 83, 74, NULL, true, false); --id_organization
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (264, 83, 114, NULL, true, false); --min_period
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (265, 83, 115, NULL, true, false); --last_sync

--
--Информационный обмен с внешними системами (к примеру, ИК ТСО)
--

--таблица типов XML-запросов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (266, 85, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (267, 85, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (268, 85, 6, NULL, true, false); --code

--таблица видов XML-запросов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (269, 89, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (270, 89, 2, NULL, true, false); --name

--таблица XML-тэгов (форматов)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (271, 87, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (272, 87, 120, NULL, true, false); --id_xml_query_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (273, 87, 121, NULL, true, false); --xpath
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (274, 87, 122, NULL, true, false); --xcode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (275, 87, 123, NULL, false, false); --ref_table
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (276, 87, 128, false, true, false); --is_mandatory
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (277, 87, 130, NULL, true, false); --data_type

--таблица XML-запросов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (278, 91, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (279, 91, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (280, 91, 120, NULL, true, false); --id_xml_query_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (281, 91, 124, NULL, true, false); --id_xml_query_kind
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (282, 91, 125, NULL, true, false); --xml_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (283, 91, 16, NULL, true, false); --insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (284, 91, 126, NULL, false, false); --exec_period_h
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (285, 91, 3, NULL, false, false); --description

--таблица XML-ответов на запросы
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (286, 93, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (287, 93, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (288, 93, 127, NULL, true, false); --id_xml_query
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (289, 93, 125, NULL, true, false); --xml_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (290, 93, 129, NULL, true, false); --exec_time

---
---Справочники ИК ТСО
---

--таблица должностей ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (291, 95, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (292, 95, 2, NULL, true, false); --name

--таблица типов карт доступа ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (293, 97, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (294, 97, 2, NULL, true, false); --name

--таблица статусов карт доступа ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (295, 99, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (296, 99, 2, NULL, true, false); --name

--таблица планов доступа ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (297, 101, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (298, 101, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (299, 101, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (300, 101, 209, NULL, false, false); --org_uid

--таблица отделов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (301, 103, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (302, 103, 2, NULL, true, false); --name

--таблица охраняемых объектов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (303, 105, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (304, 105, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (305, 105, 131, NULL, true, false); --guard_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (306, 105, 138, NULL, false, false); --guard_objects_devices

--таблица MIME-типов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (307, 107, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (308, 107, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (309, 107, 46, NULL, true, false); --type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (310, 107, 69, NULL, true, false); --subtype

--таблица неисправных устройств ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (311, 109, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (109, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (109, 133, NULL, false, false); --fault_string
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (312, 109, 134, NULL, true, false); --reg_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (313, 109, 135, NULL, true, false); --fault_state_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (314, 109, 136, NULL, true, false); --fault_state_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (315, 109, 137, NULL, true, false); --id_device

--таблица ответов на запросы о тревожных событиях ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (316, 111, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (111, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (111, 133, NULL,false, false); --fault_string
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (317, 111, 137, NULL, false, false); --id_device
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (318, 111, 139, NULL, false, false); --id_person
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (319, 111, 140, NULL, true, false); --event_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (320, 111, 141, NULL, true, false); --message
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (321, 111, 142, NULL, true, false); --source
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (322, 111, 143, NULL, true, false); --alarm
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (323, 111, 144, NULL, false, false); --card_code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (324, 111, 145, NULL, false, false); --pass
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (325, 111, 146, NULL, false, false); --direction
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (326, 111, 147, NULL, false, false); --video
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (327, 111, 217, NULL, false, false); --video_url

--таблица с информацией о персонале ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (328, 113, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 133, NULL, false, false); --fault_string
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (329, 113, 151, NULL, false, false); --id_tso_position
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (330, 113, 152, NULL, false, false); --id_tso_unit
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (331, 113, 22, NULL, true, false); --lastname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (332, 113, 23, NULL, false, false); --firstname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (333, 113, 24, NULL, false, false); --sirname

--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 144, NULL, false, false); --card_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 148, NULL, false, false); --id_card_type
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 149, NULL, false, false); --id_card_status
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 150, NULL, false, false); --id_access_plan
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (113, 153, NULL, false, false); --comment

--таблица граф. планов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (334, 115, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (335, 115, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (336, 115, 27, NULL, false, false); --id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (337, 115, 154, NULL, false, false); --bg_image
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (338, 115, 155, NULL, false, false); --svg_image
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (115, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (115, 133, NULL, false, false); --fault_string

--таблица устройств (элементов) граф. планов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (339, 117, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (340, 117, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (341, 117, 156, NULL, true, false); --id_graph_plan
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (342, 117, 155, NULL, false, false); --svg_image
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (117, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (117, 133, NULL, false, false); --fault_string

--таблица типов фигур граф. элементов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (343, 119, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (344, 119, 2, NULL, true, false); --name

--таблица типов отрезков для сегментов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (345, 121, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (346, 121, 2, NULL, true, false); --name

--таблица стилей отображения графических примитивов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (347, 123, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (348, 123, 2, NULL, true, false); --name

--таблица фигур для отображения устройств на граф. планах ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (349, 125, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (125, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (125, 133, NULL, false, false); --fault_string
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (350, 125, 157, NULL, true, false); --id_shape_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (351, 125, 137, NULL, true, false); --id_device
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (352, 125, 159, NULL, false, false); --id_graph_style

--таблица параметров отдельных отрезков графических элементов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (353, 127, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (354, 127, 160, NULL, true, false); --id_segment_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (355, 127, 161, NULL, true, false); --id_element_shape
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (356, 127, 162, NULL, true, false); --points
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (357, 127, 163, '0', true, false); --p_order
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (127, 132, NULL, false, false); --fault_code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (127, 133, NULL, false, false); --fault_string

--таблица уровней срочности сообщений (распоряжений)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (358, 129, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (359, 129, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (360, 129, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (361, 129, 164, NULL, true, false); --urgency_symbol

--таблица списков рассылки
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (362, 131, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (363, 131, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (364, 131, 3, NULL, false, false); --desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (365, 131, 166, NULL, true, false); --mail_lists_position


--ИК ТСО (ИНТГРАЛЬНЫЕ)

--атрибуты для категорий интегральных справочников
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(366, 134, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(367, 134, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(368, 136, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(369, 136, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(370, 138, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(371, 138, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(372, 140, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(373, 140, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(374, 142, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(375, 142, 220, NULL, FALSE, FALSE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(376, 102, 219, NULL, FALSE, FALSE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(377, 102, 220, NULL, FALSE, FALSE);


--интегральный справочник событий
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (378, 133, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (379, 133, 168, NULL, false, false); --id_person_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (380, 133, 140, NULL, false, false); --event_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (381, 133, 141, NULL, false, false); --message
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (382, 133, 142, NULL, false, false); --source
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (383, 133, 143, NULL, false, false); --alarm
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (384, 133, 144, NULL, false, false); --card_code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (385, 133, 145, NULL, false, false); --pass
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (386, 133, 146, NULL, false, false); --direction
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (387, 133, 22, NULL, false, false); --lastname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (388, 133, 23, NULL, false, false); --firstname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (389, 133, 24, NULL, false, false); --sirname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (390, 133, 169, NULL, false, false); --id_device_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (391, 133, 174, NULL, false, false); --dev_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (392, 133, 170, NULL, false, false); --id_unit_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (393, 133, 171, NULL, false, false); --unit_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (394, 133, 172, NULL, false, false); --id_dolzh_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (395, 133, 173, NULL, false, false); --dolzh_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (396, 133, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (397, 133, 209, NULL, false, false); --org_uid
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (398, 133, 217, NULL, false, false); --video_url
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (399, 133, 218, NULL, false, false); --for_report
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (400, 133, 221, NULL, false, false); --is_deleted


--интегральный справочник неисправных устройств
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (401, 135, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (402, 135, 169, NULL, false, false); --id_device_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (403, 135, 174, NULL, false, false); --dev_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (404, 135, 134, NULL, false, false); --reg_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (405, 135, 135, NULL, false, false); --fault_state_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (406, 135, 136, NULL, false, false); --fault_state_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (407, 135, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (408, 135, 209, NULL, false, false); --org_uid
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (409, 135, 221, NULL, false, false); --is_deleted

--Интегральный справочник устройств и охраняемых объектов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (410, 137, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (411, 137, 169, NULL, false, false); --id_device_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (412, 137, 174, NULL, false, false); --dev_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (413, 137, 175, NULL, false, false); --id_guard_obj_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (414, 137, 176, NULL, false, false); --guard_obj_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (415, 137, 177, NULL, false, false); --device_svg
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (416, 137, 178, NULL, false, false); --plan_svg
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (417, 137, 179, NULL, false, false); --is_ok
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (418, 137, 180, NULL, false, false); --ok_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (419, 137, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (420, 137, 209, NULL, false, false); --org_uid

--Интегральный справочник персонала объектов ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (421, 139, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (422, 139, 22, NULL, false, false); --lastname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (423, 139, 23, NULL, false, false); --firstname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (424, 139, 24, NULL, false, false); --sirname
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (425, 139, 181, NULL, false, false); --access_cards
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (426, 139, 172, NULL, false, false); --id_dolzh_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (427, 139, 173, NULL, false, false); --dolzh_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (428, 139, 170, NULL, false, false); --id_unit_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (429, 139, 171, NULL, false, false); --unit_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (430, 139, 153, NULL, false, false); --comment
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (431, 139, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (432, 139, 209, NULL, false, false); --org_uid

--Интегральный справочник карт доступа к объектам ИК ТСО
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (433, 141, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (434, 141, 6, NULL, true, true); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (435, 141, 153, NULL, false, false); --comment
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (436, 141, 182, NULL, false, false); --id_card_type_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (437, 141, 183, NULL, false, false); --card_type_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (438, 141, 184, NULL, false, false); --id_card_status_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (439, 141, 185, NULL, false, false); --card_status_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (440, 141, 186, NULL, false, false); --access_plans
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (441, 141, 3, NULL, false, false); --desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (442, 141, 208, NULL, false, false); --org_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (443, 141, 209, NULL, false, false); --org_uid

--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (151, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (444, 151, 210, NULL, true, false); --max_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (445, 151, 211, NULL, true, false); --tso_dl_from
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (446, 151, 212, NULL, true, false); --tso_dl_exec
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (447, 151, 213, NULL, true, false); --tso_dl_to
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (448, 151, 214, NULL, true, false); --tso_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (449, 151, 215, NULL, true, false); --tso_exectime





--УЧЕТНАЯ КАРТОЧКА ДОКУМЕНТА
--справочник типов документов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (450, 143, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (451, 143, 2, NULL, true, false); --name

--справочник подтипов документов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (452, 145, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (453, 145, 2, NULL, true, false); --name

--справочник видов документов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (454, 149, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (455, 149, 2, NULL, true, false); --name

--учетная карточка (она НЕ является справочником)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (456, 148, 191, NULL, true, false); --id_exec_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (457, 148, 35, NULL, true, false); --id_dl_executor
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (458, 148, 42, NULL, true, false); --id_dl_from
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (459, 148, 205, NULL, true, false); --create_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (460, 148, 100, NULL, true, false); --output_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (461, 148, 193, NULL, true, false); --regout_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (462, 148, 99, NULL, true, false); --input_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (463, 148, 194, NULL, true, false); --regin_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (464, 148, 189, NULL, true, false); --id_document_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (465, 148, 190, NULL, true, false); --id_document_subtype
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (466, 148, 192, NULL, true, false); --id_document_kind
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (467, 148, 195, NULL, true, false); --doc_header
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (468, 148, 196, NULL, true, false); --page_count
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (469, 148, 197, NULL, true, false); --ekz_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (470, 148, 198, NULL, true, false); --book_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (471, 148, 199, NULL, true, false); --book_subnumber
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (472, 148, 200, NULL, true, false); --class_code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (473, 148, 201, NULL, true, false); --inv_number
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (474, 148, 202, NULL, true, false); --registrator_info
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (475, 148, 3, NULL, true, false); --desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (476, 148, 203, NULL, true, false); --annotation
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (477, 148, 204, NULL, true, false); --command_body


/*
--
--ZARYA22
--
--воинские звания
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 1, NULL, true, true); --id
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 2, NULL, true, false); --name
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 6, NULL, true, false); --code
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (152, 19, NULL, true, false); --short_name

--предметные области
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (154, 6, NULL, true, false); --code

-- методы chk sum
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (156, 6, NULL, true, false); --code

-- орг единицы
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (158, 224, NULL, true, false); --uri

-- долж единицы
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (160, 6, NULL, true, false); --code

-- АСУ
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (162, 224, NULL, true, false); --uri

-- dls
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 224, NULL, true, false); --uri
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 225, NULL, true, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 226, NULL, true, false); --id_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (164, 227, NULL, true, false); --id_pos

-- addressee
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 6, NULL, true, false); --code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 224, NULL, true, false); --uri
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 225, NULL, true, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 222, NULL, true, false); --type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (166, 223, NULL, true, false); --value

--простое входящее сообщение (заря22)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 225, NULL, false, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 231, NULL, false, false); --shu_working_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 226, NULL, false, false); --id_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 228, NULL, false, false); --id_dls
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 20, NULL, false, false); --id_rank
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 25, NULL, false, false); --fio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 232, NULL, false, false); --shu_msg_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 233, NULL, false, false); --shu_creation_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 234, NULL, false, false); --shu_creation_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 235, NULL, false, false); --shu_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 236, NULL, false, false); --shu_priority
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 230, NULL, false, false); --id_shu_domain
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 2, NULL, false, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (168, 237, NULL, false, false); --shu_text

--простая входящая команда (заря22)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 225, NULL, false, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 231, NULL, false, false); --shu_working_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 226, NULL, false, false); --id_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 228, NULL, false, false); --id_dls
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 20, NULL, false, false); --id_rank
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 25, NULL, false, false); --fio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 232, NULL, false, false); --shu_msg_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 233, NULL, false, false); --shu_creation_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 234, NULL, false, false); --shu_creation_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 235, NULL, false, false); --shu_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 236, NULL, false, false); --shu_priority
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 230, NULL, false, false); --id_shu_domain
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 2, NULL, false, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 238, NULL, false, false); --shu_cmd_code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (169, 239, NULL, false, false); --shu_cmd_text

--
--Донесение о чрезвычайной ситуации (заря22)
--
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 225, NULL, false, false); --id_acs
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 231, NULL, false, false); --shu_working_mode
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 226, NULL, false, false); --id_org
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 228, NULL, false, false); --id_dls
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 20, NULL, false, false); --id_rank
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 25, NULL, false, false);  --fio
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 233, NULL, false, false); --shu_creation_date
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 234, NULL, false, false); --shu_creation_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 235, NULL, false, false); --shu_maclabel
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 236, NULL, false, false); --shu_priority
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (170, 230, NULL, false, false); --id_shu_domain
--заголовок
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 258, NULL, false, false);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 259, NULL, false, false);
--Чрезвычайная ситуация
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 240, NULL, false, false);--shu_emerg_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 241, NULL, false, false);--shu_emerg_datetime
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 244, NULL, false, false);--shu_district
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 245, NULL, false, false);--shu_region
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 246, NULL, false, false);--shu_area
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 260, NULL, false, false);--shu_town
--Документ о радиоактивном заражении
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 261, NULL, false, false);--shu_document_entry
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 242, NULL, false, false);--shu_obj_open_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 243, NULL, false, false);--shu_obj_close_name
--Принадлежность
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 262, NULL, false, false);--shu_block
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 263, NULL, false, false);--shu_country
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 264, NULL, false, false);--shu_department
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 265, NULL, false, false);--shu_company
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 266, NULL, false, false);--shu_block
--Ситуационная схема ЧС
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 257, NULL, false, false);--shu_emerg_pict
--Радиационное загрязнение
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 267, NULL, false, false);--shu_authenticity
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 268, NULL, false, false);--shu_instrumentation_event_time
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 269, NULL, false, false);--shu_authenticity
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 270, NULL, false, false);--shu_cdomain1
--краткое описание ЧС
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 249, NULL, false, false);--shu_emerg_desc
--
--радиационное загрязнение
--координаты
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 271, NULL, false, false);--shu_height_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 247, NULL, false, false);--shu_emerg_lat
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 248, NULL, false, false);--shu_emerg_lon
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 272, NULL, false, false);--shu_emerg_height
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 273, NULL, false, false);--shu_coord_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 276, NULL, false, false);--shu_cdomain2
--состояние жизненного цикла
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 274, NULL, false, false);--shu_life_cycle_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 275, NULL, false, false);--shu_life_cycle_indicators
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 276, NULL, false, false);--shu_life_cycle_type
--радиусы зон
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 251, NULL, false, false);--shu_emerg_dang_area
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 277, NULL, false, false);--shu_emerg_dang_area_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 253, NULL, false, false);--shu_emerg_dang_area1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 278, NULL, false, false);--shu_emerg_dang_area1_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 283, NULL, false, false);--shu_emerg_safe_area
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 279, NULL, false, false);--shu_emerg_safe_area_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 250, NULL, false, false);--shu_emerg_rad_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 280, NULL, false, false);--shu_emerg_rad_level_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 252, NULL, false, false);--shu_emerg_dang_area_rad_level
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 281, NULL, false, false);--shu_emerg_dang_area_rad_level_uom
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 254, NULL, false, false);--shu_emerg_dang_area_rad_level1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 282, NULL, false, false);--shu_emerg_dang_area_rad_level1_uom
--приложение
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 256, NULL, false, false);--shu_emerg_evac_act
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(170, 255, NULL, false, false);--shu_emerg_lic_act
--end of emerg
*/


--таблица операторов сравнения
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (478, 175, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (479, 175, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (480, 175, 286, NULL, true, false); --the_value

--таблица типов показателей
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (481, 177, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (482, 177, 2, NULL, true, false); --name

--таблица показателей
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (483, 179, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (484, 179, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (485, 179, 3, NULL, false, false); --desc
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (486, 179, 291, NULL, true, false); --id_indicator_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (487, 179, 27, NULL, false, false); --id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (488, 179, 5, NULL, true, false); --id_a_type
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (489, 179, 8, NULL, false, false); --table_name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (490, 179, 9, NULL, false, false); --column_name

--таблица значений показателей
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (491, 181, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (492, 181, 286, NULL, true, false); --the_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (493, 181, 292, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (494, 181, 293, NULL, false, false); --id_io_object_src
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (495, 181, 294, NULL, false, false); --id_io_object_src1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (496, 181, 295, NULL, true, false); --meas_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (497, 181, 16, NULL, true, false); --insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (498, 181, 297, NULL, true, false); --start_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (499, 181, 298, NULL, false, false); --stop_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (500, 181, 299, NULL, true, false); --id_indicator

--таблица значений атрибутов в информационных объектах
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (501, 183, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (502, 183, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (503, 183, 300, NULL, true, false); --id_attr_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (504, 183, 301, NULL, true, false); --value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (505, 183, 297, 'current_timestamp', true, false); --start_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (506, 183, 298, NULL, false, false); --stop_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (507, 183, 295, 'current_timestamp', true, false); --meas_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (508, 183, 16, 'current_timestamp', true, false); --insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (509, 183, 293, NULL, false, false); --id_io_object_src
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (510, 183, 294, NULL, false, false); --id_io_object_src1
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (511, 183, 302, 'true', true, true); --is_actual

--таблица атрибутов в категориях
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (512, 185, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (513, 185, 12, NULL, true, false); --id_io_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (514, 185, 303, NULL, true, false); --id_io_attribute
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (515, 185, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (516, 185, 304, NULL, false, false); --def_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (517, 185, 128, 'false', true, false); --is_mandatory
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (518, 185, 305, 'false', true, false); --is_read_only

--таблица значений атрибутов в атрибутах
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (526, 187, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (527, 187, 309, NULL, true, false); --id_attr_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (528, 187, 308, NULL, true, false); --id_attr_attr
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (529, 187, 301, NULL, true, false); --value

--таблица атрибутов в атрибутах
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (519, 189, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (520, 189, 306, NULL, true, false); --id_attr_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (521, 189, 307, NULL, true, false); --id_attr_child
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (522, 189, 2, NULL, false, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (523, 189, 304, NULL, false, false); --def_value
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (524, 189, 128, 'false', true, false); --is_mandatory
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (525, 189, 305, 'false', true, false); --is_read_only
--530 занят для атрибута is_qualifier в справочнике типов категорий
--531 занят для атрибута id_child2 в справочнике  категорий
--532 занят для атрибута is_archived в справочнике категорий
--533 занят для атрибута r_icon в справочнике рубрикатора
--534 занят для атрибута r_icon в справочнике информационных объектов

--таблица законов распределения
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (535, 191, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (536, 191, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (537, 191, 3, NULL, false, false);  --description

--таблица потоков сообщений
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (538, 193, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (539, 193, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (540, 193, 315, NULL, true, false); --id_partition_low
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (541, 193, 52, NULL, true, false); --id_dl_receiver
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (542, 193, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (543, 193, 316, '0', false, false); --lambda
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (544, 193, 317, '0', false, false); --sigma
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (545, 193, 318, '0', false, false); --moda
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (546, 193, 319, '0', false, false); --min_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (547, 193, 320, '0', false, false); --max_p
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (548, 193, 313, NULL, true, false); --start_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (549, 193, 314, NULL, true, false); --end_time

--таблица жизненных циклов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (550, 195, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (551, 195, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (552, 195, 321, NULL, false, false);  --id_start_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (553, 195, 322, NULL, false, false);  --life_cycle_io_states
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (554, 195, 3, NULL, false, false);  --description

--таблица переходов состояний в жизненных циклах
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (555, 197, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (556, 197, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (557, 197, 323, NULL, true, false);  --id_life_cycle
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (558, 197, 324, NULL, true, false);  --id_state_src
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (559, 197, 325, NULL, true, false);  --id_state_dest


--таблица сервисов-обработчиков
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (560, 199, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (561, 199, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (562, 199, 3, NULL, false, false);  --description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (563, 199, 326, NULL, true, false);  --service
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (564, 199, 327, NULL, false, false);  --extra_params
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (583, 199, 336, 'FALSE', true, false);  --is_external


--таблица описаний очередей
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (565, 201, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (566, 201, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (567, 201, 3, NULL, false, false);  --description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (568, 201, 328, NULL, true, false);  --id_handler
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (569, 201, 18, NULL, true, false);  --id_io_state
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (570, 201, 12, NULL, true, false);  --id_io_category


--таблица очередей
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (571, 203, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (572, 203, 329, NULL, true, false); --id_chain
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (573, 203, 330, NULL, false, false);  --id_record
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (574, 203, 48, NULL, false, false);  --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (575, 203, 16, NULL, true, true);  --insert_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (576, 203, 331, '0', true, false);  --is_handled
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (577, 203, 332, NULL, false, false);  --handled_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (578, 203, 333, NULL, false, false);  --start_service_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (579, 203, 334, NULL, false, false);  --end_service_time
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (580, 203, 335, NULL, false, false);  --return_code
--581 используется в справочнике состояний ИО
--582 используется в справочнике категорий ИО
--583 используется в справочнике сервисов

SELECT pg_catalog.setval('attrs_categories_id_seq', 2000, true); --все пользовательские атрибуты в категориях будут начинаться с номера 2001
                                                                 --это сделано для того, чтобы оставить резерв для системных атрибутов

select f_safe_drop_trigger('trgacinsert', 'attrs_categories');


create temp table notlogging_oper_table (id int4 not null);
select setAsNotLogging(1);


select f_safe_drop_trigger('trg_acl_ioctrl', 'tbl_io_objects');

--системные информационные объекты
--в том числе задаем на них права системной роли OTHERS_ROLE как (TRUE, TRUE, FALSE, FALSE)
insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-1', 1, 2, 1, 1, 'Справочник состояний информационных объектов', 'io_states', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (1, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-2', 2, 4, 1, 1, 'Справочник типов категорий информационных объектов', 'io_category_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (2, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-3', 3, 6, 1, 1, 'Справочник представлений типов атрибутов', 'a_views', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (3, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-4', 4, 8, 1, 1, 'Справочник типов атрибутов', 'a_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (4, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-5', 5, 10, 1, 1, 'Справочник атрибутов', 'attributes', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (5, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-6', 6, 12, 1, 1, 'Справочник категорий', 'io_categories', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (6, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-7', 7, 14, 1, 1, 'Справочник информационных объектов', 'io_objects', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (7, -10, TRUE, TRUE, FALSE, TRUE); --Каждый пользователь должен иметь право редактирования записей справочника информационных объектов. Т.е. создавать, удалять и изменять информационные объекты

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-8', 8, 16, 1, 1, 'Справочник шаблонов информационных объектов', 'io_templates', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (8, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-9', 9, 18, 1, 1, 'Справочник типов информационных объектов', 'io_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (9, -10, TRUE, TRUE, FALSE, TRUE); --Каждый пользователь должен иметь право редактирования записей справочника информационных объектов. Т.е. создавать, удалять и изменять информационные объекты

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-10', 10, 20, 1, 1, 'Справочник типов поисковых запросов', 'search_template_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (10, -10, TRUE, TRUE, FALSE, FALSE);

--old ОШС
 
insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-15', 15, 30, 1, 1, 'Табель срочных донесений', 'tsd', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (15, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-16', 16, 32, 1, 1, 'Справочник состояний пунктов табелей и журналов', 'jr_states', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (16, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-17', 17, 34, 1, 1, 'Журнал входящих и исходящих распоряжений', 'command_journal', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (17, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-19', 19, 38, 1, 1, 'Журнал контроля исполнения табелей и журналов распоряжений', 'tsd_control_journal', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (19, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-20', 20, 40, 1, 1, 'Журнал сообщений', 'message_journal', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (20, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-21', 21, 42, 1, 1, 'Справочник типов прикрепляемых файлов', 'url_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (21, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-22', 22, 44, 1, 1, 'Справочник известных расширений прикрепляемых файлов', 'url_extentions', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (22, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-23', 23, 46, 1, 1, 'Справочник грифов секретности', 'maclabels', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (23, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-24', 24, 48, 1, 1, 'Справочник воинских званий', 'ranks', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (24, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-25', 25, 50, 1, 1, 'Справочник пользователей', 'users', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (25, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-26', 26, 52, 1, 1, 'Справочник должностей', 'position', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (26, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-27', 27, 54, 1, 1, 'Справочник организаций', 'organization', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (27, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-28', 28, 56, 1, 1, 'Справочник подразделений', 'units', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (28, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-29', 29, 60, 1, 1, 'Справочник режимов функционирования', 'work_mode', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (29, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-30', 30, 62, 1, 1, 'Справочник состояний пользователей', 'user_state', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (30, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-31', 31, 64, 1, 1, 'Справочник типов режимов функционирования', 'work_mode_type', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (31, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-32', 32, 66, 1, 1, 'Справочник типов организаций', 'organization_type', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (32, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-33', 33, 68, 1, 1, 'Справочник транспортов межобъектового обмена', 'transport', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (33, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-34', 34, 70, 1, 1, 'Справочник адресов организаций', 'organization_transport', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (34, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-35', 35, 72, 1, 1, 'Справочник шаблонов отчетов', 'report', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (35, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-36', 36, 74, 1, 1, 'Справочник поисковых запросов (фильтров)', 'search_templates', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (36, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-37', 37, 76, 1, 1, 'Справочник пользовательских очередей', 'user_chains', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (37, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-38', 38, 78, 1, 1, 'Общесистемный рубрикатор', 'rubricator', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (38, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-39', 39, 80, 1, 1, 'Типы синхронизации информационных объектов', 'io_sync_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (39, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-40', 40, 82, 1, 1, 'Архив (история) синхронизации информационных объектов', 'io_sync_archive', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (40, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-41', 41, 84, 1, 1, 'Параметры синхронизации информационных объектов', 'io_last_sync', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (41, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-42', 42, 86, 1, 1, 'Справочник типов XML-запросов для обмена информацией с внешними системами', 'xml_query_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (42, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-43', 43, 88, 1, 1, 'Справочник XML-тэгов (форматов) ответов на запросы при обмене информацией с внешними системами', 'xml_paths', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (43, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-44', 44, 90, 1, 1, 'Справочник видов XML-запросов при обмене информацией с внешними системами', 'xml_query_kinds', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (44, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-45', 45, 92, 1, 1, 'Справочник XML-запросов при обмене информацией с внешними системами', 'xml_queries', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (45, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-46', 46, 94, 1, 1, 'Справочник ответов на XML-запросы при обмене информацией с внешними системами', 'xml_query_responses', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (46, -10, TRUE, TRUE, FALSE, FALSE);

---
---ИК ТСО
---
insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-47', 47, 96, 1, 1, 'Справочник должностей ИК ТСО', 'tso_positions', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (47, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-48', 48, 98, 1, 1, 'Справочник типов карт доступа ИК ТСО', 'card_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (48, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-49', 49, 100, 1, 1, 'Справочник статусов карт доступа ИК ТСО', 'card_statuses', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (49, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-50', 50, 102, 1, 1, 'Справочник планов доступа ИК ТСО', 'access_plan', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (50, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-51', 51, 104, 1, 1, 'Справочник отделов ИК ТСО', 'tso_units', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (51, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-52', 52, 106, 1, 1, 'Справочник охраняемых объектов ИК ТСО', 'guard_objects', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (52, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-53', 53, 108, 1, 1, 'Справочник MIME-типов', 'mimetypes', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (53, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-54', 54, 110, 1, 1, 'Справочник неисправных устройств ИК ТСО', 'fault_devices', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (54, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-55', 55, 112, 1, 1, 'Справочник ответов на запросы о тревожных событиях ИК ТСО', 'alarm_events', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (55, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-56', 56, 114, 1, 1, 'Справочник персонала ИК ТСО', 'persons', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (56, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-57', 57, 116, 1, 1, 'Справочник граф. планов ИК ТСО', 'graph_plans', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (57, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-58', 58, 118, 1, 1, 'Справочник устройств (элементов) граф. планов ИК ТСО', 'devices', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (58, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-59', 59, 120, 1, 1, 'Справочник типов фигур элементов граф. планов ИК ТСО', 'shape_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (59, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-60', 60, 122, 1, 1, 'Справочник типов сегментов элементов граф. планов ИК ТСО', 'segment_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (60, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-61', 61, 124, 1, 1, 'Справочник стилей отображения примитивов ИК ТСО', 'graph_styles', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (61, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-62', 62, 126, 1, 1, 'Справочник фигур элементов граф. планов ИК ТСО', 'element_shapes', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (62, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-63', 63, 128, 1, 1, 'Справочник параметров отдельных отрезков графических элементов ИК ТСО', 'shape_segments', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (63, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-64', 64, 130, 1, 1, 'Справочник уровней срочности сообщений (распоряжений)', 'urgency_levels', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (64, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-65', 65, 132, 1, 1, 'Справочник списков рассылки сообщений и распоряжений', 'mail_lists', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (65, -10, TRUE, TRUE, FALSE, FALSE);

--ИК ТСО
insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-66', 66, 134, 1, 1, 'Интегральный справочник событий ИК ТСО', 'alarm_events_q', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (66, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-67', 67, 138, 1, 1, 'Интегральный справочник устройств и охраняемых объектов ИК ТСО', 'dev_object_q', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (67, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-68', 68, 136, 1, 1, 'Интегральный справочник неисправных устройств ИК ТСО', 'fault_devices_q', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (68, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-69', 69, 140, 1, 1, 'Интегральный справочник персонала объектов ИК ТСО', 'personal_q', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (69, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-70', 70, 142, 1, 1, 'Интегральный справочник карт доступа к объектам ИК ТСО', 'access_cards_q', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (70, -10, TRUE, TRUE, FALSE, FALSE);


insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-71', 71, 144, 1, 1, 'Справочник типов документов', 'document_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (71, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-72', 72, 146, 1, 1, 'Справочник подтипов документов', 'document_subtypes', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (72, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-73', 73, 150, 1, 1, 'Справочник видов документов', 'document_kinds', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (73, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-74', 74, 151, 1, 1, 'Настройки системы ТСО', NULL, NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (74, -10, TRUE, TRUE, FALSE, FALSE);

/*
--insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
--values ('localorg-io_objects-75', 75, 153, 1, 1, 'Реестр воинских званий (Заря22)', NULL, NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
--insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
--values (75, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-76', 76, 155, 1, 1, 'Реестр предметных областей (Заря22)', 'shu_domains', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (76, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-77', 77, 157, 1, 1, 'Реестр методов расчета контрольных сумм (Заря22)', 'shu_chksum', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (77, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-78', 78, 159, 1, 1, 'Реестр организационных единиц (Заря22)', 'shu_orgs', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (78, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-79', 79, 161, 1, 1, 'Реестр должностных единиц (Заря22)', 'shu_positions', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (79, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-80', 80, 163, 1, 1, 'Реестр АСУ (Заря22)', 'shu_acs', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (80, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-81', 81, 165, 1, 1, 'Реестр должностных лиц (Заря22)', 'shu_dls', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (81, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-82', 82, 167, 1, 1, 'Реестр адресов (Заря22)', 'shu_addressee', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (82, -10, TRUE, TRUE, FALSE, FALSE);
*/

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-83', 83, 172, 1, 1, 'Справочник групп атрибутов', 'attrs_groups', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (83, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-84', 84, 174, 1, 1, 'Справочник типов должностей', 'position_types', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (84, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-85', 85, 176, 1, 1, 'Справочник операторов сравнения', 'operations', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (85, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-86', 86, 178, 1, 1, 'Справочник типов показателей', 'indicator_type', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (86, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-87', 87, 180, 1, 1, 'Справочник показателей', 'indicator', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (87, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-88', 88, 182, 1, 1, 'Справочник значений показателей', 'indicators_values', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (88, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-89', 89, 184, 1, 1, 'Справочник значений атрибутов в информационных объектах', 'attrs_values', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (89, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-90', 90, 186, 1, 1, 'Справочник атрибутов в категориях', 'attrs_categories', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (90, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-91', 91, 188, 1, 1, 'Справочник значений атрибутов в атрибутах', 'attrs_attrs_values', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (91, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-92', 92, 190, 1, 1, 'Справочник атрибутов в атрибутах', 'attrs_attrs', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (92, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-93', 93, 192, 1, 1, 'Справочник законов распределения', 'partition_lows', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (93, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-94', 94, 194, 1, 1, 'Справочник потоков сообщений', 'message_streams', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (94, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-95', 95, 196, 1, 1, 'Справочник жизненных циклов', 'life_cycle', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (95, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-96', 96, 198, 1, 1, 'Справочник переходов состояний в жизненных циклах', 'state_crosses', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (96, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-97', 97, 200, 1, 1, 'Справочник сервисов-обработчиков очередей', 'handlers', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (97, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-98', 98, 202, 1, 1, 'Справочник описаний очередей обработки', 'chains', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (98, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-99', 99, 204, 1, 1, 'Справочник очередей обработки (данные для обработки)', 'chains_data', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (99, -10, TRUE, TRUE, FALSE, FALSE);


SELECT pg_catalog.setval('tbl_io_objects_id_seq', 300, true); --все пользовательские информационные объекты будут начинаться с номера 301
                                                          --это сделано для того, чтобы оставить резерв для системных ИО


select f_create_trigger('trg_acl_ioctrl', 'before', 'insert or update or delete', 'io_objects', 'acl_ioctrl()');

--создание шаблона для справочника информационных объектов
insert into io_templates (id_io_category, name, code) values(13, 'Основной шаблон для справочника информационных объектов', 'SYSTEMPLATE_1');

insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, 'Название', 0);
insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, 'Об авторе', 1);
insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, 'Категория и справочник', 2);
--insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, 'Синхронизация', 2);
insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, 'Визуализация', 3);
insert into a_groups (id_io_template, id_parent, name, "order") values(1, NULL, 'Базовый справочник', 4);


insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(49, 1, 1, true, 0, NULL);--id_maclabel
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(39, 1, 1, false, 1, NULL);--name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(40, 1, 1, false, 2, NULL);--description
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(41, 1, 1, false, 3, '1');--id_io_type
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(48, 1, 1, false, 4, '1');--id_io_state

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(45, 1, 2, false, 1, NULL); --information
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(47, 1, 2, true, 2, 'current_timestamp'); --insert_time
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(50, 1, 2, true, 3, NULL);--id_object_organization
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(52, 1, 2, true, 4, NULL);--owner_org

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(44, 1, 3, false, 0, NULL);--table_name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(42, 1, 3, true, 1, NULL);--id_io_category

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(54, 1, 4, true, 0, NULL);--colors
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(55, 1, 4, true, 1, NULL);
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(534, 1, 4, false, 2, NULL);

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(56, 1, 5, true, 0, NULL);--id_search_template
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(57, 1, 5, true, 1, NULL);--ref_table_name

--создание специального шаблона для справочника атрибутов
insert into io_templates (id_io_category, name, code) values(9, 'Специальный шаблон для справочника атрибутов', 'SYSTEMPLATE_2');
insert into a_groups (id_io_template, id_parent, name, "order") values(2, NULL, 'Основные атрибуты', 0); --6
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(21, 2, 6, false, 0, NULL);
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(18, 2, 6, false, 1, NULL);

--создание специального шаблона для справочника организаций
insert into io_templates (id_io_category, name, code) values(53, 'Специальный шаблон для справочника организаций', 'SYSTEMPLATE_3');--3
insert into a_groups (id_io_template, id_parent, name, "order") values(3, NULL, 'Название организации', 0); --7
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(170, 3, 7, false, 0, NULL); --id
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(171, 3, 7, false, 1, NULL); --name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(179, 3, 7, false, 2, NULL);--short_name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(180, 3, 7, false, 3, NULL);--code_name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(181, 3, 7, false, 4, NULL);--email_prefix

insert into a_groups (id_io_template, id_parent, name, "order") values(3, NULL, 'Режимы функционирования', 1); --8
insert into a_groups (id_io_template, id_parent, name, "order") values(3, 7, 'Текущий режим', 0); --9
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(176, 3, 9, false, 0, NULL);--id_curr_mode
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(187, 3, 9, false, 1, NULL);--dt_curr_mode

insert into a_groups (id_io_template, id_parent, name, "order") values(3, 7, 'Предыдущий режим', 1); --10
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(177, 3, 10, false, 0, NULL);--id_prev_mode
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(186, 3, 10, false, 1, NULL);--dt_prev_mode

insert into a_groups (id_io_template, id_parent, name, "order") values(3, NULL, 'Подчиненность', 2); --11
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(178, 3, 11, false, 0, NULL);--id_boss
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(173, 3, 11, false, 1, NULL);--id_parent
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(174, 3, 11, false, 2, NULL);--id_parent1
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(175, 3, 11, false, 3, NULL);--id_substitutor

insert into a_groups (id_io_template, id_parent, name, "order") values(3, NULL, 'Тип, координаты и условные знаки', 3); --12
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(172, 3, 12, false, 0, NULL);--id_type_org
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(182, 3, 12, false, 1, NULL);--lat
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(183, 3, 12, false, 2, NULL);--lon
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(185, 3, 12, false, 3, NULL);--tree_symbol
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(184, 3, 12, false, 4, NULL);--map_symbol
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(188, 3, 12, false, 5, NULL);--work_modes (separate table)

--создание специального шаблона для справочника потоков сообщений
insert into io_templates (id_io_category, name, code) values(193, 'Специальный шаблон для справочника потоков сообщений', 'SYSTEMPLATE_4');
insert into a_groups (id_io_template, id_parent, name, "order") values(4, NULL, 'Основные атрибуты', 0); --13
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(538, 4, 13, false, 0, NULL); --id
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(539, 4, 13, false, 1, NULL); --name
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(541, 4, 13, false, 2, NULL); --id_dl_receiver
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(542, 4, 13, false, 3, NULL); --id_io_object

insert into a_groups (id_io_template, id_parent, name, "order") values(4, NULL, 'Параметры закона распределения', 1); --14
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(540, 4, 14, false, 0, NULL); --id_partition_low
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(543, 4, 14, false, 1, '0'); --lambda
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(544, 4, 14, false, 2, '0'); --sigma
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(545, 4, 14, false, 3, '0'); --moda
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(546, 4, 14, false, 4, '0'); --min_p
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(547, 4, 14, false, 5, '0'); --max_p

insert into a_groups (id_io_template, id_parent, name, "order") values(4, NULL, 'Диапазон генерации потока', 2); --15
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(548, 4, 15, false, 0, NULL);--start_time
insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(549, 4, 15, false, 1, NULL);--end_time

SELECT pg_catalog.setval('io_templates_id_seq', 50, true); --все пользовательские шаблоны будут начинаться с номера 51
                                                           --это сделано для того, чтобы оставить резерв для системных 
                                                           --шаблоны для зари22 создаются в спец. функции, которая определяет очередной доступный ИД
                                                           --поэтому на тот код данный вызов не влияет    

SELECT pg_catalog.setval('a_groups_id_seq', 100, true);    --шаблоны для зари22 создаются в спец. функции, которая определяет очередной доступный ИД
                                                           --поэтому на тот код данный вызов не влияет    

SELECT pg_catalog.setval('io_views_id_seq', 300, true);    --шаблоны для зари22 создаются в спец. функции, которая определяет очередной доступный ИД
                                                           --поэтому на тот код данный вызов не влияет    

update attributes set id_ref_attr_type = 9 where column_name is not null;

select setAsLogging(1);
