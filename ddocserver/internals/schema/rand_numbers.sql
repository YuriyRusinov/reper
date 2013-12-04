create table random_values (id bigint not null primary key,
                            id_partition_low integer not null default 1,
                            value float8 not null);

create sequence random_values_id;

alter table random_values ALTER column id SET DEFAULT nextval ('random_values_id');

alter table random_values add constraint fk_random_values_ref_partition FOREIGN KEY (id_partition_low) REFERENCES partition_lows(id) ON UPDATE RESTRICT ON DELETE RESTRICT;
