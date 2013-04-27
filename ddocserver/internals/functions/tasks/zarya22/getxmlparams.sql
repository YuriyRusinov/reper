select f_safe_drop_type('h_xml_parameters');

CREATE TYPE h_xml_parameters AS (attr_code varchar, attr_name varchar, attr_type varchar, attr_value varchar);

-- Function: getxmlparameteres()
CREATE OR REPLACE FUNCTION getxmlparams(character varying)
  RETURNS SETOF h_xml_parameters AS
$BODY$
declare
    theValue alias for $1;

    idUser int4;
    nRead int4;

    rr h_xml_parameters%rowtype;

    xml_val varchar;
    xml_tag_name varchar;
    xml_name varchar;
    xml_type varchar;
    
    xml_tag_numbers xml[];
    isValid boolean;

    tbody varchar;
    query varchar;
    parameter varchar;
    number varchar;

    msgType varchar;
    sectionName varchar;
    cnt int4;
    
begin
-- check xml
    query := 'SELECT xml_valid('||quote_literal(theValue)||') as xml_valid';
   -- check valid xml document
    isValid := true;
    execute query INTO isValid;

    IF isValid is FALSE
    THEN
        return;       
    END IF;


    tbody =  '/msg/passport/msg_created/as_having/kind/text()';
    select getXMLValue(tBody, theValue) INTO msgType;

    IF msgType is NULL
    THEN
	return ;
    END IF;

    if(msgType = 'cmd') then
        sectionName = 'command_data';
    elsif (msgType = 'ufdoc') then
        sectionName = 'unformalized_document_data';
    elsif (msgType = 'ufrec') then
        sectionName = 'unformalized_document_data';
    else
        return;
    end if;


    -- get all numbers of parameters
    tbody := '/msg/body/' || sectionName || '/formalized_parameters/parameters_description/parameter/@number';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '|| quote_literal(theValue) ||'::xml)) As date';
    execute query INTO xml_tag_numbers;

    cnt = array_upper (xml_tag_numbers, 1);
    --нет формализованных параметров
    if(xml_tag_numbers isnull or cnt isnull or cnt <= 0) then
        return;
    end if;

    for i in 1..cnt 
    loop
		number = xml_tag_numbers[i]::varchar;
                -- get tag_name

		tbody :=  '/msg/body/' || sectionName || '/formalized_parameters/parameters_description/parameter[@number=' || asString(number, false) || ']/tag_name/text()';
		--query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(theValue) ||'::xml))[1]::varchar) As date';
                select getXMLValue(tBody, theValue) INTO xml_tag_name;
		--execute query INTO xml_tag_name;

		-- get name

		tbody :=  '/msg/body/' || sectionName || '/formalized_parameters/parameters_description/parameter[@number=' || asString(number, false) ||']/name/text()';
                select getXMLValue(tBody, theValue) INTO xml_name;
                --query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(theValue) ||'::xml))[1]::varchar) As date';
		--execute query INTO xml_name;

		-- get type
		tbody :=  '/msg/body/' || sectionName || '/formalized_parameters/parameters_description/parameter[@number=' || asString(number, false) ||']/value_type/text()';
                select getXMLValue(tBody, theValue) INTO xml_type;
		--query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(theValue) ||'::xml))[1]::varchar) As date';
		--execute query INTO xml_type;
    

		-- get its values

		tbody :=  '/msg/body/' || sectionName || '/formalized_parameters/parameters_values/'|| asString(xml_tag_name::varchar, false)||'/text()';
                select getXMLValue(tBody, theValue) INTO xml_val;
                raise warning E'\n\n\n% \n\n\n', xml_val;
		--raise exception E'\n\n\n% \n\n\n', xml_val;

             --raise warning '___ % % % %',xml_tag_name, xml_name, xmt_type, xml_val;  
		insert into XXXX (tag_name, the_name, the_type, the_value) values (xml_tag_name, xml_name, xml_type, xml_val);
	
    end loop;
    

    for rr in select * from XXXX
    loop
        return next rr;
    end loop;

   return;
    
end
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100
  ROWS 1000;
