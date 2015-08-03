create or replace function searchTemplateUpdate() returns trigger as
$BODY$
declare
begin
    if(old.author <> getCurrentUser()) then
        raise exception 'You cannot modify not yours search templates!';
        return NULL;
    end if;

    if(new.author isnull or new.author <> getCurrentUser()) then
        new.author = getCurrentUser();
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgsearchtemplateupdate', 'search_templates');

create trigger trgSearchTemplateUpdate 
before update
on search_templates
for each row 
execute procedure searchTemplateUpdate();
