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

alter table document_types add constraint PK_DOCUMENT_TYPES primary key (id);
alter table document_kinds add constraint PK_DOCUMENT_KINDS primary key (id);
alter table document_subtypes add constraint PK_DOCUMENT_SUBTYPES primary key (id);


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-206', 206, 18, 'record_fill_color', 'Цвет фона при отображении в таблице', 'Цвет фона при отображении в таблице', NULL, NULL, 120, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-207', 207, 25, 'record_text_color', 'Цвет текста при отображении в таблице', 'Цвет текста при отображении в таблице', NULL, NULL, 120, TRUE);

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, 206, NULL, false, false);--record_fill_color
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, 207, NULL, false, false);--record_text_color

alter table io_objects add column record_fill_color int8;
alter table io_objects add column record_text_color int8;

update a_types set code = 'INT8' where id in(18, 25);


insert into a_groups (id, id_io_template, id_parent, name, "order") values(nextval('a_groups_id_seq'), 1, NULL, 'Визуализация', 3);
insert into io_views (id_io_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value) values(13, 206, 1, currval('a_groups_id_seq'), true, 0, NULL);
insert into io_views (id_io_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value) values(13, 207, 1, currval('a_groups_id_seq'), true, 1, NULL);

alter table organization alter column id_curr_mode set default 1;
alter table organization alter column id_prev_mode set default 1;
alter table io_objects alter column id_maclabel set default 1;
alter table "position" alter column id_maclabel set default 1;
alter table users alter column id_maclabel set default 1;


alter table attributes add column id_search_template int4;
alter table attributes
   add constraint FK_ATTRIBUT_REFERENCE_SEARCH_T foreign key (id_search_template)
      references search_templates (id)
      on delete restrict on update restrict;

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (9, 110, NULL, false, false);--id_search_template

alter table url_types add column unique_id varchar;
alter table url_types add column last_update timestamp;
select createTriggerUID('url_types');
update url_types set unique_id = ('localorg-url_types-' || id)::varchar, last_update = current_timestamp;
alter table url_types alter column unique_id set not null;
alter table url_types alter column last_update set not null;
alter table url_types alter column last_update set default current_timestamp;
alter table url_types inherit root_table;

alter table io_urls add column unique_id varchar;
alter table io_urls add column last_update timestamp;
select createTriggerUID('io_urls');
update io_urls set unique_id = ('localorg-io_urls-' || id)::varchar, last_update = current_timestamp;
alter table io_urls alter column unique_id set not null;
alter table io_urls alter column last_update set not null;
alter table io_urls alter column last_update set default current_timestamp;
alter table io_urls inherit root_table;

alter table maclabels add column unique_id varchar;
alter table maclabels add column last_update timestamp;
select createTriggerUID('maclabels');
update maclabels set unique_id = ('localorg-maclabels-' || id)::varchar, last_update = current_timestamp;
alter table maclabels alter column unique_id set not null;
alter table maclabels alter column last_update set not null;
alter table maclabels alter column last_update set default current_timestamp;
alter table maclabels inherit root_table;

alter table io_states add column unique_id varchar;
alter table io_states add column last_update timestamp;
select createTriggerUID('io_states');
update io_states set unique_id = ('localorg-io_states-' || id)::varchar, last_update = current_timestamp;
alter table io_states alter column unique_id set not null;
alter table io_states alter column last_update set not null;
alter table io_states alter column last_update set default current_timestamp;
alter table io_states inherit root_table;

alter table transport add column unique_id varchar;
alter table transport add column last_update timestamp;
select createTriggerUID('transport');
update transport set unique_id = ('localorg-transport-' || id)::varchar, last_update = current_timestamp;
alter table transport alter column unique_id set not null;
alter table transport alter column last_update set not null;
alter table transport alter column last_update set default current_timestamp;
alter table transport inherit root_table;

alter table organization_transport add column unique_id varchar;
alter table organization_transport add column last_update timestamp;
select createTriggerUID('organization_transport');
update organization_transport set unique_id = ('localorg-organization_transport-' || id)::varchar, last_update = current_timestamp;
alter table organization_transport alter column unique_id set not null;
alter table organization_transport alter column last_update set not null;
alter table organization_transport alter column last_update set default current_timestamp;
alter table organization_transport inherit root_table;

alter table io_types add column unique_id varchar;
alter table io_types add column last_update timestamp;
select createTriggerUID('io_types');
update io_types set unique_id = ('localorg-io_types-' || id)::varchar, last_update = current_timestamp;
alter table io_types alter column unique_id set not null;
alter table io_types alter column last_update set not null;
alter table io_types alter column last_update set default current_timestamp;
alter table io_types inherit root_table;

alter table a_views add column unique_id varchar;
alter table a_views add column last_update timestamp;
select createTriggerUID('a_views');
update a_views set unique_id = ('localorg-a_views-' || id)::varchar, last_update = current_timestamp;
alter table a_views alter column unique_id set not null;
alter table a_views alter column last_update set not null;
alter table a_views alter column last_update set default current_timestamp;
alter table a_views inherit root_table;

alter table a_types add column unique_id varchar;
alter table a_types add column last_update timestamp;
select createTriggerUID('a_types');
update a_types set unique_id = ('localorg-a_types-' || id)::varchar, last_update = current_timestamp;
alter table a_types alter column unique_id set not null;
alter table a_types alter column last_update set not null;
alter table a_types alter column last_update set default current_timestamp;
alter table a_types inherit root_table;


select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgioinsert', 'io_objects');

select f_safe_drop_trigger('trgsyncrecords', 'io_categories');

update io_categories set is_global = true where is_system = true;
update io_objects set is_global = true, id_sync_type = 5 where is_system = true;
create or replace function aaa() returns void as
$BODY$
declare
    r record;
    q varchar;
begin
    for r in
        select table_name from io_objects where is_system = true
    loop
        perform f_safe_drop_trigger('trgsyncrecords', r.table_name);
        q := 'create trigger trgSyncRecords before insert or update or delete on ' || r.table_name || ' for each row execute procedure syncRecords(); ';
        execute q;
    end loop;
    return;

end
$BODY$
language 'plpgsql';

select aaa();

drop function aaa();

/*
CREATE TRIGGER trgsyncrecords
  BEFORE INSERT OR UPDATE OR DELETE
  ON io_categories
  FOR EACH ROW
  EXECUTE PROCEDURE syncrecords();
*/

create trigger trgCheckCatForGlobal
before insert or update
on io_categories
for each row 
execute procedure checkCatForGlobal();


CREATE TRIGGER trgioinsert
  BEFORE INSERT OR UPDATE
  ON io_objects
  FOR EACH ROW
  EXECUTE PROCEDURE ioinsertcheck();
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
