create or replace function searchTemplateDelete() returns trigger as
$BODY$
declare
begin
    if(old.author <> getCurrentUser()) then
        raise exception 'You cannot delete not yours search templates';
        return NULL;
    end if;

    return old;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgsearchtemplatedelete', 'search_templates');

create trigger trgSearchTemplateDelete
before delete
on search_templates
for each row 
execute procedure searchTemplateDelete();
