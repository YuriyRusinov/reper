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


delete from xml_query_responses;
delete from xml_queries;
delete from xml_paths;
delete from xml_formats;
delete from xml_query_types;

select setval('xml_formats_id_seq', 1);
select setval('xml_paths_id_seq', 1);
select setval('xml_query_types_id_seq', 1);
select setval('xml_queries_id_seq', 1);
select setval('xml_query_responses_id_seq', 1);

\i ./functions/init_data/ik_tso_data.sql


--drop table access_cards;

--drop table access_cards_access_plan;

--drop table access_cards_q;

drop table shape_segments;

drop table segment_types;

drop table element_shapes;

drop table shape_types;

drop table graph_styles;

drop table fault_devices;

drop table guard_objects_devices;

drop table guard_objects;

drop table alarm_events;

--drop table alarm_events_q;

--drop table dev_object_q;

drop table devices;

--drop table fault_devices_q;

drop table graph_plans;

--drop table personal_q;

drop table persons;

drop table card_statuses;

drop table card_types;

drop table access_plan;

drop table tso_positions;

drop table tso_units;

/*==============================================================*/
/* User: public                                                 */
/*==============================================================*/
/*==============================================================*/
/* Table: access_cards                                          */
/*==============================================================*/
create table access_cards (
   id                   SERIAL               not null,
   id_card_type         INT4                 not null,
   id_card_status       INT4                 not null,
   id_person            INT4                 null,
   comment              VARCHAR              null,
   code                 VARCHAR              null,
   extra_id             INT4                 null,
   constraint PK_ACCESS_CARDS primary key (id)
)
inherits (root_table);

comment on table access_cards is
'справочник карт доступа (ИК ТСО)';

select setMacToNULL('access_cards');
select createTriggerUID('access_cards');

/*==============================================================*/
/* Table: access_cards_access_plan                              */
/*==============================================================*/
create table access_cards_access_plan (
   id_access_plan       INT4                 not null,
   id_access_cards      INT4                 not null,
   constraint PK_ACCESS_CARDS_ACCESS_PLAN primary key (id_access_plan, id_access_cards)
);

select setMacToNULL('access_cards_access_plan');

/*==============================================================*/
/* Table: access_cards_q                                        */
/*==============================================================*/
create table access_cards_q (
   id                   SERIAL               not null,
   code                 VARCHAR              null,
   comment              VARCHAR              null,
   id_card_type_p       INT4                 null,
   card_type_name       VARCHAR              null,
   id_card_status_p     INT4                 null,
   card_status_name     VARCHAR              null,
   access_plans         INT4[]               null,
   description          VARCHAR              null,
   constraint PK_ACCESS_CARDS_Q primary key (id)
)
inherits (root_table);

comment on table access_cards_q is
'Интегральный справочник карт доступа';

select setMacToNULL('access_cards_q');
select createTriggerUID('access_cards_q');

/*==============================================================*/
/* Table: access_plan                                           */
/*==============================================================*/
create table access_plan (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   constraint PK_ACCESS_PLAN primary key (id)
)
inherits (root_table);

comment on table access_plan is
'Планы доступа (ИК ТСО)';

select setMacToNULL('access_plan');
select createTriggerUID('access_plan');

/*==============================================================*/
/* Table: alarm_events                                          */
/*==============================================================*/
create table alarm_events (
   id                   SERIAL               not null,
   id_person            INT4                 null,
   id_device            INT4                 null,
   event_date           TIMESTAMP            not null,
   message              VARCHAR              not null,
   source               VARCHAR              not null,
   alarm                INT4                 not null,
   card_code            VARCHAR              null,
   pass                 BOOL                 null,
   direction            VARCHAR              null,
   video                bytea                null,
   fault_code           INT4                 null,
   fault_string         VARCHAR              null,
   extra_id             INT4                 not null,
   constraint PK_ALARM_EVENTS primary key (id)
)
inherits (root_table);

