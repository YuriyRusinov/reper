select f_safe_drop_type('h_get_rec_attrs_attrs_values');
create type h_get_rec_attrs_attrs_values as (id_attr_attr_value int8, 
                                             id_attr_value int8,
                                             attr_attr_value varchar,
                                             id_attr_attr int4,
                                             id_attr_parent int4,
                                             id_attr_child int4);

create or replace function aGetRecAttrsAttrsValues(int8) returns setof h_get_rec_attrs_attrs_values as
$BODY$
declare
    idRecAttrValue alias for $1;

    r h_get_rec_attrs_attrs_values%rowtype;
begin

    for r in
        select 
            raav.id,
            raav.id_rec_attr_value,
            raav.id_attr_attr,
            raav.value,
            aa.id,
            aa.id_attr_parent,
            aa.id_attr_child

        from 
            attrs_attrs aa
            left join rec_attrs_attrs_values raav on (raav.id_rec_attr_value = idRecAttrValue and aa.id = raav.id_attr_attr)
            left join rec_attrs_values rav on (raav.id_rec_attr_value = rav.id)
            left join attrs_categories ac on (ac.id = rav.id_attr_category)
            left join attributes a on (ac.id_io_attribute = a.id and aa.id_attr_parent = a.id)

    loop
        return next r;
    end loop;

    return;
   

end
$BODY$
language 'plpgsql';
