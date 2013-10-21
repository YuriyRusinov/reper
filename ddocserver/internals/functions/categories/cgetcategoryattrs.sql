/*select f_safe_drop_type('h_get_category_attrs');
create type h_get_category_attrs as(id_attribute int4, 
                                    id_attr_type int4,
                                    attr_code varchar,
                                    attr_name varchar,
                                    attr_title varchar,
                                    table_name varchar,
                                    column_name varchar,
                                    def_width int4,
                                    attr_type_name varchar,
                                    attr_type_code varchar,
                                    def_value varchar,
                                    is_mandatory boolean,
                                    is_read_only boolean,
                                    ref_attr_type_id int4,
                                    ref_attr_type_name varchar,
                                    ref_attr_type_code varchar,
                                    unique_id varchar,
                                    id_search_template int4,
                                    ref_column_name varchar,
                                    id_ex,
                                    unique_id_ex);
*/

create or replace function cGetCategoryAttr(int4) returns setof h_get_attribute as
$BODY$
declare
    idCategoryAttr alias for $1;
    r h_get_attribute%rowtype;
    rr RECORD;
    query varchar;
begin

    query := E'select 
            a.id, 
            att.id, 
            a.code, 
            a.name, 
            a.title, 
            a.table_name, 
            a.column_name, 
            a.def_width, 
            att.name, 
            att.code, 
            ca.def_value, 
            ca.is_mandatory, 
            ca.is_read_only,
            a.id_ref_attr_type as ref_attr_type_id,  
            NULL as ref_attr_type_name,
            NULL as ref_attr_type_code,
            a.unique_id,
            a.id_search_template,
            a.ref_column_name,
            a.id_attr_group,
            ag.name,
            ca.id,
            ca.unique_id
        from  
            attrs_categories ca 
            inner join attributes a on (ca.id = ' || idCategoryAttr || E' and ca.id_io_attribute = a.id) 
            inner join a_types att on (a.id_a_type = att.id) 
            inner join attrs_groups ag on (a.id_attr_group = ag.id)';
    for r in 
        execute query
    loop
        if(r.column_name is not null) then
            for rr in 
                select * from atGetAttrType(r.ref_attr_type_id)
            loop
                --r.ref_attr_type_id = rr.id;
                r.ref_attr_type_name = rr.name;
                r.ref_attr_type_code = rr.code;
            end loop;  
        end if;
        
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function cGetCategoryAttr(int4, int4) returns setof h_get_attribute as
$BODY$
declare
    idCategory alias for $1;
    idAttr alias for $2;

    r h_get_attribute%rowtype;
    rr RECORD;
    query varchar;
begin

    query := E'select 
            a.id, 
            att.id, 
            a.code, 
            a.name, 
            a.title, 
            a.table_name, 
            a.column_name, 
            a.def_width, 
            att.name, 
            att.code, 
            ca.def_value, 
            ca.is_mandatory, 
            ca.is_read_only,
            a.id_ref_attr_type as ref_attr_type_id,  
            NULL as ref_attr_type_name,
            NULL as ref_attr_type_code,
            a.unique_id,
            a.id_search_template,
            a.ref_column_name,
            a.id_attr_group,
            ag.name,
            ca.id,
            ca.unique_id
        from  
            attrs_categories ca 
            inner join attributes a on (ca.id_io_category = ' || idCategory || E' and ca.id_io_attribute = ' || idAttr || E' and ca.id_io_attribute = a.id) 
            inner join a_types att on (a.id_a_type = att.id) 
            inner join attrs_groups ag on (a.id_attr_group = ag.id)';
    for r in 
        execute query
    loop
        if(r.column_name is not null) then
            for rr in 
                select * from atGetAttrType(r.ref_attr_type_id)
            loop
                --r.ref_attr_type_id = rr.id;
                r.ref_attr_type_name = rr.name;
                r.ref_attr_type_code = rr.code;
            end loop;  
        end if;
        
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function cGetCategoryAttrs(int4) returns setof h_get_attribute as
$BODY$
declare
    idCategory alias for $1;
    r h_get_attribute%rowtype;
    rr RECORD;
    query varchar;
begin

    query := E'select 
            a.id, 
            att.id, 
            a.code, 
            a.name, 
            a.title, 
            a.table_name, 
            a.column_name, 
            a.def_width, 
            att.name, 
            att.code, 
            ca.def_value, 
            ca.is_mandatory, 
            ca.is_read_only,
            a.id_ref_attr_type as ref_attr_type_id,  
            NULL as ref_attr_type_name,
            NULL as ref_attr_type_code,
            a.unique_id,
            a.id_search_template,
            a.ref_column_name,
            a.id_attr_group,
            ag.name,
            ca.id,
            ca.unique_id
        from  
            attrs_categories ca inner join attributes a on (ca.id_io_category = ' || idCategory || E' and ca.id_io_attribute = a.id) inner join a_types att on (a.id_a_type = att.id) inner join attrs_groups ag on (a.id_attr_group = ag.id)';
    raise warning 'query is %', query;
--(case when a.column_name isnull then NULL else (select a1.id_a_type from attributes a1 where a1.code = a.column_name) end) as ref_attr_type
    for r in 
        execute query
    loop
        if(r.column_name is not null) then
            for rr in 
                select * from atGetAttrType(r.ref_attr_type_id)
            loop
                --r.ref_attr_type_id = rr.id;
                r.ref_attr_type_name = rr.name;
                r.ref_attr_type_code = rr.code;
            end loop;  
        end if;
        
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function cGetSystemAttrs () returns setof h_get_attribute as
$BODY$
declare
    r h_get_attribute%rowtype;
    rr RECORD;
    idCategory int4;
begin

    select into idCategory c.id_child from io_categories c inner join f_sel_io_objects(7) io on (c.id=io.id_io_category and io.id=7);

    for r in 
        select 
            a.id, 
            att.id, 
            a.code, 
            a.name, 
            a.title, 
            a.table_name, 
            a.column_name, 
            a.def_width, 
            att.name, 
            att.code, 
            ca.def_value, 
            ca.is_mandatory, 
            ca.is_read_only,
            a.id_ref_attr_type as ref_attr_type_id, --(case when a.column_name isnull then NULL else (select a1.id_a_type from attributes a1 where a1.code = a.column_name) end) as ref_attr_type 
            NULL as ref_attr_type_name,
            NULL as ref_attr_type_code,
            a.unique_id,
            a.id_search_template,
            a.ref_column_name,
            a.id_attr_group,
            ag.name,
            ca.id,
            ca.unique_id
        from  
            attrs_categories ca,
            attributes a,
            a_types att ,
            attrs_groups ag
        where 
	    ca.id_io_category = idCategory
	    and ca.id_io_attribute = a.id 
	    and a.id_a_type = att.id
            and ag.id = a.id_attr_group
    loop
        if(r.column_name is not null) then
            for rr in 
                select * from atGetAttrType(r.ref_attr_type_id)
            loop
                --r.ref_attr_type_id = rr.id;
                r.ref_attr_type_name = rr.name;
                r.ref_attr_type_code = rr.code;
            end loop;  
        end if;
        
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';
