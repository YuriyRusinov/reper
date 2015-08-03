select f_safe_drop_type('h_get_search_template_type');
create type h_get_search_template_type as (id int4,
                                           name varchar,
                                           description varchar,
                                           id_parent int4,
                                           rec_order int4);

create or replace function ioGetSearchTemplateTypes () returns setof h_get_search_template_type as
$BODY$
declare
    r h_get_search_template_type%rowtype;
begin

    for r in

        with recursive search_template_types_rec (id, "name", "description", "id_parent", ii_rec_order) 
        as 
        (
            select distinct 
                search_template_types.id, 
                search_template_types."name", 
                search_template_types."description", 
                search_template_types."id_parent", 
                0 
            from search_template_types 
            where search_template_types."id_parent" isnull 

            UNION ALL 

            select 
                search_template_types.id, 
                search_template_types."name", 
                search_template_types."description", 
                search_template_types."id_parent", 
                search_template_types_rec.ii_rec_order+1 
            from search_template_types_rec, search_template_types 
            where search_template_types_rec.id = search_template_types."id_parent"
        )

        select distinct 
            search_template_types_rec.id, 
            search_template_types_rec."name", 
            search_template_types_rec."description", 
            search_template_types_rec."id_parent" as "id_parent_fk", 
            search_template_types_rec.ii_rec_order 
        from 
            search_template_types_rec  
        where 1=1  
        order by search_template_types_rec.ii_rec_order

    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


select f_safe_drop_type('h_get_search_template');
create type h_get_search_template as (id int4,
                                      id_group int4,
                                      name varchar,
                                      author int4,
                                      id_search_template_type int4,
                                      id_io_category int4,
                                      authorName varchar,
                                      categoryName varchar,
                                      creation_datetime timestamp,
                                      description varchar);

create or replace function ioGetSearchTemplate () returns setof h_get_search_template as
$BODY$
declare
    r h_get_search_template%rowtype;
begin

    for r in
        select
            st.id,
            st.id_group,
            st.name,
            st.author,
            st.id_search_template_type,
            st.id_io_category,
            case when st.author isnull then null else u.fio end as authorName,
            case when st.id_io_category isnull then null else c.name end as categoryName,
            st.creation_datetime,
            st.description
        from
            search_templates st 
            left join users u on st.author = u.id
            left join io_categories c on st.id_io_category = c.id
        order by st.id
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function ioGetSearchTemplate (int4) returns setof h_get_search_template as
$BODY$
declare
    idSearchTemplate alias for $1;
    r h_get_search_template%rowtype;
begin

    for r in
        select
            st.id,
            st.id_group,
            st.name,
            st.author,
            st.id_search_template_type,
            st.id_io_category,
            case when st.author isnull then null else u.fio end as authorName,
            case when st.id_io_category isnull then null else c.name end as categoryName,
            st.creation_datetime,
            st.description
        from
            search_templates st
            left join users u on st.author = u.id
            left join io_categories c on st.id_io_category = c.id
        where
            st.id = idSearchTemplate
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function ioGetSearchTemplateByType (int4) returns setof h_get_search_template as
$BODY$
declare
    idSearchTemplateType alias for $1;
    r h_get_search_template%rowtype;
begin

    for r in
        select
            st.id,
            st.id_group,
            st.name,
            st.author,
            st.id_search_template_type,
            st.id_io_category,
            case when st.author isnull then null else u.fio end as authorName,
            case when st.id_io_category isnull then null else c.name end as categoryName,
            st.creation_datetime,
            st.description
        from
            search_templates st
            left join users u on st.author = u.id
            left join io_categories c on st.id_io_category = c.id
        where
            st.id_search_template_type = idSearchTemplateType
        order by 1            
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
