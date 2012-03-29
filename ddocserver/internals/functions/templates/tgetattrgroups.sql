select f_safe_drop_type('h_get_attr_groups');
create type h_get_attr_groups as(
                                 id int4,
                                 id_parent_group int4,
                                 id_attr_type int4,
                                 code varchar,
                                 name varchar,
                                 title varchar,
                                 table_name varchar,
                                 column_name varchar,
                                 def_width int4,
                                 type_name varchar,
                                 type_code varchar,
                                 c_is_mandatory boolean,
                                 t_is_read_only boolean,
                                 e_order int4,
                                 t_def_value varchar,
                                 ref_attr_type_id int4,
                                 ref_attr_type_name varchar,
                                 ref_attr_type_code varchar,
                                 entity_type int4,
                                 ref_column_name varchar,
                                 attr_group_id int4,
                                 attr_group_name varchar
                                 );
                                 
create or replace function tGetAttrGroups(int4) returns setof h_get_attr_groups as
$BODY$
declare
    idTemplate alias for $1;
    r h_get_attr_groups%rowtype;
    rr h_get_attr_groups%rowtype;
begin

    for r in
        select
            g.id,
            g.id_parent,
            NULL,
            NULL,
            g.name,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            g."order" as g_order,
            NULL,
            NULL,
            NULL,
            NULL,
            1,
            NULL,
            NULL,
            NULL
        from
            a_groups g
        where
            g.id_parent is null
            and g.id_io_template = idTemplate
        order by
            g_order
    loop
        return next r;
        for rr in
            select * from tGetAttributesFromGroup (r.id, idTemplate)
        loop
            return next rr;
        end loop;

        for rr in
            select * from tGetAttrsSubGroups (r.id, idTemplate)
        loop
            return next rr;
        end loop;
    end loop;
    return;
end
$BODY$
language 'plpgsql';

create or replace function tGetAttributesFromGroup (int4, int4) returns setof h_get_attr_groups as
$BODY$
declare
    idAttrGroup alias for $1;
    idIOTemplate alias for $2;

    r h_get_attr_groups%rowtype;
    rr RECORD;
begin

    for r in
        select distinct
            a.id,
            idAttrGroup,
            att.id,
            a.code,
            a.name,
            a.title,
            a.table_name,
            a.column_name,
            a.def_width,
            att.name as type_name,
            att.code as type_code,
            ac.is_mandatory as c_is_mandatory,
            iv.is_read_only as t_is_read_only,
            iv."order" as e_order,
            iv.def_value as t_def_value,
            att_ref.id as ref_attr_type_id,
            att_ref.name as ref_attr_type_name,
            att_ref.code as ref_attr_type_code,
            0,
            a.ref_column_name,
            a.id_attr_group,
            ag.name
        from
            attributes a inner join a_types att on (a.id_a_type=att.id)
            inner join attrs_categories ac on (a.id=ac.id_io_attribute)
            inner join io_views iv on (ac.id=iv.id_attr_category and iv.id_a_group=idAttrGroup and iv.id_io_template=idIOTemplate)
            inner join attrs_groups ag on (a.id_attr_group = ag.id)

            left join (
                        (select a1.id, a1.code as a_code, a1.id_a_type, io.table_name, a1.column_name from attributes a1
                         inner join io_objects io on (io.table_name is not null)
                         inner join io_categories c on (c.id = io.id_io_category)
                         inner join attrs_categories ac on (c.id_child = ac.id_io_category and ac.id_io_attribute = a1.id)
                        ) as a_ref
                        inner join a_types as att_ref on (a_ref.id_a_type=att_ref.id)
                      )
              on (a_ref.a_code = a.column_name and a.column_name is not null and a_ref.table_name = a.table_name)
            
        order by
            e_order

    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';


create or replace function tGetAttrsSubGroups(int4, int4) returns setof h_get_attr_groups as
$BODY$
declare
    idParentGroup alias for $1;
    idTemplate alias for $2;

    r h_get_attr_groups%rowtype;
    rr h_get_attr_groups%rowtype;
begin
    for r in
        select
            g.id,
            g.id_parent,
            NULL,
            NULL,
            g.name,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            g."order" as g_order,
            NULL,
            NULL,
            NULL,
            NULL,
            1,
            NULL,
            NULL,
            NULL
        from
            a_groups g
        where
            g.id_parent = idParentGroup 
        order by
            g_order
    loop
        return next r;

        for rr in
            select * from tGetAttributesFromGroup (r.id, idTemplate)
        loop
            return next rr;
        end loop;

        for rr in
            select * from tGetAttrsSubGroups (r.id, idTemplate)
        loop
            return next rr;
        end loop;
    end loop;
    return;
end
$BODY$
language 'plpgsql';
