create or replace function xRefQualifierToXML(int4, varchar, varchar, varchar) returns varchar as
$BODY$
declare
    idObject alias for $1;
    ioUUID alias for $2;
    ioUniqueId alias for $3;
    ioTableName alias for $4;

    xml_str varchar;
begin

    xml_str = E'<ref_qualifier>\n';

    xml_str := xml_str || E'<id> ' || idObject || E'</id>\n';    
    xml_str := xml_str || E'<uuid_t> <![CDATA[ ' || ioUUID || E' ]]> </uuid_t>\n';
    xml_str := xml_str || E'<unique_id> <![CDATA[ ' || ioUniqueId || E' ]]> </unique_id>\n';
    xml_str := xml_str || E'<table_name> <![CDATA[ ' || ioTableName || E' ]]> </table_name>\n';

    xml_str = xml_str || E'</ref_qualifier>\n';

    return xml_str;
end
$BODY$
language 'plpgsql';
