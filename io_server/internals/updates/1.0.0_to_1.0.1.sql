\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql

\i ./functions/contribs/readd_contribs.sql

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/categories/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql

select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/zarya22/readd_zarya_func.sql

drop table io_life_cycle;

alter table io_states add column is_system bool;
update io_states set is_system = true;
alter table io_states alter column is_system set not null;
alter table io_states alter column is_system set default false;

insert into io_states (id, name, description, is_system) values (5, 'Произошло изменение значений атрибутов', NULL, true);
--предусматриваем возможность в дальнейшем безболезненно добавлять новые системные состояния
SELECT pg_catalog.setval('io_states_id_seq', 40, true);

--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-195', 195, 10, NULL, false, 'Жизненные циклы категорий', 'SYSCATEGORY_195', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-196', 196, 8, 195, true, 'Справочник жизненных циклов категорий', 'SYSCATEGORY_196', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-197', 197, 10, NULL, false, 'Переходы состояний в жизненных циклах', 'SYSCATEGORY_197', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-198', 198, 8, 197, true, 'Справочник переходов состояний в жизненных циклах', 'SYSCATEGORY_198', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-199', 199, 10, NULL, false, 'Сервисы-обработчики очередей', 'SYSCATEGORY_199', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-200', 200, 8, 199, true, 'Справочник сервисов-обработчиков очередей', 'SYSCATEGORY_200', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-201', 201, 10, NULL, false, 'Описания очередей обработки', 'SYSCATEGORY_201', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-202', 202, 8, 201, true, 'Справочник описаний очередей обработки', 'SYSCATEGORY_202', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-203', 203, 10, NULL, false, 'Очереди обработки (данные для обработки)', 'SYSCATEGORY_203', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-204', 204, 8, 203, true, 'Справочник очередей обработки (данные для обработки)', 'SYSCATEGORY_204', NULL::varchar, true, true, 1);

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

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (581, 1, 15, 'false', true, false);--is_system
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (582, 11, 323, NULL, false, false);--id_life_cycle

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


select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

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


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--СЮДА ЗАПРОСЫ НА СОЗДАНИЕ ТАБЛИЦ СПРАВОНИКОВ (ЕСЛИ СОЗДАВАЛИСЬ ИО)

create table life_cycle (
   id                   SERIAL               not null,
   id_start_state       INT4                 null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_LIFE_CYCLE primary key (id)
)
inherits (root_table);

comment on table life_cycle is
'Жизненный цикл
определяет набор состояний ИО, а также последовательность переходов между ними
ЖЦ является характеристикой категории. Соответственно ИО может иметь только те состояния, которые определены в ЖЦ его категории.';

comment on column life_cycle.id_start_state is
'Начальное состояние, в котором создаются ИО заданной категории, имеющей данный жизненный цикл';

select setMacToNULL('life_cycle');
select createTriggerUID('life_cycle');

alter table life_cycle
   add constraint FK_LIFE_CYC_REFERENCE_IO_STATE foreign key (id_start_state)
      references io_states (id)
      on delete restrict on update restrict;

create table life_cycle_io_states (
   id_life_cycle        INT4                 not null,
   id_io_states         INT4                 not null,
   constraint PK_LIFE_CYCLE_IO_STATES primary key (id_life_cycle, id_io_states)
);

comment on table life_cycle_io_states is
'Набор допустимых состояний в данном жизненном цикле.
Данная таблица определяет лишь набор допустимых состояний.
Последовательность переходов между состояниями определяется таблицей переходов';

comment on column life_cycle_io_states.id_life_cycle is
'Жизненный цикл';

comment on column life_cycle_io_states.id_io_states is
'состояние, в него входящее';

select setMacToNULL('life_cycle_io_states');

alter table life_cycle_io_states
   add constraint FK_LIFE_CYC_REFERENCE_LIFE_CYC foreign key (id_life_cycle)
      references life_cycle (id)
      on delete restrict on update restrict;

