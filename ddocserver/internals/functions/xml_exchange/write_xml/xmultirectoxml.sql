create or replace function xMultiRecToXML(int4, int8[]) returns varchar as
$BODY$
declare
    idObject alias for $1;
    idRecords alias for $2;

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
    cat_xml_str varchar;
    infres_xml_str varchar;

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
        raise exception 'Cannot find records with ids = % in qualifier with id = %', idRecords, idObject;
        return null;
    end if;

    perform xCreateTempTable();
    perform xCategoryToXML(idChild, 'table_category');
    perform xCategoryToXML(idChild2, 'table_indicators_category');

    xml_str := E'<inf_resource root="true" uid="00000000-0000-0000-0000-000000000000">\n';

    xml_str := xml_str || E'<type> MultiRecord </type>\n';
    
    xml_str := ml_str || E'<category_description>\n';
    
    xml_str := xml_str || E'<table_category_uid> <![CDATA[ ' || tableCategoryUid || E' ]]> </table_category_uid>\n';
    if(indCategoryUid is not null) then 
        xml_str := xml_str || E'<table_indicators_category_uid> <![CDATA[ ' || indCategoryUid || E' ]]> </table_indicators_category_uid>\n';
    end if;
    xml_str := xml_str || E'</category_description>\n';

    xml_str = xml_str || xRefQualifierToXML(idObject, ioUUID, ioUniqueId, ioTableName);

    xml_str := xml_str || xRecordsToXML(idObject, idRecords);

    xml_str := xml_str || E'</inf_resource>\n';

    cat_xml_str := E'<categories>\n';
    cat_xml_str := cat_xml_str || xGetPreparedCategories();
    cat_xml_str := cat_xml_str || '</categories>';

    infres_xml_str := E'<inf_resources>\n';
    infres_xml_str := infres_xml_str || xml_str;
    xml_str := xGetPreparedRecords();
    if(xml_str is not null) then
        infres_xml_str := infres_xml_str || xml_str;
    end if;
    infres_xml_str := infres_xml_str || E'</inf_resources>\n';

    xml_str := E'<irl_body>\n' || cat_xml_str || infres_xml_str || E'</irl_body>\n';

    perform xDropTempTable();

    return xml_str;
end
$BODY$
language 'plpgsql';
