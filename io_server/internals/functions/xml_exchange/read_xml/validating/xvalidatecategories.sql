create or replace function xValidateCategories(xml) returns bool as
$BODY$
declare
    xmlBody alias for $1;
    tCount int4;
begin

    select 
        count(t.cat) into tCount 
    from
        (SELECT unnest(xpath('/irl_body/categories/document_category', xmlBody))::varchar as cat) t;

    if(tCount is null and tCount = 0) then
        select 
            count(t.cat) into tCount 
        from
            (SELECT unnest(xpath('/irl_body/categories/table_category', xmlBody))::varchar as cat) t;
    end if;

    if(tCount isnull or tCount = 0) then
        return false;
    end if;

    
    return true;


end
$BODY$
language 'plpgsql';
