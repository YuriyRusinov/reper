create or replace function aRecInsertCheck() returns trigger as
$BODY$
declare
    idCategory bigint;

    idMainCategory bigint;
    idType bigint;
    isCatKind int4;
    cnt bigint;
begin

    idCategory := new.id_io_category;
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
            -- If main category have not inserted, but the record or table category attributes are inserted
            --
            return new;
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
    elsif (isCatKind = 0 or isCatKind = 2) then
        --
        -- insert attribute into category or rec_attrs_category
        --
        select count(*) into cnt from io_objects io inner join io_objects_organization ioo on (io.id=ioo.id_io_objects and io.id_owner_org!=ioo.id_organization and io.id_io_category= idMainCategory);
        if (cnt = 0) then
            --
            -- syncronization did not make
            --
            return new;
        elsif (cnt > 0 and (not new.is_mandatory) or (new.is_mandatory and new.def_value is not null)) then
            raise warning 'Insert nonmandatory attribute or mandatory attribute with default value ';
            --
            -- TODO: mechanism of existing IOs update has to be developped.
            --
            return null;
        elsif (cnt > 0 and new.is_mandatory and new.def_value is null) then
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
