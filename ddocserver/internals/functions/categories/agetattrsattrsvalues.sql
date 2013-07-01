select f_safe_drop_type('h_get_attrs_attrs_values');
create type h_get_attrs_attrs_values as (id_attr_attr_value int8, 
                                         id_attr_value int8,
                                         attr_attr_value varchar,
                                         id_attr_attr int4,
                                         id_attr_parent int4,
                                         id_attr_child int4);

create or replace function aGetAttrsAttrsValues(int8, int4) returns setof h_get_attrs_attrs_values as
$BODY$
declare
    idAttrValue alias for $1;
    idAttrParent alias for $2;

    r h_get_attrs_attrs_values%rowtype;
begin

    for r in
        select 
            aav.id,
            aav.id_attr_value,
            aav.value,
            aa.id,
            aa.id_attr_parent,
            aa.id_attr_child

        from 
            attrs_attrs aa
            left join attrs_attrs_values aav on (aav.id_attr_value = idAttrValue and aa.id = aav.id_attr_attr)
            left join attrs_values av on (aav.id_attr_value = av.id)
            left join attrs_categories ac on (ac.id = av.id_attr_category)
            left join attributes a on (ac.id_io_attribute = a.id and aa.id_attr_parent = a.id)
        where aa.id_attr_parent = idAttrParent
        order by aa."order", aa.id
    loop
        return next r;
    end loop;

    return;
   

end
$BODY$
language 'plpgsql';
