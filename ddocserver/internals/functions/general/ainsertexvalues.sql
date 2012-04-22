create or replace function aInsertExValues(varchar, int8, int4[], varchar, varchar) returns int4 as
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
    v int4;  --value for insert;
    deleteQuery varchar;
    insertQuery varchar;
begin


    deleteQuery := ' delete from ' || tableName || ' where ' || mainAttr || ' = ' || id;
    execute deleteQuery;
    

    count := array_upper(valuesArray, 1);

    raise notice ' % ', count;
    
    if(count isnull or count = 0) then
        return 1;
    end if;

    
    insertQuery := ' ';
    for i in 1..count
    loop
        v := valuesArray[i];

        insertQuery := insertQuery || ' insert into ' || tableName || ' (' || mainAttr || ', ' || childAttr || ') values(' || id || ', ' || v || ');';
    end loop;

    raise notice '%s', insertQuery;

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

    iArray int4[];
    query varchar;
    r record;

    res int4;
begin

    query := 'select ' || quote_literal(valuesArray) || '::int4[] as a';
    for r in execute query 
    loop
        iArray = r.a;
    end loop;


    select aInsertExValues(tableName, id, iArray, mainAttr, childAttr) into res;

    return res;

end
$BODY$
language 'plpgsql';
