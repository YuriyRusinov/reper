select f_safe_drop_type('h_get_orgs');
create type h_get_orgs as (id int4,
                           id_type_org int4,
                           id_parent int4,
                           id_parent1 int4,
                           id_substitutor int4,
                           id_curr_mode int4,
                           id_prev_mode int4,
                           id_boss int4,
                           org_name varchar,
                           org_short_name varchar,
                           org_code_name varchar,
                           org_email_prefix varchar,
                           org_longitude float8,
                           org_latitude float8,
                           org_mapsymbol varchar,
                           org_treesymbol varchar,
                           dt_curr_mode timestamp,
                           dt_prev_mode timestamp,
                           type_uid varchar,
                           type_name varchar,
                           type_short_name varchar,
                           curr_mode_name varchar,
                           prev_mode_name varchar,
                           id_transport int4,
                           transport_name varchar,
                           transport_addr varchar,
                           transport_uid varchar,
                           transport_active boolean,
                           transport_port int4,
                           transport_use_gateway bool
                           );

create or replace function getOrgs (int4) returns setof h_get_orgs as
$BODY$
declare
    idOrganization alias for $1;

    r h_get_orgs%rowtype;
    query varchar;
begin

    query := 'select 
                  o.id, 
                  o.id_type_org, 
                  o.id_parent, 
                  o.id_parent1, 
                  o.id_substitutor, 
                  o.id_curr_mode, 
                  o.id_prev_mode, 
                  o.id_boss, 
                  o.name as org_name, 
                  o.short_name as org_short_name, 
                  o.code_name as org_code_name, 
                  o.email_prefix as org_email_prefix, 
                  o.longitude as org_longitude, 
                  o.latitude as org_latitude, 
                  o.map_symbol, 
                  o.tree_symbol, 
                  o.dt_curr_mode, 
                  o.dt_prev_mode, 
                  ot.unique_id as type_uid, 
                  ot.name as type_name, 
                  ot.short_name as type_short_name, 
                  wmc.name as curr_mode_name, 
                  wmp.name as prev_mode_name, 
                  t.id as id_transport, 
                  t.name as transport_name, 
                  otr.address as transport_addr, 
                  t.unique_id as transport_uid, 
                  (otr.is_active and t.is_active) as transport_active,
                  otr.port as transport_port,
                  otr.use_gateway
              from organization o inner join organization_type ot on (o.id_type_org = ot.id';

    if (idOrganization is null) then
        query := query || ')';
    else
        query := query || ' and o.id=' || idOrganization || ')';
    end if;

    query := query || ' inner join work_mode as wmc on (o.id_curr_mode=wmc.id) inner join work_mode as wmp on (o.id_curr_mode=wmp.id) left join organization_transport otr on (o.id=otr.id_organization) left join transport t on (otr.id_transport=t.id) order by id';

    for r in
        execute query
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
