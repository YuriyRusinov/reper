create or replace function getOrgByAddr (varchar) returns setof h_get_orgs as
$BODY$
declare
    address alias for $1;

    r h_get_orgs%rowtype;
    query varchar;
begin

    if (address is null) then
        raise notice 'Organization address cannot be null';
        return;
    end if;

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
                  otr.port as transport_port
              from organization o inner join organization_type ot on (o.id_type_org = ot.id )';

    query := query || ' inner join work_mode as wmc on (o.id_curr_mode=wmc.id) inner join work_mode as wmp on (o.id_curr_mode=wmp.id) inner join organization_transport otr on (o.id=otr.id_organization) inner join transport t on (otr.id_transport=t.id and otr.address=' || quote_literal (address) ||' )';

    for r in
        execute query
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
