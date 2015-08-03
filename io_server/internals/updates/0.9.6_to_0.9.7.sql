\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');


insert into segment_types(name) values('moveTo');
insert into segment_types(name) values('lineTo');
insert into segment_types(name) values('quadTo');
insert into segment_types(name) values('cubicTo');


update io_objects set id_owner_org = getLocalOrgId() where id <= 200;
update io_objects set id_io_category = 138 where id = 67;
update io_objects set id_io_category = 136 where id = 68;

update attributes set id_a_type = 23 where id in(155, 177, 178);



insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (117, 155, NULL, false, false); --svg_image

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-187', 187, 9, 'r_name', 'Название (мн.ч.)', 'Название (мн.ч.)', NULL, NULL, 300, TRUE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (3, 187, NULL, true, false);--r_name

alter table io_types add column r_name varchar;
update io_types set r_name = 'Справочники' where id = 1;
update io_types set r_name = 'Журналы' where id = 2;
update io_types set r_name = 'Документы' where id = 3;
update io_types set r_name = 'Формуляры' where id = 4;
update io_types set r_name = 'Сообщения' where id = 5;
update io_types set r_name = 'Сообщения в трафарете изделия 83т20' where id = 6;
update io_types set r_name = 'Шаблоны документов' where id = 7;
update io_types set r_name = 'Системные справочники' where id = 8;
update io_types set r_name = 'Системные справочники категорий документов' where id = 9;
update io_types set r_name = 'Подчиненные категории' where id = 10;


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-188', 188, 5, 'accepted_datetime', 'Дата-время принятия к исполнению', 'Принято к исполнению', NULL, NULL, 120, TRUE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (33, 55, NULL, false, false);--receive_datetime
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (33, 188, NULL, false, false);--accepted_datetime


alter table io_types alter column r_name set not null;

--УЧЕТНАЯ КАРТОЧКА ДОКУМЕНТА
insert into io_types (id, name, r_name, description) values(11, 'Документ в документообороте', 'Документы в документообороте', 'Используется в качестве типа для категорий, экземпляры которых обрабатываются средствами подсистемы электронного документооборота');
SELECT pg_catalog.setval('io_types_id_seq', 11, true);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-143', 143, 10, NULL, false, 'Типы документов', 'SYSCATEGORY_143', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-144', 144, 8, 143, true, 'Справочник типов документов', 'SYSCATEGORY_144', NULL::varchar, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-145', 145, 10, NULL, false, 'Подтипы документов', 'SYSCATEGORY_145', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-146', 146, 8, 145, true, 'Справочник подтипов документов', 'SYSCATEGORY_146', NULL::varchar, true, 1);

--insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-147', 147, 10, NULL, false, 'Учетные карточки документов', 'SYSCATEGORY_147', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-148', 148, 11, NULL, true, 'Учетная карточка документа', 'SYSCATEGORY_148', NULL::varchar, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-149', 149, 10, NULL, false, 'Виды документов', 'SYSCATEGORY_149', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-150', 150, 8, 149, true, 'Справочник видов документов', 'SYSCATEGORY_150', NULL::varchar, true, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (143, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (143, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (144, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (144, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (145, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (145, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (146, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (146, 2, 1);

--insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (147, 1, 2);
--insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (147, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (148, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (148, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (149, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (149, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (150, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (150, 2, 1);

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

--УЧЕТНАЯ КАРТОЧКА ДОКУМЕНТА
--справочник типов документов
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (143, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (143, 2, NULL, true, false); --name

--справочник подтипов документов
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (145, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (145, 2, NULL, true, false); --name

--справочник видов документов
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (149, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (149, 2, NULL, true, false); --name

--учетная карточка (она НЕ является справочником)
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 191, NULL, true, false); --id_exec_org
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 35, NULL, true, false); --id_dl_executor
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 42, NULL, true, false); --id_dl_from
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 205, NULL, true, false); --create_datetime
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 100, NULL, true, false); --output_number
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 193, NULL, true, false); --regout_datetime
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 99, NULL, true, false); --input_number
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 194, NULL, true, false); --regin_datetime
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 189, NULL, true, false); --id_document_type
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 190, NULL, true, false); --id_document_subtype
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 192, NULL, true, false); --id_document_kind
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 195, NULL, true, false); --doc_header
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 196, NULL, true, false); --page_count
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 197, NULL, true, false); --ekz_number
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 198, NULL, true, false); --book_number
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 199, NULL, true, false); --book_subnumber
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 200, NULL, true, false); --class_code
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 201, NULL, true, false); --inv_number
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 202, NULL, true, false); --registrator_info
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 3, NULL, true, false); --desc
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 203, NULL, true, false); --annotation
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (148, 204, NULL, true, false); --command_body


insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-71', 71, 144, 1, 1, 'Справочник типов документов', 'q_document_types', 'document_types', NULL, 'Системный объект', true, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (71, -10, TRUE, TRUE, FALSE, FALSE);

insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-72', 72, 146, 1, 1, 'Справочник подтипов документов', 'q_document_subtypes', 'document_subtypes', NULL, 'Системный объект', true, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (72, -10, TRUE, TRUE, FALSE, FALSE);

insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-73', 73, 150, 1, 1, 'Справочник видов документов', 'q_document_kinds', 'document_kinds', NULL, 'Системный объект', true, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (73, -10, TRUE, TRUE, FALSE, FALSE);


--///////////////////////

create trigger zz_trgZIOInserttableAfter
after insert
on io_objects
for each row
execute procedure ioInsertCheckTableAfter();

select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

commit;
