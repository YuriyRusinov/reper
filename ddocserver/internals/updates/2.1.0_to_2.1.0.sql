\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

--ÑŞÄÀ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÒÀÁËÈÖ ÑÏĞÀÂÎ×ÍÈÊÎÂ (ÅÑËÈ ÑÎÇÄÀÂÀËÈÑÜ ÈÎ)
--ÈÕ ÎÁßÇÀÒÅËÜÍÎ ÏÅĞÅÄ ÑÎÇÄÀÍÈÅÌ ÔÓÍÊÖÈÉ!!!

alter table in_external_queue drop column if exists id_external;
alter table in_external_queue add column id_external INT8;
update in_external_queue set id_external = -1;
alter table in_external_queue alter column id_external set default -1;
alter table in_external_queue alter column id_external set not null;

drop function insertIncomeXML(int4, int4, int4, varchar);

---------------------------
---------------------------


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


\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/readd_tasks_func.sql




--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÀ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÊÀÒÅÃÎĞÈÉ (ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_CATEGORIES_TABLE ÍÅ ÍÀÄÎ)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');
select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');
select f_safe_drop_trigger('trgacinsert', 'attrs_categories');


select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacinsert', 'before', 'insert or update', 'attrs_categories', 'acinsertcheck()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÎÂ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÈÎ (ÍÀÄÎ ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
select f_safe_drop_trigger('trgsyncrecords', 'io_objects');



select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');
select f_create_trigger('trgsyncrecords', 'before', 'insert or update or delete', 'io_objects', 'syncrecords()');

select setAsLogging(1);
select setAsLogging(2);

----------
----------


--ÂÑÅ ÎÑÒÀËÜÍÎÅ ÇÄÅÑÜ

insert into a_types (id, id_a_view, name, code) values (38, 14, 'Áèíàğíûå äàííûå', 'BYTEA');
SELECT pg_catalog.setval('a_types_id_seq', 38, true);

--ïî÷åìó-òî èíîãäà ğÿä àòğèáóòîâ íå îáíîâëÿåòñÿ.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
