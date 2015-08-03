create or replace function getXMLAttrType(varchar) returns int4 as
$BODY$
declare
    xmlType alias for $1;
begin

    if(xmlType = 'String') then
        return 9;
    elseif(xmlType = 'Boolean') then
        return 1;
    elseif(xmlType = 'Integer') then
        return 30; --64 bits
    elseif(xmlType = 'Double') then
        return 6;
    elseif(xmlType = 'Date') then
        return 4;
    elseif(xmlType = 'DateTime') then
        return 5;
    elseif(xmlType = 'Time') then
        return 27;

    end if;

    return 9;

end
$BODY$
language 'plpgsql';
