\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql
\i ./functions/general/createtriggeruidex.sql





\i ./functions/contribs/readd_contribs.sql

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/categories/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql

alter table chains_data drop column if exists handler_in_data;
alter table chains_data drop column if exists handler_out_data;
alter table chains_data add column handler_in_data varchar;
alter table chains_data add column handler_out_data varchar;
alter table chains_data add column lc_is_parent bool;
update chains_data set lc_is_parent = true;
alter table chains_data alter column lc_is_parent set default true;
alter table chains_data alter column lc_is_parent set not null;

create table handler_params (
   id                   SERIAL               not null,
   name                 VARCHAR              null,
   handler_version      VARCHAR              not null,
   h_host               VARCHAR              null,
   h_port               int4                 null,
   service              VARCHAR              not null,
   extra_params         VARCHAR              null,
   handler_in_data      VARCHAR              null,
   constraint PK_HANDLER_PARAMS primary key (id)
)
inherits (root_table);

comment on table handler_params is
'Параметры сервисов-обработчиков
Обеспечивают версионность. 
Вынесение параметров сервиса в отдельную таблицу позволяет задавать для обработки очереди некоторый фиксированный сервис, а потом осуществлять ведение его различных версий';

comment on column handler_params.name is
'название сервиса';

comment on column handler_params.handler_version is
'версия сервиса';

comment on column handler_params.h_host is
'IP-адрес хоста, на котором функционирует сервис-обработчик';

comment on column handler_params.h_port is
'Порт хоста, на котором функционирует сервис-обработчик';

comment on column handler_params.service is
'параметры сервиса (сигнатура сервиса)
Каждый сервис должен иметь следующие вх. параметры:
-Идентификатор записи таблицы очередей, инициировавшей обработчик
Каждый сервис при его вызове обязан:
-произвести отметку записи в таблице очередей, когда он начал обработку
-произвести отметку записи в таблице очередей, когда он завершил обработку
-выдать результат обработки в выходную очередь (в общем случае- в очереди)
';

comment on column handler_params.extra_params is
'Дополнительные параметры для запуска (если требуются)';

comment on column handler_params.handler_in_data is
'XML-атрибут сервиса';

select setMacToNULL('handler_params');
select createTriggerUID('handler_params');

alter table handlers drop column h_host;
alter table handlers drop column h_port;
alter table handlers drop column service;
alter table handlers drop column extra_params;
alter table handlers add column id_handler_params int4;
alter table handlers
   add constraint FK_HANDLERS_REFERENCE_HANDLER_ foreign key (id_handler_params)
      references handler_params (id)
      on delete restrict on update restrict;


/*==============================================================*/
/* Table: logictic                                              */
/*==============================================================*/
create table logistic (
   id                   SERIAL               not null,
   name                 VARCHAR              null,
   id_processing_scenario INT4                 not null,
   id_processing_variant INT4                 not null,
   id_io_object         INT4                 not null,
   id_handler           INT4                 not null,
   return_code          INT4                 not null,
   handler_in_data      VARCHAR              null,
   handler_out_data     VARCHAR              null,
   description          VARCHAR              null,
   constraint PK_LOGISTIC primary key (id)
)
inherits (root_table);

comment on table logistic is
'Управляющая таблица, обеспечивающая реализацию логики обработки ИО различных категорий цепочкой взаимосвязанных сервисов (т.е. обеспечивающая вызов необходимых для дальнейшей обработки сервисов в зависимости от результата обработки текущим сервисом)

данная таблица фактически несет следующий смысл:
Если Активным является некоторый зафиксированный сценарий обработки и в chains_data некоторым сервисом обработан заданный ИО (ЭИО), и при этом код возврата равен заданному, то необходимо данный ИО (ЭИО) поместить в указанную очередь (набор очередей)';

comment on column logistic.id_processing_scenario is
'Сценарий обработки (для различных сценариев могут быть различные последовательности)
';

comment on column logistic.id_processing_variant is
'Вариант обработки';

