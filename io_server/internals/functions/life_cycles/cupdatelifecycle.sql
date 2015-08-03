create or replace function cUpdateLifeCycle(int4, varchar, varchar, int4, int4, int4) returns int4 as
$BODY$
declare
    idLifeCycle alias for $1;
    lcName alias for $2;
    lcDesc alias for $3;
    startState alias for $4;
    lcAutoStateAttr alias for $5;
    lcAutoStateInd alias for $6;
    idLC int4;
begin

    select id into idLC from life_cycle where id = idLifeCycle;
    if(idLC isnull) then
        return -1;
    end if;

    update life_cycle set name = lcName, description = lcDesc, id_start_state = startState, id_auto_state_attr = lcAutoStateAttr, id_auto_state_ind = lcAutoStateInd where id = idLifeCycle;
    return 1;

end
$BODY$
language 'plpgsql';
