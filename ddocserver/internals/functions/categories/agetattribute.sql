select f_safe_drop_type('h_get_attribute');

create type h_get_attribute as(     id_attribute int4, 
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
                                    attr_group_id int4,
                                    attr_group_name varchar,
                                    id_ex int4, --id либо из таблицы attrs_attrs либо attrs_categories (в зависимости от вызываемой функции)
                                    unique_id_ex varchar);--unique_id либо из таблицы attrs_attrs либо attrs_categories (в зависимости от вызываемой функции)

create or replace function aGetAttribute(int4) returns setof h_get_attribute as
$BODY$
declare
    idAttr alias for $1;
    r h_get_attribute%rowtype;
    rr RECORD;
begin

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
            NULL, --def_value
            NULL, --is_mandatory
            NULL, --is_read_only
            a.id_ref_attr_type as ref_attr_type_id, --(case when a.column_name isnull then NULL else (select a1.id_a_type from attributes a1 where a1.code = a.column_name) end) as ref_attr_type 
            NULL as ref_attr_type_name,
            NULL as ref_attr_type_code,
            a.unique_id,
            a.id_search_template,
            a.ref_column_name,
            a.id_attr_group,
            ag.name,
            NULL,
            NULL
        from  
            attributes a,
            a_types att,
            attrs_groups ag
        where 
	    a.id = idAttr
	    and a.id_a_type = att.id
            and a.id_attr_group = ag.id
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

create or replace function aGetAttributeByUID(varchar) returns setof h_get_attribute as
$BODY$
declare
    attrUID alias for $1;
    r h_get_attribute%rowtype;
    rr RECORD;
begin

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
            NULL, --def_value
            NULL, --is_mandatory
            NULL, --is_read_only
            a.id_ref_attr_type as ref_attr_type_id, --(case when a.column_name isnull then NULL else (select a1.id_a_type from attributes a1 where a1.code = a.column_name) end) as ref_attr_type 
            NULL as ref_attr_type_name,
            NULL as ref_attr_type_code,
            a.unique_id,
            a.id_search_template,
            a.ref_column_name,
            a.id_attr_group,
            ag.name,
            NULL,
            NULL
        from  
            attributes a,
            a_types att,
            attrs_groups ag
        where 
	    a.unique_id = attrUID
	    and a.id_a_type = att.id
            and a.id_attr_group = ag.id
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

create or replace function aGetAttribute(varchar, varchar) returns setof h_get_attribute as
$BODY$
declare
    attrCode alias for $1;
    tableName alias for $2;

    r h_get_attribute%rowtype;
    rr RECORD;
begin

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
            ac.def_value,
            ac.is_mandatory,
            ac.is_read_only,
            a.id_ref_attr_type as ref_attr_type_id, --(case when a.column_name isnull then NULL else (select a1.id_a_type from attributes a1 where a1.code = a.column_name) end) as ref_attr_type 
            NULL as ref_attr_type_name,
            NULL as ref_attr_type_code,
            a.unique_id,
            a.id_search_template,
            a.ref_column_name,
            a.id_attr_group,
            ag.name,
            ac.id,
            ac.unique_id

        from  
            attributes a,
            a_types att,
            io_objects io,
            attrs_categories ac,
            io_categories c,
            attrs_groups ag
        where 
	    a.code = attrCode
	    and a.id_a_type = att.id
            and io.table_name = tableName
            and io.id_io_category = c.id
            and c.id_child = ac.id_io_category
            and ac.id_io_attribute = a.id
            and a.id_attr_group = ag.id
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
