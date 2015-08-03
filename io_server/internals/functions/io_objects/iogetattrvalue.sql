create or replace function ioGetAttrValue(int4, int8, int4) returns varchar as
$BODY$
declare
    idAttribute alias for $1;
    iValue alias for $2;
    idObject alias for $3;

    r record;
    q varchar;
    ret varchar;
    tableName varchar;
    isExist int4;
begin

    for r in 
        select table_name, column_name from attributes where id = idAttribute
    loop
        if(r.column_name isnull) then
            return NULL;
        end if;

        if(r.table_name isnull) then
            if(idObject isnull) then
                return NULL;
            end if;
            
            select table_name into tableName from f_sel_io_objects(idObject) where id = idObject;
            if(tableName isnull) then
                return NULL;
            end if;
        else
            tableName = r.table_name;
        end if;
        
        --Hack для выбора реального названия таблицы (tbl_ или не tbl_)
        select f_is_table_exist(tableName, NULL) into isExist; 
        if(isExist <> 1) then
            select f_is_table_exist('tbl_' || tableName, NULL) into isExist; 
            if(isExist = 1) then
                tableName := 'tbl_' || tableName;
            else
                raise exception 'Table % does not exist!', tableName;
                return NULL;
            end if;
        end if;


        q = 'select "' || r.column_name || '"::varchar as result from ' || tableName || ' where id = ' || iValue;
    end loop;

    if(q isnull) then
        return NULL;
    end if;
    
    for r in execute q
    loop
        ret = r.result;
    end loop;

    return ret;

end
$BODY$
language 'plpgsql' security definer;

create or replace function ioGetAttrValue(int4, int8[], int4) returns varchar as
$BODY$
declare
    idAttribute alias for $1;
    iValue alias for $2;
    idObject alias for $3;

    r record;
    q varchar;
    ret varchar;
    tableName varchar;
    isExist int4;
begin

    for r in 
        select table_name, column_name from attributes where id = idAttribute
    loop
        if(r.column_name isnull) then
            return NULL;
        end if;

        if(r.table_name isnull) then
            if(idObject isnull) then
                return NULL;
            end if;
            
            select table_name into tableName from f_sel_io_objects(idObject) where id = idObject;
            if(tableName isnull) then
                return NULL;
            end if;
        else
            tableName = r.table_name;
        end if;

        --Hack для выбора реального названия таблицы (tbl_ или не tbl_)
        select f_is_table_exist(tableName, NULL) into isExist; 
        if(isExist <> 1) then
            select f_is_table_exist('tbl_' || tableName, NULL) into isExist; 
            if(isExist = 1) then
                tableName := 'tbl_' || tableName;
            else
                raise exception 'Table % does not exist!', tableName;
                return NULL;
            end if;
        end if;
        
        q = 'select array_agg("' || r.column_name || '")::varchar as result from ' || tableName || ' where id = ANY(' || asString(iValue, true) || ')';
    end loop;

    if(q isnull) then
        return NULL;
    end if;
    
    for r in execute q
    loop
        ret = r.result;
    end loop;

    return ret;

end
$BODY$
language 'plpgsql' security definer;

create or replace function ioGetAttrValueEx(varchar, int4, varchar, int4, varchar) returns varchar as
$BODY$
declare
    theValue alias for $1;
    idType alias for $2;
    tName alias for $3;
    idObject alias for $4;
    columnName alias for $5;


    q varchar;
    valueArray varchar[];
    r record;
    val varchar;
    cnt int4;

    idAttrAttr int4;
    aValue varchar;
    tableName varchar;
    isExist int4;

begin

    if(idType <> 2 and 
       idType <> 3 and 
       idType <> 7 and 
       idType <> 12 and 
       idType <> 17 and 
       idType <> 19 and 
       idType <> 26 and 
       idType <> 32 and 
       idType <> 39) 
    then
        return theValue;
    end if;

    if(idType = 32) then --atComplex
        if(theValue isnull or theValue = '') then
            return NULL;
        end if;

        val = convertAttrAttrValue2(theValue); --from id to unique_id

        return val;
    end if;

    if(tName isnull) then
        if(idObject isnull) then
            return NULL;
        end if;
            
        select table_name into tableName from f_sel_io_objects(idObject) where id = idObject;
        if(tableName isnull) then
            return NULL;
        end if;
    else
        tableName = tName;
    end if;
        

    if(tableName isnull or columnName isnull or theValue isnull or idType isnull) then
        return NULL;
    end if;

    --Hack для выбора реального названия таблицы (tbl_ или не tbl_)
    select f_is_table_exist(tableName, NULL) into isExist; 
    if(isExist <> 1) then
        select f_is_table_exist('tbl_' || tableName, NULL) into isExist; 
        if(isExist = 1) then
            tableName := 'tbl_' || tableName;
        else
            raise exception 'Table % does not exist!', tableName;
            return NULL;
        end if;
    end if;


    if(idType <> 12 and idType <> 17) then -- РЅРµ РјР°СЃСЃРёРІС‹ Р·РЅР°С‡РµРЅРёР№
  
        q = 'select "' || columnName || '" as theVal from "' || tableName || '" where id = ' || theValue;
        for r in execute q
        loop
            val = r.theVal;
        end loop;

        return val;
    end if;

    if(theValue = '{}') then
        return theValue;
    end if;

    q := 'select ' || quote_literal(theValue) || '::varchar[] as a';

    for r in execute q
    loop
        valueArray = r.a;
    end loop;

    cnt := array_upper(valueArray, 1);

    --val = '{';
    --for i in 1..cnt
    --loop

        q = 'select array_agg("' || columnName || '") as theVal from ' || tableName || ' where id = ANY(' || quote_literal(valueArray) || ')';  
        
        for r in execute q
        loop
            val = r.theVal;
        end loop;

    --end loop;

    --val = val || '}';

    return val;
end
$BODY$
language 'plpgsql' security definer;
