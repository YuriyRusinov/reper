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
        --
        -- insert attribute into table category
        --
        select into cnt count(*) from io_objects where id_io_category = idMainCategory;
        if (cnt = 0) then
            --
            -- IOs with tables for this category do not exist
            --
            return new;
        elsif (cnt > 0 and new.is_mandatory and new.def_value is null ) then
            raise exception 'Insert mandatory attribute with null default values into category % with existing objects.', idMainCategory;
            return null;
        elsif (cnt > 0 and (not new.is_mandatory) or (new.is_mandatory and new.def_value is not null)) then
            raise warning 'Insert nonmandatory attribute or mandatory attribute with default value ';
            --
            -- TODO: mechanism of existing tables update has to be developped.
            --
            return null;
        end if;
    else
        --
        -- insert attribute into rec_attrs_category
        --
        select count(*) into cnt from io_objects io inner join io_objects_organization ioo on (io.id=ioo.id_io_objects and io.id_owner_org!=ioo.id_organization and io.id_io_category= idMainCategory);
        if (cnt = 0 or TG_OP = 'UPDATE') then
            --
            -- syncronization did not make
            --
            return new;
        elsif (cnt > 0) then
            raise exception 'Insert indicator into category with syncronized objects.';
            return null;
        end if;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');

select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
