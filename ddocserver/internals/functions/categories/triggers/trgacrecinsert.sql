create or replace function aRecInsertCheck() returns trigger as
$BODY$
declare
    idCategory bigint;

    idMainCategory bigint;
    idType bigint;
    isCatKind int4;
    cnt bigint;
    r record;
    rr record;
    q varchar;
    attrCode varchar;
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

    if(TG_OP = 'UPDATE') then
        if(new.is_mandatory = old.is_mandatory or (new.is_mandatory = false and old.is_mandatory = true)) then
            return new; --nothing to check
        end if;
    end if;

    if(TG_OP = 'INSERT') then
        if(new.is_mandatory = false) then
            return new; --nothing to check
        end if;
    end if;

    if(new.def_value is not null) then
        return new; --nothing to check
    end if;

    --in all other cases new.is_mandatory = true and/or its value was changed from false and new.def_value isnull

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

        elsif (cnt > 0) then
            select code into attrCode from attributes where id = new.id_io_attribute;
            if(attrCode isnull) then
                raise exception 'Incorrect attribute added to attrs_categories table!';
                return null;
            end if;

            for r in select table_name from io_objects where id_io_category = idMainCategory and table_name is not null
            loop
                q = 'select count(id) as cnt from ' || r.table_name || ' where "' || attrCode || '" isnull';
                for rr in execute q
                loop
                    if(rr.cnt > 0) then
                        raise exception 'Cannot change is_mandatory flag';
                        return null;
                    end if;
                end loop;
            end loop;

            return new;
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
