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


create or replace function searchTemplateInsertAfter() returns trigger as
$BODY$
declare
    ok bool;
begin

    if(new.id_io_category is not null) then
        ok := isApplicable(new.id, new.id_io_category);
        if(ok <> TRUE) then
            raise exception 'Given search template is not applicable with its category! Choose another category!';
            return NULL;
        end if;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgsearchtemplateinsertafter', 'search_templates');

create trigger trgSearchTemplateInsertAfter
after insert or update
on search_templates
for each row 
execute procedure searchTemplateInsertAfter();
