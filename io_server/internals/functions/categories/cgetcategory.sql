select f_safe_drop_type('h_get_category');
create type h_get_category as(id int4,
                              id_io_category_type int4,
                              id_child int4,
                              c_name varchar,
                              c_desc varchar,
                              t_name varchar,
                              t_desc varchar,
                              is_main boolean,
                              c_code varchar,
                              unique_id varchar,
                              is_system boolean,
                              id_io_state int4,
                              st_name varchar,
                              st_desc varchar,
                              is_global boolean,
                              is_qualifier boolean,
                              id_child2 int4,
                              id_life_cycle int4);

create or replace function cGetCategory(int4) returns setof h_get_category as
$BODY$
declare
    idCategory alias for $1;
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
            c.id_child2,
            c.id_life_cycle
        from 
            io_categories c,
            io_category_types t,
            io_states st
        where 
            c.id = idCategory
            and c.id_io_category_type = t.id
            and st.id = c.id_io_state
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function cGetCategory (varchar) returns setof h_get_category as
$BODY$
declare
    tableName alias for $1;
    r h_get_category%rowtype;
begin

    for r in
        select distinct
            cc.id, 
            cc.id_io_category_type, 
            cc.id_child, 
            cc.name, 
            cc.description, 
            t.name, 
            t.description, 
            cc.is_main, 
            cc.code,
            cc.unique_id,
            cc.is_system,
            cc.id_io_state,
            st.name,
            st.description,
            cc.is_global,
            t.is_qualifier,
            cc.id_child2,
            cc.id_life_cycle
        from 
            f_sel_io_objects_tbl(tableName) io
            inner join io_categories c on (c.id = io.id_io_category and io.table_name = tableName)
            inner join io_categories cc on (c.id_child = cc.id)
            inner join io_category_types t on (cc.id_io_category_type = t.id)
            inner join io_states st on (cc.id_io_state = st.id)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function cGetCategoryIO (int4) returns setof h_get_category as
$BODY$
declare
    idObject alias for $1;
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
            c.id_child2,
            c.id_life_cycle
        from 
            io_categories c inner join
            io_category_types t on (c.id_io_category_type = t.id)
            inner join io_states st on (c.id_io_state = st.id)
            inner join f_sel_io_objects(idObject) io on (c.id = io.id_io_category and io.id = idObject)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function cGetParentCategory(int4) returns setof h_get_category as
$BODY$
declare
    idCategory alias for $1;
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
            c.id_child2,
            c.id_life_cycle
        from 
            io_categories c,
            io_category_types t,
            io_states st
        where 
            c.id_child = idCategory
            and c.id_io_category_type = t.id
            and st.id = c.id_io_state
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function cGetChildCategory(int4) returns setof h_get_category as
$BODY$
declare
    idCategory alias for $1;
    r h_get_category%rowtype;
begin

    for r in
        select 
            cc.id, 
            cc.id_io_category_type, 
            cc.id_child, 
            cc.name, 
            cc.description, 
            t.name, 
            t.description, 
            cc.is_main, 
            cc.code,
            cc.unique_id,
            cc.is_system,
            cc.id_io_state,
            st.name,
            st.description,
            cc.is_global,
            t.is_qualifier,
            cc.id_child2,
            cc.id_life_cycle
        from 
            io_categories c
            inner join io_categories cc on (c.id_child = cc.id)
            inner join io_category_types t on (cc.id_io_category_type = t.id)
            inner join io_states st on (cc.id_io_state = st.id)
        where 
            c.id = idCategory
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';



