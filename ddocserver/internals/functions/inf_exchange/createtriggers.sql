--invoked in ./internals/initio_triggers.sql

select f_safe_drop_trigger('trgsyncrecords', 'io_objects');

select f_create_trigger('trgsyncrecords', 'before', 'insert or update or delete', 'io_objects', 'syncrecords()');

/*
create trigger trgSyncRecords
before insert or update or delete
on tbl_io_objects
for each row 
execute procedure syncRecords();
*/

select f_safe_drop_trigger('trgsyncrecords', 'io_categories');

select f_create_trigger('trgsyncrecords', 'before', 'insert or update or delete', 'io_categories', 'syncrecords()');

/*create trigger trgSyncRecords
before insert or update or delete
on io_categories
for each row 
execute procedure syncRecords();
*/

select f_safe_drop_trigger('trgsyncrecords', 'object_ref_tables');

create trigger trgSyncRecords
before insert or update or delete
on object_ref_tables
for each row 
execute procedure syncRecords();

select f_safe_drop_trigger('trgsyncrecords', 'alarm_events_q');

create trigger trgSyncRecords
before insert or update or delete
on alarm_events_q
for each row 
execute procedure syncRecords();

select f_safe_drop_trigger('trgsyncrecords', 'fault_devices_q');

create trigger trgSyncRecords
before insert or update or delete
on fault_devices_q
for each row 
execute procedure syncRecords();

select f_safe_drop_trigger('trgsyncrecords', 'dev_object_q');

create trigger trgSyncRecords
before insert or update or delete
on dev_object_q
for each row 
execute procedure syncRecords();


select f_safe_drop_trigger('trgsyncrecords', 'personal_q');

create trigger trgSyncRecords
before insert or update or delete
on personal_q
for each row 
execute procedure syncRecords();

select f_safe_drop_trigger('trgsyncrecords', 'access_cards_q');

create trigger trgSyncRecords
before insert or update or delete
on access_cards_q
for each row 
execute procedure syncRecords();

select f_safe_drop_trigger('trgsyncrecords', 'access_plan');

create trigger trgSyncRecords
before insert or update or delete
on access_plan
for each row 
execute procedure syncRecords();


select f_safe_drop_trigger('trgfirstsyncio', 'io_objects_organization');

create trigger trgFirstSyncIO
before insert 
on io_objects_organization
for each row 
execute procedure firstSyncIO();

