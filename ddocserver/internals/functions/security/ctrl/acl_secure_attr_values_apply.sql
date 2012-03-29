--invoked in ./initdb.sql
--alter table attrs_values rename to tbl_attrs_values;


create or replace view attrs_values as select * from f_sel_attrs_values(NULL);

revoke all on tbl_attrs_values from public;

grant all on attrs_values to public;


create or replace rule "r_ins_attrs_values" as on insert to "attrs_values" do instead select "f_ins_attrs_values"(new.id_io_object, new.id_attr_category, new.value, new.start_time, new.stop_time, new.meas_time, new.id_io_object_src, new.id_io_object_src1, new.description);


create or replace rule "r_del_attrs_values" as on delete to "attrs_values" do instead select "f_del_attrs_values"(old.id_io_object, old.id_attr_category, true);


create or replace rule "r_upd_attrs_values" as on update to "attrs_values" do instead select "f_upd_attrs_values"(new.id_io_object, new.id_attr_category, new.value, new.start_time, new.stop_time, new.meas_time, new.id_io_object_src, new.id_io_object_src1, new.description, old.id_io_object, old.id_attr_category);

