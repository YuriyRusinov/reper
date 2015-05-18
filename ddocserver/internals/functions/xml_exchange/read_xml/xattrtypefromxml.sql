create or replace function xAttrTypeFromXML(varchar) returns int4 as
$BODY$
declare
    valueType alias for $1;
    idAttrType int4;
begin

    if(valueType = 'Boolean') then
        idAttrType = 1;
    elsif(valueType = 'Qualifier') then
        idAttrType = 2;
    elsif(valueType = 'Parent') then
        idAttrType = 3;
    elsif(valueType = 'Date') then
        idAttrType = 4;
    elsif(valueType = 'DateTime') then
        idAttrType = 5;
    elsif(valueType = 'Double') then
        idAttrType = 6;
--    elsif(idType = 7) then
    elsif(valueType = 'Integer') then
        idAttrType = 8;
    elsif(valueType = 'String') then
        idAttrType = 9;
    elsif(valueType = 'Interval2') then
        idAttrType = 10;
--    elsif(idType = 11) then
    elsif(valueType = 'CheckBoxes') then
        idAttrType = 12;
    elsif(valueType = 'Text') then
        idAttrType = 13;
    elsif(valueType = 'FixString') then
        idAttrType = 14;
    elsif(valueType = 'Image') then
        idAttrType = 15;
--    elsif(idType = 16) then
    elsif(valueType = 'MultiQualifier') then
        idAttrType = 17;
    elsif(valueType = 'ColorBg') then
        idAttrType = 18;
    elsif(valueType = 'ColorRefBg') then
        idAttrType = 19;
    elsif(valueType = 'XML') then
        idAttrType = 20;
    elsif(valueType = 'Interval3') then
        idAttrType = 21;
--    elsif(idType = 22) then
    elsif(valueType = 'SVG') then
        idAttrType = 23;
    elsif(valueType = 'Video') then
        idAttrType = 24;
    elsif(valueType = 'ColorFore') then
        idAttrType = 25;
    elsif(valueType = 'ColorRefFore') then
        idAttrType = 26;
    elsif(valueType = 'Time') then
        idAttrType = 27;
    elsif(valueType = 'Coords') then
        idAttrType = 28;
--    elsif(idType = 29) then
    elsif(valueType = 'Long') then
        idAttrType = 30;
    elsif(valueType = 'UUID') then
        idAttrType = 31;
    elsif(valueType = 'Complex') then
        idAttrType = 32;
    elsif(valueType = 'Histogram') then
        idAttrType = 33;
--    elsif(idType = 34) then
--    elsif(idType = 35) then
    elsif(valueType = 'Map') then
        idAttrType = 36;
    elsif(valueType = 'DateTimeMs') then
        idAttrType = 37;
    else
        idAttrType = 9; --String
    end if;

    return idAttrType;
end
$BODY$
language 'plpgsql';
