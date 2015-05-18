drop function if exists aaInsert(int4, int4, varchar, varchar, varchar, varchar, varchar, int4, varchar, boolean, boolean, varchar, varchar);
create or replace function aaInsert(int4,
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
                                    varchar,
                                    varchar,
                                    int4) returns int4 as
$BODY$
declare
    idAttrParent alias for $1;
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
    attrAttrUniqueId alias for $13;
    aaDirectives alias for $14;
    aaOrder alias for $15;

    idAttr int4;
    ok int4;
    aTableName varchar;
begin

    aTableName := ioGetTableNameByUid (aTableUID);
    select aInsert(idAttrType, aCode, aName, aTitle, aTableName, aColumn, aDefWidth, uniqueID) into idAttr;
    if(idAttr <= 0) then
        return -1;
    end if;

    select aInsertAttrAttr(idAttrParent, idAttr, aDefValue, isMandatory, isReadOnly, attrAttrUniqueId, aaDirectives, aaOrder) into ok;
    if(ok <= 0) then
        return -1;
    end if;

    return idAttr;
end
$BODY$
language 'plpgsql';
