create or replace function xRecordFieldsToXML(varchar, varchar, timestamp, varchar[], varchar[]) returns varchar as
$BODY$
declare
    rUUID alias for $1;
    rUniqueId alias for $2;
    rLastUpdate alias for $3;
    rFields alias for $4;
    rFieldValues alias for $5;

    fCount int4;
    vCount int4;
    i int4;

    xml_str varchar;

    rField varchar;
    rFieldValue varchar;
begin

    fCount = array_upper (rFields, 1);
    vCount = array_upper (rFieldValues, 1);

    if ( fCount <> vCount or rUUID isnull or rUniqueId isnull ) then
        raise exception 'incorrect dimensions';
        return NULL;
    end if;

    xml_str = E'<sys_attributes>\n';
    xml_str = xml_str || E'<uuid_t> <![CDATA[ ' || rUUID || E' ]]> </uuid_t>\n';
    xml_str = xml_str || E'<unique_id> <![CDATA[ ' || rUniqueId || E' ]]> </unique_id>\n';
    xml_str = xml_str || E'<last_update> <![CDATA[ ' || asString(rLastUpdate, false) || E' ]]> </last_update>\n';
    xml_str = xml_str || E'</sys_attributes>\n';

    xml_str = xml_str || E'<attr_values>\n';


    for i in 1..fCount 
    loop
        rField = rFields[i];
        rFieldValue = rFieldValues[i];
        if(rFieldValue is not null) then
            xml_str := xml_str || E'<' || xAttCodeToTag(rField) || E'> <![CDATA[ ' || rFieldValue || E' ]]> </' || xAttCodeToTag(rField) || E'>\n';
        else
            xml_str := xml_str || E'<' || xAttCodeToTag(rField) || E'>  </' || xAttCodeToTag(rField) || E'>\n';
        end if;

    end loop;

    xml_str = xml_str || E'</attr_values>\n';

    return xml_str;
end
$BODY$
language 'plpgsql';
