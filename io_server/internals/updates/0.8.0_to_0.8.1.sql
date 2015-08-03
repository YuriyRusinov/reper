\encoding win1251

begin;

alter table position add column address varchar;
alter table position alter column address set default '$$';
update position set address = '$$';
alter table position alter column address set not null;

\i ./functions/readd_functions.sql

commit;
