create or replace function xParseCategoryParams(xml, int4) returns int4 as
$BODY$
declare
    xmlBody alias for $1;
    cType alias for $2; --1 - document_category, 2 - table_category, 3 - table_indicators_category

    idCategory int4;
    idCategoryReal int4;

    uuid_t uuid;
    uniqueId varchar;
    catName varchar;
    descr varchar;

    childCatUniqueId varchar;
    child2CatUniqueId varchar;
    idChild int4;
    idChild2 int4;

    bExist bool; --true if category already exist in database

    query varchar;
    tmpVar varchar;
begin

    SELECT btrim(array_to_string(xpath('/parameters/id/text()', xmlBody), '', ''))::varchar::int4 into idCategory;
    SELECT btrim(array_to_string(xpath('/parameters/uuid_t/text()', xmlBody), '', ''))::varchar into tmpVar;
    if(tmpVar isnull or tmpVar = '') then
        uuid_t = NULL;
    else
        uuid_t = tmpVar::uuid;
    end if;

    SELECT btrim(array_to_string(xpath('/parameters/unique_id/text()', xmlBody), '', ''))::varchar into uniqueId;
    if(uniqueId isnull or uniqueId = '') then
        return -1;
    end if;

    SELECT btrim(array_to_string(xpath('/parameters/name/text()', xmlBody), '', ''))::varchar into catName;
    if(catName isnull or catName = '') then
        return -1;
    end if;

    SELECT btrim(array_to_string(xpath('/parameters/description/text()', xmlBody), '', ''))::varchar into descr;
    if(descr = '') then
        descr = NULL;
    end if;

    if(cType is not null and cType = 1) then
        SELECT btrim(array_to_string(xpath('/parameters/table_category_uid/text()', xmlBody), '', ''))::varchar into childCatUniqueId;
        SELECT btrim(array_to_string(xpath('/parameters/table_indicators_category_uid/text()', xmlBody), '', ''))::varchar into child2CatUniqueId;

        --we garantee that table_categories and table_indicators_categories will be always earlier parsed then document_categories!
        if(childCatUniqueId is not null and childCatUniqueId <> '') then
            select real_id into idChild from x_parsed_categories where unique_id = childCatUniqueId;
        end if;
        if(child2CatUniqueId is not null and child2CatUniqueId <> '') then
            select real_id into idChild2 from x_parsed_categories where unique_id = child2CatUniqueId;
        end if;
    end if;

    bExist = true;

    query = 'select id from io_categories where unique_id = ' || quote_literal(uniqueId) || ' and name = ' || quote_literal(catName);
    if(cType is not null and cType = 1) then
        if(idChild is not null) then
            query = query || ' and id_child = ' || idChild;
        end if;
        if(idChild2 is not null) then
            query = query || ' and id_child2 = ' || idChild2;
        end if;
    end if;

    execute query into idCategoryReal;
    if(idCategoryReal isnull) then
        idCategoryReal = getNextSeq('io_categories', 'id');
        bExist = false;
    end if;

    insert into x_parsed_categories (unique_id, real_id, name, description, id_child, id_child2, c_type, b_exist) 
    values (uniqueId, idCategoryReal, catName, descr, idChild, idChild2, cType, bExist);     

    return idCategoryReal;
end
$BODY$
language 'plpgsql';

create or replace function xParseCategoryAttr(int4, xml) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    xmlBody alias for $2;

    tagName varchar;
    idAttr int4;
    uuid_t uuid;
    uniqueId varchar;
    attrCode varchar;
    attrTitle varchar;
    attrName varchar;
    valueType varchar;
    tableName varchar;
    columnName varchar;
    refColumnName varchar;
    bMandatory bool;
    bReadOnly bool;
    defWidth int4;
    defValue varchar;

    idAttrType int4;
    idAttribute int4;
    bExist bool; --true if atribute already exist in database

    tmpVar varchar;
