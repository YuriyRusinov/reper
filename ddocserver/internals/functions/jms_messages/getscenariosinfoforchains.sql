select f_safe_drop_type('h_get_sc_info_chains');
create type h_get_sc_info_chains as(id int4, name varchar);

create or replace function getScenariosInfoForChains() returns setof h_get_sc_info_chains as
$BODY$
declare
    --idStreamParams alias for $1;
    r h_get_sc_info_chains % rowtype;
begin

    for r in 
        select distinct sc.id, sc.name
        from 
            processing_scenario sc 
            inner join chains_data d on (sc.id=d.id_processing_scenario)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
