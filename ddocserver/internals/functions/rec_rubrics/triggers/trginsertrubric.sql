create or replace function insertRubricCheck() returns trigger as
$BODY$
declare
    idCategory int4;
    idTemplate int4;
begin
    
    if(new.id_parent is not null and new.id_io_object is not null) then
        --Now not allowed to set id_parent and id_io_object together
        return NULL;
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginsertrubric', 'rubricator');

create trigger trgInsertRubric
before insert or update
on rubricator
for each row 
execute procedure insertRubricCheck();
