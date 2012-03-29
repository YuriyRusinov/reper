select f_safe_drop_type('h_organization_desc');

create type h_organization_desc as(id_organization int4,
                                   org_name varchar,
                                   address varchar,
                                   the_uid varchar
                                  );

create or replace function getMyOrganization() returns setof h_organization_desc as
$BODY$
declare
    r h_organization_desc%rowtype;

    idDl int4;
    idUser int4;
begin

    idUser = getCurrentUser();
    if(idUser = -100) then --postgres!
        return;
    end if;


    idDl = getCurrentDl();
    --raise notice ' CURRENT_DL = %', idDl;

    if(idUser = 2 or idDl = 4) then --admin and jupiter
        -- in that case get first organization on local address
        for r in 
            select 
                o.id, 
                o.name, 
                ot.address, 
                o.email_prefix
            from
                organization o,
                organization_transport ot,
                transport t
            where
                o.id = ot.id_organization
                and ot.id_transport = t.id
                and t.local_address = ot.address
        loop
            return next r;
            return;
        end loop;

        return;
    end if;


    for r in 
        select 
            o.id, 
            o.name, 
            ot.address, 
            o.email_prefix
        from 
            organization o, 
            organization_transport ot,
            units u, 
            "position" p
        where
            p.id = idDl
            and p.id_unit = u.id
            and u.id_organization = o.id
            and o.id = ot.id_organization
    loop
        return next r;
        return;
    end loop;

    return;

end 
$BODY$
language 'plpgsql' security definer;