comment on column logistic.id_io_object is
'ИО';

comment on column logistic.id_handler is
'Сервис (ссылка на таблицу каталога сервисов)
';

comment on column logistic.handler_in_data is
'XML-вх. атрибут сервиса (входные параметры сервиса)';

comment on column logistic.handler_out_data is
'XML-атрибут сервиса (возвращаемые параметры)
';

select setMacToNULL('logistic');
select createTriggerUID('logistic');

/*==============================================================*/
/* Index: i_main_index                                          */
/*==============================================================*/
create unique index i_main_index on logistic (
id_processing_scenario,
id_processing_variant,
id_io_object,
id_handler,
return_code
);

alter table logistic
   add constraint FK_LOGISTIC_REF_PROCESSING_SCENARIO foreign key (id_processing_scenario)
      references processing_scenario (id)
      on delete restrict on update restrict;

alter table logistic
   add constraint FK_LOGISTIC_REF_PROCESSING_VARIANT foreign key (id_processing_variant)
      references processing_variant (id)
      on delete restrict on update restrict;

alter table logistic
   add constraint FK_LOGISTIC_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references tbl_io_objects (id)
      on delete restrict on update restrict;

alter table logistic
   add constraint FK_LOGISTIC_REFERENCE_HANDLERS foreign key (id_handler)
      references handlers (id)
      on delete restrict on update restrict;

/*==============================================================*/
/* Table: logistic_chains                                       */
/*==============================================================*/
create table logistic_chains (
   id_chains            INT4                 not null,
   id_logistic          INT4                 not null,
   constraint PK_LOGISTIC_CHAINS primary key (id_chains, id_logistic)
);

comment on table logistic_chains is
'Перечень очередей для
последующей обработки ИО.
(для одного и того-же кода возврата может быть несколько очередей обработки)
';

select setMacToNULL('logistic_chains');

alter table logistic_chains
   add constraint FK_LOGISTIC_REFERENCE_CHAINS foreign key (id_chains)
      references chains (id)
      on delete restrict on update restrict;

alter table logistic_chains
   add constraint FK_LOGISTIC_REFERENCE_LOGISTIC foreign key (id_logistic)
      references logistic (id)
      on delete restrict on update restrict;


/*==============================================================*/
/* Table: histogram_params_streams                              */
/*==============================================================*/
create table histogram_params_streams (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   scenarios            INT4[]               null,
   variants             INT4[]               null,
   dl_froms             INT4[]               null,
   dl_tos               INT4[]               null,
   io_categories        INT4[]               null,
   io_objects           INT4[]               null,
   partition_lows       INT4[]               null,
   constraint PK_HISTOGRAM_PARAMS_STREAMS primary key (id)
)
inherits (root_table);

comment on table histogram_params_streams is
'Таблица запросов для построения гистограмм по потокам сообщений';

select setMacToNULL('histogram_params_streams');
select createTriggerUID('histogram_params_streams');


/*==============================================================*/
/* Table: histogram_params_chains                               */
/*==============================================================*/
create table histogram_params_chains (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   scenarios            INT4[]               null,
   variants             INT4[]               null,
   life_cycles          INT4[]               null,
   io_objects           INT4[]               null,
   services             INT4[]               null,
   constraint PK_HISTOGRAM_PARAMS_CHAINS primary key (id)
)
inherits (root_table);

comment on table histogram_params_chains is
'Таблица запросов для построения гистограмм по сервисам';

select setMacToNULL('histogram_params_chains');
select createTriggerUID('histogram_params_chains');

/*==============================================================*/
/* Table: histogram_graphics_streams                            */
/*==============================================================*/
create table histogram_graphics_streams (
   id                   SERIAL               not null,
   id_histogram_params_streams INT4                 not null,
   h_order              INT4                 not null,
   h_x                  float8               not null,
   h_y                  float8               not null,
   constraint PK_HISTOGRAM_GRAPHICS_STREAMS primary key (id)
)
inherits (root_table);

comment on table histogram_graphics_streams is
'Таблица графиков гистограмм';

