select f_safe_drop_type('h_get_category_types');
create type h_get_category_types as(id int4,
                                    name varchar,
                                    r_name varchar,
                                    t_desc varchar,
                                    is_qualifier boolean);

create or replace function cGetCategoryType(int4) returns setof h_get_category_types as
$BODY$
declare
    idType alias for $1;
    r h_get_category_types%rowtype;
begin

    for r in
        select 
            id,
            name,
            r_name,
            description,
            is_qualifier
        from
            io_category_types
        where
            id = idType  
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function cGetCategoryTypes() returns setof h_get_category_types as
$BODY$
declare
    r h_get_category_types%rowtype;
begin

    for r in
        select 
            id,
            name,
            r_name,
            description,
            is_qualifier
        from
            io_category_types
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
