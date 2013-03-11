create or replace function aUpdateExValues(varchar, int8, int8[], varchar, varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    id alias for $2;
    valuesArray alias for $3;
    mainAttr alias for $4;
    childAttr alias for $5;
    
    r record;
    i int4;
    count int4;
    v int8;  --value for update;
    deleteQuery varchar;
    insertQuery varchar;
begin

    deleteQuery = 'delete from ' || tableName || ' where ' || mainAttr || ' = ' || id || 
                  ' and ' || childAttr || ' <> ALL(' || asString(valuesArray, true) || '::int8[])';


    execute deleteQuery;
    
    insertQuery = 'insert into ' || tableName || ' (' || mainAttr || ', ' || childAttr || ')
                   select ' || id || ', t.' || childAttr || '
                   from
                       (select ' || id || ' as ' || mainAttr || ', unnest(' || asString(valuesArray, true) || '::int8[]) as ' || childAttr || ' ) as t
                        where t.' || childAttr || ' not in (select ' || childAttr || ' from ' || tableName || ' where ' || mainAttr || ' = ' || id || ')';

    execute insertQuery;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function aUpdateExValues(varchar, int8, varchar, varchar, varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    id alias for $2;
    valuesArray alias for $3;
    mainAttr alias for $4;
    childAttr alias for $5;

    iArray int8[];
    query varchar;
    r record;

    res int4;
begin

    query := 'select ' || quote_literal(valuesArray) || '::int8[] as a';
    for r in execute query 
    loop
        iArray = r.a;
    end loop;


    select aUpdateExValues(tableName, id, iArray, mainAttr, childAttr) into res;

    return res;

end
$BODY$
language 'plpgsql';
