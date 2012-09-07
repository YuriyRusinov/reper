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

--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÀ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÊÀÒÅÃÎĞÈÉ (ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_CATEGORIES_TABLE ÍÅ ÍÀÄÎ)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-312', 312, 15, 'r_icon', 'Óñëîâíûé çíàê ıëåìåíòà', 'Óñëîâíûé çíàê ıëåìåíòà', NULL, NULL, 150, TRUE);
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (533, 77, 312, NULL, false, false);--r_icon
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (534, 13, 312, NULL, false, false);--r_icon

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(534, 1, 4, false, 2, NULL);

update attrs_categories set is_mandatory = false where id = 198;
--insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (198, 55, 78, NULL, false, false);--tree_symbol

--ñşäà
select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÎÂ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÈÎ (ÍÀÄÎ ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
--ñşäà
select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--ÂÑÅ ÎÑÒÀËÜÍÎÅ ÇÄÅÑÜ

alter table rubricator add column  r_icon varchar;
alter table io_rubricator add column r_icon varchar;
alter table record_rubricator add column r_icon varchar;
alter table rubric_records add column r_icon varchar;

alter table tbl_io_objects add column r_icon varchar;
\i ./functions/security/ctrl/acl_secure_io_objects_apply.sql

--ïî÷åìó-òî èíîãäà ğÿä àòğèáóòîâ íå îáíîâëÿåòñÿ.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
