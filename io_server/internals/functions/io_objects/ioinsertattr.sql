create or replace function ioInsertAttr(int4, int4, varchar, timestamp, timestamp, int4, int4, varchar) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idAttr alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;
   

    idCategory int4;
    idCatAttr int4;

    idAttrValue int4;
begin

    select id_io_category into idCategory from f_sel_io_objects(idObject) where id = idObject;
    if(idCategory isnull) then
        raise warning 'Object does not exist!';
        return -1;
    end if;

    if(ioValue isnull) then
        return 0;
    end if;

    idCatAttr = acGetAttrCategoryId(idCategory, idAttr);

    select getNextSeq('attrs_values', 'id') into idAttrValue;

    insert into attrs_values(id, id_io_object, id_attr_category, value, start_time, stop_time, id_io_object_src, id_io_object_src1, description)
    values (idAttrValue, idObject, idCatAttr, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc);
--    if(FOUND = FALSE) then
--        return -1;
--    end if;

    return idAttrValue;

end
$BODY$
language 'plpgsql';

create or replace function ioInsertAttr(int4, varchar, varchar, timestamp, timestamp, int4, int4, varchar) returns int4 as
$BODY$
declare
    idObject alias for $1;
    uniqueId alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;

    idAttr int4;
    ok int4;
begin

    if(session_user = 'jupiter') then
        return ioInsertAttrEx(idObject, uniqueId, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc);
    end if;

    return -1;
end
$BODY$
language 'plpgsql';

--используется при информационном обмене
create or replace function ioInsertAttrEx(int4, varchar, varchar, timestamp, timestamp, int4, int4, varchar) returns int4 as
$BODY$
declare
    idObject alias for $1;
    uniqueId alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;

    idAttr int4;
    idType int4;
    ok int4;
    tableName varchar;
    theValue varchar;
    uniqueArray varchar[];
    val varchar;
    r record;
    query varchar;
    cnt int4;
    i int4;
begin

    if(idObject isnull or uniqueId isnull or ioValue isnull) then
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

    if(idType <> 2 and  --atList
       idType <> 3 and  --atParent
       idType <> 12 and --old checkboxes
       idType <> 17 and --new checkboxes
       idType <> 19 and --ref to color (fill)
       idType <> 26 and    --ref to color (text)
       idType <> 39) --ref to category
    then
        
        select ioInsertAttr(idObject, idAttr, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into ok;
        return ok;
    end if;

    if(ioValue = '{}')  then
        
        select ioInsertAttr(idObject, idAttr, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into ok;
        return ok;
    end if;

    if(idType <> 12 and idType <> 17) then --наборы элементов справочника
        theValue = getIDByUID(tableName, ioValue);

        if(theValue is not null) then
            select ioInsertAttr(idObject, idAttr, theValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into ok;
        end if;

        return ok;
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

    select ioInsertAttr(idObject, idAttr, theValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into ok;
    
    return ok;

end
$BODY$
language 'plpgsql';

