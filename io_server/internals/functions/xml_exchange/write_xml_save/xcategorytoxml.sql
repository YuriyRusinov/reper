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

    xml_str varchar;
begin

    catUUID := '';
    xml_str = '';

    if(idCategory isnull) then
        return xml_str;
    end if;
    
    if(tagName <> 'document_category' and tagName <> 'table_category' and tagName <> 'table_indicators_category') then
        return '';
    end if;

    for r in select unique_id, name, description, id_child, id_child2 from io_categories where id = idCategory
    loop
        catUniqueId = r.unique_id;
        catName = r.name;
        catDesc = r.description;
        idChild = r.id_child;
        idChild2 = r.id_child2;
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

    if(catDesc is not null) then
        xml_str := xml_str || E'<description> <![CDATA[ ' || catDesc || E' ]]> </description>\n';
    else
        xml_str := xml_str || E'<description>  </description>\n';
    end if;

    xml_str := xml_str || E'</parameters>\n';

    xml_str := xml_str || xCategoryAttrsToXML(idCategory);

    xml_str := xml_str || E'</' || tagName || E'>\n';


    if(tagName = 'document_category' and idChild is not null and idChild > 0) then
        xml_str := xml_str || xCategoryToXML(idChild, 'table_category');

        if(idChild2 is not null and idChild2 > 0) then
            xml_str := xml_str || xCategoryToXML(idChild2, 'table_indicators_category');
        end if;
    end if;

    return xml_str;
end
$BODY$
language 'plpgsql';
