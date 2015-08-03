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

create unique index i_attrs_values on attrs_values using BTREE (
id_io_object,
id_io_attribute
);

\i ./functions/readd_functions.sql

create  index i_attrs_values_o on tbl_attrs_values using BTREE (
id_io_object
);

alter table io_objects rename to tbl_io_objects;
alter sequence io_objects_id_seq rename to tbl_io_objects_id_seq;
alter table attrs_values rename to tbl_attrs_values;

\i ./functions/security/readd_security.sql

select setGrantsEx(NULL);

\i ./functions/security/ctrl/acl_secure_io_objects_apply.sql
\i ./functions/security/ctrl/acl_secure_attr_values_apply.sql


create or replace view io_objects as select * from f_sel_io_objects(NULL);
grant all on io_objects to public;

create or replace view attrs_values as select * from f_sel_attrs_values(NULL);
grant all on attrs_values to public;



--!!!!! Разграничение доступа в этот момент должно быть применено!!!
alter table access_table drop constraint FK_ACCESS_T_REFERENCE_IO_OBJEC;
alter table access_table
   add constraint FK_ACCESS_T_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references tbl_io_objects (id)
      on delete cascade on update cascade;

update access_table set allow_update = true where id_io_object = 7 and id_role = -10;

alter table access_categories_table drop constraint FK_ACCESS_C_REFERENCE_IO_CATEG;
alter table access_categories_table
   add constraint FK_ACCESS_C_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete cascade on update cascade;


select setAsLogging(1);
select setAsLogging(2);


commit;
