select f_safe_drop_type('h_get_histogram_params_chains');
create type h_get_histogram_params_chains as (id int4, 
                                              h_min float8, 
                                              h_max float8, 
                                              h_count int4, 
                                              scenarios int4[], 
                                              variants int4[],
                                              io_categories int4[],
                                              services int4[]);

create or replace function getHistogramParamsChains(int4) returns setof h_get_histogram_params_chains as
$BODY$
declare
    idParams alias for $1;

    r h_get_histogram_params_chains%rowtype;
begin
    for r in select 
                  id,
                  h_min, 
                  h_max, 
                  h_count, 
                  scenarios, 
                  variants,
                  io_categories,
                  services
              from
                  histogram_params_chains
              where 
                  id = idParams
    loop
        return next r;
    end loop;

    return; 
end
$BODY$
language 'plpgsql';
