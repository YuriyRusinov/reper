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

delete from attrs_categories where id_io_category = 113 and id_io_attribute in(144, 148, 149, 150, 153);


select f_safe_drop_trigger('trgioupdate', 'io_objects');
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinsertafter', 'io_objects');

select f_safe_drop_table('tso_params');

update io_objects set table_name = NULL, name = 'Настройки системы ТСО', id_io_category = 151 where id_io_category = 152;
update attrs_categories set is_read_only = false where id_io_category = 151;
delete from attrs_categories where id_io_category = 151 and id_io_attribute = 1;

delete from io_life_cycle where id_io_category = 152;

update io_categories set id_io_type = 11, is_main = true, id_child = NULL where id = 151;
delete from io_categories where id = 152;

update attributes set id_a_type = 21 where id = 215;

CREATE TRIGGER trgioinsert
  BEFORE INSERT OR UPDATE
  ON io_objects
  FOR EACH ROW
  EXECUTE PROCEDURE ioinsertcheck();

CREATE TRIGGER trgioupdate
  AFTER UPDATE
  ON io_objects
  FOR EACH ROW
  EXECUTE PROCEDURE ioupdatechecktable();

CREATE TRIGGER zz_trgzioinsertafter
  AFTER INSERT OR UPDATE
  ON io_objects
  FOR EACH ROW
  EXECUTE PROCEDURE ioinsertcheckafter();
  
  CREATE TRIGGER zz_trgzioinserttableafter
  AFTER INSERT
  ON io_objects
  FOR EACH ROW
  EXECUTE PROCEDURE ioinsertchecktableafter();

\i ./functions/ik_tso/clear_ik_tso_tables.sql


select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

commit;
