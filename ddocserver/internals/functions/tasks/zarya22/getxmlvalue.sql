create or replace function getXMLValue(varchar, varchar) returns varchar as
$BODY$
declare
    iPath alias for $1;
    xmlBody alias for $2;

    rValue varchar;

    query varchar;
    cnt int4;
    i int4;
    xmlValues xml[];
    
begin

    query = 'SELECT btrim((xpath(' || quote_literal(iPath) || ', ' || quote_literal(xmlBody) || '::xml))[1]::varchar, ' || quote_literal(E' \n\r\t') || ') As rValue';
    execute query into rValue;
    if(rValue is not null and rValue <> '') then
        return rValue;
    end if;

    query = 'SELECT xpath(' || quote_literal(iPath) || ', ' || quote_literal(xmlBody) || '::xml)';
    execute query into xmlValues;
    cnt = array_upper(xmlValues, 1);

    if(cnt = 0 or cnt isnull or cnt <= 0) then
        return '';
    end if;

    for i in 1..cnt 
    loop
        rValue = btrim(xmlValues[i]::varchar, E' \n\r\t');
        if(rValue is not null and rValue <> '') then
            return rValue;
        end if;
    end loop;
    

    return rValue;

end
$BODY$
language 'plpgsql';
