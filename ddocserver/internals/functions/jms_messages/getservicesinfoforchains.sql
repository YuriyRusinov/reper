select f_safe_drop_type('h_get_serv_info_chains');
create type h_get_serv_info_chains as(id int4, name varchar);

create or replace function getServicesInfoForChains() returns setof h_get_serv_info_chains as
$BODY$
declare
    --idStreamParams alias for $1;
    r h_get_serv_info_chains % rowtype;
begin

    for r in 
        select distinct h.id, h.name
        from 
            handlers h 
            inner join chains c on (h.id = c.id_handler)
            inner join chains_data d on (c.id=d.id_chain)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
