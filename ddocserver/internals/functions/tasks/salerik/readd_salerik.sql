--\i ./functions/salerik/salerik.sql

set search_path = public, pg_catalog;

begin;

\encoding win1251;
\i ./functions/tasks/salerik/init_schema.sql

commit;

\i ./functions/tasks/salerik/readd_salerik_func.sql
