/*select f_safe_drop_type('h_get_category_attrs');
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
                                    id_ex int4, --id либо из таблицы attrs_attrs либо category_attrs (в зависимости от вызываемой функции)
                                    unique_id_ex);--unique_id либо из таблицы attrs_attrs либо category_attrs (в зависимости от вызываемой функции)
*/

create or replace function aGetAttrAttrs(int4) returns setof h_get_attribute as
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
            aa.def_value, 
            aa.is_mandatory, 
            aa.is_read_only,
            a.id_ref_attr_type as ref_attr_type_id, --(case when a.column_name isnull then NULL else (select a1.id_a_type from attributes a1 where a1.code = a.column_name) end) as ref_attr_type 
            NULL as ref_attr_type_name,
            NULL as ref_attr_type_code,
            a.unique_id,
            a.id_search_template,
            a.ref_column_name,
            a.id_attr_group,
            ag.name,
            aa.id,
            aa.unique_id
        from  
            attrs_attrs aa,
            attributes a,
            a_types att,
            attrs_groups ag
        where 
	    aa.id_attr_parent = idAttr
	    and aa.id_attr_child = a.id 
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
