\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql


--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql



select setAsLogging(1);
select setAsLogging(2);

drop function cGetCategoryAttrs(int4);
drop function cGetSystemAttrs();


\i ./functions/readd_functions.sql
\i ./functions/security/readd_security.sql


alter table attributes add column id_ref_attr_type int4;
alter table attributes
   add constraint FK_ATTRIBUT_REFERENCE_A_TYPES1 foreign key (id_ref_attr_type)
      references a_types (id)
      on delete restrict on update restrict;

update attributes set id_ref_attr_type = (select id from atGetAttrType(column_name, table_name) limit 1);


commit;
