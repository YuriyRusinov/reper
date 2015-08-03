\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

drop function getnextseq(varchar, varchar);

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

--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÀ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÊÀÒÅÃÎĞÈÉ (ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_CATEGORIES_TABLE ÍÅ ÍÀÄÎ)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-310', 310, 1, 'is_qualifier', 'ßâëÿåòñÿ ñïğàâî÷íèêîì', 'ßâëÿåòñÿ ñïğàâî÷íèêîì', NULL, NULL, 50, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-311', 311, 3, 'id_child2', 'Ïîä÷èíåííàÿ êàòåãîğèÿ ïîêàçàòåëåé', 'Ïîä÷èíåííàÿ êàòåãîğèÿ ïîêàçàòåëåé', 'io_categories', 'name', 300, TRUE);

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (3, 310, 'FALSE', true, false);--is_qualifier
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (11, 311, NULL, false, false);--id_child2

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÎÂ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÈÎ (ÍÀÄÎ ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--ÂÑÅ ÎÑÒÀËÜÍÎÅ ÇÄÅÑÜ

alter table io_category_types add column is_qualifier bool;
alter table io_category_types alter column is_qualifier set default false;
update io_category_types set is_qualifier = false; 
update io_category_types set is_qualifier = true where id in (1, 2, 8, 9);
alter table io_category_types alter column is_qualifier set not null;

alter table io_categories add column is_archived bool;
alter table io_categories alter column is_archived set default false;

select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
update io_categories set is_archived = false;
select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');

alter table io_categories alter column is_archived set not null;

alter table io_categories add column id_child2 int4;
alter table io_categories
   add constraint FK_IO_CATEG_CHILD2 foreign key (id_child2)
      references io_categories (id)
      on delete restrict on update restrict;

alter sequence io_types_id_seq rename to io_category_types_id_seq;
alter sequence io_types_id_seq1 rename to io_types_id_seq;

create table q_base_table (
   id                   BIGSERIAL            not null,
   uuid_t               UUID                 not null,
   constraint PK_Q_BASE_TABLE primary key (id)
)
inherits (root_table);

alter table q_base_table alter column uuid_t set default uuid_generate_v1();


--ïî÷åìó-òî èíîãäà ğÿä àòğèáóòîâ íå îáíîâëÿåòñÿ.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
