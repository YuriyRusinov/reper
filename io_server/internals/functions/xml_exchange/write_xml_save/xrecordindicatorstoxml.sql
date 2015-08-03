create or replace function xRecordIndicatorsToXML(int8) returns varchar as
$BODY$
declare
    idRecord alias for $1;
    xml_str varchar;
begin

    xml_str = E'<indicators> </indicators>\n';

    return xml_str;
end
$BODY$
language 'plpgsql';
