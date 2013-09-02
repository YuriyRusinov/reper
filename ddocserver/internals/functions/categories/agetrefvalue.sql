create or replace function aGetRefValue (int8, varchar, varchar) returns varchar as
$BODY$
declare
    idAttribute alias for $1;
    tName alias for $2;
    theValue alias for $3;

    query varchar;
    idType int8;
    table_name varchar;
    column_name varchar;
    r record;
    val varchar;
begin

    if (theValue is null) then
        return null;
    end if;
    select into idType a.id_a_type from attributes a where a.id=idAttribute;

    if(idType <> 2 and idType <> 3 and idType <> 7 and idType <> 12 and idType <> 17 and idType <> 19 and idType <> 26) then
        return theValue;
    end if;

    if (idType = 2 or idType = 3 or idType = 19 or idType = 26) then
        for r in
            select a.table_name, a.column_name from attributes a where a.id=idAttribute
        loop
            table_name = r.table_name;
            column_name = r.column_name;
            if (idType = 3 and table_name isnull) then
                table_name := tName;
            end if;
            if (column_name isnull) then
                column_name := E'id';
            end if;
            raise warning '% %', table_name, column_name;
        end loop;
        query := E'select "' || column_name || E'" from ' || table_name || E' where ' || table_name || E'.id=' || theValue;
        execute query into val;

        return val;
    end if;

    return null;
end
$BODY$
language 'plpgsql';
