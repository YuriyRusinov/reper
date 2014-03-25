create or replace function xRecAttrsValuesToXML(int4, varchar, int8) returns varchar as
$BODY$
declare
    idCategory alias for $1;
    tableName alias for $2;
    idRecord alias for $3;

    rFields varchar[];
    rFieldsValues varchar[];
    q varchar;
    qResult record;
    idRecords int8[];
    xml_str varchar;
begin
    
    idRecords = ARRAY[]::int8[];
    idRecords = array_append(idRecords, idRecord);

    rFields = xGetRecordFields(idCategory);

    q = xGenerateSelectRecordsQuery(idCategory, tableName, idRecords);
    
    for qResult in execute q
    loop

        rFieldsValues = string_to_array(qResult.fields, '~^^^~');
        
        xml_str = xRecordFieldsToXML(qResult.uuid_t::varchar, qResult.unique_id, qResult.last_update, rFields, rFieldsValues);

        xml_str = xml_str || xRecordIndicatorsToXML(qResult.id);

        xml_str = xml_str || xRecordFilesToXML(qResult.id);

    end loop;

    return xml_str;
end
$BODY$
language 'plpgsql';
