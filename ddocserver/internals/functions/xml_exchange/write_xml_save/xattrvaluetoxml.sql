create or replace function xAttrValueToXML(int4, int8, int4, varchar, varchar, int4, varchar, bool) returns varchar as
$BODY$
declare
    idMainObject alias for $1; --in which IO (or IO table) contains attribute value
    idMainRecord alias for $2; --inwhich record contains attribute value
    aId alias for $3;
    aCode alias for $4;
    aTableName alias for $5;
    aType alias for $6;
    aValue alias for $7;
    bForIO alias for $8;

    xml_str varchar;
    openTag varchar;
    closeTag varchar;

    idObject int4;
    refXML varchar;
    idRecords int8[];
begin

    if(aCode isnull or aCode = '') then
        return '';
    end if;

    openTag := E'<' || xAttCodeToTag(aCode) || E'>';
    closeTag := E'</' || xAttCodeToTag(aCode) || E'>\n';

    if((aValue isnull or aValue = '') and aType <> 17) then
        xml_str = openTag || closeTag;
        return xml_str;
    end if;

    xml_str = '';

    if(aType = 1) then
        xml_str = openTag || aValue || closeTag;
    elsif(aType = 2) then --list
        refXML = xCreateXMLForRef(aTableName, aValue, aType);
        xml_str = openTag || refXML || closeTag;
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
        refXML = xCreateXMLForRef(aTableName, aValue, aType);
        xml_str = openTag || refXML || closeTag;
    elsif(aType = 13) then  --Text
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 14) then  --FixString
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 15) then  --Image
        --xml_str = openTag || E' <![CDATA[ ' || encode(aValue::bytea, 'base64') || E' ]]> ' || closeTag;
        xml_str = openTag || closeTag;
--    elsif(aType = 16) then    
    elsif(aType = 17) then  --MultiQualifier
        if(bForIO = true) then
            refXML = xCreateXMLForRef(aTableName, aValue, aType);
        else
            refXML = xCreateXMLForRefEx(aId, aTableName, idMainRecord, idMainObject);
        end if;
        xml_str = openTag || refXML || closeTag;
    elsif(aType = 18) then  --ColorBg
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
    elsif(aType = 19) then  --ColorRefBg
        xml_str = openTag || closeTag;
    elsif(aType = 20) then    --XML
        --xml_str = openTag || E' <![CDATA[ ' || encode(aValue::bytea, 'base64') || E' ]]> ' || closeTag;
        xml_str = openTag || closeTag;
    elsif(aType = 21) then  --Interval3
        xml_str = openTag || E' <![CDATA[ ' || aValue || E' ]]> ' || closeTag;
--    elsif(aType = 22) then    
    elsif(aType = 23) then  --SVG
        --xml_str = openTag || E' <![CDATA[ ' || encode(aValue::bytea, 'base64') || E' ]]> ' || closeTag;
        xml_str = openTag || closeTag;
    elsif(aType = 24) then  --Video
        --xml_str = openTag || E' <![CDATA[ ' || encode(aValue::bytea, 'base64') || E' ]]> ' || closeTag;
        xml_str = openTag || closeTag;
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


create or replace function xCreateXMLForRef(varchar, varchar, int4) returns varchar as
$BODY$
declare
    aTableName alias for $1;
    aValue alias for $2;
    aType alias for $3;

    idObject int4;
    refXML varchar;
    idRecord int8;
    idRecords int8[];
    recCnt int4;
begin

    refXML = '';

    if(aValue isnull or aValue = '' or aValue = 'NULL' or aValue = 'null' or aValue = '{}' or aTableName isnull or aTableName = '') then
        return refXML;
    end if;

    select id into idObject from tbl_io_objects where table_name = aTableName;
    if(idObject is null or idObject <= 0) then
        return refXML;
    end if;
 
    if(aType = 2) then
        idRecord = aValue::int8;
        if(idRecord isnull or idRecord <= 0) then
            return refXML;
        end if;

        refXML = xRecToXML(idObject, idRecord);

    elsif(aType = 12 or aType = 17) then
        idRecords = aValue::int8[];
        recCnt = array_upper(idRecords, 1);
        if(idRecords isnull or recCnt isnull or recCnt <= 0) then
            return refXML;
        end if;

        refXML = xMultiRecToXML(idObject, idRecords);

    end if;

    return refXML;

end
$BODY$
language 'plpgsql' security definer;

create or replace function xCreateXMLForRefEx(int4, varchar, int8, int4) returns varchar as
$BODY$
declare
    aId alias for $1;
    aTableName alias for $2;
    idMainRecord alias for $3;
    idMainObject alias for $4;

    mainTable varchar;
    refTable varchar;
    rTable varchar;

    mainAttr varchar;
    childAttr varchar;

    refXML varchar;
    idObject int4;
    idRecords int8[];
    recCnt int4;
begin
    refXML = '';

    if(aId isnull or aId <= 0 or aTableName isnull or idMainRecord isnull or idMainRecord <= 0 or idMainObject isnull or idMainObject <= 0) then
        return refXML;
    end if;

    select table_name into mainTable from tbl_io_objects where id = idMainObject;
    if(mainTable isnull) then
        return refXML;
    end if;

    select id into idObject from tbl_io_objects where table_name = aTableName;
    if(idObject is null or idObject <= 0) then
        return refXML;
    end if;


    rTable := mainTable || '_' || aTableName;
    if (rTable = 'organization_work_mode' or 
        rTable = 'table_notifies_io_objects' or
        rTable = 'position_work_mode' or 
        rTable = 'units_work_mode' or 
        rTable = 'io_objects_organization') 
    then
        refTable := rTable;
    else
        refTable := rTable || '_ref_' || aId;
    end if;
    
    mainAttr := 'id_' || mainTable;
    childAttr := 'id_' || aTableName;

    idRecords = getExValuesId(idMainRecord, refTable, mainAttr, childAttr);
    recCnt = array_upper(idRecords, 1);
    if(idRecords isnull or recCnt isnull or recCnt <= 0) then
        return refXML;
    end if;

    refXML = xMultiRecToXML(idObject, idRecords);

    return refXML;
end
$BODY$
language 'plpgsql' security definer;


/*  2

*/

/* 12
        if(aValue = '{}') then
            xml_str = openTag || closeTag;
        else
            select id into idObject from tbl_io_objects where table_name = aTableName;
            if(idObject is not null and idObject > 0) then
                idRecords = aValue::int8[];
                refXML = xMultiRecToXML(idObject, idRecords);
                xml_str = openTag || refXML || closeTag;
            else
                xml_str = openTag || closeTag;
            end if;
        end if;

*/

/* 17
        if(aValue = '{}') then
            xml_str = openTag || closeTag;
        else
            select id into idObject from tbl_io_objects where table_name = aTableName;
            if(idObject is not null and idObject > 0) then
                idRecords = aValue::int8[];
                refXML = xMultiRecToXML(idObject, idRecords);
                xml_str = openTag || refXML || closeTag;
            else
                xml_str = openTag || closeTag;
            end if;
        end if;

*/