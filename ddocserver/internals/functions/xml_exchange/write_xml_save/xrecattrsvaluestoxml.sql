create or replace function xRecAttrsValuesToXML(int4, int4, varchar, int8) returns varchar as
$BODY$
declare
    idObject alias for $1;
    idCategory alias for $2;
    tableName alias for $3;
    idRecord alias for $4;

    rFieldsData h_x_get_record_fields;
    rFieldsValues varchar[];
    q varchar;
    qResult record;
    idRecords int8[];
    xml_str varchar;
begin
    
    idRecords = ARRAY[]::int8[];
    idRecords = array_append(idRecords, idRecord);

    rFieldsData = xGetRecordFields(idCategory);

    q = xGenerateSelectRecordsQuery(idObject, idCategory, tableName, idRecords);
    
    for qResult in execute q
    loop

        rFieldsValues = string_to_array(qResult.fields, '~^^^~');
        
        xml_str = xRecordFieldsToXML(idObject, 
                                     idRecord, 
                                     qResult.uuid_t::varchar, 
                                     qResult.unique_id, 
                                     qResult.last_update, 
                                     rFieldsData.r_ids,
                                     rFieldsData.r_fields, 
                                     rFieldsData.r_ref_tables,
                                     rFieldsData.r_types,
                                     rFieldsValues);

        xml_str = xml_str || xRecordIndicatorsToXML(qResult.id);

        xml_str = xml_str || xRecordFilesToXML(qResult.id);

    end loop;

    return xml_str;
end
$BODY$
language 'plpgsql';
