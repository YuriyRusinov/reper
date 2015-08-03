\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql


--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql



select setAsLogging(1);
select setAsLogging(2);


\i ./functions/readd_functions.sql
\i ./functions/security/readd_security.sql

\i ./functions/tasks/zarya22/readd_zarya_func.sql

--ячдю (лефдс сдюкемхел х янгдюмхел рпхццепю) онлеыюел гюопняш мю янгдюмхе йюрецнпхи (днаюбкърэ ярпнйх б ACCESS_CATEGORIES_TABLE ме мюдн)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-177', 177, 10, NULL, false, 'рХОШ ОНЙЮГЮРЕКЕИ', 'SYSCATEGORY_177', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-178', 178, 8, 177, true, 'яОПЮБНВМХЙ РХОНБ ОНЙЮГЮРЕКЕИ', 'SYSCATEGORY_178', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-179', 179, 10, NULL, false, 'оНЙЮГЮРЕКХ', 'SYSCATEGORY_179', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-180', 180, 8, 179, true, 'яОПЮБНВМХЙ ОНЙЮГЮРЕКЕИ', 'SYSCATEGORY_180', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-181', 181, 10, NULL, false, 'гМЮВЕМХЪ ОНЙЮГЮРЕКЕИ', 'SYSCATEGORY_181', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-182', 182, 8, 181, true, 'яОПЮБНВМХЙ ГМЮВЕМХИ ОНЙЮГЮРЕКЕИ', 'SYSCATEGORY_182', NULL::varchar, true, true, 1);

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (177, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (177, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (178, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (178, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (179, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (179, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (180, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (180, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (181, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (181, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (182, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (182, 2, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-291', 291, 2, 'id_indicator_type', 'рХО ОНЙЮГЮРЕКЪ', 'рХО ОНЙЮГЮРЕКЪ', 'indicator_type', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-292', 292, 2, 'id_io_object', 'нАЗЕЙР', 'нАЗЕЙР', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-293', 293, 2, 'id_io_object_src', 'хМТНПЛЮЖХЧ ОЕПЕДЮК', 'хМТНПЛЮЖХЧ ОЕПЕДЮК', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-294', 294, 2, 'id_io_object_src1', 'гЮ ЙНЦН ОЕПЕДЮК', 'гЮ ЙНЦН ОЕПЕДЮК', 'io_objects', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-295', 295, 5, 'meas_time', 'бПЕЛЪ ХГЛЕПЕМХЪ', 'бПЕЛЪ ХГЛЕПЕМХЪ', NULL, NULL, 120, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-295', 295, 5, 'meas_time', 'бПЕЛЪ ХГЛЕПЕМХЪ', 'бПЕЛЪ ХГЛЕПЕМХЪ', NULL, NULL, 120, TRUE);
--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-296', 296, 9, 'value', 'гМЮВЕМХЕ', 'гМЮВЕМХЕ', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-297', 297, 5, 'start_time', 'юЙРСЮКЕМ Я', 'юЙРСЮКЕМ Я', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-298', 298, 5, 'stop_time', 'юЙРСЮКЕМ ОН', 'юЙРСЮКЕМ ОН', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-299', 299, 2, 'id_indicator', 'хМДХЙЮРНП', 'хМДХЙЮРНП', 'indicator', 'name', 150, TRUE);


--РЮАКХЖЮ РХОНБ ОНЙЮГЮРЕКЕИ
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (177, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (177, 2, NULL, true, false); --name

--РЮАКХЖЮ ОНЙЮГЮРЕКЕИ
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 3, NULL, false, false); --desc
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 291, NULL, true, false); --id_indicator_type
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 27, NULL, false, false); --id_parent
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 5, NULL, true, false); --id_a_type
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 8, NULL, false, false); --table_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (179, 9, NULL, false, false); --column_name

--РЮАКХЖЮ ГМЮВЕМХИ ОНЙЮГЮРЕКЕИ
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 286, NULL, true, false); --the_value
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 292, NULL, true, false); --id_io_object
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 293, NULL, false, false); --id_io_object_src
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 294, NULL, false, false); --id_io_object_src1
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 295, NULL, true, false); --meas_time
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 16, NULL, true, false); --insert_time
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 297, NULL, true, false); --start_time
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 298, NULL, false, false); --stop_time
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (181, 299, NULL, true, false); --id_indicator

