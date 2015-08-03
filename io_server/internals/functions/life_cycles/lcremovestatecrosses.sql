create or replace function lcRemoveStateCrosses(int4, int4[]) returns int4 as
$BODY$
declare
    idLifeCycle alias for $1;
    excludes alias for $2;

    cnt int4;
    r RECORD;
begin

    if(excludes isnull) then
        delete from state_crosses where id_life_cycle = idLifeCycle;
        return 1;
    end if;

    delete from state_crosses 
    where 
        id_life_cycle = idLifeCycle 
        and id not in (select id from state_crosses where id = ANY(excludes));

    return 1;

end
$BODY$
language 'plpgsql';