select setMacToNULL('histogram_graphics_streams');
select createTriggerUID('histogram_graphics_streams');

alter table histogram_graphics_streams
   add constraint FK_HISTOGRAM_REF_HIST_STREAMS foreign key (id_histogram_params_streams)
      references histogram_params_streams (id)
      on delete restrict on update restrict;

/*==============================================================*/
/* Table: histogram_graphics_chains                             */
/*==============================================================*/
create table histogram_graphics_chains (
   id                   SERIAL               not null,
   id_histogram_params_chains INT4                 not null,
   h_order              INT4                 not null,
   h_x                  float8               not null,
   h_y                  float8               not null,
   constraint PK_HISTOGRAM_GRAPHICS_CHAINS primary key (id)
)
inherits (root_table);

comment on table histogram_graphics_chains is
'Таблица графиков гистограмм для сервисов';

select setMacToNULL('histogram_graphics_chains');
select createTriggerUID('histogram_graphics_chains');

alter table histogram_graphics_chains
   add constraint FK_HISTOGRA_REFERENCE_HISTOGRA foreign key (id_histogram_params_chains)
      references histogram_params_chains (id)
      on delete restrict on update restrict;


select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/readd_tasks_func.sql

--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');
select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-211', 211, 10, NULL, false, 'Параметры сервисов-обработчиков', 'SYSCATEGORY_211', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-212', 212, 8, 211, true, 'Справочник параметров сервисов-обработчиков ', 'SYSCATEGORY_212', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-213', 213, 10, NULL, false, 'Параметры логики обработки ИО сервисами', 'SYSCATEGORY_213', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-214', 214, 8, 213, true, 'Справочник параметров логики обработки ИО сервисами', 'SYSCATEGORY_214', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-215', 215, 10, NULL, false, 'Сценарии обработки ИО', 'SYSCATEGORY_215', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-216', 216, 8, 215, true, 'Справочник сценариев обработки ИО', 'SYSCATEGORY_216', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-217', 217, 10, NULL, false, 'Варианты обработки ИО', 'SYSCATEGORY_217', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-218', 218, 8, 217, true, 'Справочник вариантов обработки ИО', 'SYSCATEGORY_218', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-219', 219, 3, NULL, true, 'Текущие вариант и сценарий обработки ИО (Пядь)', 'SYSCATEGORY_219', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-220', 220, 10, NULL, false, 'Параметры гистограмм для потоков', 'SYSCATEGORY_220', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-221', 221, 8, 220, true, 'Справочник параметров гистограмм для потоков', 'SYSCATEGORY_221', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-222', 222, 10, NULL, false, 'Параметры гистограмм для сервисов', 'SYSCATEGORY_222', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-223', 223, 8, 222, true, 'Справочник параметров гистограмм для сервисов', 'SYSCATEGORY_223', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-224', 224, 10, NULL, false, 'Графики гистограмм для потоков', 'SYSCATEGORY_224', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-225', 225, 8, 224, true, 'Справочник графиков гистограмм для потоков', 'SYSCATEGORY_225', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-226', 226, 10, NULL, false, 'Графики гистограмм для сервисов', 'SYSCATEGORY_226', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-227', 227, 8, 226, true, 'Справочник графиков гистограмм для сервисов', 'SYSCATEGORY_227', NULL::varchar, true, true, 1);


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-350', 350, 1, 'lc_is_parent', 'ЖЦ является инициатором', 'ЖЦ является инициатором', NULL, NULL, 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-351', 351, 17, 'logistic_chains', 'Перечень очередей для последующей обработки ИО', 'Перечень очередей для последующей обработки ИО', 'chains', 'name', 200, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-352', 352, 2, 'id_processing_scenario', 'Сценарий обработки', 'Сценарий обработки', 'processing_scenario', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-353', 353, 2, 'id_processing_variant', 'Вариант обработки', 'Вариант обработки', 'processing_variant', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-354', 354, 8, 'what_happens', 'Код события', 'Код события', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-355', 355, 13, 'handler_in_data', 'Входящие данные сервиса', 'Входящие данные сервиса', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-356', 356, 13, 'handler_out_data', 'Исходящие данные сервиса', 'Исходящие данные сервиса', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-357', 357, 9, 'handler_version', 'Версия сервиса', 'Версия сервиса', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-358', 358, 2, 'id_handler_params', 'Параметры сервиса-обработчика', 'Параметры сервиса-обработчика', 'handler_params', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-359', 359, 3, 'id_parent', 'Родитель', 'Родитель', NULL, 'id', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-360', 360, 2, 'id_histogram_params_streams', 'Параметры гистограммы', 'Параметры гистограммы', 'histogram_params_streams', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-361', 361, 2, 'id_histogram_params_chains', 'Параметры гистограммы', 'Параметры гистограммы', 'histogram_params_chains', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-362', 362, 8, 'h_order', 'Порядок следования', 'Порядок следования', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-363', 363, 6, 'h_x', 'Абсцисса', 'Абсцисса', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-364', 364, 6, 'h_y', 'Ордината', 'Ордината', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-365', 365, 12, 'scenarios', 'Сценарии', 'Сценарии', 'processing_scenario', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-366', 366, 12, 'variants', 'Варианты', 'Варианты', 'processing_variant', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-367', 367, 12, 'dl_froms', 'Источники', 'Источники', 'position', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-368', 368, 12, 'dl_tos', 'Получатели', 'Получатели', 'position', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-369', 369, 12, 'io_categories', 'Категории', 'Категории', 'io_categories', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-370', 370, 12, 'io_objects', 'ИО', 'ИО', 'io_objects', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-371', 371, 12, 'partition_lows', 'Законы распределения', 'Законы распределения', 'partition_lows', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-372', 372, 12, 'life_cycles', 'ЖЦ', 'ЖЦ', 'life_cycle', 'name', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-373', 373, 12, 'services', 'Сервисы', 'Сервисы', 'handlers', 'name', 100, TRUE);


insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (636, 203, 359, NULL, false, false);  --id_parent
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (637, 203, 352, NULL, true, false);  --id_processing_scenario
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (638, 203, 353, NULL, true, false);  --id_processing_variant
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (639, 203, 354, NULL, true, false);  --what_happens
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (640, 203, 355, NULL, false, false);  --handler_in_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (641, 203, 356, NULL, false, false);  --handler_out_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (642, 203, 350, 'TRUE', true, false);  --lc_is_parent

--изменение справочника сервисов-обработчиков
delete from attrs_categories where id in (563, 564, 592, 593);

--таблица параметров сервисов-обработчиков
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (610, 211, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (611, 211, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (612, 211, 357, NULL, true, false);  --handler_version
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (613, 211, 326, NULL, true, false);  --service
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (614, 211, 327, NULL, false, false);  --extra_params
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (615, 211, 341, NULL, false, false);  --h_host
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (616, 211, 342, NULL, false, false);  --h_port
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (617, 211, 355, NULL, false, false);  --handler_in_data

--таблица сервисов-обработчиков
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (609, 199, 358, NULL, true, false);  --id_handler_params

--таблица logistic
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (618, 213, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (619, 213, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (620, 213, 352, NULL, true, false); --id_processing_scenario
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (621, 213, 353, NULL, true, false); --id_processing_variant
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (622, 213, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (623, 213, 328, NULL, true, false);  --id_handler
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (624, 213, 335, NULL, false, false);  --return_code
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (625, 213, 355, NULL, false, false);  --handler_in_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (626, 213, 356, NULL, false, false);  --handler_out_data
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (627, 213, 3, NULL, false, false);  --description
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (643, 213, 351, NULL, false, false);  --logistic_chains

--таблица сценариев обработки
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (628, 215, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (629, 215, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (630, 215, 3, NULL, false, false);  --description

--таблица вариантов обработки
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (631, 217, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (632, 217, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (633, 217, 3, NULL, false, false);  --description

--настроечный ИО (Пядь)
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (634, 219, 352, NULL, true, false); --id_processing_scenario
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (635, 219, 353, NULL, true, false); --id_processing_variant

--таблица параметров гистограмм для потоков
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (644, 220, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (645, 220, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (646, 220, 365, NULL, false, false); --scenarios
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (647, 220, 366, NULL, false, false); --variants
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (648, 220, 367, NULL, false, false); --dl_froms
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (649, 220, 368, NULL, false, false); --dl_tos
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (650, 220, 369, NULL, false, false); --io_categories
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (651, 220, 370, NULL, false, false); --io_objects
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (652, 220, 371, NULL, false, false); --partition_lows

--таблица параметров гистограмм для сервисов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (653, 222, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (654, 222, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (655, 222, 365, NULL, false, false); --scenarios
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (656, 222, 366, NULL, false, false); --variants
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (657, 222, 372, NULL, false, false); --life_cycles
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (658, 222, 370, NULL, false, false); --io_objects
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (659, 222, 373, NULL, false, false); --services

--таблица графиков гистограмм для потоков
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (660, 224, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (661, 224, 360, NULL, true, false); --id_histogram_params_streams
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (662, 224, 362, NULL, true, false); --h_order
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (663, 224, 363, NULL, true, false); --h_x
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (664, 224, 364, NULL, true, false); --h_y

--таблица графиков гистограмм для сервисов
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (665, 226, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (666, 226, 361, NULL, true, false); --id_histogram_params_chains
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (667, 226, 362, NULL, true, false); --h_order
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (668, 226, 363, NULL, true, false); --h_x
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (669, 226, 364, NULL, true, false); --h_y


select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-103', 103, 212, 1, 1, 'Справочник параметров сервисов-обработчиков', 'handler_params', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (103, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-104', 104, 214, 1, 1, 'Справочник параметров логики обработки ИО сервисами', 'logistic', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (104, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-105', 105, 216, 1, 1, 'Справочник сценариев обработки ИО', 'processing_scenario', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (105, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-106', 106, 218, 1, 1, 'Справочник вариантов обработки ИО', 'processing_variant', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (106, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-107', 107, 219, 1, 1, 'Настроечный ИО (Пядь)', NULL, NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (107, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-108', 108, 221, 1, 1, 'Справочник параметров гистограмм для потоков', 'histogram_params_streams', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (108, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-109', 109, 223, 1, 1, 'Справочник параметров гистограмм для сервисов', 'histogram_params_chains', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (109, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-110', 110, 225, 1, 1, 'Справочник графиков гистограмм для потоков', 'histogram_graphics_streams', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (110, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-111', 111, 227, 1, 1, 'Справочник графиков гистограмм для сервисов', 'histogram_graphics_chains', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (111, -10, TRUE, TRUE, FALSE, FALSE);


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--СЮДА ЗАПРОСЫ НА СОЗДАНИЕ ТАБЛИЦ СПРАВОНИКОВ (ЕСЛИ СОЗДАВАЛИСЬ ИО)


----------
----------


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ
insert into a_types (id, id_a_view, name, code) values (37, 4, 'Дата-время (с миллисекундами)', 'TIMESTAMP');
SELECT pg_catalog.setval('a_types_id_seq', 37, true);
update attributes set id_a_type = 9 where id in (64, 162);
update attributes set id_a_type = 37 where id in (338, 16, 332, 333, 334);
delete from a_types where id in (16, 22, 29, 34, 35);

update processing_scenario set name = 'Сценарий по умолчанию' where id = 1;
update processing_variant set name = 'Вариант по умолчанию' where id = 1;

update chains_data set what_happens = 1 where what_happens isnull;
alter table chains_data alter column what_happens set not null;
alter table chains_data drop constraint if exists CKC_WHAT_HAPPENS_CHAINS_D;
alter table chains_data add constraint CKC_WHAT_HAPPENS_CHAINS_D check (what_happens in (1,2,3,4,5,6,7,8));


--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
