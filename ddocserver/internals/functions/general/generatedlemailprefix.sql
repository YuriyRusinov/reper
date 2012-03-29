create or replace function generateDlEmailPrefix(int4) returns varchar as
$BODY$
declare
    idDl alias for $1;
    orgPrefix varchar;
    dlPrefix varchar;
begin

    select
        o.email_prefix into orgPrefix
    from 
        organization o,
        units u,
        "position" p
    where 
        p.id = idDl
        and p.id_unit = u.id
        and u.id_organization = o.id;

    if(orgPrefix isnull) then
        orgPrefix = 'localorg';
    end if;

    dlPrefix = orgPrefix || '_position_' || idDl;
    
    return dlPrefix;

end
$BODY$
language 'plpgsql';