comment on table alarm_events is
'Таблица ответов на запросы о тревожных событиях
(в ИК ТСО)';

select setMacToNULL('alarm_events');
select createTriggerUID('alarm_events');

/*==============================================================*/
/* Table: alarm_events_q                                        */
/*==============================================================*/
create table alarm_events_q (
   id                   SERIAL               not null,
   id_person_p          INT4                 null,
   event_date           TIMESTAMP            not null,
   message              VARCHAR              not null,
   source               VARCHAR              not null,
   alarm                INT4                 not null,
   card_code            VARCHAR              null,
   pass                 BOOL                 null,
   direction            VARCHAR              null,
   lastname             VARCHAR              not null,
   firstname            VARCHAR              null,
   sirname              VARCHAR              null,
   id_device_p          INT4                 null,
   dev_name             VARCHAR              null,
   id_unit_p            INT4                 null,
   unit_name            VARCHAR              null,
   id_dolzh_p           INT4                 null,
   dolzh_name           VARCHAR              null,
   constraint PK_ALARM_EVENTS_Q primary key (id)
)
inherits (root_table);

comment on table alarm_events_q is
'Интегральный справочник событий ИК ТСО';

select setMacToNULL('alarm_events_q');
select createTriggerUID('alarm_events_q');

/*==============================================================*/
/* Table: card_statuses                                         */
/*==============================================================*/
create table card_statuses (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   constraint PK_CARD_STATUSES primary key (id)
)
inherits (root_table);

comment on table card_statuses is
'Статусы  карт доступа (ИК ТСО)';

select setMacToNULL('card_statuses');
select createTriggerUID('card_statuses');

/*==============================================================*/
/* Table: card_types                                            */
/*==============================================================*/
create table card_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   constraint PK_CARD_TYPES primary key (id)
)
inherits (root_table);

comment on table card_types is
'Типы карт доступа (ИК ТСО)';

select setMacToNULL('card_types');
select createTriggerUID('card_types');

/*==============================================================*/
/* Table: dev_object_q                                          */
/*==============================================================*/
create table dev_object_q (
   id                   SERIAL               not null,
   id_device_p          INT4                 not null,
   dev_name             VARCHAR              not null,
   id_guard_obj_p       INT4                 not null,
   guard_obj_name       VARCHAR              not null,
   device_svg           XML                  not null,
   plan_svg             XML                  not null,
   is_ok                BOOL                 not null,
   ok_datetime          TIMESTAMP            not null,
   constraint PK_DEV_OBJECT_Q primary key (id)
)
inherits (root_table);

comment on table dev_object_q is
'Интегральный справочник устройств и охраняемых объектов (ИК ТСО)';

select setMacToNULL('dev_object_q');
select createTriggerUID('dev_object_q');

/*==============================================================*/
/* Table: devices                                               */
/*==============================================================*/
create table devices (
   id                   SERIAL               not null,
   id_graph_plan        INT4                 null,
   name                 VARCHAR              null,
   fault_code           INT4                 null,
   fault_string         VARCHAR              null,
   extra_id             INT4                 not null,
   constraint PK_DEVICES primary key (id)
)
inherits (root_table);

comment on table devices is
'Устройства (элементы) графических планов (ИК ТСО)';

comment on column devices.id_graph_plan is
'Это поле является необязательным, поскольку иногда бывает ситуация, когда приходит устройство, а информации о граф. плане в нем нет. Так происходит, например, при некорректной смене очередности пакетов (ответов на запросы).';

select setMacToNULL('devices');
select createTriggerUID('devices');

/*==============================================================*/
/* Table: element_shapes                                        */
/*==============================================================*/
create table element_shapes (
   id                   SERIAL               not null,
   id_shape_type        INT4                 not null,
   id_device            INT4                 not null,
   id_graph_style       INT4                 null,
   is_closed            BOOL                 not null default TRUE,
   constraint PK_ELEMENT_SHAPES primary key (id)
)
inherits (root_table);

