create or replace function searchTemplateInsert() returns trigger as
$BODY$
declare
begin
    if(new.author isnull or new.author <> getCurrentUser()) then
        new.author = getCurrentUser();
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgsearchtemplateinsert', 'search_templates');

create trigger trgSearchTemplateInsert 
before insert
on search_templates
for each row 
execute procedure searchTemplateInsert();
