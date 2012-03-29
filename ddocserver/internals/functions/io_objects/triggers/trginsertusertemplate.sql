create or replace function insertUserTemplateCheck() returns trigger as
$BODY$
declare
    idCategory int4;
    idTemplate int4;
begin
    
    if (new.type = 0) then
        select c.id_child into idCategory 
        from io_categories c, f_sel_io_objects(new.id_io_object) o 
        where
            o.id = new.id_io_object 
            and o.id_io_category = c.id;
        
        if (idCategory isnull) then
            return NULL;
        end if;

        select t.id into idTemplate from io_templates t where t.id_io_category = idCategory and t.id = new.id_template;
        if (idTemplate isnull) then
            return NULL;
        end if;
        
        return new;
    end if;

    if (new.type = 1) then
        
        select c.id into idCategory 
        from io_categories c, f_sel_io_objects(new.id_io_object) o 
        where
            o.id = new.id_io_object 
            and o.id_io_category = c.id;
        
        if (idCategory isnull) then
            return NULL;
        end if;

        select t.id into idTemplate from io_templates t where t.id_io_category = idCategory and t.id = new.id_template;
        if (idTemplate isnull) then
            return NULL;
        end if;
        
        return new;
    end if;

    return NULL;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginsertusertemplate', 'user_templates');


create trigger trgInsertUserTemplate
before insert or update
on user_templates
for each row 
execute procedure insertUserTemplateCheck();
