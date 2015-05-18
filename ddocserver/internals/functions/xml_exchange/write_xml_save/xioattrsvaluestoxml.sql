create or replace function xIOAttrsValuesToXML(int4) returns varchar as
$BODY$
declare
    idObject alias for $1;

    query varchar;
    r record;
    xml_str varchar;
begin

    xml_str := E'\t<attr_values>\n';

    query := 'select 
                a.id as id_attribute, 
                a.code as a_code, 
                a.name as a_name, 
                a.title as a_title, 
                a.table_name, 
                a.column_name,
                a.ref_column_name, 
                a.id_a_type,
                av.value as attr_value 
              from
                attrs_values av 
                right join attrs_categories ac on (av.id_attr_category = ac.id)
                inner join attributes a on (ac.id_io_attribute = a.id)
              where
                av.id_io_object = ' || idObject;

    for r in execute query
    loop

        xml_str := xml_str || xAttrValueToXML(  idObject,
                                                NULL,
                                                r.id_attribute,
                                                r.a_code,
                                                r.table_name,
                                                r.id_a_type,
                                                r.attr_value,
                                                true
                                               );

    end loop;

    xml_str := xml_str || E'\t</attr_values>\n';

    return xml_str;

end
$BODY$
language 'plpgsql';
