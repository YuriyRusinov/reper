create or replace function xCategoryAttrsToXML(int4) returns varchar as
$BODY$
declare
    idCategory alias for $1;
    query varchar;
    r record;
    i int4;
    xml_str_all varchar;
    xml_str varchar;
begin

    xml_str := '';

    query := 'select 
                  a.id as id_attribute, 
                  a.unique_id as unique_id,
                  a.code as a_code, 
                  a.title as a_title, 
                  a.name as a_name, 
                  a.table_name, 
                  a.column_name, 
                  a.ref_column_name as ref_column_name,
                  a.def_width as def_width,
                  at.id as id_a_type, 
                  at.code as type_code,
                  ac.is_mandatory as is_mandatory,
                  ac.is_read_only as is_read_only,
                  ac.def_value as def_value 
              from 
                  attributes a inner join
                  attrs_categories ac on (ac.id_io_attribute = a.id and ac.id_io_category = ' || idCategory || ') inner join
                  a_types at on (a.id_a_type = at.id)
              order by ac.order';

    i := 0;

    for r in
        execute query
    loop

        i := i+1;

        xml_str := xml_str || xCategoryAttrToXML(i, --index of attribute in list
                                                 r.id_attribute, 
                                                 '', --uuid_t
                                                 r.unique_id, 
                                                 r.a_code, 
                                                 r.a_title, 
                                                 r.a_name,
                                                 r.id_a_type, 
                                                 r.table_name, 
                                                 r.column_name, 
                                                 r.ref_column_name, 
                                                 r.is_mandatory, 
                                                 r.is_read_only, 
                                                 r.def_width,
                                                 r.def_value);


    end loop;

    if(i=0) then
        return E'<attributes count="0"> </attributes>\n';
    end if;
  
    xml_str_all = E'<attributes count="' || i || E'">\n';
    xml_str_all = xml_str_all || xml_str;
    xml_str_all = xml_str_all || E'</attributes>\n';

    return xml_str_all;
end
$BODY$
language 'plpgsql';

create or replace function xCategoryAttrToXML(int4,    --index
                                              int4,    --id
                                              varchar, --uuid_t
                                              varchar, --unique_id
                                              varchar, --code
                                              varchar, --title 
                                              varchar, --name
                                              int4,    --type
                                              varchar, --table_name
                                              varchar, --column_name
                                              varchar, --ref_column_name
                                              bool,    --is_mandatory
                                              bool,    --is_read_only
                                              int4,    --def_width
                                              varchar  --def_value
                                             ) returns varchar as
$BODY$
declare
    aIndex alias for $1;
    aId alias for $2;
    aUUID alias for $3;
    aUniqueId alias for $4;
    aCode alias for $5;
    aTitle alias for $6;
    aName alias for $7;
    aIdType alias for $8;
    aTableName alias for $9;
    aColumnName alias for $10;
    aRefColumnName alias for $11;
    aIsMandatory alias for $12;
    aIsReadOnly alias for $13;
    aDefWidth alias for $14;
    aDefValue alias for $15;

    xml_str varchar;
begin

    xml_str := '';

    xml_str := xml_str || E'<attribute number="' || aIndex::varchar || E'">\n';

    xml_str := xml_str || E'<tag_name> ' || xAttCodeToTag(aCode) || E' </tag_name>\n';
    xml_str := xml_str || E'<id> ' || aId || E' </id>\n';
    xml_str := xml_str || E'<uuid_t> <![CDATA[ ' || aUUID || E' ]]> </uuid_t>\n';
    xml_str := xml_str || E'<unique_id> <![CDATA[ ' || aUniqueId || E' ]]> </unique_id>\n';
    xml_str := xml_str || E'<code> ' || aCode || E' </code>\n';
    xml_str := xml_str || E'<title> <![CDATA[ ' || aTitle || E' ]]> </title>\n';
    xml_str := xml_str || E'<name> <![CDATA[ ' || aName || E' ]]> </name>\n';

    if(aTableName is not null) then
        xml_str := xml_str || E'<table_name> ' || aTableName || E' </table_name>\n';
    else
        xml_str := xml_str || E'<table_name> </table_name>\n';
    end if;
    
    if(aColumnName is not null) then
        xml_str := xml_str || E'<column_name> ' || aColumnName || E' </column_name>\n';
    else
        xml_str := xml_str || E'<column_name> </column_name>\n';
    end if;
    
    if(aRefColumnName is not null) then
        xml_str := xml_str || E'<ref_column_name> ' || aRefColumnName || E' </ref_column_name>\n';
    else
        if(aColumnName is not null) then
            xml_str := xml_str || E'<ref_column_name> id </ref_column_name>\n';
        else
            xml_str := xml_str || E'<ref_column_name> </ref_column_name>\n';
        end if;
    end if;
    
    xml_str := xml_str || E'<mandatory> ' || asString(aIsMandatory, false) || E' </mandatory>\n';
    xml_str := xml_str || E'<read_only> ' || asString(aIsReadOnly, false) || E' </read_only>\n';
    xml_str := xml_str || E'<def_width> ' || asString(aDefWidth, false) || E' </def_width>\n';
    if(aDefValue is not null) then
        xml_str := xml_str || E'<def_value> <![CDATA[ ' || aDefValue || E' ]]> </def_value>\n';
    else
        xml_str := xml_str || E'<def_value> </def_value>\n';
    end if;

    xml_str := xml_str || E'<value_type> ' || xAttrTypeToXML(aIdType) || E' </value_type>\n';

    xml_str := xml_str || E'</attribute>\n';
    
    return xml_str;

end
$BODY$
language 'plpgsql';