--ячдю (лефдс сдюкемхел х янгдюмхел рпхццепнб) онлеыюел гюопняш мю янгдюмхе хн (мюдн днаюбкърэ ярпнйх б ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-86', 86, 178, 1, 1, 'яОПЮБНВМХЙ РХОНБ ОНЙЮГЮРЕКЕИ', 'indicator_type', NULL, 'яХЯРЕЛМШИ НАЗЕЙР', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (86, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-87', 87, 180, 1, 1, 'яОПЮБНВМХЙ ОНЙЮГЮРЕКЕИ', 'indicator', NULL, 'яХЯРЕЛМШИ НАЗЕЙР', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (87, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-88', 88, 182, 1, 1, 'яОПЮБНВМХЙ ГМЮВЕМХИ ОНЙЮГЮРЕКЕИ', 'indicators_values', NULL, 'яХЯРЕЛМШИ НАЗЕЙР', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (88, -10, TRUE, TRUE, FALSE, FALSE);

select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

create table indicator (
   id                   SERIAL               not null,
   id_indicator_type    INT4                 not null,
   id_parent            INT4                 null,
   id_a_type            INT4                 not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_INDICATOR primary key (id)
)
inherits (root_table);

comment on table indicator is
'Таблица показателей информационных объектов.
Для соответствия модели Заря22 введен такой тип информации как показатели информационных объектов.
Отличаются от обычных атрибутов тем, что имеют набор более расширенных характеристик. А также сохраняют историю значений';

select setMacToNULL('indicator');
select createTriggerUID('indicator');

create table indicator_type (
   id                   SERIAL               not null,
   name                 VARCHAR              null,
   constraint PK_INDICATOR_TYPE primary key (id)
)
inherits (root_table);

select setMacToNULL('indicator_type');
select createTriggerUID('indicator_type');

create table indicators_values (
   id                   SERIAL               not null,
   id_io_object         INT4                 not null,
   id_indicator         INT4                 not null,
   id_io_object_src     INT4                 null,
   id_io_object_src1    INT4                 null,
   meas_time            TIMESTAMP            not null,
   insert_time          TIMESTAMP            not null,
   the_value            VARCHAR              not null,
   start_time           TIMESTAMP            not null,
   stop_time            TIMESTAMP            null
)
inherits (root_table);

comment on table indicators_values is
'Значения показателей для информационных объектов';

comment on column indicators_values.id_io_object_src is
'Идентификатор ИО, который явился источником информации о значении показателя.';

comment on column indicators_values.id_io_object_src1 is
'Идентификатор объекта, за кого передали информацию. Если источник передаёт информацию сам за себя, то значение в этом поле совпадает со значением в поле <Источник>';

comment on column indicators_values.meas_time is
'Дата и время измерения значения показателя';

comment on column indicators_values.insert_time is
'Дата и время доведения значения показателя в систему';

comment on column indicators_values.the_value is
'Значение показателя';

comment on column indicators_values.start_time is
'Дата и время начала актуальности показателя';

comment on column indicators_values.stop_time is
'Дата и время окончания актуальности показателя';

select setMacToNULL('indicators_values');
select createTriggerUID('indicators_values');

alter table indicator
   add constraint FK_INDICATO_REFERENCE_INDICATO foreign key (id_indicator_type)
      references indicator_type (id)
      on delete restrict on update restrict;

alter table indicator
   add constraint FK_INDICATO_REFERENCE_PARENT foreign key (id_parent)
      references indicator (id)
      on delete restrict on update restrict;

alter table indicator
   add constraint FK_INDICATO_REFERENCE_A_TYPES foreign key (id_a_type)
      references a_types (id)
      on delete restrict on update restrict;

alter table indicators_values
   add constraint FK_INDICATO_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references tbl_io_objects (id)
      on delete restrict on update restrict;

alter table indicators_values
   add constraint FK_INDICATOR_IO_OBJECT foreign key (id_indicator)
      references indicator (id)
      on delete restrict on update restrict;

alter table indicators_values
   add constraint FK_INDICATO_IO_OBJECT_SRC foreign key (id_io_object_src)
      references tbl_io_objects (id)
      on delete restrict on update restrict;

alter table indicators_values
   add constraint FK_INDICATO_IO_OBJECT_SRC1 foreign key (id_io_object_src1)
      references tbl_io_objects (id)
      on delete restrict on update restrict;

--бяе нярюкэмне гдеяэ


commit;
