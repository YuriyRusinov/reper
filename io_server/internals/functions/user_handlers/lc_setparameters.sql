CREATE OR REPLACE FUNCTION lc_setparameters(integer, integer, integer)
  RETURNS integer AS
$BODY$
declare
    IdChain alias for $1;
    ReturnCode alias for $2;
    IsHandled alias for $3;
begin

    update chains_data set 
                            return_code = ReturnCode,
                            is_handled = IsHandled,
                            end_service_time = CURRENT_TIMESTAMP 
                            
    where chains_data.id=IdChain;

    return IdChain;
end
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION lc_setparameters(integer, integer, integer)
  OWNER TO yrusinov;
