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

alter table life_cycle add column id_auto_state_attr int4;
alter table life_cycle add column id_auto_state_ind int4;
alter table life_cycle
   add constraint FK_LIFE_CYC_REF_IO_STATE_ATTR foreign key (id_auto_state_attr)
      references io_states (id)
      on delete restrict on update restrict;
alter table life_cycle
   add constraint FK_LIFE_CYC_REF_IO_STATE_IND foreign key (id_auto_state_ind)
      references io_states (id)
      on delete restrict on update restrict;

alter table chains drop column id_io_state;
alter table chains drop column id_io_category;



create table io_processing_order (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   id_chain             INT4                 not null,
   id_state_src         INT4                 null,
   id_state_dest        INT4                 not null,
   id_io_category       INT4                 not null,
   constraint PK_IO_PROCESSING_ORDER primary key (id)
)
inherits (root_table);

comment on table io_processing_order is
'Справочник подярка обработки информационных объектов в различных состояниях жизненного цикла';

comment on column io_processing_order.id_chain is
'Очередь обработки';

comment on column io_processing_order.id_state_src is
'Из какого (id_io_state_from) состояния в какое (id_io_state_to) переходит ИО, которые должны обрабатываться данной очередью';

comment on column io_processing_order.id_state_dest is
'Из какого (id_io_state_from) состояния в какое (id_io_state_to) переходит ИО, которые должны обрабатываться данной очередью';

comment on column io_processing_order.id_io_category is
'ИО какой категории обрабатываются данной очередью';

select setMacToNULL('io_processing_order');
select createTriggerUID('io_processing_order');

alter table io_processing_order
   add constraint FK_IO_PROCE_REFERENCE_CHAINS foreign key (id_chain)
      references chains (id)
      on delete restrict on update restrict;

alter table io_processing_order
   add constraint FK_IO_PROC_REF_IO_STATE_DEST foreign key (id_state_dest)
      references io_states (id)
      on delete restrict on update restrict;

alter table io_processing_order
   add constraint FK_IO_PROC_REF_IO_STATE_SRC foreign key (id_state_src)
      references io_states (id)
      on delete restrict on update restrict;

alter table io_processing_order
   add constraint FK_IO_PROCE_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;







drop view shu_chksum;
select f_drop_funcs_shu_chksum();
alter sequence shu_chksum_id_seq rename to tbl_shu_chksum_id_seq;
alter table tbl_shu_chksum rename to shu_chksum;
alter table shu_chksum add column unique_id varchar;
alter table shu_chksum add column last_update timestamp;
update shu_chksum set unique_id = generateUID(id, 'shu_chksum'), last_update = current_timestamp;
alter table shu_chksum alter column unique_id set not null;
alter table shu_chksum alter column last_update set not null;
alter table shu_chksum alter column last_update set default current_timestamp;
alter table shu_chksum inherit root_table;
select setMacToNULL('shu_chksum');
select createTriggerUID('shu_chksum');
select acl_secureTable('shu_chksum');

drop view shu_domains;
select f_drop_funcs_shu_domains();
alter sequence shu_domains_id_seq rename to tbl_shu_domains_id_seq;
alter table tbl_shu_domains rename to shu_domains;
alter table shu_domains add column unique_id varchar;
alter table shu_domains add column last_update timestamp;
update shu_domains set unique_id = generateUID(id, 'shu_domains'), last_update = current_timestamp;
alter table shu_domains alter column unique_id set not null;
alter table shu_domains alter column last_update set not null;
alter table shu_domains alter column last_update set default current_timestamp;
alter table shu_domains inherit root_table;
select setMacToNULL('shu_domains');
select createTriggerUID('shu_domains');
select acl_secureTable('shu_domains');


drop view shu_positions;
select f_drop_funcs_shu_positions();
alter sequence shu_positions_id_seq rename to tbl_shu_positions_id_seq;
alter table tbl_shu_positions rename to shu_positions;
alter table shu_positions add column unique_id varchar;
alter table shu_positions add column last_update timestamp;
update shu_positions set unique_id = generateUID(id, 'shu_positions'), last_update = current_timestamp;
alter table shu_positions alter column unique_id set not null;
alter table shu_positions alter column last_update set not null;
alter table shu_positions alter column last_update set default current_timestamp;
alter table shu_positions inherit root_table;
select setMacToNULL('shu_positions');
select createTriggerUID('shu_positions');
select acl_secureTable('shu_positions');


drop view shu_acs;
select f_drop_funcs_shu_acs();
alter sequence shu_acs_id_seq rename to tbl_shu_acs_id_seq;
alter table tbl_shu_acs rename to shu_acs;
alter table shu_acs add column unique_id varchar;
alter table shu_acs add column last_update timestamp;
update shu_acs set unique_id = generateUID(id, 'shu_acs'), last_update = current_timestamp;
alter table shu_acs alter column unique_id set not null;
alter table shu_acs alter column last_update set not null;
alter table shu_acs alter column last_update set default current_timestamp;
alter table shu_acs inherit root_table;
select setMacToNULL('shu_acs');
select createTriggerUID('shu_acs');
select acl_secureTable('shu_acs');




