create or replace function acInsert(int4,
                                   int4,
                                   varchar,
                                   varchar,
                                   varchar,
                                   varchar,
                                   varchar,
                                   int4,
                                   varchar,
                                   boolean,
                                   boolean,
                                   varchar) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    idAttrType alias for $2;
    aCode alias for $3;
    aName alias for $4;
    aTitle alias for $5;
    aTableUID alias for $6;
    aColumn alias for $7;
    aDefWidth alias for $8;
    aDefValue alias for $9;
    isMandatory alias for $10;
    isReadOnly alias for $11;
    uniqueID alias for $12;

    idAttr int4;
    ok int4;
    aTableName varchar;
begin

    aTableName := ioGetTableNameByUid (aTableUID);
    select aInsert(idAttrType, aCode, aName, aTitle, aTableName, aColumn, aDefWidth, uniqueID) into idAttr;
    if(idAttr <= 0) then
        return -1;
    end if;

    select cAddAttr(idCategory, idAttr, aDefValue, isMandatory, isReadOnly) into ok;
    if(ok <= 0) then
        return -1;
    end if;

    return idAttr;
end
$BODY$
language 'plpgsql';

-- используется при информационном обмене. Основное отличие от предыдущей функции - 
-- она вызывает aInsertEx, которая осуществляет поиск такого же атрибута, но
-- с таким же уникальным идентификатором. 
create or replace function acInsertEx(int4,
                                      int4,
                                      varchar,
                                      varchar,
                                      varchar,
                                      varchar,
                                      varchar,
                                      int4,
                                      varchar,
                                      boolean,
                                      boolean,
                                      varchar) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    idAttrType alias for $2;
    aCode alias for $3;
    aName alias for $4;
    aTitle alias for $5;
    aTableUID alias for $6;
    aColumn alias for $7;
    aDefWidth alias for $8;
    aDefValue alias for $9;
    isMandatory alias for $10;
    isReadOnly alias for $11;
    uniqueID alias for $12;

    idAttr int4;
    ok int4;
    aTableName varchar;
begin

    aTableName := ioGetTableNameByUid (aTableUID);
    select aInsertEx(idAttrType, aCode, aName, aTitle, aTableName, aColumn, aDefWidth, uniqueID) into idAttr;
    if(idAttr <= 0) then
        return -1;
    end if;

    select cAddAttr(idCategory, idAttr, aDefValue, isMandatory, isReadOnly) into ok;
    if(ok <= 0) then
        return -2;
    end if;

    return idAttr;
end
$BODY$
language 'plpgsql';

/*
Данная функция используется при "погружении" существующей структуры БД в среду DynamicDocs (см. ф-ю translateTablesToSchema)
*/
create or replace function acInsert1(int4,
                                   int4,
                                   varchar,
                                   varchar,
                                   varchar,
                                   varchar,
                                   varchar,
                                   int4,
                                   varchar,
                                   boolean,
                                   boolean,
                                   varchar,
                                   varchar) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    idAttrType alias for $2;
    aCode alias for $3;
    aName alias for $4;
    aTitle alias for $5;
    aTableName alias for $6;
    aColumn alias for $7;
    aDefWidth alias for $8;
    aDefValue alias for $9;
    isMandatory alias for $10;
    isReadOnly alias for $11;
    uniqueID alias for $12;
    refColumn alias for $13;

    idAttr int4;
    ok int4;
begin

    select aInsert1(idAttrType, aCode, aName, aTitle, aTableName, aColumn, aDefWidth, uniqueID, refColumn) into idAttr;
    if(idAttr <= 0) then
        return -1;
    end if;

    select cAddAttr(idCategory, idAttr, aDefValue, isMandatory, isReadOnly) into ok;
    if(ok <= 0) then
        return -2;
    end if;

    return idAttr;
end
$BODY$
language 'plpgsql';
