create table time_units (
    id SERIAL not null,
    name VARCHAR not null,
    short_name VARCHAR not null,
    constraint PK_A_TIME_UNITS primary key (id)
)
inherits (root_table);

select setMacToNULL('time_units');

create trigger trgsetuid BEFORE INSERT OR UPDATE ON time_units 
for each row execute procedure uidCheck();

alter table message_streams add column id_time_unit int4 not null default 1;
alter table message_streams add constraint fk_time_units FOREIGN KEY (id_time_unit)
    REFERENCES time_units (id);

create table message_series (
    id serial not null,
    id_message_stream int4 not null,
    time timestamp not null,
    time_step float8 not null default 1.0,
    constraint PK_MESSAGE_SERIES primary key (id)
)
inherits (root_table);

select setMacToNULL ('message_series');

create trigger trgsetuid BEFORE INSERT OR UPDATE ON message_series 
for each row execute procedure uidCheck();

alter table message_series add constraint fk_message_stream FOREIGN KEY (id_message_stream)
    REFERENCES message_streams (id);

