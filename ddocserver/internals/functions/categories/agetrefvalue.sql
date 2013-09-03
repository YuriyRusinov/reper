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
    vals varchar[];
    cnt int8;
    i int8;
begin

    if (theValue is null) then
        return null;
    end if;
    select into idType a.id_a_type from attributes a where a.id=idAttribute;

    if(idType <> 2 and idType <> 3 and idType <> 7 and idType <> 12 and idType <> 17 and idType <> 19 and idType <> 26) then
        return theValue;
    end if;

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

    if (idType <> 12 and idType <> 17) then
        query := E'select "' || column_name || E'" from ' || table_name || E' where ' || table_name || E'.id=' || theValue;
        execute query into val;

        return val;
    else
        if (theValue = E'{}') then
            return theValue;
        end if;

        query := E'select "' || column_name || E'" from ' || table_name || E' where ' || table_name || E'.id in' || theValue || '::varchar[] as a';
        execute query into vals;
        cnt := array_upper (vals, 1);
        val := E'{';
        for i in 1..cnt
        loop
            val := val || vals[i];
            if (i < cnt-1 and char_length(val) > 1) then
                val := val || E',';
            end if;
        end loop;
        val := val || E'}';
        return val;
    end if;

    return null;
end
$BODY$
language 'plpgsql';