comment on table element_shapes is
'Фигуры, которыми отображено устройство на граф. плане (ИК ТСО)';

select setMacToNULL('element_shapes');
select createTriggerUID('element_shapes');

/*==============================================================*/
/* Table: fault_devices                                         */
/*==============================================================*/
create table fault_devices (
   id                   SERIAL               not null,
   id_device            INT4                 not null,
   fault_state_name     VARCHAR              not null,
   fault_state_value    VARCHAR              not null,
   reg_datetime         TIMESTAMP            not null,
   fault_code           INT4                 null,
   fault_string         VARCHAR              null,
   constraint PK_FAULT_DEVICES primary key (id)
)
inherits (root_table);

comment on table fault_devices is
'Справочник неисправных устройств (ИК ТСО)';

select setMacToNULL('fault_devices');
select createTriggerUID('fault_devices');

/*==============================================================*/
/* Table: fault_devices_q                                       */
/*==============================================================*/
create table fault_devices_q (
   id                   SERIAL               not null,
   id_device_p          INT4                 null,
   dev_name             VARCHAR              null,
   fault_state_name     VARCHAR              not null,
   fault_state_value    VARCHAR              not null,
   reg_datetime         TIMESTAMP            not null,
   constraint PK_FAULT_DEVICES_Q primary key (id)
)
inherits (root_table);

comment on table fault_devices_q is
'Интегральный справочник неисправных устройств';

select setMacToNULL('fault_devices_q');
select createTriggerUID('fault_devices_q');

/*==============================================================*/
/* Table: graph_plans                                           */
/*==============================================================*/
create table graph_plans (
   id                   SERIAL               not null,
   id_parent            INT4                 null,
   name                 VARCHAR              not null,
   bg_image             VARCHAR              null,
   fault_code           INT4                 null,
   fault_string         VARCHAR              null,
   svg_image            XML                  null,
   extra_id             INT4                 not null,
   constraint PK_GRAPH_PLANS primary key (id)
)
inherits (root_table);

comment on table graph_plans is
'Справочник граф. планов (ИК ТСО)';

select setMacToNULL('graph_plans');
select createTriggerUID('graph_plans');

/*==============================================================*/
/* Table: graph_styles                                          */
/*==============================================================*/
create table graph_styles (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_GRAPH_STYLES primary key (id)
)
inherits (root_table);

comment on table graph_styles is
'Справочник стилей отображения графических примитивов (ИК ТСО)';

select setMacToNULL('graph_styles');
select createTriggerUID('graph_styles');

/*==============================================================*/
/* Table: guard_objects                                         */
/*==============================================================*/
create table guard_objects (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   guard_datetime       TIMESTAMP            not null,
   extra_id             INT4                 not null,
   constraint PK_GUARD_OBJECTS primary key (id)
)
inherits (root_table);

comment on table guard_objects is
'Справочник охраняемых объектов (ИК ТСО)';

select setMacToNULL('guard_objects');
select createTriggerUID('guard_objects');

/*==============================================================*/
/* Table: guard_objects_devices                                 */
/*==============================================================*/
create table guard_objects_devices (
   id_guard_objects     INT4                 not null,
   id_device            INT4                 not null,
   constraint PK_GUARD_OBJECTS_DEVICES primary key (id_guard_objects, id_device)
);

comment on table guard_objects_devices is
'Набор элементов справочника устройств, входящих в объект охраны (ИК ТСО)';

select setMacToNULL('guard_objects_devices');

/*==============================================================*/
/* Table: personal_q                                            */
/*==============================================================*/
create table personal_q (
   id                   SERIAL               not null,
   lastname             VARCHAR              not null,
   firstname            VARCHAR              null,
   sirname              VARCHAR              null,
   access_cards         INT4[]               null,
   id_dolzh_p           INT4                 null,
   dolzh_name           VARCHAR              null,
   id_unit_p            INT4                 null,
   unit_name            VARCHAR              null,
   comment              VARCHAR              null,
   constraint PK_PERSONAL_Q primary key (id)
)
inherits (root_table);

