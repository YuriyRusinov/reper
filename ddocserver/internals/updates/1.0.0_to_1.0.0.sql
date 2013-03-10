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

--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-191', 191, 10, NULL, false, 'Законы распределения', 'SYSCATEGORY_191', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-192', 192, 8, 191, true, 'Справочник законов распределения', 'SYSCATEGORY_192', NULL::varchar, true, true, 1);

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-193', 193, 10, NULL, false, 'Потоки сообщений', 'SYSCATEGORY_193', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-194', 194, 8, 193, true, 'Справочник потоков сообщений', 'SYSCATEGORY_194', NULL::varchar, true, true, 1);

insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (191, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (191, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (192, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (192, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (193, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (193, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (194, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (194, 2, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-313', 313, 5, 'start_time', 'Начало потока', 'Начало потока', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-314', 314, 5, 'stop_time', 'Окончание потока', 'Окончание потока', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-315', 315, 2, 'id_partition_low', 'Закон распределения', 'Закон распределения', 'partition_lows', 'name', 150, TRUE);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-316', 316, 6, 'lambda', 'Лямбда', 'Лямбда', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-317', 317, 6, 'sigma', 'Сигма', 'Сигма', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-318', 318, 6, 'moda', 'Мода', 'Мода', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-319', 319, 6, 'min_p', 'Мин.', 'Мин.', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-320', 320, 6, 'max_p', 'Макс.', 'Макс.', NULL, NULL, 120, TRUE);

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

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-93', 93, 192, 1, 1, 'Справочник законов распределения', 'partition_lows', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (93, -10, TRUE, TRUE, FALSE, FALSE);

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-94', 94, 194, 1, 1, 'Справочник потоков сообщений', 'message_streams', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (94, -10, TRUE, TRUE, FALSE, FALSE);

select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--СЮДА ЗАПРОСЫ НА СОЗДАНИЕ ТАБЛИЦ СПРАВОНИКОВ (ЕСЛИ СОЗДАВАЛИСЬ ИО)
create table partition_lows (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_PARTITION_LOWS primary key (id)
)
inherits (root_table);

comment on table partition_lows is
'Пядь
Справочник законов распределения';

select setMacToNULL('partition_lows');
select createTriggerUID('partition_lows');

create table message_streams (
   id                   SERIAL               not null,
   id_partition_low     INT4                 not null,
   id_io_object         INT4                 not null,
   id_dl_receiver       INT4                 not null,
   name                 VARCHAR              not null,
   lambda               FLOAT8               null default 0,
   sigma                FLOAT8               null default 0,
   moda                 FLOAT8               null default 0,
   min_p                FLOAT8               null default 0,
   max_p                FLOAT8               null default 0,
   start_time           TIMESTAMP            not null,
   end_time             TIMESTAMP            not null,
   constraint PK_MESSAGE_STREAMS primary key (id)
)
inherits (root_table);

comment on table message_streams is
'Пядь
Справочник генерации потоков сообщений (информационых объектов)';

comment on column message_streams.id_io_object is
'ссылка на информационный объект типа справочник, на основании  которого <генерятся> сообщения потока путем последовательной выборки записей справочника, при достижении конечной записи справочника производится переключение на первую запись и т.д.';

comment on column message_streams.id_dl_receiver is
'Должностное лицо-адресат  потока';

comment on column message_streams.lambda is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.sigma is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.moda is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.min_p is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.max_p is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.start_time is
'Дата и время начала интервала генерации потока';

comment on column message_streams.end_time is
'Дата и время завершения генерации потока';

select setMacToNULL('message_streams');
select createTriggerUID('message_streams');

alter table message_streams
   add constraint FK_MESSAGE_STREAMS__REF_PARTITION foreign key (id_partition_low)
      references partition_lows (id)
      on delete restrict on update restrict;

alter table message_streams
   add constraint FK_MESSAGE_STREAMS__REF_IO_OBJEC foreign key (id_io_object)
      references tbl_io_objects (id)
      on delete restrict on update restrict;

alter table message_streams
   add constraint FK_MESSAGE_STREAMS__REF_POSITION foreign key (id_dl_receiver)
      references "position" (id)
      on delete restrict on update restrict;

----------
----------


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ

create or replace function aaa() returns int4 as
$BODY$
declare
    idTemplate int4;
    idGroup int4;
begin
    select getNextSeq('io_templates', 'id') into idTemplate;
    select getNextSeq('a_groups', 'id') into idGroup;

    --создание специального шаблона для справочника потоков сообщений
    insert into io_templates (id, id_io_category, name, code) values(idTemplate, 193, 'Специальный шаблон для справочника потоков сообщений', 'SYSTEMPLATE_' || idTemplate);
    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup, idTemplate, NULL, 'Основные атрибуты', 0);
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(538, idTemplate, idGroup, false, 0, NULL); --id
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(539, idTemplate, idGroup, false, 1, NULL); --name
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(541, idTemplate, idGroup, false, 2, NULL); --id_dl_receiver
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(542, idTemplate, idGroup, false, 3, NULL); --id_io_object

    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+1, idTemplate, NULL, 'Параметры закона распределения', 1); 
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(540, idTemplate, idGroup+1, false, 0, NULL); --id_partition_low
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(543, idTemplate, idGroup+1, false, 1, '0'); --lambda
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(544, idTemplate, idGroup+1, false, 2, '0'); --sigma
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(545, idTemplate, idGroup+1, false, 3, '0'); --moda
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(546, idTemplate, idGroup+1, false, 4, '0'); --min_p
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(547, idTemplate, idGroup+1, false, 5, '0'); --max_p

    insert into a_groups (id, id_io_template, id_parent, name, "order") values(idGroup+2, idTemplate, NULL, 'Диапазон генерации потока', 2); 
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(548, idTemplate, idGroup+2, false, 0, NULL);--start_time
    insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(549, idTemplate, idGroup+2, false, 1, NULL);--end_time

    perform pg_catalog.setval('a_groups_id_seq', idGroup+4, true);
    perform pg_catalog.setval('io_templates_id_seq', idTemplate+2, true);


    return 1;
end
$BODY$
language 'plpgsql';

select aaa();
drop function aaa();

--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
