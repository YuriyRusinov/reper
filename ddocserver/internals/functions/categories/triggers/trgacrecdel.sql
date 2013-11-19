create or replace function aRecDelCheck() returns trigger as
$BODY$
declare
    idCategory bigint;

    idMainCategory bigint;
    idType bigint;
    isCatKind int4;
    cnt bigint;
begin
    idCategory := old.id_io_category;
    select into idType id_io_category_type from io_categories where id=idCategory;
    if (idType <> 10) then
        idMainCategory := idCategory;
        isCatKind := 0;
    else
        --
        -- table or indiator category
        --
        select into idMainCategory id from io_categories where id_child = idCategory;
        isCatKind := 1;
        if (idMainCategory is null) then
            select into idMainCategory id from io_categories where id_child2 = idCategory;
            isCatKind := 2;
        end if;
        if (idMainCategory is null) then
            --
            -- If main category already deleted, but the record or table category attributes are inserted
            --
            return old;
        end if;

    end if;
    if (isCatKind = 1) then
        --
        -- insert or update attribute in table category
        --
        select into cnt count(*) from io_objects where id_io_category = idMainCategory;
        if (cnt = 0) then
            --
            -- IOs with tables for this category do not exist
            --
            return old;
        else
            raise exception 'Delete attribute from category with existing objects';
            return null;
        end if;
    return old;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');

select f_create_trigger('trgacrecinsert', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');