comment on table personal_q is
'Интегральный справочник о  персонале объекта, зарегистрированного в ИК ТСО объекта их картах доступа и планах доступа (на основе отчета о персонале)';

select setMacToNULL('personal_q');
select createTriggerUID('personal_q');

/*==============================================================*/
/* Table: persons                                               */
/*==============================================================*/
create table persons (
   id                   SERIAL               not null,
   id_tso_position      INT4                 null,
   id_tso_unit          INT4                 null,
   lastname             VARCHAR              not null,
   firstname            VARCHAR              null,
   sirname              VARCHAR              null,
   fault_code           INT4                 null,
   fault_string         VARCHAR              null,
   extra_id             INT4                 not null,
   constraint PK_PERSONS primary key (id)
)
inherits (root_table);

comment on table persons is
'Информация о персонале (ИК ТСО)';

select setMacToNULL('persons');
select createTriggerUID('persons');

/*==============================================================*/
/* Table: segment_types                                         */
/*==============================================================*/
create table segment_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_SEGMENT_TYPES primary key (id)
)
inherits (root_table);

comment on table segment_types is
'Типы отрезков для сегментов (ИК ТСО)';

select setMacToNULL('segment_types');
select createTriggerUID('segment_types');

/*==============================================================*/
/* Table: shape_segments                                        */
/*==============================================================*/
create table shape_segments (
   id                   SERIAL               not null,
   id_segment_type      INT4                 not null,
   id_element_shape     INT4                 not null,
   points               POINT[]              not null,
   p_order              INT4                 not null default 0,
   constraint PK_SHAPE_SEGMENTS primary key (id)
)
inherits (root_table);

comment on table shape_segments is
'параметры отдельного отрезка графического элемента (ИК ТСО)';

select setMacToNULL('shape_segments');
select createTriggerUID('shape_segments');

/*==============================================================*/
/* Table: shape_types                                           */
/*==============================================================*/
create table shape_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_SHAPE_TYPES primary key (id)
)
inherits (root_table);

comment on table shape_types is
'Типы фигур графических элементов (ИК ТСО):
- прямоугольник;
- эллипс;
- набор сегментов.';

select setMacToNULL('shape_types');
select createTriggerUID('shape_types');

/*==============================================================*/
/* Table: tso_positions                                         */
/*==============================================================*/
create table tso_positions (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   constraint PK_TSO_POSITIONS primary key (id)
)
inherits (root_table);

comment on table tso_positions is
'Справочник должностей (ИК ТСО)';

select setMacToNULL('tso_positions');
select createTriggerUID('tso_positions');

/*==============================================================*/
/* Table: tso_units                                             */
/*==============================================================*/
create table tso_units (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   constraint PK_TSO_UNITS primary key (id)
)
inherits (root_table);

comment on table tso_units is
'Справочник отделов (ИК ТСО)';

select setMacToNULL('tso_units');
select createTriggerUID('tso_units');

alter table access_cards
   add constraint FK_ACCESS_C_REFERENCE_CARD_TYP foreign key (id_card_type)
      references card_types (id)
      on delete restrict on update restrict;

alter table access_cards
   add constraint FK_ACCESS_C_REFERENCE_CARD_STA foreign key (id_card_status)
      references card_statuses (id)
      on delete restrict on update restrict;

alter table access_cards
   add constraint FK_ACCESS_C_REFERENCE_PERSONS foreign key (id_person)
      references persons (id)
      on delete restrict on update restrict;

alter table access_cards_access_plan
   add constraint FK_ACCESS_C_REFERENCE_ACCESS_P foreign key (id_access_plan)
      references access_plan (id)
      on delete restrict on update restrict;

alter table access_cards_access_plan
   add constraint FK_ACCESS_C_REFERENCE_ACCESS_C foreign key (id_access_cards)
      references access_cards (id)
      on delete restrict on update restrict;

