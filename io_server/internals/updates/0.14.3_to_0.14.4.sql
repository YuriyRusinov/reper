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
--ñşäà
select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÎÂ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÈÎ (ÍÀÄÎ ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
--ñşäà
select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--ÂÑÅ ÎÑÒÀËÜÍÎÅ ÇÄÅÑÜ

alter table io_templates add column unique_id varchar;
alter table io_templates add column last_update timestamp;
update io_templates set unique_id = generateUID(id, 'io_templates'), last_update = current_timestamp;
alter table io_templates alter column unique_id set not null;
alter table io_templates alter column last_update set not null;
alter table io_templates alter column last_update set default current_timestamp;
alter table io_templates inherit root_table;
select createTriggerUID('io_templates');

alter table categories_rubrics add column unique_id varchar;
alter table categories_rubrics add column last_update timestamp;
update categories_rubrics set unique_id = generateUID(id, 'categories_rubrics'), last_update = current_timestamp;
alter table categories_rubrics alter column unique_id set not null;
alter table categories_rubrics alter column last_update set not null;
alter table categories_rubrics alter column last_update set default current_timestamp;
alter table categories_rubrics inherit root_table;
select createTriggerUID('categories_rubrics');

alter table a_groups add column unique_id varchar;
alter table a_groups add column last_update timestamp;
update a_groups set unique_id = generateUID(id, 'a_groups'), last_update = current_timestamp;
alter table a_groups alter column unique_id set not null;
alter table a_groups alter column last_update set not null;
alter table a_groups alter column last_update set default current_timestamp;
alter table a_groups inherit root_table;
select createTriggerUID('a_groups');

alter table io_last_sync add column unique_id varchar;
alter table io_last_sync add column last_update timestamp;
update io_last_sync set unique_id = generateUID(id, 'io_last_sync'), last_update = current_timestamp;
alter table io_last_sync alter column unique_id set not null;
alter table io_last_sync alter column last_update set not null;
alter table io_last_sync alter column last_update set default current_timestamp;
alter table io_last_sync inherit root_table;
select createTriggerUID('io_last_sync');

alter table io_sync_archive add column unique_id varchar;
alter table io_sync_archive add column last_update timestamp;
update io_sync_archive set unique_id = generateUID(id, 'io_sync_archive'), last_update = current_timestamp;
alter table io_sync_archive alter column unique_id set not null;
alter table io_sync_archive alter column last_update set not null;
alter table io_sync_archive alter column last_update set default current_timestamp;
alter table io_sync_archive inherit root_table;
select createTriggerUID('io_sync_archive');

alter table io_sync_types add column unique_id varchar;
alter table io_sync_types add column last_update timestamp;
update io_sync_types set unique_id = generateUID(id, 'io_sync_types'), last_update = current_timestamp;
alter table io_sync_types alter column unique_id set not null;
alter table io_sync_types alter column last_update set not null;
alter table io_sync_types alter column last_update set default current_timestamp;
alter table io_sync_types inherit root_table;
select createTriggerUID('io_sync_types');

alter table tsd_control_journal add column unique_id varchar;
alter table tsd_control_journal add column last_update timestamp;
update tsd_control_journal set unique_id = generateUID(id, 'tsd_control_journal'), last_update = current_timestamp;
alter table tsd_control_journal alter column unique_id set not null;
alter table tsd_control_journal alter column last_update set not null;
alter table tsd_control_journal alter column last_update set default current_timestamp;
alter table tsd_control_journal inherit root_table;
select createTriggerUID('tsd_control_journal');

alter table out_sync_queue add column unique_id varchar;
alter table out_sync_queue add column last_update timestamp;
update out_sync_queue set unique_id = generateUID(id, 'out_sync_queue'), last_update = current_timestamp;
alter table out_sync_queue alter column unique_id set not null;
alter table out_sync_queue alter column last_update set not null;
alter table out_sync_queue alter column last_update set default current_timestamp;
alter table out_sync_queue inherit root_table;
select createTriggerUID('out_sync_queue');

alter table jr_states add column unique_id varchar;
alter table jr_states add column last_update timestamp;
update jr_states set unique_id = generateUID(id, 'jr_states'), last_update = current_timestamp;
alter table jr_states alter column unique_id set not null;
alter table jr_states alter column last_update set not null;
alter table jr_states alter column last_update set default current_timestamp;
alter table jr_states inherit root_table;
select createTriggerUID('jr_states');

alter table "organization" add column unique_id varchar;
alter table "organization" add column last_update timestamp;
update "organization" set unique_id = generateUID(id, 'organization'), last_update = current_timestamp;
alter table "organization" alter column unique_id set not null;
alter table "organization" alter column last_update set not null;
alter table "organization" alter column last_update set default current_timestamp;
alter table "organization" inherit root_table;
select createTriggerUID('organization');

alter table "groups" add column unique_id varchar;
alter table "groups" add column last_update timestamp;
update "groups" set unique_id = generateUID(id, 'groups'), last_update = current_timestamp;
alter table "groups" alter column unique_id set not null;
alter table "groups" alter column last_update set not null;
alter table "groups" alter column last_update set default current_timestamp;
alter table "groups" inherit root_table;
select createTriggerUID('groups');

alter table criteria_types add column unique_id varchar;
alter table criteria_types add column last_update timestamp;
update criteria_types set unique_id = generateUID(id, 'criteria_types'), last_update = current_timestamp;
alter table criteria_types alter column unique_id set not null;
alter table criteria_types alter column last_update set not null;
alter table criteria_types alter column last_update set default current_timestamp;
alter table criteria_types inherit root_table;
select createTriggerUID('criteria_types');

alter table criteria add column unique_id varchar;
alter table criteria add column last_update timestamp;
update criteria set unique_id = generateUID(id, 'criteria'), last_update = current_timestamp;
alter table criteria alter column unique_id set not null;
alter table criteria alter column last_update set not null;
alter table criteria alter column last_update set default current_timestamp;
alter table criteria inherit root_table;
select createTriggerUID('criteria');

alter table mail_lists add column unique_id varchar;
alter table mail_lists add column last_update timestamp;
update mail_lists set unique_id = generateUID(id, 'mail_lists'), last_update = current_timestamp;
alter table mail_lists alter column unique_id set not null;
alter table mail_lists alter column last_update set not null;
alter table mail_lists alter column last_update set default current_timestamp;
alter table mail_lists inherit root_table;
select createTriggerUID('mail_lists');

alter table user_handlers_data add column unique_id varchar;
alter table user_handlers_data add column last_update timestamp;
update user_handlers_data set unique_id = generateUID(id, 'user_handlers_data'), last_update = current_timestamp;
alter table user_handlers_data alter column unique_id set not null;
alter table user_handlers_data alter column last_update set not null;
alter table user_handlers_data alter column last_update set default current_timestamp;
alter table user_handlers_data inherit root_table;
select setMacToNULL('user_handlers_data');
select createTriggerUID('user_handlers_data');

alter table search_templates add column unique_id varchar;
alter table search_templates add column last_update timestamp;
update search_templates set unique_id = generateUID(id, 'search_templates'), last_update = current_timestamp;
alter table search_templates alter column unique_id set not null;
alter table search_templates alter column last_update set not null;
alter table search_templates alter column last_update set default current_timestamp;
alter table search_templates inherit root_table;
select createTriggerUID('search_templates');

alter table record_rubricator add column unique_id varchar;
alter table record_rubricator add column last_update timestamp;
update record_rubricator set unique_id = generateUID(id, 'record_rubricator'), last_update = current_timestamp;
alter table record_rubricator alter column unique_id set not null;
alter table record_rubricator alter column last_update set not null;
alter table record_rubricator alter column last_update set default current_timestamp;
alter table record_rubricator inherit root_table;
select setMacToNULL('record_rubricator');
select createTriggerUID('record_rubricator');



--ïî÷åìó-òî èíîãäà ğÿä àòğèáóòîâ íå îáíîâëÿåòñÿ.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
