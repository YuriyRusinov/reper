\encoding win1251

begin;

select createTempTables();
select setAsNotLogging(1);
select setAsNotLogging(2);

create table access_rubric_table (
   id_role              INT4                 not null,
   id_rubric            INT4                 not null,
   allow_read           BOOL                 not null default true,
   allow_readlist       BOOL                 not null default true,
   allow_delete         BOOL                 not null default true,
   allow_update         BOOL                 not null default true,
   constraint PK_ACCESS_RUBRIC_TABLE primary key (id_role, id_rubric)
);

alter table access_rubric_table
   add constraint FK_ACCESS_R_REFERENCE_KKS_ROLE foreign key (id_role)
      references kks_roles (id)
      on delete restrict on update restrict;

alter table access_rubric_table
   add constraint FK_ACCESS_R_REFERENCE_RUBRICAT foreign key (id_rubric)
      references rubricator (id)
      on delete restrict on update restrict;

alter table organization add column is_completed BOOL;
update organization set is_completed = true;
alter table organization alter column is_completed set not null;
alter table organization alter column is_completed set default false;

alter table organization add column is_created INT4;
update organization set is_created = 1;
alter table organization alter column is_created set not null;
alter table organization alter column is_created set default 0;
alter table organization add constraint CKC_IS_CREATED_ORGANIZA check (is_created in (0,1,2));

update attributes set is_system = TRUE where id <= 200;
update io_categories set is_system = true where id < 200;

alter table io_categories add column id_io_state INT4;
update io_categories set id_io_state = 1;
alter table io_categories alter column id_io_state set not null;
alter table io_categories
   add constraint FK_IO_CATEG_REFERENCE_IO_STATE foreign key (id_io_state)
      references io_states (id)
      on delete restrict on update restrict;


alter table io_categories add column is_global bool;
update io_categories set is_global = false;
alter table io_categories alter column is_global set not null;
alter table io_categories alter column is_global set default false;

alter table io_categories add column is_completed bool;
update io_categories set is_completed = true;
alter table io_categories alter column is_completed set not null;
alter table io_categories alter column is_completed set default false;

create table io_sync_archive (
   id                   SERIAL               not null,
   id_organization      INT4                 not null,
   id_io_object         INT4                 not null,
   sync_datetime        TIMESTAMP            not null,
   sync_result          INT4                 not null,
   constraint PK_IO_SYNC_ARCHIVE primary key (id)
);

alter table io_sync_archive
   add constraint FK_IO_SYNC_ORG foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table io_sync_archive
   add constraint FK_IO_SYNC_IO_OBJECT foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

create table io_last_sync (
   id                   SERIAL               not null,
   id_io_object         INT4                 not null,
   id_organization      INT4                 not null,
   min_period           INT4[]               not null,
   last_sync            TIMESTAMP            not null,
   constraint PK_IO_LAST_SYNC primary key (id)
);

alter table io_last_sync
   add constraint FK_IO_LAST_SYNC_IO_OBJECT foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table io_last_sync
   add constraint FK_IO_LAST_SYNC_ORG foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

create table io_sync_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_IO_SYNC_TYPES primary key (id)
);

insert into io_sync_types (name, description) values ('Не синхронизируется', 'НС');
insert into io_sync_types (name, description) values ('Глобальные централизованного ведения', 'ГЦВ');
insert into io_sync_types (name, description) values ('Глобальные формуляры', 'ГФ');
insert into io_sync_types (name, description) values ('Глобальные объектового ведения, интегрируемые', 'ГОВИ');
insert into io_sync_types (name, description) values ('Глобальные объектового ведения, автономные', 'ГОВА');


alter table io_objects add column id_sync_type int4;
update io_objects set id_sync_type = 1;
alter table io_objects alter column id_sync_type set not null;
alter table io_objects alter column id_sync_type set default 1;
alter table io_objects
   add constraint FK_IO_OBJECT_IO_SYNC_TYPE foreign key (id_sync_type)
      references io_sync_types (id)
      on delete restrict on update restrict;

