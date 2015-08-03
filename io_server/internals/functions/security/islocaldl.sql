create or replace function isLocalDl(int4) returns boolean as
$BODY$
declare
    idDl alias for $1;
    cnt int4;
begin


    if(idDl = 4) then --admin
        return true;
    end if;


        select 
            count(t.id) into cnt
        from
            "position" p,
            units u,
--            organization o,
            organization_transport ot,
            transport t
        where
            p.id = idDl

            and p.id_unit = u.id
            and u.id_organization = ot.id_organization
--            and o.id = ot.id_organization
            and ot.id_transport = t.id
            and ot.is_active = TRUE
            and (t.local_address = ot.address
                 or ot.address = 'local')
            and (
                  (ot.port isnull and t.local_port isnull)
                  or (ot.port = t.local_port)
                );

        
    if(cnt > 0) then
        return true;
    end if;


    return false;

end
$BODY$
language 'plpgsql' security definer;
