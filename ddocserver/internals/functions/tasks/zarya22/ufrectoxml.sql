create or replace function ufRecToXML(int4, int4) returns xml as
$BODY$
declare
    idObject alias for $1;
    idRec alias for $2;

    tableName varchar;

    attr_name varchar;
    attr_code varchar;
    attr_title varchar;
    id_attr_type int4;
    attr_type varchar;
    attr_value varchar;

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
    select into tableName table_name from io_objects where id = idObject;
    if (tableName is null) then
        raise warning 'Invalid IO id=%, it has to be a reference,', idObject;
        return null;
    end if;

    
    xml_str := E'<formalized_parameters>\n';
    xml_str := xml_str || E'\t<parameters_description>\n';
    xml_val_str := E'\t<parameters_values>\n';

    query := 'select 
                  a.id as id_attribute, 
                  a.code as a_code, 
                  a.name as a_name, 
                  a.title as a_title, 
                  a.table_name, 
                  a.column_name, 
                  at.id as id_a_type, 
                  at.code as type_code 
              from 
                  attributes a inner join
                  attrs_categories ac on (ac.id_io_attribute = a.id) inner join
                  io_categories c on (ac.id_io_category=c.id_child) inner join tbl_io_objects io on (io.id_io_category=c.id and io.id = ' || idObject || ') inner join
                  a_types at on (a.id_a_type = at.id)';

    i := 1;
    for rattr in
        execute query
    loop

        --raise warning 'attributes are % %', rattr.id_attribute, rattr.a_code;
        xml_str := xml_str || E'\t\t<parameter number="' || i::varchar || E'">\n';
        xml_str := xml_str || E'\t\t\t<tag_name>' || attCodeToTag(rattr.a_code) || E'</tag_name>\n';
        query1 := E'select ' || rattr.a_code || E' from ' || tableName || E' where id = ' || idRec;
        execute query1 into attr_value;
        if (attr_value is null) then
            attr_value := E'';
        end if;
        --raise warning '% is %', rattr.a_code, attr_value;

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
            if(rattr.column_name isnull or trim(rattr.column_name) = '') then
                rColumn := 'name';
            else
                rColumn := rattr.column_name;
            end if;

            if (rattr.id_a_type = 3) then
                rTable := tableName;
            else
                rTable := rattr.table_name;
            end if;

            if (rattr.id_a_type = 3 and trim(attr_value) = E'') then
                attr_value := E'';
                xml_val_str := xml_val_str || E'\t\t<' || attCodeToTag(rattr.a_code) || E'><![CDATA[]]></' || attCodeToTag(rattr.a_code) || E'>\n';
            else
                query1 := 'select unique_id, id, asString(' || rColumn || ', false) as aval from ' || rTable || ' where id = ' || attr_value;
                execute query1 into rval;
                if (rval is not null) then
                    attr_value := rval.aval;
                end if;
                raise warning 'value is % %', rval.aval, attr_value;
                xml_val_str := xml_val_str || E'\t\t<' || attCodeToTag(rattr.a_code) || E'><![CDATA[' || rval.unique_id || E' ' || rval.id || E' ' || attr_value || E']]></' || attCodeToTag(rattr.a_code) || E'>\n';
            end if;

         else
            xml_val_str := xml_val_str || E'\t\t<' || attCodeToTag(rattr.a_code) || E'><![CDATA[' || attr_value || E']]></' || attCodeToTag(rattr.a_code) || E'>\n';
        end if;

        raise warning '%', xml_val_str;
        xml_str := xml_str || E'\t\t\t<name><![CDATA[' || rattr.a_name || E' ]]></name>\n';
        xml_str := xml_str || E'\t\t\t<text_description>\n\t\t\t\t<![CDATA[' || rattr.a_title || E' ]]>\n\t\t\t</text_description>\n';
        xml_str := xml_str || E'\t\t\t<value_type>\n';
        xml_str := xml_str || E'\t\t\t\t';
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
        xml_str := xml_str || E'\n';
        xml_str := xml_str || E'\t\t\t</value_type>\n';
        xml_str := xml_str || E'\t\t\t<possible_values>\n';
        xml_str := xml_str || E'\t\t\t</possible_values>\n';
        xml_str := xml_str || E'\t\t</parameter>\n';

        i := i+1;
    end loop;

    xml_val_str := xml_val_str || E'\t</parameters_values>\n';
    xml_str := xml_str || E'\t</parameters_description>\n';
    xml_str := xml_str || xml_val_str;
    xml_str := xml_str || E'</formalized_parameters>\n';


--    raise warning '%', xml_str;

    result := xmlparse (document (xml_str));

    return result;
end
$BODY$
language 'plpgsql';