alter table alarm_events
   add constraint FK_ALARM_EV_REFERENCE_DEVICES foreign key (id_device)
      references devices (id)
      on delete restrict on update restrict;

alter table alarm_events
   add constraint FK_ALARM_EV_REFERENCE_PERSONS foreign key (id_person)
      references persons (id)
      on delete restrict on update restrict;

alter table devices
   add constraint FK_DEVICES_REFERENCE_GRAPH_PL foreign key (id_graph_plan)
      references graph_plans (id)
      on delete restrict on update restrict;

alter table element_shapes
   add constraint FK_ELEMENT__REFERENCE_GRAPH_ST foreign key (id_graph_style)
      references graph_styles (id)
      on delete restrict on update restrict;

alter table element_shapes
   add constraint FK_ELEMENT__REFERENCE_SHAPE_TY foreign key (id_shape_type)
      references shape_types (id)
      on delete restrict on update restrict;

alter table element_shapes
   add constraint FK_ELEMENT__REFERENCE_DEVICES foreign key (id_device)
      references devices (id)
      on delete restrict on update restrict;

alter table fault_devices
   add constraint FK_FAULT_DE_REFERENCE_DEVICES foreign key (id_device)
      references devices (id)
      on delete restrict on update restrict;

alter table graph_plans
   add constraint FK_GRAPH_PL_REFERENCE_GRAPH_PL foreign key (id_parent)
      references graph_plans (id)
      on delete restrict on update restrict;

alter table guard_objects_devices
   add constraint FK_GUARD_OB_REFERENCE_DEVICES foreign key (id_device)
      references devices (id)
      on delete restrict on update restrict;

alter table guard_objects_devices
   add constraint FK_GUARD_OB_REFERENCE_GUARD_OB foreign key (id_guard_objects)
      references guard_objects (id)
      on delete restrict on update restrict;

alter table persons
   add constraint FK_PERSONS_REFERENCE_TSO_POSI foreign key (id_tso_position)
      references tso_positions (id)
      on delete restrict on update restrict;

alter table persons
   add constraint FK_PERSONS_REFERENCE_TSO_UNIT foreign key (id_tso_unit)
      references tso_units (id)
      on delete restrict on update restrict;

alter table shape_segments
   add constraint FK_SHAPE_SE_REFERENCE_SEGMENT_ foreign key (id_segment_type)
      references segment_types (id)
      on delete restrict on update restrict;

alter table shape_segments
   add constraint FK_SHAPE_SE_REFERENCE_ELEMENT_ foreign key (id_element_shape)
      references element_shapes (id)
      on delete restrict on update restrict;

--=================================
--=================================
--=================================


create table urgency_levels (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   code                 VARCHAR              not null,
   urgency_symbol       VARCHAR              not null,
   constraint PK_URGENCY_LEVELS primary key (id)
);

comment on table urgency_levels is
'уровни срочности сообщений (распоряжений)';

insert into urgency_levels (name, code, urgency_symbol) values('Обычная', 'А', '');
insert into urgency_levels (name, code, urgency_symbol) values('Повышенная', 'Б', '');
insert into urgency_levels (name, code, urgency_symbol) values('Высокая', 'С', '');


create table mail_lists (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_MAIL_LISTS primary key (id)
);

comment on table mail_lists is
'Списки рассылки должностным лицам сообщений и распоряжений. В том числе данные списки используются для упрощения задания прав доступа к ИО. Данный системный справочник используется только в целях упрощения работы пользователей';

create table mail_lists_position (
   id_position          INT4                 not null,
   id_mail_lists        INT4                 not null,
   constraint PK_MAIL_LISTS_POSITION primary key (id_position, id_mail_lists)
);

comment on table mail_lists_position is
'перечень ДЛ, входящих в список рассылки';

