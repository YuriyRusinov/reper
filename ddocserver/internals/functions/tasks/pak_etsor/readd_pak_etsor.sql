--\i ./functions/salerik/salerik.sql

set search_path = public, pg_catalog;

begin;

\encoding win1251;
--\i ./functions/tasks/pak_etsor/init_schema.sql

commit;

\i ./functions/tasks/pak_etsor/readd_pak_etsor_func.sql
