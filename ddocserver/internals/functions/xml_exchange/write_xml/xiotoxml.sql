create or replace function xIOToXML (int4) returns varchar as
$BODY$
declare
    idObject alias for $1;

    r record;
    ioUUID varchar;
    ioUniqueId varchar;
    ioName varchar;
    ioDesc varchar;
    idCategory int4;
    idTableCategory int4;
    idIndCategory int4;
    ioTableName varchar;

    xml_str varchar;
    cat_xml_str varchar;
    infres_xml_str varchar;
  
    ioType varchar;
begin

    if (idObject is null or idObject <= 0) then
        return null;
    end if;

    for r in 
        select io.unique_id, io.uuid_t, io.name, io.description, io.id_io_category, io.table_name, c.id_child, c.id_child2 
        from io_objects io, io_categories c 
        where io.id = idObject and io.id_io_category = c.id
    loop
        idCategory = r.id_io_category;
        idTableCategory = r.id_child;
        idIndCategory = r.id_child2;
        ioUUID = r.uuid_t;
        ioUniqueId = r.unique_id;
        ioName = r.name;
        ioDesc = r.description;
        ioTableName = r.table_name;
    end loop;

    if(ioUniqueId isnull) then
        raise warning 'Cannot find document with id=%', idObject;
        return null;
    end if;

    if(ioTableName is null) then
        ioType = 'Document';
    else
        ioType = 'Qualifier';
    end if;


    perform xCreateTempTable();

    perform xCategoryToXML(idCategory, 'document_category');


    xml_str := E'<inf_resource root ="true" uid="' || ioUUID || E'">\n';

    xml_str := xml_str || E'<type> ' || ioType || E' </type>\n';

    xml_str := xml_str || E'<category_description>\n';
    xml_str := xml_str || E'<document_category_uid> <![CDATA[ ' || asString(idCategory, false) || E' ]]> </document_category_uid>\n';
    if(idTableCategory is not null) then 
        xml_str := xml_str || E'<table_category_uid> <![CDATA[ ' || asString(idTableCategory, false) || E' ]]> </table_category_uid>\n';
    end if;
    if(idIndCategory is not null) then 
        xml_str := xml_str || E'<table_indicators_category_uid> <![CDATA[ ' || asString(idIndCategory, false) || E' ]]> </table_indicators_category_uid>\n';
    end if;
    xml_str := xml_str || E'</category_description>\n';


    xml_str := xml_str || xIOSysAttributesToXML(idObject, ioUUID, ioUniqueId, ioName, ioDesc);

    xml_str := xml_str || xIOAttrsValuesToXML(idObject);

    if(ioTableName is not null) then
        xml_str := xml_str || xIORecordsToXML(idObject, NULL);
    end if;

    xml_str := xml_str || xReadIOFiles  (idObject);

    xml_str := xml_str || E'</inf_resource>\n';

    cat_xml_str := E'<categories>\n';
    cat_xml_str := cat_xml_str || xGetPreparedCategories();
    cat_xml_str := cat_xml_str || E'</categories>\n';

    infres_xml_str := E'<inf_resources>\n';
    infres_xml_str := infres_xml_str || xml_str;
    xml_str := xGetPreparedRecords();
    if(xml_str is not null) then
        infres_xml_str := infres_xml_str || xml_str;
    end if;
    infres_xml_str := infres_xml_str || E'</inf_resources>\n';

    xml_str := cat_xml_str || infres_xml_str;


    perform xDropTempTable();

    return xml_str;
end
$BODY$
language 'plpgsql';
