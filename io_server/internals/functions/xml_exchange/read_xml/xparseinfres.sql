create or replace function xParseInfResources(xml) returns int4 as
$BODY$
declare
    xmlBody alias for $1;

    r record;
    res int4;
begin

    for r in select unnest(xpath('/inf_resources/inf_resource', xmlBody)) as xml_part
    loop
        select xParseInfResource(r.xml_part) into res;
        if(res <= 0) then
            return -1;
        end if;
    end loop;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xParseInfResource(xml) returns int4 as
$BODY$
declare
    xmlBody alias for $1;

    infResType varchar;
    res int4;
begin

    SELECT btrim(array_to_string(xpath('/inf_resource/type/text()', xmlBody), '', ''))::varchar into infResType;
    if(infResType isnull or infResType = '') then
        return -1;
    end if;

    if(infResType = 'Document') then
        select xParseDocument(xmlBody) into res;
    elsif(infResType = 'Qualifier') then
        select xParseQualifier(xmlBody) into res;
    elsif(infResType = 'Record') then
        select xParseRecord(xmlBody) into res;
    elsif(infResType = 'MultiRecord') then
        select xParseMultiRecord(xmlBody) into res;
    else
        return -1;
    end if;

    if(res <= 0) then
        return -1;
    end if;
    
    return 1;
end
$BODY$
language 'plpgsql';


create or replace function xParseDocument(xml) returns int4 as
$BODY$
declare
    xmlBody alias for $1;

    infResType varchar;
    idInfResReal int8;
    res int4;

    attrValuesXML xml;
begin

    SELECT btrim(array_to_string(xpath('/inf_resource/type/text()', xmlBody), '', ''))::varchar into infResType;
    if(infResType <> 'Document') then
        return -1;
    end if;

    select xParseInfResSysParams(xmlBody) into idInfResReal;
    if(idInfResReal <= 0) then
        return -1;
    end if;

    select unnest(xpath('/inf_resource/attr_values', xmlBody)) into attrValuesXML;
    select xParseAttrValues(idInfResReal, infResType, attrValuesXML) into res;
    if(res <= 0) then
        return -1;
    end if;
    
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xParseQualifier(xml) returns int4 as
$BODY$
declare
    xmlBody alias for $1;

    infResType varchar;
    idInfResReal int8;
    res int4;

    attrValuesXML xml;
begin

    SELECT btrim(array_to_string(xpath('/inf_resource/type/text()', xmlBody), '', ''))::varchar into infResType;
    if(infResType <> 'Qualifier') then
        return -1;
    end if;

    select xParseInfResSysParams(xmlBody) into idInfResReal;
    if(idInfResReal <= 0) then
        return -1;
    end if;

    select unnest(xpath('/inf_resource/attr_values', xmlBody)) into attrValuesXML;
    select xParseAttrValues(idInfResReal, infResType, attrValuesXML) into res;
    if(res <= 0) then
        return -1;
    end if;

    for r in select unnest(xpath('/inf_resource/table_records/record', xmlBody)) as xml_part
    loop
        select xParseRecord(infResReal, r.xml_part) into res;
        if(res <= 0) then
            return -1;
        end if;
    end loop;
    
    return 1;
end
$BODY$
language 'plpgsql';

--create or replace function xParseRecords(xml)

create or replace function xParseRecord(int8, xml) returns int4 as
$BODY$
declare
    idQualifier alias for $1;
    xmlBody alias for $2;

    infResType varchar;
    idInfResReal int8;
    res int4;

    attrValuesXML xml;
begin


    select xParseRecordSysParams(xmlBody) into idInfResReal;
    if(idInfResReal <= 0) then
        return -1;
    end if;

    select unnest(xpath('/inf_resource/attr_values', xmlBody)) into attrValuesXML;
    select xParseAttrValues(idInfResReal, infResType, attrValuesXML) into res;
    if(res <= 0) then
        return -1;
    end if;
    
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xParseRecordSysParams(xml) returns int8 as
$BODY$
declare

    xmlBody alias for $1;

    --idObject int4;
    uuidT uuid;
    uniqueId varchar;
    ioName varchar;
    ioDesc varchar;
    ioTableName varchar;

    tmpVar varchar;

begin

    --parse sys params
