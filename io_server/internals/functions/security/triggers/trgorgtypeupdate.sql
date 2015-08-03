create or replace function orgTypeUpdateCheck() returns trigger as
$BODY$
declare
begin
    if(new.id <= 3) then
        raise exception 'You cannot update system records of organization types!';
        return NULL;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgorgtypeupdate', 'organization_type');

create trigger trgOrgTypeUpdate
before update
on organization_type
for each row 
execute procedure orgTypeUpdateCheck();

