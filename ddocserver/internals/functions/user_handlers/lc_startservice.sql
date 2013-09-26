create or replace function lc_startService(int4) returns int4 as
$BODY$
declare
    idChain alias for $1;
begin
    update chains_data set 
                            start_service_time = CURRENT_TIMESTAMP 
                            
    where chains_data.id=IdChain;

    return 1;

end
$BODY$ 
language 'plpgsql' security definer;
