create or replace function getExValuesId(int4, varchar, varchar, varchar) returns int4[] as
$BODY$
declare
    idRecord alias for $1;
    refTable alias for $2;
    mainColumn alias for $3;
    childColumn alias for $4;

    r record;
    q varchar;
    iValues int4[];
begin

    iValues := '{}'::int4[];

    q = 'select ' || childColumn || ' as ids from ' || refTable || ' where ' || mainColumn || ' = ' || idRecord;

    for r in execute q
    loop
        iValues = iValues || r.ids;
    end loop;

    return iValues;

end
$BODY$
language 'plpgsql';