alter table mail_lists_position
   add constraint FK_MAIL_LIS_REFERENCE_POSITION foreign key (id_position)
      references "position" (id)
      on delete restrict on update restrict;

alter table mail_lists_position
   add constraint FK_MAIL_LIS_REFERENCE_MAIL_LIS foreign key (id_mail_lists)
      references mail_lists (id)
      on delete restrict on update restrict;


alter table command_journal add column id_urgency_level int4;
update command_journal set id_urgency_level = 1;
alter table command_journal alter column id_urgency_level set not null;
alter table command_journal alter column id_urgency_level set default 1;
alter table command_journal
   add constraint FK_CMD_JOURNAL_REF_URG_LEVEL foreign key (id_urgency_level)
      references urgency_levels (id)
      on delete restrict on update restrict;

alter table command_journal add column id_io_object_att int4;
alter table command_journal
   add constraint FK_CMD_JOURNAL_REF_IO_OBJECT foreign key (id_io_object_att)
      references io_objects(id)
      on delete restrict on update restrict;

alter table message_journal add column id_urgency_level int4;
update message_journal set id_urgency_level = 1;
alter table message_journal alter column id_urgency_level set not null;
alter table message_journal alter column id_urgency_level set default 1;
alter table message_journal
   add constraint FK_MSG_JOURNAL_REF_URG_LEVEL foreign key (id_urgency_level)
      references urgency_levels (id)
      on delete restrict on update restrict;


insert into a_views (id, name) values (15, 'Время');

update a_types set name = 'Цвет записи в справочнике (фон)' where id = 18;
update a_types set name = 'Ссылка на цвет записи (фон)' where id = 19;

insert into a_types (id, id_a_view, name, code) values (25, 11, 'Цвет записи в справочнике (текст)', 'INT4');
insert into a_types (id, id_a_view, name, code) values (26, 11, 'Ссылка на цвет записи (текст)', 'INT4');
insert into a_types (id, id_a_view, name, code) values (27, 15, 'Время', 'TIME');

SELECT pg_catalog.setval('a_views_id_seq', 15, true);
SELECT pg_catalog.setval('a_types_id_seq', 27, true);

