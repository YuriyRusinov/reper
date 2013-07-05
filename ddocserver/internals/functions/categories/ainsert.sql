create or replace function aInsert(int4,
                                   varchar,
                                   varchar,
                                   varchar,
                                   varchar,
                                   varchar,
                                   int4,
                                   varchar) returns int4 as
$BODY$
declare
    idAttrType alias for $1;
    aCode alias for $2;
    aName alias for $3;
    aTitle alias for $4;
    aTable alias for $5;
    aColumn alias for $6;
    aDefWidth alias for $7;
    uniqueID alias for $8;

    idAttr int4;
    ok int4;
    existCode varchar;
    defWidth int4;
begin

    select aFindEqual(idAttrType, aName, aTitle, aTable, aColumn, aDefWidth) into idAttr;
    if(idAttr is not null and idAttr > 0) then
        return idAttr;
    end if;

    select getNextSeq('attributes', 'id') into idAttr;
    
/*
    if(aCode is not null) then
        select code into existCode from attributes where code = aCode;
        if(existCode is not null) then
            existCode = NULL;
        else 
            existCode = aCode;
        end if;
    else
        existCode = aCode;
    end if;
*/
    --existCode = uniqueID; --may be NULL

    if(aDefWidth isnull) then
        defWidth = 150;
    else
        defWidth = aDefWidth;
    end if;

    insert into attributes (id, id_a_type, code, name, title, table_name, column_name, def_width, is_system, unique_id)
    values(idAttr, idAttrType, /*existCode*/aCode, aName, aTitle, aTable, aColumn, defWidth, false, uniqueID);

    if(FOUND = FALSE) then
        return -1;
    end if;

    return idAttr;
end
$BODY$
language 'plpgsql';

/*Данная функция используется при "погружении" существующей структуры БД в среду DynamicDocs (см. ф-ю translateTablesToSchema)*/
create or replace function aInsertEx(int4,
                                     varchar,
                                     varchar,
                                     varchar,
                                     varchar,
                                     varchar,
                                     int4,
                                     varchar,
                                     varchar) returns int4 as
$BODY$
declare
    idAttrType alias for $1;
    aCode alias for $2;
    aName alias for $3;
    aTitle alias for $4;
    aTable alias for $5;
    aColumn alias for $6;
    aDefWidth alias for $7;
    uniqueID alias for $8;
    refColumn alias for $9;

    idAttr int4;
    ok int4;
    existCode varchar;
    defWidth int4;
begin

    select aFindEqualEx(idAttrType, aCode, aName, aTitle, aTable, aColumn, aDefWidth) into idAttr;
    if(idAttr is not null and idAttr > 0) then
        return idAttr;
    end if;

    select getNextSeq('attributes', 'id') into idAttr;

    if(aDefWidth isnull) then
        defWidth = 150;
    else
        defWidth = aDefWidth;
    end if;

    --Здесь важным отличием является то, что задается значение для ref_column_name. И оно в большинстве случаев не будет id
    insert into attributes (id, id_a_type, code, name, title, table_name, column_name, def_width, is_system, unique_id, ref_column_name) 
    values(idAttr, idAttrType, aCode, aName, aTitle, aTable, aColumn, defWidth, false, uniqueID, refColumn);

    if(FOUND = FALSE) then
        return -1;
    end if;

    return idAttr;
end
$BODY$
language 'plpgsql';
