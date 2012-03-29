create or replace function ufdocToXML(int4) returns xml as
$BODY$
declare
    idObject alias for $1;

    attr_name varchar;
    attr_code varchar;
    attr_title varchar;
    id_attr_type int4;
    attr_type varchar;

    rattr record;
    query varchar;

    query1 varchar;
    rval record;
    rColumn varchar;
    rTable varchar;

    i int4;

    result xml;
    cnt int4;
    xml_str varchar;
    xml_val_str varchar;
begin
    if (idObject is null) then
        return null;
    end if;

    select count(*) into cnt from io_objects where id = idObject;
    if (cnt is null or cnt = 0) then
        raise warning 'Cannot find document with id=%', idObject;
        return null;
    end if;

    
    xml_str := E'<formalized_parameters>\n';
    xml_str := xml_str || E'\t<parameters_description>';
    xml_val_str := E'\t<parameters_values>\n';

    query := 'select 
                  a.id as id_attribute, 
                  a.code as a_code, 
                  a.name as a_name, 
                  a.title as a_title, 
                  a.table_name, 
                  a.column_name, 
                  at.id as id_a_type, 
                  at.code as type_code, 
                  av.value as attr_value 
              from 
                  attributes a,
                  attrs_categories ac,
                  attrs_values av,
                  a_types at
              where
                  av.id_io_object = ' || idObject || '
                  and av.id_attr_category = ac.id
                  and ac.id_io_attribute = a.id
                  and a.id_a_type = at.id';

    i := 1;
    for rattr in
        execute query
    loop

        --raise warning 'attributes are % %', rattr.id_attribute, rattr.a_code;
        xml_str := xml_str || E'\t\t<parameter number="' || i::varchar || E'">\n';
        xml_str := xml_str || E'\t\t\t<tag_name>' || attCodeToTag(rattr.a_code) || E'</tag_name>\n';

        if (
            rattr.id_a_type = 2 or 
            --rattr.id_a_type = 3 or --parent не учитываем, поскольку атрибут типа родитель-потомок не имеет смысла для атрибутов, описываемых в таблице attrs_values
            rattr.id_a_type = 7 or 
            rattr.id_a_type = 12 or 
            rattr.id_a_type = 17 or 
            rattr.id_a_type = 19 or 
            rattr.id_a_type = 26
           ) 
        then
            if(rattr.column_name isnull or trim(rattr.column_name) = '') then
                rColumn = 'name';
            else
                rColumn = rattr.column_name;
            end if;

            query1 = 'select asString(' || rColumn || ', false) from ' || rattr.table_name || ' where id = ' || rattr.attr_value;
            execute query1 into rval;
         
            xml_val_str := xml_val_str || E'<' || attCodeToTag(rattr.a_code) || E'><![CDATA[' || rval || E']]></' || attCodeToTag(rattr.a_code) || E'>\n';
        else
            xml_val_str := xml_val_str || E'<' || attCodeToTag(rattr.a_code) || E'><![CDATA[' || rattr.attr_value || E']]></' || attCodeToTag(rattr.a_code) || E'>\n';
        end if;

        xml_str := xml_str || E'\t\t\t<name><![CDATA[' || rattr.a_name || E' ]]></name>\n';
        xml_str := xml_str || E'\t\t\t<text_description>\n\t\t\t\t<![CDATA[' || rattr.a_title || E' ]]></text_description>\n';
        xml_str := xml_str || E'\t\t\t<value_type>\n';
        if (
            rattr.id_a_type = 2 or 
            rattr.id_a_type = 3 or 
            rattr.id_a_type = 7 or 
            rattr.id_a_type = 12 or 
            rattr.id_a_type = 17 or 
            rattr.id_a_type = 19 or 
            rattr.id_a_type = 26
           ) 
        then
            xml_str := xml_str || E'String';
        elsif (rattr.id_a_type = 1) then
            xml_str := xml_str || E'Boolean';
        elsif (rattr.id_a_type = 8) then
            xml_str := xml_str || E'Integer';
        elsif (rattr.id_a_type = 6) then
            xml_str := xml_str || E'Double';
        elsif (rattr.id_a_type = 4) then
            xml_str := xml_str || E'Date';
        elsif (rattr.id_a_type = 5) then
            xml_str := xml_str || E'DateTime';
        elsif (rattr.id_a_type = 27) then
            xml_str := xml_str || E'Time';
        else
            xml_str := xml_str || E'String'; --9
        end if;
        xml_str := xml_str || E'\t\t\t</value_type>\n';
        xml_str := xml_str || E'\t\t\t<possible_values>\n';
        xml_str := xml_str || E'\t\t\t</possible_values>\n';
        xml_str := xml_str || E'\t\t\t</parameter>\n';

        i := i+1;
    end loop;

    xml_val_str := xml_val_str || E'\t\t</parameters_values>\n';
    xml_str := xml_str || E'\t</parameters_description>\n';
    xml_str := xml_str || xml_val_str;
    xml_str := xml_str || E'\t</formalized_parameters>\n';


--    raise warning '%', xml_str;

    result := xmlparse (document (xml_str));

    return result;
end
$BODY$
language 'plpgsql';