--URGENCY_LEVELS

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-129', 129, 10, NULL, false, 'Уровни срочности сообщений (распоряжений)', 'SYSCATEGORY_129', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-130', 130, 8, 129, true, 'Справочник уровней срочности сообщений (распоряжений)', 'SYSCATEGORY_130', NULL::varchar, true, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (129, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (129, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (130, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (130, 2, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-164', 164, 15, 'urgency_symbol', 'Условный знак уровня срочности', 'Условный знак уровня срочности', NULL, NULL, 150, TRUE);

--таблица уровней срочности сообщений (распоряжений)
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (129, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (129, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (129, 6, NULL, true, false); --code
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (129, 164, NULL, true, false); --urgency_symbol


insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-64', 64, 130, 1, 1, 'Справочник уровней срочности сообщений (распоряжений)', 'q_urgency_levels', 'urgency_levels', NULL, 'Системный объект', true, current_timestamp, 1, 1);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (64, -10, TRUE, TRUE, FALSE, FALSE);


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-165', 165, 2, 'id_urgency_level', 'Уровень срочности', 'Уровень срочности', 'urgency_levels', 'name', 150, TRUE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (33, 165, '1', true, false);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (39, 165, '1', true, false);

--ЦИРКУЛЯРНАЯ РАССЫЛКА

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-131', 131, 10, NULL, false, 'Списки рассылки распоряжений и сообщений', 'SYSCATEGORY_131', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-132', 132, 8, 131, true, 'Справочник списков рассылки распоряжений и сообщений', 'SYSCATEGORY_132', NULL::varchar, true, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (131, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (131, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (132, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (132, 2, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-166', 166, 17, 'mail_lists_position', 'Перечень должностных лиц в списке рассылки', 'Перечень должностных лиц в списке рассылки', 'position', 'name', 200, TRUE);

--таблица списков рассылки
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (131, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (131, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (131, 3, NULL, false, false); --desc
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (131, 166, NULL, true, false); --mail_lists_position

insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-65', 65, 132, 1, 1, 'Справочник списков рассылки сообщений и распоряжений', 'q_mail_lists', 'mail_lists', NULL, 'Системный объект', true, current_timestamp, 1, 1);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (65, -10, TRUE, TRUE, FALSE, FALSE);


----
update attributes set name = 'Руководитель', title = 'Руководитель' where id = 76;
update attributes set name = 'Является руководителем', title = 'Является руководителем' where id = 26;
update attributes set name = 'Адрес транспорта', title = 'Адрес транспорта' where id = 28;

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-167', 167, 2, 'id_io_object_att', 'Образец исполнения', 'Образец исполнения', 'io_objects', 'name', 150, TRUE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (33, 167, NULL, false, false);

----

--ИК ТСО (ИНТЕГРАЛЬНЫЕ СПРАВОЧНИКИ)

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-133', 133, 10, NULL, false, 'События ИК ТСО (интегральный)', 'SYSCATEGORY_133', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-134', 134, 8, 133, true, 'Интегральный справочник событий ИК ТСО', 'SYSCATEGORY_134', NULL::varchar, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-135', 135, 10, NULL, false, 'Неисправные устройства ИК ТСО (интегральный)', 'SYSCATEGORY_135', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-136', 136, 8, 135, true, 'Интегральный справочник неисправных устройств ИК ТСО', 'SYSCATEGORY_136', NULL::varchar, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-137', 137, 10, NULL, false, 'Устройства и охраняемые объекты ИК ТСО (интегральный)', 'SYSCATEGORY_137', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-138', 138, 8, 137, true, 'Интегральный справочник устройств и охраняемых объектов ИК ТСО', 'SYSCATEGORY_138', NULL::varchar, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-139', 139, 10, NULL, false, 'Персонал объектов ИК ТСО (интегральный)', 'SYSCATEGORY_139', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-140', 140, 8, 139, true, 'Интегральный справочник персонала объектов ИК ТСО', 'SYSCATEGORY_140', NULL::varchar, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-141', 141, 10, NULL, false, 'Карты доступа к объектам ИК ТСО (интегральный)', 'SYSCATEGORY_141', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-142', 142, 8, 141, true, 'Интегральный справочник карт доступа к объектам ИК ТСО', 'SYSCATEGORY_142', NULL::varchar, true, 1);



insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (133, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (133, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (134, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (134, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (135, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (135, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (136, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (136, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (137, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (137, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (138, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (138, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (139, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (139, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (140, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (140, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (141, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (141, 2, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (142, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (142, 2, 1);


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
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-177', 177, 24, 'device_svg', 'Граф-план устройства', 'Граф. план устройства', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-178', 178, 24, 'plan_svg', 'Граф-план', 'Граф. план', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-179', 179, 1, 'is_ok', 'Устройство исправно', 'Устройство исправно', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-180', 180, 5, 'ok_datetime', 'Работоспособность восстановлена (нарушена)', 'Работоспособность восстановлена (нарушена)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-181', 181, 12, 'access_cards', 'Карты доступа', 'Карты доступа', 'access_cards_q', 'code', 100, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-182', 182, 8, 'id_card_type_p', 'Тип карты (ИД)', 'Тип карты (ИД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-183', 183, 9, 'card_type_name', 'Тип карты', 'Тип карты', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-184', 184, 8, 'id_card_status_p', 'Статус карты (ИД)', 'Статус карты (ИД)', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-185', 185, 9, 'card_status_name', 'Статус карты', 'Статус карты', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-186', 186, 12, 'access_plans', 'Планы доступа', 'Планы доступа', 'access_plan', 'name', 100, TRUE);


--интегральный справочник событий
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 168, NULL, false, false); --id_person_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 140, NULL, false, false); --event_date
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 141, NULL, false, false); --message
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 142, NULL, false, false); --source
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 143, NULL, false, false); --alarm
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 144, NULL, false, false); --card_code
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 145, NULL, false, false); --pass
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 146, NULL, false, false); --direction
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 22, NULL, false, false); --lastname
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 23, NULL, false, false); --firstname
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 24, NULL, false, false); --sirname
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 169, NULL, false, false); --id_device_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 174, NULL, false, false); --dev_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 170, NULL, false, false); --id_unit_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 171, NULL, false, false); --unit_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 172, NULL, false, false); --id_dolzh_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (133, 173, NULL, false, false); --dolzh_name

--интегральный справочник неисправных устройств
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 169, NULL, false, false); --id_device_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 174, NULL, false, false); --dev_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 134, NULL, false, false); --reg_datetime
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 135, NULL, false, false); --fault_state_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (135, 136, NULL, false, false); --fault_state_value

--Интегральный справочник устройств и охраняемых объектов ИК ТСО
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 169, NULL, false, false); --id_device_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 174, NULL, false, false); --dev_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 175, NULL, false, false); --id_guard_obj_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 176, NULL, false, false); --guard_obj_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 177, NULL, false, false); --device_svg
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 178, NULL, false, false); --plan_svg
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 179, NULL, false, false); --is_ok
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (137, 180, NULL, false, false); --ok_datetime

--Интегральный справочник персонала объектов ИК ТСО
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 22, NULL, false, false); --lastname
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 23, NULL, false, false); --firstname
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 24, NULL, false, false); --sirname
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 181, NULL, false, false); --access_cards
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 172, NULL, false, false); --id_dolzh_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 173, NULL, false, false); --dolzh_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 170, NULL, false, false); --id_unit_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 171, NULL, false, false); --unit_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (139, 153, NULL, false, false); --comment

