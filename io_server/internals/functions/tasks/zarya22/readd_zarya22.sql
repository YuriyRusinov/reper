--database script in powerdesigner always generated in koi8

\i ./functions/tasks/zarya22/jms/schema.sql
\i ./functions/tasks/zarya22/jms/funcs.sql
\i ./functions/tasks/zarya22/jms/data.sql

set search_path = public, pg_catalog;

begin;

\encoding koi8r
\i ./functions/tasks/zarya22/init_schema.sql

commit;

\i ./functions/tasks/zarya22/readd_zarya_func.sql


select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

\i ./functions/tasks/zarya22/init_data.sql

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');

create trigger trginsertioxml1
after insert
on jms_schema.in_mes
for each row
execute procedure xmlInsertIORec();
