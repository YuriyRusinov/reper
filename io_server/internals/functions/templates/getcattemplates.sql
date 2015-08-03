select f_safe_drop_type('h_get_cat_templates');
create type h_get_cat_templates as(id int4, id_io_category int4, name varchar, code varchar, description varchar, a_count int4);

create or replace function getCatTemplates(int4, bool) returns setof h_get_cat_templates as
$BODY$
declare
    idCategory alias for $1;
    parm alias for $2;

    r h_get_cat_templates%rowtype;
    aCount int4;
begin

    if(parm = FALSE) then
        for r in select * from getCatTemplates(idCategory)
        loop
            return next r;
        end loop;

        return;
    end if;

    --exclusion ONLY for IO_OBJECTS table (qualifier).
    --returns all templates in database for that qualifier
    if(idCategory = 13) then
        for r in select * from getCatTemplates(idCategory)
        loop
            return next r;
        end loop;

        return;
    end if;

    select count(id_io_attribute) into aCount from attrs_categories where id_io_category = idCategory and is_mandatory = true;
    if(aCount isnull or aCount = 0) then
        for r in select * from getCatTemplates(idCategory)
        loop
            return next r;
        end loop;
        return;
    end if;

    

    for r in
        select 
            v.id_io_template,
            ac.id_io_category,
            t.name, 
            t.code,
            t.description,
            count(ac.id_io_attribute) as cnt
        from
            io_views v,
            attrs_categories ac,
            io_templates t
        where
            ac.id_io_category = idCategory
            and v.id_attr_category = ac.id
            and ac.is_mandatory = true
            and v.id_io_template = t.id
        group by 1, 2, 3, 4, 5
    loop
        if(aCount = r.a_count) then
            return next r;
        end if;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function getCatTemplates(int4) returns setof h_get_cat_templates as
$BODY$
declare
    idCategory alias for $1;

    r h_get_cat_templates%rowtype;
begin

    for r in
        select 
            t.id,
            t.id_io_category,
            t.name, 
            t.code,
            t.description,
            0
        from
            io_templates t
        where
            t.id_io_category = idCategory
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