--Интегральный справочник карт доступа к объектам ИК ТСО
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 6, NULL, true, true); --code
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 153, NULL, false, false); --comment
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 182, NULL, false, false); --id_card_type_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 183, NULL, false, false); --card_type_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 184, NULL, false, false); --id_card_status_p
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 185, NULL, false, false); --card_status_name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 186, NULL, false, false); --access_plans
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (141, 3, NULL, false, false); --desc

--ИК ТСО
insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-66', 66, 134, 1, 1, 'Интегральный справочник событий ИК ТСО', 'q_alarm_events_q', 'alarm_events_q', NULL, 'Системный объект', true, current_timestamp, 1, 1);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (66, -10, TRUE, TRUE, FALSE, FALSE);

insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-67', 67, 136, 1, 1, 'Интегральный справочник устройств и охраняемых объектов ИК ТСО', 'q_dev_object_q', 'dev_object_q', NULL, 'Системный объект', true, current_timestamp, 1, 1);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (67, -10, TRUE, TRUE, FALSE, FALSE);

insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-68', 68, 138, 1, 1, 'Интегральный справочник неисправных устройств ИК ТСО', 'q_fault_devices_q', 'fault_devices_q', NULL, 'Системный объект', true, current_timestamp, 1, 1);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (68, -10, TRUE, TRUE, FALSE, FALSE);

insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-69', 69, 140, 1, 1, 'Интегральный справочник персонала объектов ИК ТСО', 'q_personal_q', 'personal_q', NULL, 'Системный объект', true, current_timestamp, 1, 1);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (69, -10, TRUE, TRUE, FALSE, FALSE);

insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-70', 70, 142, 1, 1, 'Интегральный справочник карт доступа к объектам ИК ТСО', 'q_access_cards_q', 'access_cards_q', NULL, 'Системный объект', true, current_timestamp, 1, 1);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (70, -10, TRUE, TRUE, FALSE, FALSE);

create trigger zz_trgZIOInserttableAfter
after insert
on io_objects
for each row
execute procedure ioInsertCheckTableAfter();

select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

commit;
