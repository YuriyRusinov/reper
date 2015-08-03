create or replace function cInsertState(varchar, varchar) returns int4 as
$BODY$
declare
    sName alias for $1;
    sDesc alias for $2;

    idState int4;
begin

    select getNextSeq('io_states', 'id') into idState;

    insert into io_states (id,
                           name,
                           description,
                           is_system)
                   values (idState,
                           sName,
                           sDesc,
                           false);

    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return idState;

end
$BODY$
language 'plpgsql';
