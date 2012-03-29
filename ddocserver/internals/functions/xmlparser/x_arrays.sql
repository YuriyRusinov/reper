
create or replace function createXPathArray(int4, int4) returns varchar[] as
$BODY$
declare
    idXMLQueryType alias for $1;
    xmlLevel alias for $2;
    
    xArray varchar[];
begin

    xArray = createXPathArray1(idXMLQueryType, xmlLevel, true)::varchar[];

    return xArray;

end
$BODY$
language 'plpgsql';

create or replace function createXPathArray1(int4, int4, bool) returns varchar as
$BODY$
declare
    idXMLQueryType alias for $1;
    xmlLevel alias for $2;
    bQuoted alias for $3;

    xArray varchar;
    r record;
    xLevel int4;
    q varchar;
    isQuoted bool;
begin
    xLevel = xmlLevel;

    if(bQuoted isnull) then
        isQuoted = false;
    else
        isQuoted = bQuoted;
    end if;
    
    if(xLevel isnull) then
        q = 'select x.xpath from xml_paths x where id_xml_query_type = ' || idXMLQueryType || ' order by x.id';
    else
        q = 'select x.xpath from xml_paths x where id_xml_query_type = ' || idXMLQueryType || ' and xml_level = ' || xLevel || ' order by x.id';
    end if;

    if(isQuoted = FALSE) then
        xArray = 'ARRAY[';
    else
        xArray = '{';
    end if;

    for r in 
        execute q
    loop
        if(xArray = 'ARRAY[' or xArray = '{') then 
            xArray = xArray || quote_literal(r.xpath);
        else
            xArray = xArray || ', ' || quote_literal(r.xpath);
        end if;

    end loop;

    if(isQuoted = FALSE) then
        xArray = xArray || ']';
    else
        xArray = xArray || '}';
    end if;

    return xArray;
end
$BODY$
language 'plpgsql';

create or replace function createXPathArray2(int4, int4) returns varchar as
$BODY$
declare
    idXMLQueryType alias for $1;
    xmlLevel alias for $2;

    xArray varchar;
    r record;
    xLevel int4;
begin
    xLevel = xmlLevel;
    if(xLevel isnull) then
        xLevel = 0;
    end if;


    xArray = 'ARRAY[';
    for r in 
        select ('/' || f.node_name || '/' || x.xpath) as x_path 
        from xml_paths x, xml_formats f 
        where 
            x.id_xml_query_type = idXMLQueryType 
            and x.xml_level = xLevel
            and f.xml_level = x.xml_level
            and f.id_xml_query_type = x.id_xml_query_type
        order by x.id
    loop
        if(xArray = 'ARRAY[') then 
            xArray = xArray || quote_literal(r.x_path);
        else
            xArray = xArray || ', ' || quote_literal(r.x_path);
        end if;

    end loop;

    xArray = xArray || ']';

    return xArray;
end
$BODY$
language 'plpgsql';


create or replace function createXCodeArray(int4, int4) returns varchar[] as
$BODY$
declare
    idXMLQueryType alias for $1;
    xmlLevel alias for $2;

    xArray varchar[];
begin

    xArray = createXCodeArray1(idXMLQueryType, xmlLevel, true)::varchar[];
    
    return xArray;
end
$BODY$
language 'plpgsql';

create or replace function createXCodeArray1(int4, int4, bool) returns varchar as
$BODY$
declare
    idXMLQueryType alias for $1;
    xmlLevel alias for $2;
    bQuoted alias for $3;


    xArray varchar;
    r record;
    xLevel int4;
    q varchar;
    isQuoted bool;
begin
    xLevel = xmlLevel;

    if(bQuoted isnull) then
        isQuoted = false;
    else
        isQuoted = bQuoted;
    end if;
    
    if(xLevel isnull) then
        q = 'select x.xcode from xml_paths x where id_xml_query_type = ' || idXMLQueryType || ' order by x.id';
    else
        q = 'select x.xcode from xml_paths x where id_xml_query_type = ' || idXMLQueryType || ' and xml_level = ' || xLevel || ' order by x.id';
    end if;

    if(isQuoted = FALSE) then
        xArray = 'ARRAY[';
    else
        xArray = '{';
    end if;

    for r in 
        execute q
    loop
        if(xArray = 'ARRAY[' or xArray = '{') then 
            xArray = xArray || quote_literal(r.xcode);
        else
            xArray = xArray || ', ' || quote_literal(r.xcode);
        end if;

    end loop;

    if(isQuoted = FALSE) then
        xArray = xArray || ']';
    else
        xArray = xArray || '}';
    end if;

    return xArray;
end
$BODY$
language 'plpgsql';

create or replace function createXCodeArray2(int4, int4) returns varchar as
$BODY$
declare
    idXMLQueryType alias for $1;
    xmlLevel alias for $2;


    xArray varchar;
    r record;
    xLevel int4;
    q varchar;
begin
    xLevel = xmlLevel;

    if(xLevel isnull) then
        q = 'select x.xcode from xml_paths x where id_xml_query_type = ' || idXMLQueryType || ' order by x.id';
    else
        q = 'select x.xcode from xml_paths x where id_xml_query_type = ' || idXMLQueryType || ' and xml_level = ' || xLevel || ' order by x.id';
    end if;

    xArray = '';
    for r in 
        execute q
    loop
        if(xArray = '') then 
            xArray = xArray || r.xcode;
        else
            xArray = xArray || ', ' || r.xcode;
        end if;

    end loop;

    return xArray;
end
$BODY$
language 'plpgsql';

create or replace function createXRecordArray(int4, int4) returns varchar as
$BODY$
declare
    idXMLQueryType alias for $1;
    xmlLevel alias for $2;

    xArray varchar;
    r record;
    xLevel int4;
    q varchar;
begin
    xLevel = xmlLevel;
    if(xLevel isnull) then
        q = 'select x.xcode, x.data_type from xml_paths x where id_xml_query_type = ' || idXMLQueryType || ' order by x.id';
    else 
        q = 'select x.xcode, x.data_type from xml_paths x where id_xml_query_type = ' || idXMLQueryType || ' and xml_level = ' || xLevel || ' order by x.id';
    end if;

    xArray = '';
    for r in 
        execute q
    loop
        if(xArray = '') then 
            xArray = xArray || r.xcode || ' ' || r.data_type;
        else
            xArray = xArray || ', ' || r.xcode || ' ' || r.data_type;
        end if;

    end loop;

    return xArray;
end
$BODY$
language 'plpgsql';

