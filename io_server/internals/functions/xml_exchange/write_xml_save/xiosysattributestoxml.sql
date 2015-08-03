create or replace function xIOSysAttributesToXML(int4, varchar, varchar, varchar, varchar) returns varchar as
$BODY$
declare
    idObject alias for $1;
    ioUUID alias for $2;
    ioUniqueId alias for $3;
    ioName alias for $4;
    ioDesc alias for $5;

    xml_str varchar;
begin

    xml_str = '';

    xml_str := xml_str || E'<sys_attributes>\n';
    xml_str := xml_str || E'<id> ' || idObject || E' </id>\n';
    xml_str := xml_str || E'<uuid_t> <![CDATA[ ' || ioUUID || E' ]]> </uuid_t>\n';
    xml_str := xml_str || E'<unique_id> <![CDATA[ ' || ioUniqueId || E' ]]> </unique_id>\n';
    xml_str := xml_str || E'<name> <![CDATA[ ' || ioName || E' ]]> </name>\n';

    if(ioDesc is not null) then
        xml_str := xml_str || E'<description> <![CDATA[ ' || ioDesc || E' ]]> </description>\n';
    else
        xml_str := xml_str || E'<description> </description>\n';
    end if;

    xml_str := xml_str || E'</sys_attributes>\n';

    return xml_str;
end
$BODY$
language 'plpgsql';