begin

    SELECT btrim(array_to_string(xpath('/attribute/tag_name/text()', xmlBody), '', ''))::varchar into tagName;
    if(tagName isnull or tagName = '') then
        return -1;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/id/text()', xmlBody), '', ''))::varchar::int4 into idAttr;

    SELECT btrim(array_to_string(xpath('/attribute/uuid_t/text()', xmlBody), '', ''))::varchar into tmpVar;
    if(tmpVar isnull or tmpVar = '') then
        uuid_t = NULL;
    else
        uuid_t = tmpVar::uuid;
    end if;
    

    SELECT btrim(array_to_string(xpath('/attribute/unique_id/text()', xmlBody), '', ''))::varchar into uniqueId;
    if(uniqueId isnull or uniqueId = '') then
        return -1;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/code/text()', xmlBody), '', ''))::varchar into attrCode;
    if(attrCode isnull or attrCode = '') then
        return -1;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/name/text()', xmlBody), '', ''))::varchar into attrName;
    if(attrName isnull or attrName = '') then
        return -1;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/title/text()', xmlBody), '', ''))::varchar into attrTitle;
    if(attrTitle isnull or attrTitle = '') then
        attrTitle = attrName;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/value_type/text()', xmlBody), '', ''))::varchar into valueType;
    if(valueType isnull) then
        return -1;
    end if;
    select xAttrTypeFromXML(valueType) into idAttrType;
    if(idAttrType isnull) then
        return -1;
    end if;


    SELECT btrim(array_to_string(xpath('/attribute/table_name/text()', xmlBody), '', ''))::varchar into tableName;
    if(tableName = '') then
        tableName = NULL;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/column_name/text()', xmlBody), '', ''))::varchar into columnName;
    if(columnName = '') then
        columnName = NULL;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/ref_column_name/text()', xmlBody), '', ''))::varchar into refColumnName;
    if(refColumnName = '') then
        refColumnName = NULL;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/mandatory/text()', xmlBody), '', ''))::varchar into tmpVar;
    if(tmpVar isnull or btrim(tmpVar) = '') then
        bMandatory = false;
    else
        bMandatory = tmpVar::bool;
    end if;
   
    SELECT btrim(array_to_string(xpath('/attribute/read_only/text()', xmlBody), '', ''))::varchar into tmpVar;
    if(tmpVar isnull or btrim(tmpVar) = '') then
        bReadOnly = false;
    else
        bReadOnly = tmpVar::bool;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/def_width/text()', xmlBody), '', ''))::varchar into tmpVar;
    if(tmpVar isnull or btrim(tmpVar) = '') then
        defWidth = 100;
    else
        defWidth = tmpVar::int4;
    end if;

    SELECT btrim(array_to_string(xpath('/attribute/def_value/text()', xmlBody), '', ''))::varchar into defValue;
    if(defValue = '') then
        defValue = NULL;
    end if;

    idAttribute = aFindEqualEx(idAttrType, attrCode, attrName, attrTitle, tableName, columnName, defWidth);
    bExist = true;
    if(idAttribute <= 0) then
        idAttribute = getNextSeq('attributes', 'id');
        bExist = false;
    end if;
        

    insert into x_parsed_attributes (cat_real_id, real_id, unique_id, tag_name, code, title, name, table_name, column_name, ref_column_name, mandatory, read_only, def_width, def_value, id_attr_type, b_exist)
    values(idCategory, idAttribute, uniqueId, tagName, attrCode, attrTitle, attrName, tableName, columnName, refColumnName, bMandatory, bReadOnly, defWidth, defValue, idAttrType, bExist);

    return idAttribute;
end
$BODY$
language 'plpgsql';

create or replace function xParseCategoryAttrs(int4, xml) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    xmlBody alias for $2;

    r record;
    res int4;
begin

    for r in select unnest(xpath('/attributes/attribute', xmlBody)) as xml_part
    loop
        select xParseCategoryAttr(idCategory, r.xml_part) into res; 
        if(res <= 0) then
            return -1;
        end if;
    end loop;

    return 1;
end
$BODY$
language 'plpgsql';


create or replace function xParseCategory(xml, int4) returns int4 as
$BODY$
declare
    xmlBody alias for $1;
    cType alias for $2;

    query varchar;
    tagName1 varchar;
    tagName2 varchar;

    paramsXML xml;
    attrsXML xml;

    res int4;
    catRealId int4;
begin

    if(cType = 1) then
        tagName1 = '/document_category/parameters';
        tagName2 = '/document_category/attributes';
    elsif(cType = 2) then
        tagName1 = '/table_category/parameters';
        tagName2 = '/table_category/attributes';
    elsif(cType = 3) then
        tagName1 = '/table_indicators_category/parameters';
        tagName2 = '/table_indicators_category/attributes';
    else
        return -1;
    end if;

    query := 'select unnest(xpath(' || quote_literal(tagName1) || ', ' || quote_literal(xmlBody::varchar) || '::xml))';
    execute query into paramsXML;
    if(paramsXML isnull) then
        return -1;
    end if;

    select xParseCategoryParams(paramsXML, cType) into catRealId;
    if(catRealId <= 0) then
        return -1;
    end if;

    query := 'select unnest(xpath(' || quote_literal(tagName2) || ', ' || quote_literal(xmlBody::varchar) || '::xml))';
    execute query into attrsXML;
    if(attrsXML isnull) then
        return -1;
    end if;

    select xParseCategoryAttrs(catRealId, attrsXML) into res;
    if(res <= 0) then
        return -1;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';


create or replace function xParseCategories(xml) returns int4 as
$BODY$
declare
    xmlBody alias for $1;

    r record;
    res int4;
begin

    for r in select unnest(xpath('/categories/table_indicators_category', xmlBody)) as xml_part
    loop
        select xParseCategory(r.xml_part, 3) into res;
        if(res <= 0) then
            return -1;
        end if;
    end loop;

    for r in select unnest(xpath('/categories/table_category', xmlBody)) as xml_part
    loop
        select xParseCategory(r.xml_part, 2) into res;
        if(res <= 0) then
            return -1;
        end if;
    end loop;

    for r in select unnest(xpath('/categories/document_category', xmlBody)) as xml_part
    loop
        select xParseCategory(r.xml_part, 1) into res;
        if(res <= 0) then
            return -1;
        end if;
    end loop;

    return 1;
end
$BODY$
language 'plpgsql';
