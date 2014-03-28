create or replace function xMultiRecToXML(int4, int8[]) returns varchar as
$BODY$
declare
    idObject alias for $1;
    idRecords alias for $2;

    idCategory int4;
    idChild int4;
    idChild2 int4;
    ioUUID varchar;
    ioUniqueId varchar;
    ioTableName varchar;

    xml_str varchar;
    r record;
    cnt int4;
begin
    if (idObject is null or idObject <= 0 or idRecords isnull) then
        return '';
    end if;

    cnt = array_upper(idRecords, 1);
    if(cnt isnull or cnt <= 0) then
        return '';
    end if;
    
    for r in 
        select 
            io.unique_id,
            io.uuid_t,
            io.table_name,
            c.id_child, 
            c.id_child2 
        from io_objects io, io_categories c 
        where io.id = idObject and io.id_io_category = c.id
    loop
        idChild = r.id_child;
        idChild2 = r.id_child2;
        ioUUID = r.uuid_t;
        ioUniqueId = r.unique_id;
        ioTableName = r.table_name;
    end loop;

    if (idChild is null or idChild <= 0 or ioTableName is null) then
        raise exception 'Cannot find records with ids = % in qualifier with id = %', idRecords, idObject;
        return null;
    end if;

    xml_str := E'<inf_resource>\n';

    xml_str := xml_str || E'<type> MultiRecord </type>\n';
    
    xml_str := xml_str || E'<category_description>\n';
    xml_str := xml_str || xCategoryToXML(idChild, 'table_category');
    if(idChild2 is not null) then
        xml_str := xml_str || xCategoryToXML(idChild2, 'table_indicators_category');
    end if;
    xml_str := xml_str || E'</category_description>\n';

    xml_str = xml_str || xRefQualifierToXML(idObject, ioUUID, ioUniqueId);

    xml_str := xml_str || xIORecordsToXML(idObject, idRecords);

    xml_str := xml_str || E'</inf_resource>\n';

    --raise exception '%', xml_str;

    return xml_str;
end
$BODY$
language 'plpgsql';
