\encoding win1251

begin;
--\i ./functions/misc/f_create_lang.sql
\i ./functions/misc/setmactonull.sql
\i ./functions/misc/f_is_type_exist.sql
\i ./functions/misc/f_safe_drop_type.sql
\i ./functions/general/generateuid.sql
\i ./functions/general/createtriggeruidex.sql
commit;

\i ./functions/contribs/readd_contribs.sql


\encoding koi8r

create extension IF NOT EXISTS postgis;

begin;
\i ./schema/all_tables.sql
commit;

\encoding win1251

begin;

\i ./functions/readd_functions.sql

--in readd_commands use encoding utf8 - so, set cp1251 back
\encoding win1251

alter table io_objects rename to tbl_io_objects;
alter sequence io_objects_id_seq rename to tbl_io_objects_id_seq;
alter table attrs_values rename to tbl_attrs_values;
alter table rec_attrs_values rename to tbl_rec_attrs_values;

\i ./functions/security/readd_security.sql

commit;

