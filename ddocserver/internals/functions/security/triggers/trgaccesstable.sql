create or replace function accessTableInsertCheck() returns trigger as
$BODY$
declare
    uid int4;
begin
    
    if(new.allow_delete = TRUE or new.allow_update = TRUE) then
        new.allow_read = true;
    end if;

    if(new.allow_read = true) then
        new.allow_readlist = true;
    end if;

    

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgaccesstable', 'access_table');

create trigger trgAccessTable
before insert or update
on access_table 
for each row 
execute procedure accessTableInsertCheck();
