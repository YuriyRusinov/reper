drop function if exists createHistogramFilterQueryService(int4);
create or replace function createHistogramFilterQueryService(int4[], int4[], int4[], int4[]) returns varchar as
$BODY$
declare
    scenarios alias for $1;
    variants alias for $2;
    io_categories alias for $3;
    services alias for $4;
    
    --r record;
    q varchar;
    ioObjects int4[];
    --ioCategories int4[];
begin


    q = 'select cd.id as id, cd.id_chain as id_chain, getSeconds(cd.end_service_time - cd.start_service_time) as time_step
         from 
             chains_data cd,
             chains c,
             tbl_io_objects io
         where
             cd.start_service_time is not null
             and cd.end_service_time is not null
             and c.id = cd.id_chain
             and io.id = cd.id_io_object ';

    if(scenarios is not null and array_upper(scenarios, 1) > 0) then
        q = q || '
             and cd.id_processing_scenario = ANY(' || asString(scenarios, true) || ') ';
    end if;

    if(variants is not null and array_upper(variants, 1) > 0) then
        q = q || '
             and cd.id_processing_variant = ANY(' || asString(variants, true) || ') ';
    end if;

    if(io_categories is not null and array_upper(io_categories, 1) > 0) then 
        q = q || '
             and io.id_io_category = ANY(' || asString(io_categories, true) || ') ';
    end if;

    if(services is not null and array_upper(services, 1) > 0) then 
        q = q || '
             and c.id_handler = ANY(' || asString(services, true) || ') ';
    end if;
        
    return q;

end
$BODY$
LANGUAGE 'plpgsql';
