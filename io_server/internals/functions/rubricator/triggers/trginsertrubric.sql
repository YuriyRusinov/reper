create or replace function insertRubricCheck() returns trigger as
$BODY$
declare
    idCategory int4;
    idTemplate int4;
    r record;
begin
    
    if(new.id_parent is not null and new.id_io_object is not null) then
        --Now not allowed to set id_parent and id_io_object together
        return NULL;
    end if;

    if(TG_OP = 'UPDATE') then
        if(new.id_io_category is not null) then
            for r in 
                select io.id_io_category 
                from io_objects io, io_rubricator rr 
                where
                    rr.id_rubric = new.id
                    and rr.id_io_object = io.id
            loop
                if(r.id_io_category <> new.id_io_category) then
                    raise exception 'Rubric contain IO that does not correspond to current rubric category!';
                    return NULL;
                end if;
            end loop;
        end if;
 
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