alter table life_cycle_io_states
   add constraint FK_LIFE_CYC_REFERENCE_IO_STATE foreign key (id_io_states)
      references io_states (id)
      on delete restrict on update restrict;

create table state_crosses (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   id_life_cycle        INT4                 not null,
   id_state_src         INT4                 not null,
   id_state_dest        INT4                 not null,
   constraint PK_STATE_CROSSES primary key (id)
)
inherits (root_table);

comment on table state_crosses is
'таблица содержит возможные переходы состояний ИО в данном жизненном цикле
Состояния должны входить в жизненный цикл категории!
ИО могут переходить из любого состояния в любое. Наличие соответствующей записи в данной таблице означает лишь то, что ИО после обработки в исходном жизненном цикле должен перейти в заданное состояние (и соответственно поставлен в очередь обработки, если таковая назначена для целевого состояния)';

comment on column state_crosses.id_life_cycle is
'в каком жизненном цикле переход допустим';

comment on column state_crosses.id_state_src is
'Исходное состояние';

comment on column state_crosses.id_state_dest is
'Состояние, в которое можно перейти из исходного';

select setMacToNULL('state_crosses');
select createTriggerUID('state_crosses');

/*==============================================================*/
/* Index: i_lc_states                                           */
/*==============================================================*/
create unique index i_lc_states on state_crosses (
id_life_cycle,
id_state_src,
id_state_dest
);

alter table state_crosses
   add constraint FK_IO_LIFE__REF_IO_STATE_SRC foreign key (id_state_src)
      references io_states (id)
      on delete restrict on update restrict;

alter table state_crosses
   add constraint FK_IO_LIFE__REF_IO_STATE_DST foreign key (id_state_dest)
      references io_states (id)
      on delete restrict on update restrict;

alter table state_crosses
   add constraint FK_STATE_CR_REFERENCE_LIFE_CYC foreign key (id_life_cycle)
      references life_cycle (id)
      on delete restrict on update restrict;


create table handlers (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   service              VARCHAR              not null,
   extra_params         VARCHAR              null,
   constraint PK_HANDLERS primary key (id)
)
inherits (root_table);

comment on table handlers is
'Сервисы-обработчики ИО, которые попали в определенную очередь обработки.
Для каждой очереди может быть определен только один сервис-обработчик.
Сервис-обработчик представляет собой автономную подпрограмму, которая получает на вход идентификатор записи в таблице очередей и приступает к ообработке ИО, определяемого данным идентификатором.';

comment on column handlers.service is
'параметры сервиса (сигнатура сервиса)
Каждый сервис должен иметь следующие вх. параметры:
-Идентификатор записи таблицы очередей, инициировавшей обработчик
Каждый сервис при его вызове обязан:
-произвести отметку записи в таблице очередей, когда он начал обработку
-произвести отметку записи в таблице очередей, когда он завершил обработку
-выдать результат обработки в выходную очередь (в общем случае- в очереди)
';

comment on column handlers.extra_params is
'Дополнительные параметры для запуска (если требуются)';

select setMacToNULL('handlers');
select createTriggerUID('handlers');

create table chains (
   id                   SERIAL               not null,
   id_handler           INT4                 not null,
   id_io_state          INT4                 not null,
   id_io_category       INT4                 not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_CHAINS primary key (id)
)
inherits (root_table);

comment on table chains is
'Таблица описаний очередей,  в которые направляются ИО заданной категории и в заданном состоянии для обработки (ссылка на таблицу состояний и категорий).
Сами очереди (ИО в них попавшие) содержатся в таблице очередей (chains_data).
Для обработки ИО заданной категории и находящихся в заданном состоянии допустима только одна очередь.
Поэтому поля "категория"  и "состояние" объявлены уникальным индексом.
В итоге мы добиваемся независимости группы понятий "жизненный цикл" + "состояние" от группы понятий "очередь" + "сервис обработки очереди".
';

