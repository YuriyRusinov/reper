create or replace function xRecordFieldsToXML(int4, int8, varchar, varchar, timestamp, int4[], varchar[], varchar[], int4[], varchar[]) returns varchar as
$BODY$
declare
    idMainObject alias for $1;
    idMainRecord alias for $2;
    rUUID alias for $3;
    rUniqueId alias for $4;
    rLastUpdate alias for $5;
    rIds alias for $6;
    rFields alias for $7;
    rRefTables alias for $8;
    rTypes alias for $9;
    rFieldValues alias for $10;

    iCount int4;
    fCount int4;
    tCount int4;
    ttCount int4;
    vCount int4;
    
    i int4;

    xml_str varchar;

    rId int4;
    rField varchar;
    rRefTable varchar;
    rType int4;
    rFieldValue varchar;

begin

    iCount = array_upper (rIds, 1);
    fCount = array_upper (rFields, 1);
    tCount = array_upper (rRefTables, 1);
    ttCount = array_upper (rTypes, 1);
    vCount = array_upper (rFieldValues, 1);

    if(iCount isnull or fCount isnull or tCount isnull or ttCount isnull or vCount isnull) then
        raise exception 'There are no any attributes!';
        return NULL;
    end if;
    

    if ( iCount <> fCount or fCount <> vCount or vCount <> tCount or tCount <> ttCount or rUUID isnull or rUniqueId isnull ) then
        raise exception E'incorrect dimensions: \niCount = %\nfCount = %\ntCount = %\nttCount = %\nvCount = %', iCount, fCount, tCount, ttCount, vCount;
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
        rId = rIds[i];
        rField = rFields[i];
        rRefTable = rRefTables[i];
        rType = rTypes[i];
        rFieldValue = rFieldValues[i];

        xml_str := xml_str || xAttrValueToXML(idMainObject, idMainRecord, rId, rField, rRefTable, rType, rFieldValue, false);
    end loop;

    xml_str = xml_str || E'</attr_values>\n';

    return xml_str;
end
$BODY$
language 'plpgsql';
