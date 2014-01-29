select f_safe_drop_type('h_get_v_info_chains');
create type h_get_v_info_chains as(id int4, name varchar);

create or replace function getVariantsInfoForChains() returns setof h_get_v_info_chains as
$BODY$
declare
    --idStreamParams alias for $1;
    r h_get_v_info_chains % rowtype;
begin

    for r in 
        select distinct v.id, v.name
        from 
            processing_variant v 
            inner join chains_data d on (v.id=d.id_processing_variant)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
