create or replace function cUpdateState(int4, varchar, varchar) returns int4 as
$BODY$
declare
    idState alias for $1;
    sName alias for $2;
    sDesc alias for $3;
    idS int4;
begin

    select  id into idS from io_states where id = idState;
    if(idS is null) then
        return -1;
    end if; 

    update io_states set name = sName, description = sDesc where id = idState;

    return 1;

end
$BODY$
language 'plpgsql';
