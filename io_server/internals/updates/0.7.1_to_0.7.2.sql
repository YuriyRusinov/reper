\encoding win1251

begin;

update tsd set exec_period = NULL;
alter table tsd drop column exec_period;
alter table tsd add column exec_period int4[2];


update command_journal set exec_period = NULL;
alter table command_journal drop column exec_period;
alter table command_journal add column exec_period int4[2];
alter table command_journal add column last_exec_datetime timestamp;

update a_types set code = 'int4[2]' where id = 10;

create table cmd_confirmations (id_cmd int4 not null);
alter table cmd_confirmations
   add constraint FK_CMD_CONF_REFERENCE_COMMAND_ foreign key (id_cmd)
      references command_journal (id)
      on delete restrict on update restrict;

alter table message_journal add column extra_id int4;

drop function uInsertInCmd(int4, int4, varchar, int4, varchar, int4, varchar, int4, varchar, timestamp, timestamp, interval, varchar);

\i ./functions/readd_functions.sql

commit;
