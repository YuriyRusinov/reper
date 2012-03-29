select f_safe_drop_type('h_get_org_wm');
create type h_get_org_wm as (wm_id int4,
                             wm_name varchar,
                             wm_description varchar,
                             wm_symbol varchar,
                             wm_uid varchar,
                             wmtype_id int4,
                             wmtype_name varchar,
                             wmtype_short_name varchar,
                             wmtype_uid varchar
                            );

create or replace function getWorkMode (int4) returns setof h_get_org_wm as
$BODY$
declare
    idWorkMode alias for $1;

    r h_get_org_wm%rowtype;
    query varchar;
begin

    query := 'select wm.id as wm_id, wm.name as wm_name, wm.description as wm_desc, wm.symbol as wm_symbol, wm.unique_id as wm_uid, wmt.id as wmt_id, wmt.name as wmt_name, wmt.short_name as wmt_short_name, wmt.unique_id as wmt_uid from work_mode wm inner join work_mode_type wmt on (wm.id_mode_type = wmt.id ';
    if (idWorkMode is not null) then
        query := query || 'and wm.id=' || idWorkMode || ')';
    else
        query := query || ')';
    end if;
    for r in
        execute query
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function getOrgsWorkModes (int4) returns setof h_get_org_wm as
$BODY$
declare
    idOrganization alias for $1;

    r h_get_org_wm%rowtype;
    query varchar;
begin

    if (idOrganization is null) then
        raise warning 'Organization cannot be null';
        return;
    end if;
    query := 'select wm.id as wm_id, wm.name as wm_name, wm.description as wm_desc, wm.symbol as wm_symbol, wm.unique_id as wm_uid, wmt.id as wmt_id, wmt.name as wmt_name, wmt.short_name as wmt_short_name, wmt.unique_id as wmt_uid from work_mode wm inner join work_mode_type wmt on (wm.id_mode_type = wmt.id) inner join organization_work_mode owm on (owm.id_work_mode=wm.id and owm.id_organization=' || idOrganization || ') order by wm_id';
    for r in
        execute query
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
