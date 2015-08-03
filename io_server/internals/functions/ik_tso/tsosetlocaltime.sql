CREATE OR REPLACE FUNCTION tsosetlocaltime(character varying)
  RETURNS timestamp without time zone AS
$BODY$
declare
    zone alias for $1;
    localTime timestamp;
    q varchar;
    r record;
begin

    if(zone isnull or trim(zone) = '') then
        return current_timestamp;
    end if;

    /*select substring(zone from 1 for 1) into c;
    if(c = '-') then
    end if
    */

    q = zone || ' hours';
    q = 'select current_timestamp + interval ' || asString(q, true) || ' as local_time' ;
    for r in execute q
    loop
        localTime = r.local_time;
    end loop;

    return localTime;

end
$BODY$
LANGUAGE 'plpgsql';
