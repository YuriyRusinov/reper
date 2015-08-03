--добавляемые в рубрику ИО. должны соответствовать категории,
--которая указана в  качестве параметра рубрики (если задана)
create or replace function insertIORubricCheck() returns trigger as
$BODY$
declare
    idCategory int4;
    idCat int4;
begin
    
    select id_io_category into idCategory from rubricator where id = new.id_rubric;
    if(idCategory isnull) then
        return new;
    end if;

    select id_io_category into idCat from io_objects where id = new.id_io_object;
    if(idCat <> idCategory) then
        return NULL;
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginsertiorubric', 'io_rubricator');

create trigger trgInsertIORubric
before insert or update
on io_rubricator
for each row 
execute procedure insertIORubricCheck();
