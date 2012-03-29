\encoding win1251

begin;

--alter table position add column address varchar;
--alter table position alter column address set default '$$';
--update position set address = '$$';
--alter table position alter column address set not null;

\i ./functions/readd_functions.sql

commit;

begin;
--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql


commit;
