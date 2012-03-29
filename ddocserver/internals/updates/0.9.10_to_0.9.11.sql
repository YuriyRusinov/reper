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


alter table ranks add column unique_id VARCHAR;
alter table ranks add column last_update TIMESTAMP;
update ranks set unique_id = ('localorg-ranks-' || id)::varchar, last_update = current_timestamp;

alter table ranks alter column last_update set not null;
alter table ranks alter column last_update set default CURRENT_TIMESTAMP;
alter table ranks alter column unique_id set not null;

alter table ranks inherit root_table;
select setMacToNULL('ranks');
select createTriggerUID('ranks');


alter table work_mode_type add column unique_id VARCHAR;
alter table work_mode_type add column last_update TIMESTAMP;
update work_mode_type set unique_id = ('localorg-work_mode_type-' || id)::varchar, last_update = current_timestamp;

alter table work_mode_type alter column last_update set not null;
alter table work_mode_type alter column last_update set default CURRENT_TIMESTAMP;
alter table work_mode_type alter column unique_id set not null;

alter table work_mode_type inherit root_table;
select setMacToNULL('work_mode_type');
select createTriggerUID('work_mode_type');


alter table user_state add column unique_id VARCHAR;
alter table user_state add column last_update TIMESTAMP;
update user_state set unique_id = ('localorg-user_state-' || id)::varchar, last_update = current_timestamp;

alter table user_state alter column last_update set not null;
alter table user_state alter column last_update set default CURRENT_TIMESTAMP;
alter table user_state alter column unique_id set not null;

alter table user_state inherit root_table;
select setMacToNULL('user_state');
select createTriggerUID('user_state');


alter table organization_type  add column unique_id VARCHAR;
alter table organization_type add column last_update TIMESTAMP;
update organization_type set unique_id = ('localorg-organization_type-' || id)::varchar, last_update = current_timestamp;

alter table organization_type alter column last_update set not null;
alter table organization_type alter column last_update set default CURRENT_TIMESTAMP;
alter table organization_type alter column unique_id set not null;

alter table organization_type inherit root_table;
select setMacToNULL('organization_type');
select createTriggerUID('organization_type');

alter table urgency_levels add column unique_id VARCHAR;
alter table urgency_levels add column last_update TIMESTAMP;
update urgency_levels set unique_id = ('localorg-urgency_levels-' || id)::varchar, last_update = current_timestamp;

alter table urgency_levels alter column last_update set not null;
alter table urgency_levels alter column last_update set default CURRENT_TIMESTAMP;
alter table urgency_levels alter column unique_id set not null;

alter table urgency_levels inherit root_table;
select setMacToNULL('urgency_levels');
select createTriggerUID('urgency_levels');

delete from attrs_categories where id_io_category = 113 and id_io_attribute in(144, 148, 149, 150, 153);

--create unique index i_transport_name on transport using BTREE (
--name
--);

update transport set name = 'Транспорт TCP/IP' where id = 1;

select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

commit;
