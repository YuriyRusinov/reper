create or replace function getTSOMaxLevel() returns int4 as
$BODY$
declare
    maxLevel int4;
begin
    --select to_int4("value") into maxLevel from attrs_values where id_io_category = 151 and id_io_object = 74 and id_io_attribute = 210;
    --select tso_max_level into maxLevel from tso_params where id = 1;

    return maxLevel;
end
$BODY$
language 'plpgsql';

create or replace function getTSOFromDl() returns int4 as
$BODY$
declare
    idDl int4;
    uidDl varchar;
begin
    --select tso_dl_from into uidDl from tso_params where id = 1;
    --select "value" into uidDl from attrs_values where id_io_category = 151 and id_io_object = 74 and id_io_attribute = 211;

    if(uidDl isnull) then
        return -1;
    end if;
    
    select id into idDl from "position" where unique_id = uidDl;

    return idDl;
end
$BODY$
language 'plpgsql';

create or replace function getTSOExecutorDl() returns int4 as
$BODY$
declare
    idDl int4;
    uidDl varchar;
begin
    --select tso_dl_exec into uidDl from tso_params where id = 1;
    --select "value" into uidDl from attrs_values where id_io_category = 151 and id_io_object = 74 and id_io_attribute = 212;
    if(uidDl isnull) then
        return -1;
    end if;
    
    select id into idDl from "position" where unique_id = uidDl;
    return idDl;
end
$BODY$
language 'plpgsql';

create or replace function getTSOToDl() returns int4 as
$BODY$
declare
    uidDl varchar;
    idDl int4;
begin
    --select tso_dl_to into uidDl from tso_params where id = 1;
    --select "value" into uidDl from attrs_values where id_io_category = 151 and id_io_object = 74 and id_io_attribute = 213;

    if(uidDl isnull) then
        return -1;
    end if;
    
    select id into idDl from "position" where unique_id = uidDl;
    return idDl;
end
$BODY$
language 'plpgsql';

create or replace function getTSOCommandCategory() returns int4 as
$BODY$
declare
    uidCat varchar;
    idCat int4;
begin
    --select tso_category into uidCat from tso_params where id = 1;
    --select "value" into uidCat from attrs_values where id_io_category = 151 and id_io_object = 74 and id_io_attribute = 214;
    if(uidCat isnull) then
        return -1;
    end if;
    
    select id into idCat from io_categories where unique_id = uidCat;

    return idCat;
end
$BODY$
language 'plpgsql';

create or replace function getTSOExecDatetime() returns timestamp as
$BODY$
declare
    execInterval varchar;
    q varchar;
    intArray int4[3];
    theTime timestamp;
    r record;
begin
    --select "value" into execInterval from attrs_values where id_io_category = 151 and id_io_object = 74 and id_io_attribute = 215;
    if(execInterval isnull) then
        return NULL;
    end if;

    q = 'select ' || quote_literal(execInterval) || ' :: int4[] as a';
    --raise warning '%', q;
    
    for r in execute q
    loop
        intArray = r.a;
    end loop;

    q = 'select current_timestamp + interval ' || asString(intArray[1] || ' hour', true) || 
                                ' + interval ' || asString(intArray[2] || ' minute', true) ||
                                ' + interval ' || asString(intArray[2] || ' second', true) || ' as a';

    for r in execute q
    loop
        theTime = r.a;
    end loop;

    return theTime;
end
$BODY$
language 'plpgsql';
