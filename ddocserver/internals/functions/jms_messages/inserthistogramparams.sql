create or replace function insertHistogramParams(int8, varchar, int4) returns int4 as
$BODY$
declare
    hId alias for $1;
    hValue alias for $2;
    type alias for $3;

    ok int4;
    tableName varchar;
    hParamsId int4;
    hParamsName varchar;
    hCount int4;
    q varchar;
    r record;
    tmp int4;
    hValArr text[]; 
begin

    ok = 1;

    if( hId <= 0 or hValue isnull or (type <> 0 and type <> 1 and type <> 2)) then
        return -1;
    end if;

    if(type = 0) then
        tableName = 'attrs_values';
    elsif(type = 1) then
        tableName = 'rec_attrs_values';
    else
        raise exception 'Incorrect type of histogram!';
        return -1;
    end if;

    q = '
        select a.table_name from attributes a, attrs_categories ac, ' || tableName || ' t 
        where 
            t.id = ' || hId || '
            and t.id_attr_category = ac.id 
            and ac.id_io_attribute = a.id ';
    
    execute q into tableName;

    if(tableName isnull) then
        raise exception 'Incorrect parameters for histogram! Cannot identify table for parameters!';
        return -1;
    end if;

    hValArr := string_to_array(hValue, '^~^~^');
    select hValArr[1] into hParamsId;
    if(hParamsId is null or hParamsId <= 0) then
        hParamsId := getnextseq (tableName, 'id');
        if (hParamsId is null) then
            raise exception 'Incorrect ID of record with histogram parameters!';
            return -1;
        end if;
        hValArr[1] := hParamsId;
    end if;

    hParamsName = 'histogram_params_' || hParamsId;
    select hValArr[4] into hCount;
    if(hCount is null or hCount <= 0) then
        raise exception 'Incorrect h_count of record with histogram parameters!';
        return -1;
    end if;
    

    q = 'select id from ' || tableName || ' where id = ' || hParamsId;
    execute q into tmp;
    if(tmp isnull) then
        if(tableName = 'histogram_params_streams') then
            q = 'insert into ' || tableName || ' (id, name, h_min, h_max, h_count, scenarios, variants, io_categories, io_objects, partition_lows) values (' 
                                                 || hParamsId || ', ' 
                                                 || quote_literal(hParamsName) || ', ' 
                                                 || 'NULL' || ', ' 
                                                 || 'NULL' || ', ' 
                                                 || hCount || ', ' 
                                                 || asString(parseHistogramParams(hValue, 'scenario'), true) || ', ' 
                                                 || asString(parseHistogramParams(hValue, 'variant'), true) || ', ' 
                                                 || asString(parseHistogramParams(hValue, 'categories'), true) || ', ' 
                                                 || asString(parseHistogramParams(hValue, 'IO'), true) || ', ' 
                                                 || asString(parseHistogramParams(hValue, 'part_lows'), true) || ')';
        else
            q = 'insert into ' || tableName || ' (id, name, h_min, h_max, h_count, scenarios, variants, io_categories, services) values (' 
                                                 || hParamsId || ', ' 
                                                 || quote_literal(hParamsName) || ', ' 
                                                 || 'NULL' || ', ' 
                                                 || 'NULL' || ', ' 
                                                 || hCount || ', ' 
                                                 || asString(parseHistogramParams(hValue, 'scenario'), true) || ', ' 
                                                 || asString(parseHistogramParams(hValue, 'variant'), true) || ', ' 
                                                 || asString(parseHistogramParams(hValue, 'categories'), true) || ', ' 
                                                 || asString(parseHistogramParams(hValue, 'services'), true) || ')';
        end if;
    else
        if(tableName = 'histogram_params_streams') then
            q = 'update ' || tableName || ' set h_count = ' || hCount || ', ' 
                                          || 'scenarios = ' || asString(parseHistogramParams(hValue, 'scenario'), true) || ', ' 
                                          || 'variants = ' || asString(parseHistogramParams(hValue, 'variant'), true) || ', ' 
                                          || 'io_categories = ' || asString(parseHistogramParams(hValue, 'categories'), true) || ', ' 
                                          || 'io_objects = ' || asString(parseHistogramParams(hValue, 'IO'), true) || ', ' 
                                          || 'partition_lows = ' || asString(parseHistogramParams(hValue, 'part_lows'), true) 
                                          || ' where id = ' || hParamsId;
        else
            q = 'update ' || tableName || ' set h_count = ' || hCount || ', ' 
                                          || 'scenarios = ' || asString(parseHistogramParams(hValue, 'scenario'), true) || ', ' 
                                          || 'variants = ' || asString(parseHistogramParams(hValue, 'variant'), true) || ', ' 
                                          || 'io_categories = ' || asString(parseHistogramParams(hValue, 'categories'), true) || ', ' 
                                          || 'services = ' || asString(parseHistogramParams(hValue, 'services'), true) 
                                          || ' where id = ' || hParamsId;
        end if;
    end if;

    execute q;

    return ok;
end
$BODY$
language 'plpgsql';
