--invoked in ./initdb.sql
--alter table rec_attrs_values rename to tbl_rec_attrs_values;


create or replace view rec_attrs_values as select * from f_sel_rec_attrs_values(NULL::int8);

revoke all on tbl_rec_attrs_values from public;

grant all on rec_attrs_values to public;


create or replace rule "r_ins_rec_attrs_values" as on insert to "rec_attrs_values" do instead select "f_ins_rec_attrs_values"(new.id_record, new.id_attr_category, new.value, new.start_time, new.stop_time, new.meas_time, new.id_io_object_src, new.id_io_object_src1, new.description);


create or replace rule "r_del_rec_attrs_values" as on delete to "rec_attrs_values" do instead select "f_del_rec_attrs_values"(old.id_record, old.id_attr_category, true);


create or replace rule "r_upd_rec_attrs_values" as on update to "rec_attrs_values" do instead select "f_upd_rec_attrs_values"(new.id_record, new.id_attr_category, new.value, new.start_time, new.stop_time, new.meas_time, new.id_io_object_src, new.id_io_object_src1, new.description, old.id_record, old.id_attr_category, true);