drop view shu_dls;
select f_drop_funcs_shu_dls();
alter sequence shu_dls_id_seq rename to tbl_shu_dls_id_seq;
alter table tbl_shu_dls rename to shu_dls;
alter table shu_dls add column unique_id varchar;
alter table shu_dls add column last_update timestamp;
update shu_dls set unique_id = generateUID(id, 'shu_dls'), last_update = current_timestamp;
alter table shu_dls alter column unique_id set not null;
alter table shu_dls alter column last_update set not null;
alter table shu_dls alter column last_update set default current_timestamp;
alter table shu_dls inherit root_table;
select setMacToNULL('shu_dls');
select createTriggerUID('shu_dls');
select acl_secureTable('shu_dls');

drop view shu_orgs;
select f_drop_funcs_shu_orgs();
alter sequence shu_orgs_id_seq rename to tbl_shu_orgs_id_seq;
alter table tbl_shu_orgs rename to shu_orgs;
alter table shu_orgs add column unique_id varchar;
alter table shu_orgs add column last_update timestamp;
update shu_orgs set unique_id = generateUID(id, 'shu_orgs'), last_update = current_timestamp;
alter table shu_orgs alter column unique_id set not null;
alter table shu_orgs alter column last_update set not null;
alter table shu_orgs alter column last_update set default current_timestamp;
alter table shu_orgs inherit root_table;
select setMacToNULL('shu_orgs');
select createTriggerUID('shu_orgs');
select acl_secureTable('shu_orgs');

insert into shu_acs(code, name, uri) values ('zarya22', 'zarya22', 'http://zaria22.acs.mil');
insert into shu_acs(code, name, uri) values ('vershina2', 'vershina2', 'http://vershina2.acs.mil');

insert into shu_domains(code, name) values('intelligence.Ground', 'intelligence.Ground');
insert into shu_domains(code, name) values('intelligence.Air', 'intelligence.Air');
insert into shu_domains(code, name) values('intelligence.Sea', 'intelligence.Sea');

insert into shu_positions(code, name) values('nachGRNapr', 'nachGRNapr');
insert into shu_positions(code, name) values('operDezh', 'operDezh');

insert into shu_orgs(code, name, uri) values('oskUVO', 'oskUVO', 'http://zaria22.acs.mil/ou/oskUVO');
insert into shu_orgs(code, name, uri) values('orgUnit', 'orgUnit', 'http://vershina2.acs.mil/ou/orgUnit');

insert into shu_dls(name, uri, id_acs, id_org, id_pos) values('nachGRNapr', 'http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr', 1, 1, 1);
insert into shu_dls(name, uri, id_acs, id_org, id_pos) values('operDezh', 'http://vershina2.acs.mil/ou/orgUnit/pu/operDezh', 2, 2, 2);


delete from attrs_categories where id in (569, 570);
--that will delete next attrs chains qualifier:
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (569, 201, 18, NULL, true, false);  --id_io_state
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (570, 201, 12, NULL, true, false);  --id_io_category


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРА) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ КАТЕГОРИЙ (ДОБАВЛЯТЬ СТРОКИ В ACCESS_CATEGORIES_TABLE НЕ НАДО)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-209', 209, 10, NULL, false, 'Порядок обработки ИО в различных состояниях ЖЦ', 'SYSCATEGORY_209', NULL::varchar, true, true, 1);
insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-210', 210, 8, 209, true, 'Справочник порядка обработки ИО в различных состояниях ЖЦ', 'SYSCATEGORY_210', NULL::varchar, true, true, 1);

--таблица порядка обработки ИО в различных состояниях ЖЦ
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (594, 209, 1, NULL, true, true); --id
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (595, 209, 2, NULL, true, false); --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (596, 209, 12, NULL, true, false); --id_io_category
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (597, 209, 324, NULL, false, false); --id_state_src
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (598, 209, 325, NULL, true, false); --id_state_dest
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (599, 209, 329, NULL, true, false); --id_chain

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-343', 343, 2, 'id_auto_state_attr', 'Автовозврат состояния при изменении атрибутов', 'Автовозврат состояния при изменении атрибутов', 'io_states', 'name', 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-344', 344, 2, 'id_auto_state_ind', 'Автовозврат состояния при изменении показателей', 'Автовозврат состояния при изменении показателей', 'io_states', 'name', 150, TRUE);

insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (600, 195, 343, NULL, false, false);  --id_auto_state_attr
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (601, 195, 344, NULL, false, false);  --id_auto_state_ind


select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--СЮДА (МЕЖДУ УДАЛЕНИЕМ И СОЗДАНИЕМ ТРИГГЕРОВ) ПОМЕЩАЕМ ЗАПРОСЫ НА СОЗДАНИЕ ИО (НАДО ДОБАВЛЯТЬ СТРОКИ В ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org, id_io_type)
values ('localorg-io_objects-102', 102, 210, 1, 1, 'Справочник порядка обработки ИО в различных состояниях ЖЦ', 'io_processing_order', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL, 3);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (102, -10, TRUE, TRUE, FALSE, FALSE);

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
