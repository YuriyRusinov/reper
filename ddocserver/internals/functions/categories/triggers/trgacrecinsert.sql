create or replace function aRecInsertCheck() returns trigger as
$BODY$
declare
    idCategory bigint;

    idMainCategory bigint;
    idType bigint;
    isTableCat boolean;
    cnt bigint;
begin

    idCategory := new.id_io_category;
    select into idType id_io_category_type from io_categories where id=idCategory;
    if (idType = 10) then
        select into idMainCategory id from io_categories where id_child = idCategory;
        if (idMainCategory is not null) then
            ---
            --- table category
            ---
            isTableCat := true;
        else
            select into idMainCategory id from io_categories where id_child2 = idCategory;
            if (idMainCategory is null) then
                raise exception 'Incorrect category';
                return null;
            end if;
            isTableCat := false;
        end if;
    else
        idMainCategory := idCategory;
        isTableCat := false;
    end if;
    if (isTableCat) then
        select into cnt count(*) from io_objects where id_io_category = idMainCategory;
        if (cnt > 0) then
            raise exception 'Insert attribute into category with existing objects.';
            return null;
        end if;
    else
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');

select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
