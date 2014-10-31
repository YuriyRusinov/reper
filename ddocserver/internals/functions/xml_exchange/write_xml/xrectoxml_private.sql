create or replace function xRecToXML_private(int4, int8) returns varchar as
$BODY$
declare
    idObject alias for $1;
    idRecord alias for $2;

    idCategory int4;
    idChild int4;
    idChild2 int4;

    categoryUid varchar;
    tableCategoryUid varchar;
    indCategoryUid varchar;    

    ioUUID varchar;
    ioUniqueId varchar;
    ioTableName varchar;

    xml_str varchar;
    simple_xml_str varchar;

    r record;
    query varchar;
    recUUID varchar;
    pkField varchar; --primary key of a table. Defined because of pk NOT always is ID
begin
    if (idObject is null or idObject <= 0 or idRecord isnull or idRecord <= 0) then
        return null;
    end if;

    simple_xml_str = xIsRecordPrepared(idObject, idRecord);
    if(simple_xml_str <> '') then
        simple_xml_str := E'<inf_resource_ref> <![CDATA[ ' || simple_xml_str || E' ]]> </inf_resource_ref>\n';
        return simple_xml_str;
    end if;

    for r in 
        select
            io.unique_id,
            io.uuid_t,
            io.table_name,
            c.id_child, 
            c.id_child2,
            c.unique_id as c_uid,
            c1.unique_id as c1_uid,
            c2.unique_id as c2_uid
        from 
            io_objects io
            inner join io_categories c on (io_id_io_category = c.id)
            left join io_categories c1 on (c.id_child = c1.id)
            left join io_categories c2 on (c.id_child2 = c2.id)
        where io.id = idObject
    loop
        idChild = r.id_child;
        idChild2 = r.id_child2;

        categoryUid = r.c_uid;
        tableCategoryUid = r.c1_uid;
        indCategoryUid = r.c2_uid;

        ioUUID = r.uuid_t;
        ioUniqueId = r.unique_id;
        ioTableName = r.table_name;
    end loop;
                           
    if (idChild is null or idChild <= 0 or ioTableName is null) then
        raise exception 'Cannot find record with id = % in qualifier with id = %', idRecord, idObject;
        return null;
    end if;

    perform xCategoryToXML(idChild, 'table_category');
    perform xCategoryToXML(idChild2, 'table_indicators_category');

    pkField = getPrimaryKey(ioTableName);
    if(pkField is null) then
        raise exception 'Primary key of table % does not exist!', ioTableName;
    end if;

    if(idObject < 300) then
        query = 'select unique_id as uid from ' || ioTableName || ' where ' || pkField || ' = ' || idRecord;
    else
        query = 'select asString(uuid_t, false) as uid from ' || ioTableName || ' where ' || pkField || ' = ' || idRecord;
    end if;

    for r in execute query
    loop
        recUUID = r.uid;
    end loop;
    
    if(recUUID isnull) then
        raise exception 'record uuid is NULL! Unexpected! tableName = %, idRecord = %', ioTableName, idRecord;
        return NULL;
    end if;

    xml_str := E'<inf_resource root="false" uid="' || recUUID || E'">\n';

    xml_str := xml_str || E'<type> Record </type>\n';
    
    xml_str := xml_str || E'<category_description>\n';
    xml_str := xml_str || E'<table_category_uid> <![CDATA[ ' || tableCategoryUid || E' ]]> </table_category_uid>\n';
    if(indCategoryUid is not null) then
        xml_str := xml_str || E'<table_indicators_category_uid> <![CDATA[ ' || indCategoryUid || E' ]]> </table_indicators_category_uid>\n';
    end if;
    xml_str := xml_str || E'</category_description>\n';

    xml_str = xml_str || xRefQualifierToXML(idObject, ioUUID, ioUniqueId, ioTableName);

    xml_str := xml_str || xRecAttrsValuesToXML(idObject, idChild, ioTableName, idRecord);

    xml_str := xml_str || E'</inf_resource>\n';

    perform xInsertIntoTempTable(2::int2, idObject, idRecord, recUUID, xml_str);

    simple_xml_str := E'<inf_resource_ref> <![CDATA[ ' || recUUID || E' ]]> </inf_resource_ref>\n';
    return simple_xml_str;

    --return xml_str;
end
$BODY$
language 'plpgsql';
