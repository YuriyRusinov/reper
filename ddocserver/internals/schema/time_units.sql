create table time_units (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   constraint PK_TIME_UNITS primary key (id)
)
inherits (root_table);

comment on table time_units is
'Единицы измерения времени';

select setMacToNULL('time_units');
select createTriggerUID('time_units');




alter table message_streams add column id_time_unit int4 not null default 1;
alter table message_streams add constraint fk_time_units FOREIGN KEY (id_time_unit)
    REFERENCES time_units (id);

create table message_series (
   id                   SERIAL               not null,
   id_message_stream    INT4                 not null,
   "time"               TIMESTAMP            not null,
   time_step            FLOAT8               not null default 1.0,
   constraint PK_MESSAGE_SERIES primary key (id)
)
inherits (root_table);

select setMacToNULL('message_series');
select createTriggerUID('message_series');

alter table message_series
   add constraint FK_MESSAGE_SERIES__REF_MESSAGE_STREAMS foreign key (id_message_stream)
      references message_streams (id)
      on delete restrict on update restrict;

