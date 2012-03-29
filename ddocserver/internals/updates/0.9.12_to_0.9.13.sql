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


alter table graph_plans add column is_completed bool;
alter table devices add column is_completed bool;
alter table persons add column is_completed bool;
alter table fault_devices add column is_completed bool;
alter table alarm_events add column is_completed bool;
alter table access_cards add column is_completed bool;
alter table access_plan add column is_completed bool;
alter table guard_objects add column is_completed bool;

alter table alarm_events_q add column id_ae int4;
alter table personal_q add column id_p int4;
alter table access_cards_q add column id_ac int4;


select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

commit;
