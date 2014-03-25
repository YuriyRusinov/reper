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
    ioTableName varchar;

    xml_str varchar;
    ioType varchar;
begin

    if (idObject is null or idObject <= 0) then
        return null;
    end if;

    for r in select unique_id, uuid_t, name, description, id_io_category, table_name from io_objects where id = idObject
    loop
        idCategory = r.id_io_category;
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

    xml_str := E'<inf_resource>\n';

    xml_str := xml_str || E'<type> ' || ioType || E' </type>\n';

    xml_str := xml_str || E'<category_description>\n';
    xml_str := xml_str || xCategoryToXML(idCategory, 'document_category');
    xml_str := xml_str || E'</category_description>\n';


    xml_str := xml_str || xIOSysAttributesToXML(idObject, ioUUID, ioUniqueId, ioName, ioDesc);

    xml_str := xml_str || xIOAttrsValuesToXML(idObject);

    if(ioTableName is not null) then
        xml_str := xml_str || xIORecordsToXML(idObject, NULL);
    end if;

    xml_str := xml_str || xReadIOFiles  (idObject);

    xml_str := xml_str || E'</inf_resource>\n';

    --raise exception '%', xml_str;

    return xml_str;
end
$BODY$
language 'plpgsql';
