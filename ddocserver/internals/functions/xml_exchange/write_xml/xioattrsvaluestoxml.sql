create or replace function xIOAttrsValuestoXML(int4) returns varchar as
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

        xml_str := xml_str || xIOAttrValueToXML(
                                                r.a_code,
                                                r.table_name,
                                                r.column_name,
                                                r.ref_column_name,
                                                r.id_a_type,
                                                r.attr_value 
                                               );

    end loop;

    xml_str := xml_str || E'\t</attr_values>\n';

    return xml_str;

end
$BODY$
language 'plpgsql';

create or replace function xIOAttrValueToXML(varchar, varchar, varchar, varchar, int4, varchar) returns varchar as
$BODY$
declare
    aCode alias for $1;
    aTableName alias for $2;
    aColumnName alias for $3;
    aRefColumnName alias for $4;
    aType alias for $5;
    aValue alias for $6;

    xml_str varchar;
    openTag varchar;
    closeTag varchar;
begin

    if(aCode isnull or aCode = '') then
        return '';
    end if;

    openTag := E'<' || xAttCodeToTag(aCode) || E'>';
    closeTag := E'</' || xAttCodeToTag(aCode) || E'>\n';

    if(aValue isnull or aValue = '') then
        xml_str = openTag || closeTag;
        return xml_str;
    end if;

    xml_str = '';

    if(aType = 1) then
        xml_str = openTag || aValue || closeTag;
    elsif(aType = 2) then --list
    elsif(aType = 3) then --parent
    elsif(aType = 4) then --date
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 5) then --datetime
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 6) then --float
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
--    elsif(aType = 7) then    
    elsif(aType = 8) then --Integer
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 9) then --String
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 10) then  --Interval2
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
--    elsif(aType = 11) then    
    elsif(aType = 12) then  --CheckBoxes
    elsif(aType = 13) then  --Text
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 14) then  --FixString
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 15) then  --Image
        xml_str = openTag || E' <![CDATA[ ' || encode(aValue, 'base64') || E' ]]> ' || closeTag;
--    elsif(aType = 16) then    
    elsif(aType = 17) then  --MultiQualifier
    elsif(aType = 18) then  --ColorBg
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 19) then  --ColorRefBg
        xml_str = openTag || closeTag;
    elsif(aType = 20) then    --XML
        xml_str = openTag || E' <![CDATA[ ' || encode(aValue, 'base64') || E' ]]> ' || closeTag;
    elsif(aType = 21) then  --Interval3
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
--    elsif(aType = 22) then    
    elsif(aType = 23) then  --SVG
        xml_str = openTag || E' <![CDATA[ ' || encode(aValue, 'base64') || E' ]]> ' || closeTag;
    elsif(aType = 24) then  --Video
        xml_str = openTag || E' <![CDATA[ ' || encode(aValue, 'base64') || E' ]]> ' || closeTag;
    elsif(aType = 25) then  --ColorFore
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 26) then  --ColorRefFore
        xml_str = openTag || closeTag;
    elsif(aType = 27) then    --Time
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 28) then  --Coords
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
--    elsif(aType = 29) then    
    elsif(aType = 30) then  --Long
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 31) then    --UUID
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 32) then  --Complex
        xml_str = openTag || closeTag;
    elsif(aType = 33) then  --Histogram
        xml_str = openTag || closeTag;
--    elsif(aType = 34) then    
--    elsif(aType = 35) then    
    elsif(aType = 36) then  --Map
        xml_str = openTag || closeTag;
    elsif(aType = 37) then --DateTimeMs
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    else
        xml_str = openTag || closeTag;
    end if;

    return xml_str;

end
$BODY$
language 'plpgsql';

