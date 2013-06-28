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



alter table q_base_table add column    r_icon VARCHAR;
alter table q_base_table add column    record_fill_color    INT8;
alter table q_base_table add column    record_text_color    INT8;

create or replace function aaa() returns int4 as
$BODY$
declare
    r record;
    tName varchar;
    q varchar;
    i int4;
begin

    for r in select table_name from io_objects where id > 300 and table_name is not null
    loop

        tName = r.table_name;

        
        select f_is_view_exist(tName) into i;
        if(i = 0) then
            continue;
        end if;
        
        q = 'drop view ' || tName || ' cascade';
        execute q;
        
        q = 'select f_drop_funcs_' || tName || '();';
        execute q;
        
        q = 'alter table tbl_' || tName || ' rename to ' || tName;
        execute q;

        perform acl_secureTable(tName);
    end loop;

    return 1;
end
$BODY$
language 'plpgsql';

select aaa();
drop function aaa();

alter table attrs_attrs add column "order" int4;
alter table attrs_attrs alter column "order" set default 1;
update attrs_attrs set "order" = 1;
alter table attrs_attrs alter column "order" set not null;

alter table attrs_categories add column "order" int4;
alter table attrs_categories alter column "order" set default 1;
update attrs_categories set "order" = 1;
alter table attrs_categories alter column "order" set not null;

alter table tbl_rec_attrs_values alter column id type INT8;
alter table tbl_rec_attrs_values alter column id set not null;
alter table tbl_rec_attrs_values alter column id set default  nextval('q_base_table_id_seq'::regclass);
drop view rec_attrs_values cascade;
\i ./functions/security/ctrl/acl_secure_rec_attr_values.sql
create or replace view rec_attrs_values as select * from f_sel_rec_attrs_values(NULL::int8);
revoke all on tbl_rec_attrs_values from public;
grant all on rec_attrs_values to public;
create or replace rule "r_ins_rec_attrs_values" as on insert to "rec_attrs_values" do instead select "f_ins_rec_attrs_values"(new.id_record, new.id_attr_category, new.value, new.start_time, new.stop_time, new.id_io_object_src, new.id_io_object_src1, new.description);
create or replace rule "r_del_rec_attrs_values" as on delete to "rec_attrs_values" do instead select "f_del_rec_attrs_values"(old.id_record, old.id_attr_category, true);
create or replace rule "r_upd_rec_attrs_values" as on update to "rec_attrs_values" do instead select "f_upd_rec_attrs_values"(new.id_record, new.id_attr_category, new.value, new.start_time, new.stop_time, new.id_io_object_src, new.id_io_object_src1, new.description, old.id_record, old.id_attr_category, true);


insert into a_views (id, name) values (18, 'Составной атрибут');
insert into a_types (id, id_a_view, name, code) values (32, 18, 'Составной атрибут', 'VARCHAR');
SELECT pg_catalog.setval('a_views_id_seq', 18, true);
SELECT pg_catalog.setval('a_types_id_seq', 32, true);

/*==============================================================*/
/* Table: rec_attrs_attrs_values                                */
/*==============================================================*/
create table rec_attrs_attrs_values (
   id                   BIGSERIAL            not null,
   id_rec_attr_value    INT8                 not null,
   id_attr_attr         INT4                 not null,
   value                VARCHAR              not null
)
inherits (root_table);

comment on table rec_attrs_attrs_values is
'Значения атрибутов, описывающих другие атрибуты (показатели) для записей справочников.
Записи в данной таблице появляются при редактировании Значения атрибута записи справочника';

comment on column rec_attrs_attrs_values.value is
'Значение (строковое представление) описывающего показатель атрибута';

select setMacToNULL('rec_attrs_attrs_values');
select createTriggerUID('rec_attrs_attrs_values');

alter table rec_attrs_attrs_values
   add constraint FK_REC_ATTR_REFERENCE_REC_ATTR foreign key (id_rec_attr_value)
      references tbl_rec_attrs_values (id)
      on delete restrict on update restrict;

alter table rec_attrs_attrs_values
   add constraint FK_REC_ATTR_REFERENCE_ATTRS_AT foreign key (id_attr_attr)
      references attrs_attrs (id)
      on delete restrict on update restrict;


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

--insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-347', 347, 17, 'io_processing_order_chains', 'Перечень очередей обработки', 'Перечень очередей обработки', 'chains', 'name', 200, TRUE);
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (605, 209, 347, NULL, false, false);  --io_processing_order_chains

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
