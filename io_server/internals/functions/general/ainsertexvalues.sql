create or replace function aInsertExValues(varchar, int8, int8[], varchar, varchar) returns int4 as
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
    v int8;  --value for insert;
    deleteQuery varchar;
    insertQuery varchar;
begin

    insertQuery = 'insert into ' || tableName || ' (' || mainAttr || ', ' || childAttr || ')
                   select ' || id || ', t.' || childAttr || '
                   from
                       (select ' || id || ' as ' || mainAttr || ', unnest(' || asString(valuesArray, true) || '::int8[]) as ' || childAttr || ' ) as t';

    execute insertQuery;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function aInsertExValues(varchar, int8, varchar, varchar, varchar) returns int4 as
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


    select aInsertExValues(tableName, id, iArray, mainAttr, childAttr) into res;

    return res;

end
$BODY$
language 'plpgsql';

/*
create or replace function aInsertExValues(varchar, int8, varchar[], varchar, varchar, bool) returns int4 as
$BODY$
declare
    tableName alias for $1;
    id alias for $2;
    valuesArray alias for $3;
    mainAttr alias for $4;
    childAttr alias for $5;
    bImported alias for $6;

    iArray int8[];
    query varchar;
    r record;

    res int4;
begin

    if(bImported = false) then
        query := 'select ' || quote_literal(valuesArray) || '::int8[] as a';
        for r in execute query 
        loop
            iArray = r.a;
        end loop;

        select aInsertExValues(tableName, id, iArray, mainAttr, childAttr) into res;

        return res;
    end if;

    

    return res;

end
$BODY$
language 'plpgsql';

*/

