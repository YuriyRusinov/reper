create or replace function updateIndicator(int4, int4, varchar, varchar) returns int4 as
$BODY$
declare
    idIndicator alias for $1;
    idIndType alias for $2;
    iName alias for $3;
    iDesc alias for $4;
begin
    
    update "indicator" 
    set 
        id_indicator_type = idIndType,
        name = iName,
        description = iDesc
    where 
        id = idIndicator;

    return 1;
end
$BODY$
language 'plpgsql';
