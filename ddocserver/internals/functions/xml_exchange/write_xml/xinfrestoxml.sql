create or replace function xInfResToXML(int4) returns xml as
$BODY$
declare
    idObject alias for $1;
    xml_str varchar;
    result xml;
begin
    
    xml_str := E'<?xml version="1.0" encoding="UTF-8"?>\n';

    xml_str := xml_str || xIOToXML(idObject);

    result := xmlroot (xmlparse(document (xml_str)), version '1.0', standalone no);

    return result;
end
$BODY$
language 'plpgsql';


create or replace function xInfResToXML(int4, int8) returns xml as
$BODY$
declare
    idObject alias for $1;
    idRecord alias for $2;

    xml_str varchar;
    result xml;
begin
    
    xml_str := E'<?xml version="1.0" encoding="UTF-8"?>\n';

    xml_str := xml_str || xRecToXML(idObject, idRecord);

    result := xmlroot (xmlparse(document (xml_str)), version '1.0', standalone no);

    return result;
end
$BODY$
language 'plpgsql';

create or replace function xInfResToXML(int4, int8[]) returns xml as
$BODY$
declare
    idObject alias for $1;
    idRecords alias for $2;

    xml_str varchar;
    result xml;
begin
    
    xml_str := E'<?xml version="1.0" encoding="UTF-8"?>\n';

    xml_str := xml_str || xMultiRecToXML(idObject, idRecords);

    result := xmlroot (xmlparse(document (xml_str)), version '1.0', standalone no);

    return result;
end
$BODY$
language 'plpgsql';

