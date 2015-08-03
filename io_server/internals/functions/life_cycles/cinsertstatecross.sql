create or replace function cInsertStateCross(varchar, int4, int4, int4) returns int4 as
$BODY$
declare
    scName alias for $1;
    scLifeCycle alias for $2;
    scSrcState alias for $3;
    scEndState alias for $4;

    idStateCross int4;
begin

    select getNextSeq('state_crosses', 'id') into idStateCross;

    insert into state_crosses (id,
                               name,
                               id_life_cycle,  
                               id_state_src,
                               id_state_dest)
                    values (idStateCross,
                            scName,
                            scLifeCycle, 
                            scSrcState,
                            scEndState);

    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return idStateCross;

end
$BODY$
language 'plpgsql';
