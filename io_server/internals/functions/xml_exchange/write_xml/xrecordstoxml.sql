create or replace function xRecordsToXML(int4, int8[]) returns varchar as
$BODY$
declare
    idObject alias for $1;
    idRecords alias for $2; --if NULL then all records will be writed to XML

    xml_str varchar;
    xml_str_all varchar;

    i int4;
    j int4;
    cnt int4;
begin
    xml_str = '';
    xml_str_all = '';
    j = 0;

    cnt = array_upper(idRecords, 1);

    for i in 1..cnt
    loop
        j = j+1;
        xml_str = xml_str || xRecToXML_private(idObject, idRecords[i]) || E'\n';
    end loop;

    xml_str_all = xml_str_all || E'<table_records record_count="' || j || E'">\n';
    xml_str_all = xml_str_all || xml_str;
    xml_str_all = xml_str_all || E'</table_records>\n';

    return xml_str_all;
end
$BODY$
language 'plpgsql';
