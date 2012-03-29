create or replace function getZaryaFromDl() returns int4 as
$BODY$
declare
    idDl int4;
    uidDl varchar;
begin
    --select tso_dl_from into uidDl from tso_params where id = 1;
    select "value" into uidDl from attrs_values where id_attr_category = acGetAttrCategoryId(175, 286) and id_io_object = 85;

    if(uidDl isnull) then
        return -1;
    end if;
    
    select id into idDl from "position" where unique_id = uidDl;

    return idDl;
end
$BODY$
language 'plpgsql';

create or replace function getZaryaExecutorDl() returns int4 as
$BODY$
declare
    idDl int4;
    uidDl varchar;
begin
    --select tso_dl_exec into uidDl from tso_params where id = 1;
    select "value" into uidDl from attrs_values where id_attr_category = acGetAttrCategoryId(175, 287) and id_io_object = 85; 
    if(uidDl isnull) then
        return -1;
    end if;
    
    select id into idDl from "position" where unique_id = uidDl;
    return idDl;
end
$BODY$
language 'plpgsql';

create or replace function getZaryaToDl() returns int4 as
$BODY$
declare
    uidDl varchar;
    idDl int4;
begin
    --select tso_dl_to into uidDl from tso_params where id = 1;
    select "value" into uidDl from attrs_values where id_attr_category = acGetAttrCategoryId(175, 288) and id_io_object = 85; 

    if(uidDl isnull) then
        return -1;
    end if;
    
    select id into idDl from "position" where unique_id = uidDl;
    return idDl;
end
$BODY$
language 'plpgsql';

create or replace function getZaryaCommandCategory() returns int4 as
$BODY$
declare
    uidCat varchar;
    idCat int4;
begin
    --select tso_category into uidCat from tso_params where id = 1;
    select "value" into uidCat from attrs_values where id_attr_category = acGetAttrCategoryId(175, 289) and id_io_object = 85;
    if(uidCat isnull) then
        return -1;
    end if;
    
    select id into idCat from io_categories where unique_id = uidCat;

    return idCat;
end
$BODY$
language 'plpgsql';

create or replace function getZaryaExecDatetime() returns timestamp as
$BODY$
declare
    execInterval varchar;
    q varchar;
    intArray int4[3];
    theTime timestamp;
    r record;
begin
    select "value" into execInterval from attrs_values where id_attr_category = acGetAttrCategoryId(175, 290) and id_io_object = 85;
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
