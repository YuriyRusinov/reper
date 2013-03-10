create or replace function cInsertLifeCycle(varchar, varchar, int4) returns int4 as
$BODY$
declare
    lcName alias for $1;
    lcDesc alias for $2;
    lcStartState alias for $3;
    idLifeCycle int4;
begin

    select getNextSeq('life_cycle', 'id') into idLifeCycle;

    insert into life_cycle (id,
                            name,
                            description,
                            id_start_state)
                    values (idLifeCycle,
                            lcName,
                            lcDesc,
                            lcStartState);

    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return idLifeCycle;

end
$BODY$
language 'plpgsql';
