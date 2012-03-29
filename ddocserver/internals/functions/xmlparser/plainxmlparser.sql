
create or replace function parseXMLResponse(int4) returns setof record as
$BODY$
declare
    idResponse alias for $1;

    res int4;
    xRecord varchar;
    query varchar;
    fullRecord record;
    idXMLQueryType int4;
    r record;
    rr record;
    rrr record;
    xCodes varchar;
    q varchar;
begin

    create temp table xml_tmp_table
    (
        id_tmp serial not null, 
        node_key int4 not null, 
        id_parent int4 null, 
        xml_level int4 not null,
        xml_data xml not null
    );

    select 
        prepareXML(qr.xml_data::xml, NULL, 0, qq.id_xml_query_type) into res
    from 
        xml_query_responses qr,
        xml_queries qq
    where 
        qr.id = idResponse
        and qr.id_xml_query = qq.id;

    
    select x.id_xml_query_type into idXMLQueryType from xml_queries x, xml_query_responses xx where xx.id = idResponse and xx.id_xml_query = x.id;
    select createXRecordArray(idXMLQueryType, NULL) into xRecord;

    query = 'create temp table  xxx (id_tmp int4, x_level int4, ' || xRecord || ')';
    --raise warning '\n\n % \n\n', query;
    execute query;

    for rr in 
        select id_tmp, xml_level from xml_tmp_table order by 1
    loop
        select createXRecordArray(idXMLQueryType, rr.xml_level) into xRecord;
        select createXCodeArray2(idXMLQueryType, rr.xml_level) into xCodes;
        

        query = 'select ' || rr.id_tmp || ', ' || rr.xml_level || ', ' || xCodes || ' from xmlParserEx(' || idResponse || ', ' || rr.xml_level || ', ' || rr.id_tmp || ') f(id_tmp int4, ' || xRecord || ')';
        q = 'insert into xxx (id_tmp, x_level, ' || xCodes || ') ' || query;
        --raise notice '\n\n --- % \n\n', q;

        execute q;
    end loop;

    for r in select * from xxx
    loop
        return next r;
    end loop;

    drop table xxx;

    drop table xml_tmp_table;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function prepareXML(xml, int4, int4, int4) returns int4 as
$BODY$
declare
    xmlData alias for $1;
    idParent alias for $2;
    xLevel alias for $3;
    idXMLQueryType alias for $4;
    
    r record;
    idXML int4;
    xPathId varchar;
    levelXPath varchar;
    idNode int4;
    q varchar;
begin

    xPathId = NULL;
    levelXPath = NULL;
    
    for r in
        select xpath, node_name, id_name from xml_formats where xml_level = xLevel and id_xml_query_type = idXMLQueryType
    loop
        if(r.id_name = '' or r.id_name isnull) then
            xPathId = '';
        else
            xPathId = '/' || r.node_name || '/' || r.id_name || '/text()';
        end if;

        levelXPath = r.xpath || '/' || r.node_name;
    end loop;

    if(xPathId isnull or levelXPath isnull) then
        return 0;
    end if;
    

    --raise notice 'xPathId = % --- xPathLevel = %', xPathId, levelXPath;
    --raise notice '%', xmlData::varchar;

    if(xPathId isnull or xPathId = '') then
        q = 'select 
                 NULL::int4 as id_node, 
                 t.data as node_data
             from
                 (select unnest(xpath(' || quote_literal(levelXPath) || ', ' || quote_literal(xmlData::varchar) || '::xml)) as data) as t';
    else
        q = 'select 
                 ((xpath(' || quote_literal(xPathId) || ', t.data))[1]::text::int) as id_node, 
                 t.data as node_data
             from
                 (select unnest(xpath(' || quote_literal(levelXPath) || ', ' || quote_literal(xmlData::varchar) || '::xml)) as data) as t';
    end if;
    
    for r in execute q
    loop
        select getNextSeq('xml_tmp_table', 'id_tmp') into idXML;
        if(r.id_node isnull) then
            idNode = idXML;
        else
            idNode = r.id_node;
        end if;
        insert into xml_tmp_table( id_tmp, node_key, id_parent, xml_level, xml_data ) values(idXML, idNode, idParent, xLevel, r.node_data);

        perform prepareXML(r.node_data, idXML, xLevel+1, idXMLQueryType);
        
    end loop;

    return 1;

end
$BODY$
language 'plpgsql';



create or replace function xmlParserEx(int4, int4, int4) returns setof record as
$BODY$
declare
    idQuery alias for $1;
    xmlLevel alias for $2;
    extraId alias for $3;

    xPaths varchar;
    xCodes varchar;
    xRecord varchar;
    query varchar;
    r record;
    xLevel int4;
    keyName varchar;
begin

    xLevel = xmlLevel;
    if(xLevel isnull) then
        xLevel = 0;
    end if;

    select createXPathArray2(x.id_xml_query_type, xLevel) into xPaths from xml_queries x, xml_query_responses xx where xx.id = idQuery and xx.id_xml_query = x.id;
    if(FOUND = FALSE) then
        return;
    end if;

    select createXCodeArray1(x.id_xml_query_type, xLevel, false) into xCodes from xml_queries x, xml_query_responses xx where xx.id = idQuery and xx.id_xml_query = x.id;
    select createXRecordArray(x.id_xml_query_type, xLevel) into xRecord from xml_queries x, xml_query_responses xx where xx.id = idQuery and xx.id_xml_query = x.id;

    if(xPaths isnull or xCodes isnull or xRecord isnull) then
        return;
    end if;

    keyName = 'id_tmp';

    query = 'select * from plainXMLParser(' || quote_literal(keyName) || ', '
                                            || extraId || ', ' 
                                            || xPaths  || ', ' 
                                            || xCodes  || ', ' 
                                            || quote_literal(xRecord) || ', '
                                            || quote_literal('xml_tmp_table') || ', ' 
                                            || quote_literal('xml_data') || ') f(' || keyName || ' int4, ' || xRecord || ')';

    for r in execute query
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function plainXmlParser (varchar, int4, varchar[], varchar[], varchar, varchar, varchar) returns setof record as
$BODY$
declare
    keyName alias for $1;--key field name in table that contain xml
    keyValue alias for $2; --id value for parsing XML 
    tagPaths alias for $3; --XML tags paths
    tagNames alias for $4; --XML tag names
    tagRecords alias for $5;
    tableName alias for $6; --table that contains XML data
    tableColumn alias for $7; --column with XML data
    
    r record;
    countP int4; --tag paths count
    countN int4; --tag names count
    i int4;
    v varchar; -- separate tag
    xPaths varchar;
    query varchar;
begin

    countP := array_upper(tagPaths, 1);
    if(countP isnull or countP = 0) then
        return;
    end if;


    xPaths = '';
    for i in 1..countP
    loop
        v := tagPaths[i];
        xPaths = xPaths || v;
        if(i <> countP) then
            xPaths = xPaths || ' | ';
        end if;
    end loop;

    query = 'select 
                 t.*
             from 
                 xpath_table(' || 
                             quote_literal(keyName) || ', ' || 
                             quote_literal(tableColumn) || ', ' || 
                             quote_literal(tableName) || ', ' || 
                             quote_literal(xPaths) || ', ' || 
                             quote_literal('true') || ') as t(' || keyName || ' int4, ' || tagRecords || ')
             join ' || tableName || ' x on t.' || keyName || ' = x.' || keyName || '
             where x.' || keyName || ' = ' || keyValue; 
    
    --raise warning '%', query;

    for r in execute query
    loop
       return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

--------------------------
--------------------------
--------------------------
