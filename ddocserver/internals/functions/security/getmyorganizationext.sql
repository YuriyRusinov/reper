select f_safe_drop_type('h_organization_desc_ext');

create type h_organization_desc_ext as(id_organization int4,
                           org_name varchar,
                           address varchar,
                           the_uid varchar,
                           map_symbol varchar,
                           tree_symbol varchar,
                           mode_name varchar
                           );

create or replace function getMyOrganizationExt() returns setof h_organization_desc_ext as
$BODY$
declare
    r h_organization_desc_ext%rowtype;

    idUser int4;
begin

    idUser = getCurrentUser();

    if(idUser = 2 or getCurrentDl() = 4) then --admin and jupiter
        -- in that case get first organization on local address
        for r in 
            select 
                o.id, 
                o.name, 
                ot.address, 
                o.email_prefix, 
                o.map_symbol, 
                o.tree_symbol, 
                wm.name
            from
                organization o inner join
                organization_transport ot on (o.id = ot.id_organization) inner join
                transport t on (ot.id_transport = t.id and t.local_address = ot.address) inner join
                work_mode wm on (wm.id = o.id_curr_mode)
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
            o.email_prefix, 
            o.map_symbol, 
            o.tree_symbol, 
            wm.name
        from 
            organization o inner join  
            organization_transport ot on (o.id = ot.id_organization) inner join
            units u on (u.id_organization = o.id) inner join
            "position" p on (p.id = getCurrentDl() and p.id_unit = u.id) inner join
            work_mode wm on (wm.id = o.id_curr_mode)
    loop
        return next r;
        return;
    end loop;

    return;

end 
$BODY$
language 'plpgsql' security definer;
