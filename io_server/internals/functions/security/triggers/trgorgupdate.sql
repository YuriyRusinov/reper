create or replace function orgUpdateCheck() returns trigger as
$BODY$
declare
begin
    
    if(new.id_curr_mode <> old.id_curr_mode) then
        raise notice 'ORGANIZATION WORK MODE CHANGED!';
        new.id_prev_mode = old.id_curr_mode;
        perform userExecutorWorkMode(new.id, new.id_curr_mode, old.id_curr_mode);
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgorgupdate', 'organization');

create trigger trgOrgUpdate
before update
on organization 
for each row 
execute procedure orgUpdateCheck();