--    SELECT btrim(array_to_string(xpath('/record/sys_attributes/id/text()', xmlBody), '', ''))::varchar::int4 into idObject;
    SELECT btrim(array_to_string(xpath('/record/sys_attributes/uuid_t/text()', xmlBody), '', ''))::varchar into tmpVar;
    if(tmpVar isnull or tmpVar = '') then
        uuidT = NULL;
    else
        uuidT = tmpVar::uuid;
    end if;

    SELECT btrim(array_to_string(xpath('/record/sys_attributes/unique_id/text()', xmlBody), '', ''))::varchar into uniqueId;
    if(uniqueId isnull or uniqueId = '') then
        return -1;
    end if;


    bExist = true;
    ioTableName = 'io_objects';

    if(infResType = 'Document' or infResType = 'Qualifier') then
        query = 'select id from tbl_io_objects where uuid_t = ' || asString(uuidT, true);
    else

        SELECT btrim(array_to_string(xpath('/inf_resource/ref_qualifier/table_name/text()', xmlBody), '', ''))::varchar into ioTableName;
        if(uuidT = '00000000-0000-0000-0000-000000000000'::uuid and (ioTableName isnull or ioTableName = '') ) then
            return -1;
        end if;
        
        if(uuidT = '00000000-0000-0000-0000-000000000000'::uuid) then --записи системных справочников. У них нет uuid_t, т.к. они не наследуются от q_base_table
            query = 'select id from ' || ioTableName || ' where unique_id = ' || quote_literal(uniqueId);
        else
            query = 'select id from q_base_table where uuid_t = ' || asString(uuidT, true);
        end if;
    end if;

    execute query into idInfResReal;
    if(idInfResReal isnull) then
        if(infResType = 'Document' or infResType = 'Qualifier') then
	    idInfResReal = getNextSeq('io_objects', 'id');
        else
            idInfResReal = getNextSeq('q_base_table', 'id');
        end if;
        
        bExist = false;
    end if;

    insert into x_parsed_inf_resources (uuid_t, unique_id, real_id, id_category, id_table_category, id_indicators_category, name, description, table_name, ir_type, b_exist) 
    values (uuidT, uniqueId, idInfResReal, idCategory, idTableCategory, idIndCategory, ioName, ioDesc, ioTableName, irType, bExist);     
    
    return idInfResReal;

end
$BODY$
language 'plpgsql';


create or replace function xParseRecord(xml) returns int4 as
$BODY$
declare
    xmlBody alias for $1;

    infResType varchar;
    idInfResReal int8;
    res int4;

    attrValuesXML xml;
begin

    SELECT btrim(array_to_string(xpath('/inf_resource/type/text()', xmlBody), '', ''))::varchar into infResType;
    if(infResType <> 'Record') then
        return -1;
    end if;

    select xParseInfResSysParams(xmlBody) into idInfResReal;
    if(idInfResReal <= 0) then
        return -1;
    end if;

    select unnest(xpath('/inf_resource/attr_values', xmlBody)) into attrValuesXML;
    select xParseAttrValues(idInfResReal, infResType, attrValuesXML) into res;
    if(res <= 0) then
        return -1;
    end if;
    
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xParseMultiRecord(xml) returns int4 as
$BODY$
declare
    xmlBody alias for $1;

    infResType varchar;
    idInfResReal int8;
    res int4;

    attrValuesXML xml;
begin

    SELECT btrim(array_to_string(xpath('/inf_resource/type/text()', xmlBody), '', ''))::varchar into infResType;
    if(infResType <> 'MultiRecord') then
        return -1;
    end if;

    select xParseInfResSysParams(xmlBody) into idInfResReal;
    if(idInfResReal <= 0) then
        return -1;
    end if;

    select unnest(xpath('/inf_resource/attr_values', xmlBody)) into attrValuesXML;
    select xParseAttrValues(idInfResReal, infResType, attrValuesXML) into res;
    if(res <= 0) then
        return -1;
    end if;
    
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xParseInfResSysParams(xml) returns int8 as
$BODY$
declare
    xmlBody alias for $1;

    categoryUid varchar;
    tableCategoryUid varchar;
    indCategoryUid varchar;

    idCategory int4;
    idTableCategory int4;
    idIndCategory int4;

    idObject int4;
    uuidT uuid;
    uniqueId varchar;
    ioName varchar;
    ioDesc varchar;
    ioTableName varchar;

    tmpVar varchar;
    query varchar;
    
    infResType varchar;
    irType int4;
    
    idInfResReal int8;
    bExist bool;

