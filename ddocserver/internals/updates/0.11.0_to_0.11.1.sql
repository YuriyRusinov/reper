\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql

create  index i_attrs_values_o on tbl_attrs_values using BTREE (
id_io_object
);


select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql
\i ./functions/security/readd_security.sql

create or replace view io_objects as select * from f_sel_io_objects(NULL);
grant all on io_objects to public;

create or replace view attrs_values as select * from f_sel_attrs_values(NULL);
grant all on attrs_values to public;


commit;