alter table io_objects add column id_owner_org int4;
alter table io_objects
   add constraint FK_IO_OBJECT_ORG foreign key (id_owner_org)
      references organization (id)
      on delete restrict on update restrict;

alter table io_objects add column is_global bool;
update io_objects set is_global = false;
alter table io_objects alter column is_global set not null;
alter table io_objects alter column is_global set default false;

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-79', 79, 10, NULL, false, 'Типы синхронизации информационных объектов', 'SYSCATEGORY_79', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-80', 80, 8, 79, true, 'Справочник типов синхронизации информационных объектов', 'SYSCATEGORY_80', NULL::varchar, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-81', 81, 10, NULL, false, 'Архив (история) синхронизации информационных объектов', 'SYSCATEGORY_81', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-82', 82, 8, 81, true, 'Архив (история) синхронизации информационных объектов', 'SYSCATEGORY_82', NULL::varchar, true, 1);

insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-83', 83, 10, NULL, false, 'Параметры синхронизации информационных объектов', 'SYSCATEGORY_83', NULL::varchar, true, 1);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, id_io_state) values ('localorg-categories-84', 84, 8, 83, true, 'Справочник параметров синхронизации информационных объектов', 'SYSCATEGORY_84', NULL::varchar, true, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (79, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (79, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (80, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (80, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (81, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (81, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (82, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (82, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (83, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (83, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (84, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (84, 2, 1);


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-111', 111, 1, 'is_global', 'Является глобальным', 'Глобальный', NULL, NULL, 60, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-112', 112, 2, 'id_sync_type', 'Тип синхронизации', 'Тип синхронизации', 'io_sync_types', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-113', 113, 8, 'sync_result', 'Результат синхронизации', 'Результат синхронизации', NULL, NULL, 80, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-114', 114, 10, 'min_period', 'Минимальный период синхронизации', 'Минимальный период синхронизации', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-115', 115, 5, 'last_sync', 'Дата и время последней синхронизации', 'Дата и время последней синхронизации', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-116', 116, 5, 'sync_datetime', 'Дата и время синхронизации', 'Дата и время синхронизации', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-117', 117, 2, 'id_owner_org', 'Организация-владелец', 'Организация-владелец', 'organization', 'name', 150, TRUE);


