CREATE or replace FUNCTION setMacToNULL ( varchar ) RETURNS int4 AS 
$BODY$
declare
    tableName alias for $1;
    query varchar;
begin
    query = 'alter table ' || tableName || ' set mac TO NULL';
    execute query;
    
    return 1;

    exception when OTHERS
              then
                    return 0;
end
$BODY$
LANGUAGE 'plpgsql';

