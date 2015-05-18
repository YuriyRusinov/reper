
create or replace function xIORecordsToXML(int4, int8[]) returns varchar as
$BODY$
declare
    idObject alias for $1;
    idRecords alias for $2; --if NULL then all records will be writed to XML

    xml_str varchar;
    xml_str_all varchar;

    r record;
    tableName varchar;
    q varchar;
    qResult record;

    rFieldsData h_x_get_record_fields;
    rFieldsValues varchar[];

    xmlRecord varchar;
    i int4;
    idCategory int4;
begin
    xml_str = '';
    xml_str_all = '';

    for r in 
        select io.table_name, c.id_child
        from tbl_io_objects io, io_categories c
        where io.id = idObject and io.id_io_category = c.id
    loop
        tableName = r.table_name;
        idCategory = r.id_child;
    end loop;
    
    if(tableName isnull) then
        return xml_str;
    end if;
    
    if(idCategory isnull) then
        return xml_str;
    end if;

    --rCount = 1;

    --if(idRecords is not null) then
    --    rCount = array_upper (idRecords, 1);
    --end if;

    rFieldsData = xGetRecordFields(idCategory);

    i = 0;

    q = xGenerateSelectRecordsQuery(idObject, idCategory, tableName, idRecords);
    for qResult in execute q
    loop
        i = i + 1;

        rFieldsValues = string_to_array(qResult.fields, '~^^^~');
        xml_str = xml_str || E'<record number = "' || i || E'">\n';
        
        xmlRecord = xRecordFieldsToXML(idObject, 
                                       qResult.id, 
                                       qResult.uuid_t::varchar, 
                                       qResult.unique_id, 
                                       qResult.last_update, 
                                       rFieldsData.r_ids,
                                       rFieldsData.r_fields, 
                                       rFieldsData.r_ref_tables,
                                       rFieldsData.r_types,
                                       rFieldsValues);

        xml_str = xml_str || xmlRecord;

        xml_str = xml_str || xRecordIndicatorsToXML(qResult.id);

        xml_str = xml_str || xRecordFilesToXML(qResult.id);

        xml_str = xml_str || E'</record>\n';
    end loop;

    xml_str_all = xml_str_all || E'<table_records record_count="' || i || E'">\n';
    xml_str_all = xml_str_all || xml_str;
    xml_str_all = xml_str_all || E'</table_records>\n';

    return xml_str_all;
end
$BODY$
language 'plpgsql';


create or replace function xGenerateSelectRecordsQuery(int4, int4, varchar, int8[]) returns varchar as
$BODY$
declare
    idObject alias for $1;
    idCategory alias for $2;
    tableName alias for $3;
    idRecords alias for $4;

    query varchar;
    q varchar;
    r record;
    recCnt int4;
begin

    if(idObject = 7 or idObject > 300) then
        q = 'select id::int8, uuid_t, unique_id, last_update, ';
    else
        q = 'select id::int8, ' || quote_literal('00000000-0000-0000-0000-000000000000') || ' as uuid_t, unique_id, last_update, ';
    end if;

    query = xGenerateSelectFieldsQuery(idCategory);
    for r in execute query
    loop
        if(r.id_a_type = 17) then --atCheckListEx (MultiQualifier)
            q = q || ' asString(' || quote_literal('DUMMY') || ', false) || ' || quote_literal('~^^^~') || ' || ';
        else
            q = q || ' asString("' || r.a_code || '", false) || ' || quote_literal('~^^^~') || ' || ';
        end if;
    end loop;

    q = left(q, length(q)-15);

    q = q || ' as fields ';

    recCnt = array_upper(idRecords, 1);
    if(idRecords is not null and recCnt is not null and recCnt > 0) then
        q = q || ' from ' || tableName || ' where id = ANY(' || asString(idRecords, true) || ') ';
    else
        q = q || ' from ' || tableName;
    end if;

    --raise exception '%', q;

    return q;
end    
$BODY$
language 'plpgsql';
