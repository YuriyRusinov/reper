select f_safe_drop_type('h_io_get_used_attrs');
create type h_io_get_used_attrs as(id int4, 
                                   name varchar, 
                                   code varchar, 
                                   title varchar);

create or replace function ioGetUsedAttrs() returns setof h_io_get_used_attrs as
$BODY$
declare
    r h_io_get_used_attrs%rowtype;
begin

    for r in
        select distinct a.id, a.name, a.code, a.title 
        from 
            attributes a, 
            attrs_categories ac, 
            attrs_values av
        where
            a.id = ac.id_io_attribute
            and ac.id = av.id_attr_category
            and av.is_actual = true
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
