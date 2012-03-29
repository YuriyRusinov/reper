create or replace function matchAttrValue(int4, varchar, varchar, varchar) returns boolean as
$BODY$
declare
    idAttr alias for $1;
    attrValueId alias for $2;
    matchingValue alias for $3;
    oper alias for $4;

    isLike boolean;
    query varchar;
    tableName varchar;
    columnName varchar;
    isArray boolean;
    idAttrType int4;
    attrTypeCode varchar;
    refAttrTypeCode varchar;
    idArr int4[];
    i int4;
    r RECORD;
    isValInt boolean;
    isValDouble boolean;
begin

    --RAISE NOTICE 'matchattrvalue function';
    isLike := (oper = 'like');
    for r in 
        select a.table_name, a.column_name, a.id_attr_type, a.attr_type_code, a.ref_attr_type_code from aGetAttribute (idAttr) as a
    loop
        tableName := r.table_name;
        columnName := r.column_name;
        idAttrType := r.id_attr_type;
        attrTypeCode := r.attr_type_code;
        refAttrTypeCode := r.ref_attr_type_code;

        raise notice 'matchattrvalue attribute type is % column_name is %', r.id_attr_type, columnName;
        isArray := (r.id_attr_type = 12);
        if(tableName isnull or columnName isnull) then
            return false;
        end if;
    end loop;

    if (not FOUND) then
        return false;
    end if;

    isValInt := is_int4 (attrValueId);
    if (not isValInt and not isArray) then
        return false;
    end if;
    raise notice 'attrValueId is % refAttrTypeCode is %', attrValueId, refAttrTypeCode;
    --if (isValInt and (refAttrTypeCode <> 'INT' or refAttrTypeCode <> 'FLOAT8')) then
    --    return false;
    --end if;
    query := 'select count(*) as cnt from ' || tableName || ' where id ';
    if (isArray) then
        query := query || ' in (';
        RAISE NOTICE E'query is %\n attr value id is %', query, attrValueId;
        select cast (string_to_array (trim (attrValueId, '{}'), ',') as int4[]) into idArr;
        for i in 1..array_upper (idArr, 1)
        loop
            query := query || idArr[i];
            if (i < array_upper (idArr, 1)) then
                query := query || ',';
            end if;
        end loop;
        query := query || ' )';
    else
        query := query || ' = ' || attrValueId ||'::int4';
    end if;
    RAISE NOTICE '%', query;

    query := query || ' and ' || asString(columnName, false);
    isValDouble := is_float8 (matchingValue);
    RAISE NOTICE '%', isValDouble;
    isValInt := is_int4 (matchingValue);
    RAISE NOTICE '%', isValInt;
    if ((attrTypeCode = 'FLOAT8' and not isValDouble) or (attrTypeCode = 'INT4' and not isValInt and not isValDouble) or (refAttrTypeCode  = 'FLOAT8' and not isValDouble) or (refAttrTypeCode = 'INT4' and not isValInt and not isValDouble) or (isValDouble and refAttrTypeCode <> 'FLOAT8' or refAttrTypeCode <> 'INT4' )) then
        return false;
    end if;

    RAISE NOTICE '%', attrTypeCode;
    if (isLike) then
        query := query || ' like (' || quote_literal(matchingValue) || ')';
    else
        query := query || oper;
        if (attrTypeCode = 'FLOAT8' ) then
            RAISE NOTICE 'float8';
            query := query || float8 (matchingValue);
        elsif ((attrTypeCode = 'INT4' or attrTypeCode = 'INT4[]') and is_int4 (matchingValue)) then
            RAISE NOTICE 'int4 int4';
            query := query || int4 (matchingValue);
        elsif ((attrTypeCode = 'INT4'or attrTypeCode = 'INT4[]' ) and is_float8 (matchingValue)) then
            RAISE NOTICE 'int4 float8';
            query := query || float8 (matchingValue);
        elsif (is_float8 (matchingValue)) then
            RAISE NOTICE 'not numeric float8';
            return false;
        else
            RAISE NOTICE 'others';
            query := query || quote_literal(matchingValue);
        end if;
    end if;

    if(query isnull) then
        return false;
    end if;

    RAISE NOTICE 'matchattrvalue parameters are % % % %', idAttr, attrValueId, matchingValue, oper;
    RAISE NOTICE 'matchattrvalue result query is %', query;
    
    for r in execute query
    loop
        if(r.cnt > 0) then
            return true;
        end if;
    end loop;

    return false;
end
$BODY$
language 'plpgsql';