begin

    SELECT btrim(array_to_string(xpath('/inf_resource/type/text()', xmlBody), '', ''))::varchar into infResType;
    if(infResType isnull or infResType = '' or infResType = 'MultiRecord') then --multiRecord опускаем, поскольку она все равно парсится как набор отдельных записей
        return -1;
    end if;

    if(infResType = 'Document') then
        irType = 1;
    elsif (infResType = 'Qualifier') then
        irType = 2;
    elsif (infResType = 'Record') then
        irType = 3;
    else
        return -1;
    end if;

    --parse categories
    SELECT btrim(array_to_string(xpath('/inf_resource/category_description/document_category_uid/text()', xmlBody), '', ''))::varchar into categoryUid;
    if(infResType = 'Document' and (categoryUid isnull or categoryUid = '') ) then
        return -1;
    end if;

    select real_id into idCategory from x_parsed_categories where unique_id = categoryUid;
    if(idCategory isnull) then
        return -1;
    end if;

    SELECT btrim(array_to_string(xpath('/inf_resource/category_description/table_category_uid/text()', xmlBody), '', ''))::varchar into tableCategoryUid;
    if((infResType = 'Record' or infResType = 'Qualifier') and (tableCategoryUid isnull or tableCategoryUid = '') ) then
        return -1;
    end if;
    if(infResType = 'Document') then
        idTableCategory = NULL;
        tableCategoryUid = NULL;
    end if;

    if(tableCategoryUid is not null) then
        select real_id into idTableCategory from x_parsed_categories where unique_id = tableCategoryUid;
        if(idTableCategory isnull) then
            return -1;
        end if;
    end if;

    SELECT btrim(array_to_string(xpath('/inf_resource/category_description/table_indicators_category_uid/text()', xmlBody), '', ''))::varchar into indCategoryUid;
    if(indCategoryUid isnull or indCategoryUid = '') then
        idIndCategory = NULL;
        indCategoryUid = NULL;
    end if;
    if(infResType = 'Document') then
        idIndCategory = NULL;
        indCategoryUid = NULL;
    end if;

    if(indCategoryUid is not null) then
        select real_id into idIndCategory from x_parsed_categories where unique_id = indCategoryUid;
        if(idIndCategory isnull) then
            return -1;
        end if;
    end if;


    --parse sys params
    SELECT btrim(array_to_string(xpath('/inf_resource/sys_attributes/id/text()', xmlBody), '', ''))::varchar::int4 into idObject;
    SELECT btrim(array_to_string(xpath('/inf_resource/sys_attributes/uuid_t/text()', xmlBody), '', ''))::varchar into tmpVar;
    if(tmpVar isnull or tmpVar = '') then
        uuidT = NULL;
    else
        uuidT = tmpVar::uuid;
    end if;

    SELECT btrim(array_to_string(xpath('/inf_resource/sys_attributes/unique_id/text()', xmlBody), '', ''))::varchar into uniqueId;
    if(uniqueId isnull or uniqueId = '') then
        return -1;
    end if;

    SELECT btrim(array_to_string(xpath('/inf_resource/sys_attributes/name/text()', xmlBody), '', ''))::varchar into ioName;
    if(ioName isnull or ioName = '') then
        return -1;
    end if;

    SELECT btrim(array_to_string(xpath('/inf_resource/sys_attributes/description/text()', xmlBody), '', ''))::varchar into ioDesc;
    if(ioDesc = '') then
        ioDesc = NULL;
    end if;

    bExist = true;
    ioTableName = 'io_objects';

    if(infResType = 'Document' or infResType = 'Qualifier') then
        query = 'select id from tbl_io_objects where uuid_t = ' || asString(uuidT, true);
    else

        SELECT btrim(array_to_string(xpath('/inf_resource/ref_qualifier/table_name/text()', xmlBody), '', ''))::varchar into ioTableName;
        if(uuidT = '00000000-0000-0000-0000-000000000000'::uuid and (ioTableName isnull or ioTableName = '') ) then
            return -1;
        end if;
        
        if(uuidT = '00000000-0000-0000-0000-000000000000'::uuid) then --записи системных справочников. У них нет uuid_t, т.к. они не наследуются от q_base_table
            query = 'select id from ' || ioTableName || ' where unique_id = ' || quote_literal(uniqueId);
        else
            query = 'select id from q_base_table where uuid_t = ' || asString(uuidT, true);
        end if;
    end if;

    execute query into idInfResReal;
    if(idInfResReal isnull) then
        if(infResType = 'Document' or infResType = 'Qualifier') then
	    idInfResReal = getNextSeq('io_objects', 'id');
        else
            idInfResReal = getNextSeq('q_base_table', 'id');
        end if;
        
        bExist = false;
    end if;

    insert into x_parsed_inf_resources (uuid_t, unique_id, real_id, id_category, id_table_category, id_indicators_category, name, description, table_name, ir_type, b_exist) 
    values (uuidT, uniqueId, idInfResReal, idCategory, idTableCategory, idIndCategory, ioName, ioDesc, ioTableName, irType, bExist);     
    
    return idInfResReal;
