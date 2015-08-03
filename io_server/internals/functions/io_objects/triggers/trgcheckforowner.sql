/*create or replace function checkForOwner() returns trigger as
$BODY$
declare
    
begin
    if(checkIOForOwner(new.id_io_object) = false) then
        return NULL;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgcheckforowner', 'urls_objects');

create trigger trgCheckForOwner
before insert or update
on urls_objects
for each row 
execute procedure checkForOwner();

select f_safe_drop_trigger('trgcheckforowner', 'io_rubricator');

create trigger trgCheckForOwner
before insert or update
on io_rubricator
for each row 
execute procedure checkForOwner();

select f_safe_drop_trigger('trgcheckforowner', 'object_ref_tables');

create trigger trgCheckForOwner
before insert or update
on object_ref_tables
for each row 
execute procedure checkForOwner();

select f_safe_drop_trigger('trgcheckforowner', 'access_table');

create trigger trgCheckForOwner
before insert or update
on access_table
for each row 
execute procedure checkForOwner();
*/
