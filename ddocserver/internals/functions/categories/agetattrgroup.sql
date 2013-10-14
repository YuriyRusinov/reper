select f_safe_drop_type('h_get_attrgroup');

create type h_get_attrgroup as (id_attr_group int4,
                                id_parent int4,
                                name varchar
                                );

create or replace function aGetAttrGroup (int4) returns setof h_get_attrgroup as
$BODY$
declare
    idAttrGroup alias for $1;

    query varchar;
    rec h_get_attrgroup%rowtype;
    rrec h_get_attrgroup%rowtype;
begin

    query := E'select ag.id, ag.id_parent, ag.name from attrs_groups ag where ag.id=' || idAttrGroup;
    for rec in
        execute query
    loop
        return next rec;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function aGetAttrGroups (int4) returns setof h_get_attrgroup as
$BODY$
declare
    idAttrPGroup alias for $1;

    query varchar;
    rec h_get_attrgroup%rowtype;
    rrec h_get_attrgroup%rowtype;
begin

    query := E'select ag.id, ag.id_parent, ag.name from attrs_groups ag where ';
    if (idAttrPGroup is null) then
        query := query || E' ag.id_parent is null';
    else
        query := query || E' ag.id_parent = ' || idAttrPGroup;
    end if;
    for rec in
        execute query
    loop
        return next rec;
        for rrec in
            select * from aGetAttrGroups (rec.id_attr_group)
        loop
            return next rrec;
        end loop;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function aGetAttributesList (int4) returns setof h_get_attribute as
$BODY$
declare
    idAttrGroup alias for $1;

    r h_get_attribute%rowtype;
    rr record;
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
            att_ref.name as ref_attr_type_name,
            att_ref.code as ref_attr_type_code,
            a.unique_id,
            a.id_search_template,
            a.ref_column_name,
            a.id_attr_group,
            ag.name,
            NULL
        from  
            attributes a left join a_types att_ref on (att_ref.id = a.id_ref_attr_type)
            inner join a_types att on (a.id_a_type = att.id)
            inner join attrs_groups ag on(a.id_attr_group = ag.id and a.id_attr_group = idAttrGroup )
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';
