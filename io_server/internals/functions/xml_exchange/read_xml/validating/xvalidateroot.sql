create or replace function xValidateRoot(xml) returns bool as
$BODY$
declare
    xmlBody alias for $1;
    tCount int4;
begin

    select count(t.is_root) into tCount
    from 
        (SELECT unnest(xpath('/irl_body/inf_resources/inf_resource/@root', xmlBody))::varchar::bool as is_root) t
    where 
        t.is_root = true;
    
    if(tCount is null or tCount <> 1) then 
        return false;
    end if;

    return true;
end
$BODY$
language 'plpgsql';