comment on column chains.id_handler is
'обработчик очереди (ссылка на таблицу сервисов-обработчиков)
Каждая очередь может иметь только один обработчик
';

comment on column chains.id_io_state is
'Состояние ИО, в котором информационные объекты поступают в данную очередь для обработки.';

comment on column chains.id_io_category is
'Категория, информационные объекты которой обрабатываются данной очередью';

comment on column chains.name is
'название очереди';

select setMacToNULL('chains');
select createTriggerUID('chains');

/*==============================================================*/
/* Index: i_ch_cat_state                                        */
/*==============================================================*/
create unique index i_ch_cat_state on chains (
id_io_state,
id_io_category
);

alter table chains
   add constraint FK_CHAINS_REFERENCE_HANDLERS foreign key (id_handler)
      references handlers (id)
      on delete restrict on update restrict;

alter table chains
   add constraint FK_CHAINS_REFERENCE_IO_STATE foreign key (id_io_state)
      references io_states (id)
      on delete restrict on update restrict;

alter table chains
   add constraint FK_CHAINS_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;


create table chains_data (
   id                   SERIAL               not null,
   id_chain             INT4                 not null,
   id_record            INT8                 null,
   id_io_object         INT4                 null,
   insert_time          TIMESTAMP            not null default CURRENT_TIMESTAMP,
   is_handled           INT2                 not null default 0
      constraint CKC_IS_HANDLED_CHAINS_D check (is_handled in (0,1)),
   handled_time         TIMESTAMP            null,
   start_service_time   TIMESTAMP            null,
   end_service_time     TIMESTAMP            null,
   return_code          INT4                 null,
   constraint PK_CHAINS_DATA primary key (id)
)
inherits (root_table);

comment on table chains_data is
'Таблица очередей.
Содержит ссылки на ИО или ЭИО, которые попали в очередь обработки.
Одновременно должен быть задан только ИО или ЭИО';

comment on column chains_data.id_chain is
'Ссылка на описание очереди обработки';

comment on column chains_data.id_record is
'ссылка на ЭИО, записанный в очередь (запись таблицы справочника)';

comment on column chains_data.id_io_object is
'ссылка на ИО, записанный в очередь (тип-документ)';

comment on column chains_data.insert_time is
'дата, время создания записи в таблице очередей (формируется автоматически при создании записи)';

comment on column chains_data.is_handled is
'признак обработки записи хранимой процедурой (в исходном состоянии=0, после обработки=1). Состояние=1 свидетельствует о том, что хранимая процедура обработки данной таблицы вызвала необходимый сервис обработки и передала ему исходные данные для обработки(номер записи в таблице очередей)';

comment on column chains_data.handled_time is
'дата, время обработки данной записи хранимой процедурой (триггером)';

comment on column chains_data.start_service_time is
'дата, время начала работы сервиса обработки очереди (формируется сервисом)';

comment on column chains_data.end_service_time is
'дата, время завершения  работы сервиса обработки очереди (формируется сервисом)';

comment on column chains_data.return_code is
'код возврата сервиса обработки';

select setMacToNULL('chains_data');
select createTriggerUID('chains_data');

alter table chains_data
   add constraint FK_CHAINS_D_REFERENCE_CHAINS foreign key (id_chain)
      references chains (id)
      on delete restrict on update restrict;

alter table chains_data
   add constraint FK_CHAINS_D_REFERENCE_Q_BASE_T foreign key (id_record)
      references q_base_table (id)
      on delete restrict on update restrict;

alter table chains_data
   add constraint FK_CHAINS_D_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;


alter table io_categories add column id_life_cycle int4;
alter table io_categories
   add constraint FK_IO_CATEG_REFERENCE_LIFE_CYC foreign key (id_life_cycle)
      references life_cycle (id)
      on delete restrict on update restrict;

----------
----------


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ

--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
