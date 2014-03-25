create or replace function xRecordFilesToXML(int8) returns varchar as
$BODY$
declare
    idRecord alias for $1;
    xml_str varchar;
begin

    xml_str = E'<attached_files count="0"> </attached_files>\n';

    return xml_str;
end
$BODY$
language 'plpgsql';
