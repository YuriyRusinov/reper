create or replace function cGetCategoriesForRubricator() returns setof h_get_category as
$BODY$
declare
    r h_get_category%rowtype;
begin

    for r in
        select 
            c.id, 
            c.id_io_category_type, 
            c.id_child, 
            c.name, 
            c.description, 
            t.name, 
            t.description, 
            c.is_main, 
            c.code,
            c.unique_id,
            c.is_system,
            c.id_io_state,
            st.name,
            st.description,
            c.is_global,
            t.is_qualifier,
            c.id_child2
        from 
            io_categories c inner join
            io_category_types t on (c.is_main = true and c.id_io_category_type not in (8, 9, 10) and c.id_io_category_type = t.id) inner join
            io_states st on (st.id = c.id_io_state)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
