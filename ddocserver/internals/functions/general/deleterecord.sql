create or replace function deleterecord (int4, varchar) returns int4 as
$BODY$
declare
    idrec alias for $1;
    tableName alias for $2;

    query varchar;
begin

    if (idrec is null or idrec < 0 or tableName is null) then
        raise notice 'invalid parameters % %', idrec, tableName;
        return -1;
    end if;

    query := 'delete from ' || tableName || ' where id=' || idrec;
    execute query;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function deleterecord (varchar, varchar, varchar, varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    columnName alias for $2;
    oper alias for $3;
    value alias for $4;

    query varchar;
begin

    if (tableName is null or columnName is null) then
        raise notice 'invalid parameters';
        return -1;
    end if;

    query := 'delete from ' || tableName || ' where ' || columnName;
    raise notice '%', query;

    if (value is null) then
        query := query || ' is null';
    else
        query := query || ' ' || oper || quote_literal (value);
    end if;

    raise notice '%', query;
    execute query;
    return 1;
end
$BODY$
language 'plpgsql';
