
set search_path = public, pg_catalog;

begin;

\encoding win1251;
\i ./functions/tasks/gis_stream_sample/init_schema.sql

commit;

\i ./functions/tasks/gis_stream_sample/readd_gis_stream_sample_func.sql
