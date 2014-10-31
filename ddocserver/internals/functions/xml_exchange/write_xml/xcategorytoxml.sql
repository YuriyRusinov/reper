create or replace function xCategoryToXML(int4, varchar) returns varchar as
$BODY$
declare
    idCategory alias for $1;
    tagName alias for $2;

    r record;
    catUUID varchar;
    catUniqueId varchar;
    catName varchar;
    catDesc varchar;

    idChild int4;
    idChild2 int4;
    childCatUniqueId varchar;
    child2CatUniqueId varchar;


    xml_str varchar;
    full_xml_str varchar;
begin

    catUUID := '';
    xml_str = '';
    full_xml_str = '';

    if(idCategory isnull) then
        return '';
    end if;
    
    if(tagName <> 'document_category' and tagName <> 'table_category' and tagName <> 'table_indicators_category') then
        return '';
    end if;

    if(xIsCategoryPrepared(idCategory) <> '') then
        return '';
    end if;

    for r in 
        select c1.unique_id, c1.name, c1.description, c1.id_child, c1.id_child2, c2.unique_id as child_unique_id, c3.unique_id as child2_unique_id
        from 
            io_categories c1 
            left join io_categories c2 on (c1.id_child = c2.id)
            left join io_categories c3 on (c1.id_child2 = c3.id)
        where c1.id = idCategory

    loop
        catUniqueId = r.unique_id;
        catName = r.name;
        catDesc = r.description;
        idChild = r.id_child;
        idChild2 = r.id_child2;
        childCatUniqueId = r.child_unique_id;
        child2CatUniqueId = r.child2_unique_id;
    end loop;

    if(catUniqueId isnull) then
        return '';
    end if;

    xml_str := xml_str || E'<' || tagName || E'>\n';

    xml_str := xml_str || E'<parameters>\n';
    xml_str := xml_str || E'<id> ' || idCategory || E' </id>\n';
    xml_str := xml_str || E'<uuid_t> <![CDATA[ ' || catUUID || E' ]]> </uuid_t>\n';
    xml_str := xml_str || E'<unique_id> <![CDATA[ ' || catUniqueId || E' ]]> </unique_id>\n';
    xml_str := xml_str || E'<name> <![CDATA[ ' || catName || E' ]]> </name>\n';
    if(idChild is not null) then
        xml_str := xml_str || E'<table_category_uid> <![CDATA[ ' || childCatUniqueId || E' ]]> </table_category_uid>\n';
    end if;
    if(idChild2 is not null) then
        xml_str := xml_str || E'<table_indicators_category_uid> <![CDATA[ ' || child2CatUniqueId || E' ]]> </table_indicators_category_uid>\n';
    end if;

    if(catDesc is not null) then
        xml_str := xml_str || E'<description> <![CDATA[ ' || catDesc || E' ]]> </description>\n';
    else
        xml_str := xml_str || E'<description>  </description>\n';
    end if;

    xml_str := xml_str || E'</parameters>\n';

    xml_str := xml_str || xCategoryAttrsToXML(idCategory);

    xml_str := xml_str || E'</' || tagName || E'>\n';


    if(tagName = 'document_category' and idChild is not null and idChild > 0) then
        full_xml_str := xml_str || xCategoryToXML(idChild, 'table_category');

        if(idChild2 is not null and idChild2 > 0) then
            full_xml_str := full_xml_str || xCategoryToXML(idChild2, 'table_indicators_category');
        end if;
    else
        full_xml_str := xml_str;
    end if;

    perform xInsertIntoTempTable(3::int2, NULL::int4, idCategory, asString(idCategory, false), xml_str);

    return full_xml_str;
end
$BODY$
language 'plpgsql';
