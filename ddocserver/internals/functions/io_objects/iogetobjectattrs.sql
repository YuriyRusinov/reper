select f_safe_drop_type('h_get_object_attrs');
create type h_get_object_attrs as(
                                  id_io_object int4,
                                  id_io_category int4,
                                  id_io_attribute int4,
                                  value varchar,
                                  attr_code varchar,  --при информационном обмене (в ф-и ioGetObjectAttrsEx() в качестве значения данного поля используется unique_id)
                                  id_attr_type int4,
                                  id_attr_category int4,
                                  id_attr_value int4, 
                                  start_time timestamp,
                                  stop_time timestamp,
                                  insert_time timestamp,
                                  id_io_object_src int4,
                                  id_io_object_src1 int4,
                                  is_actual boolean,
                                  description varchar,
                                  attr_name varchar,
                                  attr_order int4);

create or replace function ioGetObjectAttrs(int4, bool, timestamp, timestamp) returns setof h_get_object_attrs as
$BODY$
declare
    idObject alias for $1;
    isActual alias for $2;
    startTime alias for $3;
    stopTime alias for $4;

    r h_get_object_attrs%rowtype;

    iStartTime timestamp;
    iStopTime timestamp;
    iIsActual boolean;
begin

    if(isActual isnull) then
        iIsActual := true;
    else
        iIsActual := isActual;
    end if;

    if(startTime isnull) then
        iStartTime := '-infinity'::timestamp;
    else 
        iStartTime := startTime;
    end if;

    if(stopTime isnull) then
        iStopTime := 'infinity'::timestamp;
    else 
        iStopTime := stopTime;
    end if;

    for r in 
        select 
            av.id_io_object, 
            ac.id_io_category, 
            ac.id_io_attribute, 
            (case when session_user = 'jupiter' then getUniqueValue(av.value, a.id_a_type, a.table_name) else av.value end ) as value,
            a.code,
            a.id_a_type,
            av.id_attr_category,
            av.id,
            av.start_time,
            av.stop_time,
            av.insert_time,
            av.id_io_object_src,
            av.id_io_object_src1,
            av.is_actual,
            av.description,
            a.name,
            ac.order
        from 
            (f_sel_attrs_values(idObject) av inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and av.id_io_object = idObject))
        where 
            case when isActual = true then av.is_actual = true else (av.start_time >= iStartTime and av.stop_time <= iStopTime) end
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

--используется в информационном обмене
create or replace function ioGetObjectAttrsEx(int4) returns setof h_get_object_attrs as
$BODY$
declare
    idObject alias for $1;
    r h_get_object_attrs%rowtype;
begin

    for r in 
        select 
            av.id_io_object, 
            ac.id_io_category, 
            ac.id_io_attribute,--!!! 
            getUniqueValue(av.value, a.id_a_type, a.table_name),
            a.unique_id,
            a.id_a_type,
            av.id_attr_category,
            av.id,
            av.start_time,
            av.stop_time,
            av.insert_time,
            av.id_io_object_src,
            av.id_io_object_src1,
            av.is_actual,
            av.description,
            a.name,
            ac.order
        from 
            (f_sel_attrs_values(idObject) av inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and av.id_io_object = idObject))
        where 
            av.is_actual = true 
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function getUniqueValue(varchar, int4, varchar) returns varchar as
$BODY$
declare
    theValue alias for $1;
    idType alias for $2;
    tableName alias for $3;

    q varchar;
    uniqueField varchar;
    uniqueArray varchar[];
    r record;
    val varchar;
    cnt int4;

    idAttrAttr int4;
    aValue varchar;

begin

    if(idType <> 2 and idType <> 3 and idType <> 7 and idType <> 12 and idType <> 17 and idType <> 19 and idType <> 26 and idType <> 32) then
        return theValue;
    end if;

    if(idType = 32) then --atComplex
        if(theValue isnull or theValue = '') then
            return NULL;
        end if;

        val = convertAttrAttrValue1(theValue); --from id to unique_id

        return val;
    end if;

    if(tableName isnull or theValue isnull or idType isnull) then
        return NULL;
    end if;


    if(tableName = 'organization') then
        uniqueField = 'email_prefix';
    else
        uniqueField = 'unique_id';
    end if;

    if(idType <> 12 and idType <> 17) then -- не массивы значений
  
        q = 'select ' || uniqueField || ' as theVal from ' || tableName || ' where id = ' || theValue;
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
        uniqueArray = r.a;
    end loop;

    cnt := array_upper(uniqueArray, 1);

    val = '{';
    for i in 1..cnt
    loop

        q = 'select ' || uniqueField || ' as theVal from ' || tableName || ' where id = ' || uniqueArray[i];  

        for r in execute q
        loop
            if(char_length(val) > 1) then
                val = val || ',';
            end if;

            val = val || r.theVal;
        end loop;

    end loop;

    val = val || '}';

    return val;
end
$BODY$
language 'plpgsql';


create or replace function ioGetObjectAttrsAll(int4) returns setof h_get_object_attrs as
$BODY$
declare
    r h_get_object_attrs%rowtype;
    idObject alias for $1;
begin

for r in 
    select 
        io.id,
        ac.id_io_category,
        ac.id_io_attribute,
        NULL as a_value,
        a.code,
        a.id_a_type,
        ac.id,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        a.name,
        ac.order
    from
        f_sel_io_objects(idObject) io,
        attrs_categories ac,
        attributes a
    where 
        io.id_io_category = ac.id_io_category
        and ac.id_io_attribute = a.id
        and io.id = idObject
        and a.id not in(select ac.id_io_attribute from attrs_categories ac, f_sel_attrs_values(idObject) av where av.id_io_object = io.id and av.id_attr_category = ac.id)

    union

    select 
        io.id,
        ac.id_io_category,
        ac.id_io_attribute,
        av.value as a_value,
        a.code,
        a.id_a_type,
        av.id_attr_category,
        av.id,
        av.start_time,
        av.stop_time,
        av.insert_time,
        av.id_io_object_src,
        av.id_io_object_src1,
        av.is_actual,
        av.description,
        a.name,
        ac.order
    from
        f_sel_io_objects(idObject) io,
        attributes a,
        attrs_categories ac,
        f_sel_attrs_values(idObject) av
    where 
        io.id_io_category = ac.id_io_category
        and ac.id_io_attribute = a.id
        and io.id = idObject
        and av.id_io_object = io.id
        and ac.id = av.id_attr_category
        and is_actual = true
loop
    return next r;
end loop;

return;
end
$BODY$
language 'plpgsql';
