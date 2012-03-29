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

\i ./functions/tasks/zarya22/init_data.sql

create trigger trginsertioxml1
after insert
on jms_schema.in_mes
for each row
execute procedure xmlInsertIORec();
