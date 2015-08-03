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

/*==============================================================*/
/* Table: rand_state                                            */
/*==============================================================*/
create table rand_state (
   id                   SERIAL               not null,
   state_rand           BYTEA                null,
   rand_seed            INT8                 null,
   constraint PK_RAND_STATE primary key (id)
)
inherits (root_table);

comment on table rand_state is
'Служебная таблица для хранения состояний датчика случайных чисел';

select setMacToNULL('rand_state');
select createTriggerUID('rand_state');

update attributes set name = 'Предыдущее состояние', title = 'Предыдущее состояние' where id = 324;
update attributes set name = 'Текущее состояние', title = 'Текущее состояние' where id = 325;


alter table io_processing_order drop column id_chain;
/*==============================================================*/
/* Table: io_processing_order_chains                            */
/*==============================================================*/
create table io_processing_order_chains (
   id_io_processing_order INT4                 not null,
   id_chains            INT4                 not null,
   constraint PK_IO_PROCESSING_ORDER_CHAINS primary key (id_io_processing_order, id_chains)
);

comment on table io_processing_order_chains is
'Перечень очередей, в которые должны быть помещены ИО и ЭИО при изменении их состояния';

select setMacToNULL('io_processing_order_chains');

alter table io_processing_order_chains
   add constraint FK_IO_PROCE_REFERENCE_IO_PROCE foreign key (id_io_processing_order)
      references io_processing_order (id)
      on delete restrict on update restrict;

alter table io_processing_order_chains
   add constraint FK_IO_PROCE_REFERENCE_CHAINS foreign key (id_chains)
      references chains (id)
      on delete restrict on update restrict;

delete from attrs_categories where id = 599;

--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-347', 347, 17, 'io_processing_order_chains', 'Перечень очередей обработки', 'Перечень очередей обработки', 'chains', 'name', 200, TRUE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (605, 209, 347, NULL, false, false);  --io_processing_order_chains

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--СЮДА ЗАПРОСЫ НА СОЗДАНИЕ ТАБЛИЦ СПРАВОНИКОВ (ЕСЛИ СОЗДАВАЛИСЬ ИО)


----------
----------


--ВСЕ ОСТАЛЬНОЕ ЗДЕСЬ


--почему-то иногда ряд атрибутов не обновляется.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
