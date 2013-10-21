create or replace function eioInsertIndicator(int8, int4, varchar, timestamp, timestamp, int4, int4, varchar) returns int8 as
$BODY$
declare
    idRec alias for $1;
    idCategoryAttr alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
--    iMeasTime alias for $6;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;
   

    idCategory int4;
    idCatAttr int4;
    idRecAttrValue int8;
begin

    if(ioValue isnull) then
        return 0;
    end if;

    select getNextSeq('rec_attrs_values', 'id') into idRecAttrValue;

    insert into rec_attrs_values(id, id_record, id_attr_category, value, start_time, stop_time, id_io_object_src, id_io_object_src1, description)
    values (idRecAttrValue, idRec, idCategoryAttr, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc);
--    if(FOUND = FALSE) then
--        return -1;
--    end if;

    return idRecAttrValue;

end
$BODY$
language 'plpgsql';


--used in information exchange
create or replace function eioInsertIndicatorEx(int8, varchar, varchar, timestamp, timestamp, int4, int4, varchar) returns int8 as
$BODY$
declare
    idRecord alias for $1;
    uniqueId alias for $2; --attribute unique id
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;

    idAttr int4;
    idAttrCategory int4;
    idType int4;
    idRecAttrValue int8;
    tableName varchar;
    theValue varchar;
    uniqueArray varchar[];
    val varchar;
    r record;
    query varchar;
    cnt int4;
    i int4;
begin

    if(idRecord isnull or uniqueId isnull or ioValue isnull) then
        return 0;
    end if;
    
    for r in select id, id_a_type, table_name from attributes where unique_id = uniqueId
    loop
        idAttr = r.id;
        idType = r.id_a_type;
        tableName = r.table_name;
    end loop;

    if(idAttr isnull) then
        raise warning 'Attribute does not exist! unique_id = %', uniqueId;
        return -1;
    end if;

    idAttrCategory = acGetAttrCategoryIdByRec2(idRecord, idAttr);-- get category from id_child2 field
    if(idAttrCategory is null or idAttrCategory <= 0) then
        raise warning 'Attribute does not present in child2 category! idRecord = %, attr unique_id = %', idRecord, uniqueId;
        return -1;
    end if;

    if(idType <> 2 and  --atList
       idType <> 3 and  --atParent
       idType <> 12 and --old checkboxes
       idType <> 17 and --new checkboxes
       idType <> 19 and --ref to color (fill)
       idType <> 26)    --ref to color (text)
    then
        
        select eioInsertIndicator(idRecord, idAttrCategory, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
        return idRecAttrValue;
    end if;

    if(ioValue = '{}')  then
        
        select eioInsertIndicator(idRecord, idAttrCategory, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
        return idRecAttrValue;
    end if;

    if(idType <> 12 and idType <> 17) then --checkboxes
        theValue = getIDByUID(tableName, ioValue);

        if(theValue is not null) then
            select eioInsertIndicator(idRecord, idAttrCategory, theValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
        end if;

        return idRecAttrValue;
    end if;    

    query := 'select ' || quote_literal(ioValue) || '::varchar[] as a';
    for r in execute query 
    loop
        uniqueArray = r.a;
    end loop;

    cnt := array_upper(uniqueArray, 1);

    theValue = '{';
    for i in 1..cnt
    loop
        val = getIDByUID(tableName, uniqueArray[i]);
        if(val is not null) then
            if(char_length(theValue) > 1) then
                theValue = theValue || ',';
            end if;

            theValue = theValue || val;
        end if;
    end loop;

    theValue = theValue || '}';

    select eioInsertIndicator(idRecord, idAttrCategory, theValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
    
    return idRecAttrValue;

end
$BODY$
language 'plpgsql';
