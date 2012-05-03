CREATE OR REPLACE FUNCTION iofromxml(character varying)
  RETURNS boolean AS
$BODY$
declare
    value alias for $1;

    query varchar;
    tbody varchar;
    xml_type varchar; 
    isMakeIO boolean;
    r RECORD;
begin

    query := 'SELECT xml_valid('||quote_literal(value)||') as xml_valid';
   -- check valid xml document
    isMakeIO := true;
    execute query INTO isMakeIO;

    if (not isMakeIO ) then
        raise warning 'Invalid XML';
        return isMakeIO;       
    END IF;

   -- get type of xml message

    tbody =  '/msg/passport/msg_created/as_having/kind/text()';
 
    query := 'SELECT trim((xpath(' || quote_literal(tbody) || ', ' || quote_literal(value) || '::xml))[1]::varchar) As metadataname';

    execute query INTO xml_type;
    raise warning 'xml type is %', xml_type;

    IF xml_type is NULL
    THEN
        tbody = '/msg/passport/receipt_created/as_having/type/text()';
        query := 'SELECT trim((xpath(' || quote_literal(tbody) || ', ' || quote_literal(value) || '::xml))[1]::varchar) As metadataname';
        execute query into xml_type;
        if(xml_type isnull or xml_type <> 'DR.3') then
	    return false;
	end if;
    END IF;

    IF (xml_type = 'cmd') THEN
        query := 'select ioFromXMLCmdSimple('||quote_literal(value)||')';
        execute query INTO isMakeIO;
        --return isMakeIO;  
    ELSEIF (xml_type  = 'ufdoc') THEN
        query := 'select ioFromXMLUfdocSimple('||quote_literal(value)||')';
        execute query INTO isMakeIO;
        --return isMakeIO;  
    ELSEIF (xml_type = 'fdoc') THEN
        query := 'select ioFromXMLFdoc('||quote_literal(value)||')';
        execute query INTO isMakeIO;
        --return isMakeIO;  
    ELSEif (xml_type = 'DR.3') then
        query := 'select receiptFromXML('||quote_literal(value)||')';
        execute query INTO isMakeIO;
    else
        return false; 
   END IF;


    return isMakeIO;

end
$BODY$
LANGUAGE plpgsql;
