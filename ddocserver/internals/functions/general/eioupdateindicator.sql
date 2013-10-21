create or replace function eioUpdateIndicator(int8, int4, varchar, timestamp, timestamp, int4, int4, varchar) returns int8 as
$BODY$
declare
    idRec alias for $1;
    idCategoryAttr alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;

    idRecAttrValue int8;
begin


    idRecAttrValue = eioCatIndExist(idRec, idCategoryAttr);
    if(idRecAttrValue is not null and ioValue isnull) then
        delete from rec_attrs_values where id_record = idRec and id_attr_category = idCategoryAttr;
        return 0;
    end if;

    if(idRecAttrValue is not null) then
        update rec_attrs_values set 
            value = ioValue, 
            id_io_object_src = iIdObjectSrc, 
            id_io_object_src1 = iIdObjectSrc1, 
            description = iDesc 
        where 
            id = idRecAttrValue;
    else
        select eioInsertIndicator(idRec, idCategoryAttr, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
        return idRecAttrValue;
    end if;

    return idRecAttrValue;
end
$BODY$
language 'plpgsql';

create or replace function eioCatIndExist(int8, int4) returns int8 as
$BODY$
declare
    idRec alias for $1;
    idCategoryAttr alias for $2;
    idRecAttrValue int8;
begin
    select id into idRecAttrValue from rec_attrs_values where id_record = idRec and id_attr_category = idCategoryAttr limit 1;
    if(idRecAttrValue isnull) then
        return NULL;
    end if;

    return idRecAttrValue;
end
$BODY$
language 'plpgsql' security definer;

--используется при информационном обмене
create or replace function eioUpdateIndicatorEx(int8, varchar, varchar, timestamp, timestamp, int4, int4, varchar) returns int8 as
$BODY$
declare
    idRecord alias for $1;
    uniqueId alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;

    idAttr int4;
    idAttrCategory int8;
    idType int4;
    tableName varchar;
    theValue varchar;
    uniqueArray varchar[];
    val varchar;
    r record;
    query varchar;
    cnt int4;
    i int4;
    start_time timestamp;
    idRecAttrValue int8;
begin

    if (iStartTime is null and iStopTime is null) then
        raise warning 'All time stamps are null';
        start_time := current_timestamp;
    else
        start_time := iStartTime;
    end if;

    if(idObject isnull or uniqueId isnull or ioValue isnull) then
        return 1;
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

    idAttrCategory = acGetAttrCategoryIdByRec2(idRecord, idAttr);
    if(idAttrCategory is null or idAttrCategory <= 0) then
        raise warning 'Attribute does not present in child2 category! idRecord = %, attr unique_id = %', idRecord, uniqueId;
        return -1;
    end if;


    if(idType <> 2 and idType <> 3 and idType <> 12 and idType <> 17 and idType <> 19 and idType <> 26)  then
        raise warning 'type is %', idType;
        select eioUpdateIndicator(idRecord, idAttrCategory, ioValue, start_time, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
        return idRecAttrValue;
    end if;

    if(ioValue = '{}')  then
        
        select eioUpdateIndicator(idRecord, idAttrCategory, ioValue, start_time, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
        return idRecAttrValue;
    end if;

    if(idType <> 12 and idType <> 17) then --наборы элементов справочника
        theValue = getIDByUID(tableName, ioValue);

        if(theValue is not null) then
            select eioUpdateIndicator(idRecord, idAttrCategory, theValue, start_time, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
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

    select eioUpdateIndicator(idRecord, idAttrCategory, theValue, start_time, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
    
    return idRecAttrValue;

end
$BODY$
language 'plpgsql';