end
$BODY$
language 'plpgsql' security definer; --because of using tbl_io_objects

create or replace function xParseAttrValues(int8, varchar, xml) returns int4 as
$BODY$
declare
    idInfRes alias for $1;
    infResType alias for $2;
    xmlBody alias for $3;

    r record;
    res int4;
    idCategory int4;
begin

    if(infResType = 'Document' or infResType = 'Qualifier') then
        select id_category into idCategory from x_parsed_inf_resources where real_id = idInfRes;
    elsif(infResType = 'Record') then
        select id_table_category into idCategory from x_parsed_inf_resources where real_id = idInfRes;
    else
        return -1;
    end if;

    if(idCategory isnull or idCategory <= 0) then
        return -1;
    end if;

    for r in 
        select tag_name from x_parsed_attributes where cat_real_id = idCategory
    loop
        select xParseAttrValue(idInfRes, r.tag_name, xmlBody) into res;
        if(res <= 0) then
            return -1;
        end if;
    end loop;
            
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xParseAttrValue(int8, varchar, xml) returns int4 as
$BODY$
declare
    idInfRes alias for $1;
    tagName alias for $2;
    xmlBody alias for $3;

    thePath varchar;
    theValue varchar;
    bExists bool;
    res int4;
begin

    select xpath_exists('/attr_values/' || tagName, xmlBody) into bExists;
    if(bExists isnull or bExists = false) then
        return 1;
    end if;

    thePath = '/attr_values/' || tagName || '/inf_resource_ref';
    select xpath_exists(thePath, xmlBody) into bExists;
    if(bExists = true) then
        select xParseAttrValueRef(idInfRes, tagName, xmlBody) into res;
        return res;
    end if;

    thePath = '/attr_values/' || tagName || '/inf_resource';
    select xpath_exists(thePath, xmlBody) into bExists;
    if(bExists = true) then
        select xParseAttrValueMultiRef(idInfRes, tagName, xmlBody) into res;
        return res;
    end if;
    

    thePath = '/attr_values/' || tagName || '/text()';

    SELECT btrim(array_to_string(xpath(thePath, xmlBody), '', ''))::varchar into theValue;
    if(theValue isnull or theValue = '') then
        return 1;
    end if;

    insert into x_parsed_attr_values(id_inf_resource, tag_name, the_value, a_type) values(idInfRes, tagName, theValue, 1);

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function xParseAttrValueRef(int8, varchar, xml) returns int4 as
$BODY$
declare
    idInfRes alias for $1;
    tagName alias for $2;
    xmlBody alias for $3;

    theValue varchar;
    
begin

    SELECT btrim(array_to_string(xpath('/inf_resource_ref/text()', xmlBody), '', ''))::varchar into theValue;
    if(theValue isnull or theValue = '') then
        return -1;
    end if;
    
    insert into x_parsed_attr_values(id_inf_resource, tag_name, the_value, a_type) values(idInfRes, tagName, theValue, 2);

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xParseAttrValueMultiRef(int8, varchar, xml) returns int4 as
$BODY$
declare
    idInfRes alias for $1;
    tagName alias for $2;
    xmlBody alias for $3;

    theValue varchar;
    val varchar;
    r record;
begin

    SELECT btrim(array_to_string(xpath('/inf_resource/type/text()', xmlBody), '', ''))::varchar into theValue;
    if(theValue isnull or theValue = '') then
        return -1;
    end if;

    if(theValue <> 'MultiRecord') then
        return -1;
    end if;

    theValue = '';
    for r in select unnest(xpath('/inf_resource/table_records/inf_resource_ref/text()', xmlBody)::varchar[]) as value_as_varchar
    loop
        select btrim(r.value_as_varchar) into val;
        if(val is not null and val <> '') then
            if(theValue = '') then
                theValue = val;
            else
                theValue = theValue || ', ' || val;
            end if;
        end if;
    end loop;


    insert into x_parsed_attr_values(id_inf_resource, tag_name, the_value, a_type) values(idInfRes, tagName, theValue, 3);
    return 1;
end
$BODY$
language 'plpgsql';
