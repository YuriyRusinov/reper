create or replace function xAttrTypeToXML(int4) returns varchar as
$BODY$
declare
    idType alias for $1;
    typeStr varchar;
begin

    if(idType = 1) then
        typeStr = 'Boolean';
    elsif(idType = 2) then
        typeStr = 'Qualifier';
    elsif(idType = 3) then
        typeStr = 'Parent';
    elsif(idType = 4) then
        typeStr = 'Date';
    elsif(idType = 5) then
        typeStr = 'DateTime';
    elsif(idType = 6) then
        typeStr = 'Double';
--    elsif(idType = 7) then
    elsif(idType = 8) then
        typeStr = 'Integer';
    elsif(idType = 9) then
        typeStr = 'String';
    elsif(idType = 10) then
        typeStr = 'Interval2';
--    elsif(idType = 11) then
    elsif(idType = 12) then
        typeStr = 'CheckBoxes';
    elsif(idType = 13) then
        typeStr = 'Text';
    elsif(idType = 14) then
        typeStr = 'FixString';
    elsif(idType = 15) then
        typeStr = 'Image';
--    elsif(idType = 16) then
    elsif(idType = 17) then
        typeStr = 'MultiQualifier';
    elsif(idType = 18) then
        typeStr = 'ColorBg';
    elsif(idType = 19) then
        typeStr = 'ColorRefBg';
    elsif(idType = 20) then
        typeStr = 'XML';
    elsif(idType = 21) then
        typeStr = 'Interval3';
--    elsif(idType = 22) then
    elsif(idType = 23) then
        typeStr = 'SVG';
    elsif(idType = 24) then
        typeStr = 'Video';
    elsif(idType = 25) then
        typeStr = 'ColorFore';
    elsif(idType = 26) then
        typeStr = 'ColorRefFore';
    elsif(idType = 27) then
        typeStr = 'Time';
    elsif(idType = 28) then
        typeStr = 'Coords';
--    elsif(idType = 29) then
    elsif(idType = 30) then
        typeStr = 'Long';
    elsif(idType = 31) then
        typeStr = 'UUID';
    elsif(idType = 32) then
        typeStr = 'Complex';
    elsif(idType = 33) then
        typeStr = 'Histogram';
--    elsif(idType = 34) then
--    elsif(idType = 35) then
    elsif(idType = 36) then
        typeStr = 'Map';
    elsif(idType = 37) then
        typeStr = 'DateTimeMs';
    else
        typeStr = 'String';
    end if;

    return typeStr;

end
$BODY$
language 'plpgsql';