--таблица типов синхронизации ИО
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (79, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (79, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (79, 3, NULL, true, false); --description

--таблица архива синхронизации информационных объектов
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (81, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (81, 74, NULL, true, false); --id_organization
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (81, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (81, 116, NULL, true, false); --sync_datetime
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (81, 113, NULL, true, false); --sync_result

--таблица параметров синхронизации информационных объектов
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (83, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (83, 48, NULL, true, false); --id_io_object
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (83, 74, NULL, true, false); --id_organization
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (83, 114, NULL, true, false); --min_period
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (83, 115, NULL, true, false); --last_sync

--таблица категорий
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (11, 18, '1', true, false);--id_io_state
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (11, 111, 'false', true, false);--is_global

--таблица информационных объектов
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, 112, '1', true, false);--id_sync_type
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, 117, NULL, true, true);--id_owner_org
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, 111, 'false', true, false);--is_global

--справочник пользовательских очередей
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (75, 110, NULL, false, false);--id_search_template

create or replace function aa11() returns int4 as
$BODY$
declare
    idGroup int4;
    cnt int4;
begin

    select getNextSeq('a_groups', 'id') into idGroup;
    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup, 1, NULL, 'Синхронизация', 2);
 
    insert into io_views (id_io_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value) values(13, 112, 1, idGroup, true, 0, '1');
    insert into io_views (id_io_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value) values(13, 111, 1, idGroup, true, 1, 'false');
    insert into io_views (id_io_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value) values(13, 117, 1, idGroup, true, 2, NULL);
    
    select count(*) into cnt from organization;
    if(cnt = 0) then
        select getNextSeq('organization', 'id') into idGroup;
        insert into organization (
                                  id, 
                                  id_type_org, 
                                  id_curr_mode, 
                                  id_prev_mode, 
                                  name, 
                                  short_name,
                                  code_name, 
                                  email_prefix, 
                                  latitude, 
                                  longitude, 
                                  map_symbol, 
                                  tree_symbol, 
                                  dt_prev_mode, 
                                  dt_curr_mode
                                 )
                           values(
                                   idGroup,
                                   1, 
                                   1,
                                   1,
                                  'DUMMY ORGANIZATION',
                                  'DUMMY ORGANIZATION',
                                  'DUMMY ORGANIZATION',
                                  'localorg',
                                   0, 
                                   0,
                                  '',
                                  '',
                                   current_timestamp,
                                   current_timestamp
                                  );    
    end if;

    return 1;
end
$BODY$
language 'plpgsql';

select aa11();

drop function aa11();

update a_types set code = 'INT4[]' where id = 10;


\i ./functions/readd_functions.sql

commit;

begin;
--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

create or replace function aa11() returns int4 as
$BODY$
declare
    idOrg int4;
begin

    idOrg = getLocalOrg();
    if(idOrg isnull or idOrg < 0) then
        select id into idOrg from organization order by 1 limit 1;
    end if;

    update io_objects set id_owner_org = idOrg;
    alter table io_objects alter column id_owner_org set not null;

    insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
    values ('localorg-io_objects-39', 39, 80, 1, 1, 'Типы синхронизации информационных объектов', 'q_io_sync_types', 'io_sync_types', NULL, NULL, true, current_timestamp, 1, idOrg);
    insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
    values (39, -10, TRUE, TRUE, FALSE, FALSE);

    insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
    values ('localorg-io_objects-40', 40, 82, 1, 1, 'Архив (история) синхронизации информационных объектов', 'q_io_sync_archive', 'io_sync_archive', NULL, NULL, true, current_timestamp, 1, idOrg);
    insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
    values (40, -10, TRUE, TRUE, FALSE, FALSE);

    insert into io_objects (unique_id, id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel, id_owner_org)
    values ('localorg-io_objects-41', 41, 84, 1, 1, 'Параметры синхронизации информационных объектов', 'q_io_last_sync', 'io_last_sync', NULL, NULL, true, current_timestamp, 1, idOrg);
    insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
    values (41, -10, TRUE, TRUE, FALSE, FALSE);

    return 1;
end
$BODY$
language 'plpgsql';

select aa11();
drop function aa11();

create trigger zz_trgZIOInserttableAfter
after insert
on io_objects
for each row
execute procedure ioInsertCheckTableAfter();


alter table "position" add column email_prefix varchar;
update "position" set email_prefix = generateDlEmailPrefix(id);
alter table "position" alter column email_prefix set not null;
create unique index i_dl_email on "position" using BTREE (email_prefix);


select setAsLogging(1);
select setAsLogging(2);

--------------------------------------
-----------WEB------------------------
--------------------------------------
CREATE TABLE "module_auto_action" (
  "module_action_id" int NOT NULL,
  "auto_enabled_action_id" int NOT NULL,
  UNIQUE ("module_action_id","auto_enabled_action_id")
);

CREATE INDEX "module_action_id" ON "module_auto_action" ("module_action_id");
CREATE INDEX "auto_enabled_action_id" ON "module_auto_action" ("auto_enabled_action_id");


CREATE TABLE "role" (
  "id_role" int  NOT NULL,
  "name" text NOT NULL,
  "super_user" text NOT NULL,
  PRIMARY KEY ("id_role"),
  UNIQUE  ("name")
);

CREATE INDEX "super_user" ON "role" ("super_user");

CREATE TABLE "module" (
  "id_module" int  NOT NULL,
  "module" text NOT NULL,
  "name" text NOT NULL,
  PRIMARY KEY ("id_module"),
  UNIQUE ("module")
  
);

CREATE INDEX "name" ON "module" ("name");

CREATE TABLE "module_action" (
  "id_module_action" int not null,
  "module_id" int NOT NULL,
  "action" varchar(50) NOT NULL,
  "name" varchar(50) NOT NULL,
  "public_flag" text NOT NULL,
  PRIMARY KEY ("id_module_action") 
);


CREATE INDEX  "module_id" on module_action ("module_id");
CREATE INDEX   "action"  on module_action ("action");
CREATE INDEX   "name_module_action" on module_action ("name");
CREATE INDEX   "public_flag" on module_action ("public_flag");

CREATE TABLE "employee_day" (
  "employee_id" text  NOT NULL,
  "date" date NOT NULL,
  "start_work" time NOT NULL,
  "stop_work" time NOT NULL,
  "last_active" time NOT NULL,
  "status" text NOT NULL,
  "description" text NOT NULL,
  UNIQUE ("employee_id","date")  
);

CREATE INDEX "status" ON "employee_day" ("status");


CREATE TABLE "role_action" (
  "role_id" int NOT NULL,
  "module_action_id" int NOT NULL,
  UNIQUE ("role_id","module_action_id")
);

CREATE INDEX "role_id" on role_action("role_id");
CREATE INDEX "role_action_id" on role_action("module_action_id");



CREATE TABLE "news_config" (
  "id" serial,
  "am_news" int NOT NULL,
  "PreLengthC" int NOT NULL,
  "tab_width" varchar(5) NOT NULL,
  "p_time" int NOT NULL,
  "p_date" int NOT NULL,
  "img_height" int NOT NULL,
  "c_top" int NOT NULL,
  "c_bottom" int NOT NULL,
  PRIMARY KEY ("id")
);


CREATE TABLE "news" (
  "news_id" serial,
  "news_time" varchar(5) NOT NULL DEFAULT '00:00',
  "news_date" date NOT NULL DEFAULT '2010-01-01',
  "news_title" text NOT NULL,
  "news_content" text NOT NULL,
  "news_img" char(1) DEFAULT 'n',
  "news_exp" varchar(5) NOT NULL DEFAULT '.jpg',
  "news_stamp" int NOT NULL DEFAULT '0',
  PRIMARY KEY ("news_id")
);

CREATE TABLE "employee_calendar" (
  "calendar_id" int NOT NULL,
  "employee_id" text NOT NULL,
  UNIQUE ("calendar_id","employee_id")
);

CREATE TABLE "calendar_event" (
  "id_event" serial,
  "calendar_id" int NOT NULL,
  "what" text,
  "where" text,
  "description" text,
  "start_date" date NOT NULL,
  "start_time" time DEFAULT NULL,
  "stop_date" date NOT NULL,
  "stop_time" time DEFAULT NULL,
  "completed" text NOT NULL,
  "notice" text NOT NULL,
  "mark" text NOT NULL,
  PRIMARY KEY ("id_event")
);

CREATE INDEX "calendar_id" on calendar_event("calendar_id","start_date","start_time","stop_date","stop_time");

CREATE TABLE "calendar" (
  "id_calendar" serial,
  "author_id" text NOT NULL,
  "name" varchar(250) DEFAULT NULL,
  "description" text,
  "type" text NOT NULL DEFAULT 'default',
  PRIMARY KEY ("id_calendar")
);

INSERT INTO "module_auto_action" VALUES(4,76);
INSERT INTO "module_auto_action" VALUES(6,23);
INSERT INTO "module_auto_action" VALUES(6,25);
INSERT INTO "module_auto_action" VALUES(9,20);
INSERT INTO "module_auto_action" VALUES(13,21);
INSERT INTO "module_auto_action" VALUES(14,22);
INSERT INTO "module_auto_action" VALUES(16,25);
INSERT INTO "module_auto_action" VALUES(17,24);
INSERT INTO "module_auto_action" VALUES(26,33);
INSERT INTO "module_auto_action" VALUES(26,34);
INSERT INTO "module_auto_action" VALUES(28,21);
INSERT INTO "module_auto_action" VALUES(28,25);
INSERT INTO "module_auto_action" VALUES(28,32);
INSERT INTO "module_auto_action" VALUES(28,40);
INSERT INTO "module_auto_action" VALUES(29,43);
INSERT INTO "module_auto_action" VALUES(31,38);
INSERT INTO "module_auto_action" VALUES(31,41);
INSERT INTO "module_auto_action" VALUES(35,42);
INSERT INTO "module_auto_action" VALUES(35,62);
INSERT INTO "module_auto_action" VALUES(35,63);
INSERT INTO "module_auto_action" VALUES(35,64);
INSERT INTO "module_auto_action" VALUES(35,65);
INSERT INTO "module_auto_action" VALUES(35,66);
INSERT INTO "module_auto_action" VALUES(35,67);
INSERT INTO "module_auto_action" VALUES(35,68);
INSERT INTO "module_auto_action" VALUES(35,69);
INSERT INTO "module_auto_action" VALUES(35,70);
INSERT INTO "module_auto_action" VALUES(35,71);
INSERT INTO "module_auto_action" VALUES(35,73);
INSERT INTO "module_auto_action" VALUES(35,83);
INSERT INTO "module_auto_action" VALUES(35,91);
INSERT INTO "module_auto_action" VALUES(37,39);
INSERT INTO "module_auto_action" VALUES(37,61);
INSERT INTO "module_auto_action" VALUES(79,81);
INSERT INTO "module_auto_action" VALUES(80,82);


INSERT INTO "role" VALUES (1,'Администратор','yes');
INSERT INTO "role" VALUES (2,'Работник','no');
INSERT INTO "role" VALUES (3,'Начальник отдела','no');


INSERT INTO "module" VALUES (1,'role','Роли');
INSERT INTO "module" VALUES (2,'employee','Сотрудники');
INSERT INTO "module" VALUES (3,'department','Подразделения');
INSERT INTO "module" VALUES (4,'company','Контрагенты');
INSERT INTO "module" VALUES (6,'variable','Дополнительные поля');
INSERT INTO "module" VALUES (11,'message','Сообщения');
INSERT INTO "module" VALUES (12,'file','Файловый менеджер');
INSERT INTO "module" VALUES (13,'calendar','Календарь');
INSERT INTO "module" VALUES (14,'report','Отчеты');
INSERT INTO "module" VALUES (26,'office','Должности сотрудников');
INSERT INTO "module" VALUES (34,'news','доска обьявлений компании');
INSERT INTO "module" VALUES (40,'schedule','планировщик мероприятий');
INSERT INTO "module" VALUES (41,'schedule-period','периоды мероприятий');
INSERT INTO "module" VALUES (42,'schedule-type','типы мероприятий');
INSERT INTO "module" VALUES (45,'organizer','ежедневник');
INSERT INTO "module" VALUES (46,'uvedoml','Модуль общесистемных уведомлений');
INSERT INTO "module" VALUES (47,'journal','Журнал ККС СИТО');
INSERT INTO "module" VALUES (48,'slovar','Словарь ККС СИТО');
INSERT INTO "module" VALUES (49, 'category', 'Категории и шаблоны ККС СИТО');
INSERT INTO "module" VALUES (50, 'rubrics', 'Рубрикаторы ККС СИТО');



INSERT INTO "module_action" VALUES (1,6,'view','Просмотр','yes');
INSERT INTO "module_action" VALUES (2,6,'delete','Удаление','yes');
INSERT INTO "module_action" VALUES (3,6,'edit','Редактировать','yes');
INSERT INTO "module_action" VALUES (4,2,'view','Просмотр','yes');
INSERT INTO "module_action" VALUES (5,2,'delete','Удаление','yes');
INSERT INTO "module_action" VALUES (6,2,'edit','Редактировать','yes');
INSERT INTO "module_action" VALUES (7,4,'view','Просмотр','yes');
INSERT INTO "module_action" VALUES (8,4,'delete','Удаление','yes');
INSERT INTO "module_action" VALUES (9,4,'edit','Редактировать','yes');
INSERT INTO "module_action" VALUES (11,5,'view','Просмотр','yes');
INSERT INTO "module_action" VALUES (12,5,'delete','Удаление','yes');
INSERT INTO "module_action" VALUES (13,5,'edit','Редактировать','yes');
INSERT INTO "module_action" VALUES (14,3,'view','Просмотр','yes');
INSERT INTO "module_action" VALUES (26,8,'view','Просмотр','yes');
INSERT INTO "module_action" VALUES (45,11,'delete','Удаление','yes');
INSERT INTO "module_action" VALUES (46,11,'edit','Редактировать','yes');
INSERT INTO "module_action" VALUES (47,12,'view','Просмотр каталогов','yes');
INSERT INTO "module_action" VALUES (48,12,'delete','Удаление','yes');
INSERT INTO "module_action" VALUES (49,12,'edit','Загрузка','yes');
INSERT INTO "module_action" VALUES (50,12,'add-directory','Добавление директорий','no');
INSERT INTO "module_action" VALUES (51,12,'move','Перенос файлов и каталогов','no');
INSERT INTO "module_action" VALUES (52,12,'rename','Переименовывание каталогов и файлов','no');
INSERT INTO "module_action" VALUES (60,12,'change-role','Изменение доступа','no');
INSERT INTO "module_action" VALUES (99,17,'rows_conf','настройка отображаемых столбцов','yes');
INSERT INTO "module_action" VALUES (171,34,'view','Просмотр доски обьявлений','yes');
INSERT INTO "module_action" VALUES (172,34,'edit','Настройка доски объявлений','yes');
INSERT INTO "module_action" VALUES (173,34,'preview','Предпросмотр объявления','yes');
INSERT INTO "module_action" VALUES (174,39,'planned-payment','','yes');
INSERT INTO "module_action" VALUES (175,40,'view','','yes');
INSERT INTO "module_action" VALUES (176,40,'edit','','yes');
INSERT INTO "module_action" VALUES (177,40,'delete','','yes');
INSERT INTO "module_action" VALUES (178,41,'delete','','yes');
INSERT INTO "module_action" VALUES (179,41,'edit','','yes');
INSERT INTO "module_action" VALUES (180,41,'view','','yes');
INSERT INTO "module_action" VALUES (181,42,'delete','','yes');
INSERT INTO "module_action" VALUES (182,42,'view','','yes');
INSERT INTO "module_action" VALUES (183,42,'edit','','yes');
INSERT INTO "module_action" VALUES (184,43,'delete','','yes');
INSERT INTO "module_action" VALUES (185,43,'edit','','yes');
INSERT INTO "module_action" VALUES (186,43,'move','','yes');
INSERT INTO "module_action" VALUES (187,43,'view','','yes');
INSERT INTO "module_action" VALUES (188,43,'select-org-soglas','','yes');
INSERT INTO "module_action" VALUES (189,44,'view','','yes');
INSERT INTO "module_action" VALUES (190,44,'delete','','yes');
INSERT INTO "module_action" VALUES (191,44,'edit','','yes');
INSERT INTO "module_action" VALUES (192,44,'select-org','','yes');
INSERT INTO "module_action" VALUES (193,44,'table','','yes');
INSERT INTO "module_action" VALUES (194,46,'notice-view','Просмотр общесистемных уведомлений','yes');
INSERT INTO "module_action" VALUES (195,46,'notice-action','Действие по общесистемному уведомлению','yes');
INSERT INTO "module_action" VALUES (196,10,'comment','Комментарии к задачам проекта','yes');
INSERT INTO "module_action" VALUES (197,14,'img-gip-project-gantt','Проект закрепленные за ответственными','yes');
INSERT INTO "module_action" VALUES (198,14,'gip-project-gantt','Проекты по ответственным','yes');
INSERT INTO "module_action" VALUES (199,14,'department-gantt','Загрузка по подразделениям','yes');
INSERT INTO "module_action" VALUES (200,14,'img-deparment-gantt','График загрузки по подразделениям','yes');
INSERT INTO "module_action" VALUES (201,10,'set-today-me','Задачи поставленные сегодня мной','yes');
INSERT INTO "module_action" VALUES (202,2,'role','Управление ролями','yes');
INSERT INTO "module_action" VALUES (203,47,'view','Просмотр','yes');
INSERT INTO "module_action" VALUES (204,47,'delete','Удаление','yes');
INSERT INTO "module_action" VALUES (205,47,'edit','Редактировать','yes');


INSERT INTO "module_action" VALUES (349,47,'delete','Удалить сообщение','yes');
INSERT INTO "module_action" VALUES (350,47,'edit','Редактировать сообщение','yes');
INSERT INTO "module_action" VALUES (351,47,'report','Отчет по сообщениям','yes');
INSERT INTO "module_action" VALUES (352,47,'select-dls','Отчет по должностям','yes');
INSERT INTO "module_action" VALUES (353,47,'select-jrstate','Отчет по статусу','yes');
INSERT INTO "module_action" VALUES (354,47,'select-kio','Отчет по объектам','yes');
INSERT INTO "module_action" VALUES (355,47,'view','Просмотр','yes');
INSERT INTO "module_action" VALUES (377,48,'logs','Просмотр логов','yes');
INSERT INTO "module_action" VALUES (378,48,'rubrics','Просмотр рубрик','yes');
INSERT INTO "module_action" VALUES (379,48,'files','Просмотр файлов','yes');

INSERT INTO "module_action" VALUES (356,48,'delete','Удалить сообщение','yes');
INSERT INTO "module_action" VALUES (357,48,'edit','Редактировать сообщение','yes');
INSERT INTO "module_action" VALUES (358,48,'report','Отчет по сообщениям','yes');
INSERT INTO "module_action" VALUES (359,48,'select-dls','Отчет по должностям','yes');
INSERT INTO "module_action" VALUES (360,48,'select-jrstate','Отчет по статусу','yes');
INSERT INTO "module_action" VALUES (361,48,'select-kio','Отчет по объектам','yes');
INSERT INTO "module_action" VALUES (362,48,'view','Просмотр','yes');

INSERT INTO "module_action" VALUES (363,49,'delete','Удалить категорию (шаблон)','yes');
INSERT INTO "module_action" VALUES (364,49,'edit','Редактировать категорию(шаблон)','yes');
INSERT INTO "module_action" VALUES (365,49,'report','Отчет по категориям(шаблонам)','yes');
INSERT INTO "module_action" VALUES (369,49,'view','Просмотр','yes');


INSERT INTO "module_action" VALUES (370,50,'delete','Удалить рубрику','yes');
INSERT INTO "module_action" VALUES (371,50,'edit','Редактировать рубрику','yes');
INSERT INTO "module_action" VALUES (372,50,'report','Отчет по рубрикам','yes');
INSERT INTO "module_action" VALUES (376,50,'view','Просмотр','yes');


INSERT INTO "role_action" VALUES (2,1);
INSERT INTO "role_action" VALUES (2,11);
INSERT INTO "role_action" VALUES (2,14);
INSERT INTO "role_action" VALUES (2,17);
INSERT INTO "role_action" VALUES (2,26);
INSERT INTO "role_action" VALUES (2,28);
INSERT INTO "role_action" VALUES (2,29);
INSERT INTO "role_action" VALUES (2,31);
INSERT INTO "role_action" VALUES (2,35);
INSERT INTO "role_action" VALUES (2,37);
INSERT INTO "role_action" VALUES (2,44);
INSERT INTO "role_action" VALUES (2,46);
INSERT INTO "role_action" VALUES (2,54);
INSERT INTO "role_action" VALUES (2,55);
INSERT INTO "role_action" VALUES (2,56);
INSERT INTO "role_action" VALUES (2,57);
INSERT INTO "role_action" VALUES (2,58);
INSERT INTO "role_action" VALUES (2,59);
INSERT INTO "role_action" VALUES (2,75);
INSERT INTO "role_action" VALUES (2,78);
INSERT INTO "role_action" VALUES (2,174);
INSERT INTO "role_action" VALUES (2,178);
INSERT INTO "role_action" VALUES (2,179);
INSERT INTO "role_action" VALUES (2,180);
INSERT INTO "role_action" VALUES (2,181);
INSERT INTO "role_action" VALUES (2,182);
INSERT INTO "role_action" VALUES( 2,183);
INSERT INTO "role_action" VALUES (2,184);
INSERT INTO "role_action" VALUES (2,185);
INSERT INTO "role_action" VALUES (2,186);
INSERT INTO "role_action" VALUES (2,187);
INSERT INTO "role_action" VALUES (2,188);
INSERT INTO "role_action" VALUES (2,189);
INSERT INTO "role_action" VALUES (2,190);
INSERT INTO "role_action" VALUES (2,191);
INSERT INTO "role_action" VALUES (2,192); 
INSERT INTO "role_action" VALUES (2,193);
INSERT INTO "role_action" VALUES (2,196);
INSERT INTO "role_action" VALUES (2,201);
INSERT INTO "role_action" VALUES (3,1);
INSERT INTO "role_action" VALUES (3,4);
INSERT INTO "role_action" VALUES (3,6);
INSERT INTO "role_action" VALUES (3,7);
INSERT INTO "role_action" VALUES (3,9);
INSERT INTO "role_action" VALUES (3,11);
INSERT INTO "role_action" VALUES (3,13);
INSERT INTO "role_action" VALUES (3,14);
INSERT INTO "role_action" VALUES (3,17);
INSERT INTO "role_action" VALUES (3,19);
INSERT INTO "role_action" VALUES (3,26);
INSERT INTO "role_action" VALUES (3,28);
INSERT INTO "role_action" VALUES (3,29);
INSERT INTO "role_action" VALUES (3,31);
INSERT INTO "role_action" VALUES (3,35);
INSERT INTO "role_action" VALUES (3,37);
INSERT INTO "role_action" VALUES (3,44);
INSERT INTO "role_action" VALUES (3,46);
INSERT INTO "role_action" VALUES (3,47);
INSERT INTO "role_action" VALUES (3,48);
INSERT INTO "role_action" VALUES (3,49);
INSERT INTO "role_action" VALUES (3,54);
INSERT INTO "role_action" VALUES (3,55);
INSERT INTO "role_action" VALUES (3,56);
INSERT INTO "role_action" VALUES (3,57);
INSERT INTO "role_action" VALUES (3,58);
INSERT INTO "role_action" VALUES (3,59);
INSERT INTO "role_action" VALUES (3,74);
INSERT INTO "role_action" VALUES (3,75);
INSERT INTO "role_action" VALUES (3,77);
INSERT INTO "role_action" VALUES (3,78);
INSERT INTO "role_action" VALUES (3,79);
INSERT INTO "role_action" VALUES (3,80);

INSERT INTO "news_config" VALUES (1,11,101,'1209',1,1,20,10,1);


commit;
