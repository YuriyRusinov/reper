create or replace function existWithUID(varchar, varchar) returns bool as
$BODY$
declare
    theUID alias for $1;
    theTable alias for $2;

    query varchar;
    r record;
begin

    if(theUID isnull or theTable isnull) then
        return false;
    end if;

    query = 'select unique_id from ' || theTable || ' where unique_id = ' || quote_literal(theUID);
    for r in execute query
    loop
        if(r.unique_id is not null) then
            return true;
        end if;
    end loop;

    return false;

end
$BODY$
language 'plpgsql';
