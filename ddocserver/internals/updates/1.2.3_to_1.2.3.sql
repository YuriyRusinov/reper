\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

select createTriggerUID ('criteria');

\i ./functions/categories/agetrefvalue.sql
\i ./functions/general/getrecattrvalue.sql
drop function getioattrvaluehistory(integer, timestamp without time zone, timestamp without time zone);
\i ./functions/io_objects/getioattrvaluehistory.sql
\i ./functions/io_objects/getioattrvalue.sql

commit;
