create or replace function getExValuesId(int8, varchar, varchar, varchar) returns int8[] as
$BODY$
declare
    idRecord alias for $1;
    refTable alias for $2;
    mainColumn alias for $3;
    childColumn alias for $4;

    r record;
    q varchar;
    iValues int8[];
begin

    iValues := '{}'::int8[];

    q = 'select ' || childColumn || '::int8 as ids from ' || refTable || ' where ' || mainColumn || ' = ' || idRecord;

    for r in execute q
    loop
        iValues = iValues || r.ids;
    end loop;

    return iValues;

end
$BODY$
language 'plpgsql';


create or replace function getExValuesIdArray(int4, int8, int4) returns int8[] as
$BODY$
declare
    idObject alias for $1;
    idRecord alias for $2;
    idAttribute alias for $3;

    mainTable varchar;
    refTable varchar;
    mainAttr varchar;
    childAttr varchar;
    idChildCategory int4;

    iValues int8[];
begin

    iValues := NULL;--'{}'::int8[];

    select table_name into mainTable from tbl_io_objects where id = idObject;
    if(mainTable isnull) then
        return NULL;
    end if;

    select c.id_child into idChildCategory from io_categories c, tbl_io_objects io where io.id = idObject and io.id_io_category = c.id;
    if(idChildCategory isnull) then
        return NULL;
    end if;

    select a.table_name into refTable from attributes a, attrs_categories ac where ac.id_io_category = idChildCategory and ac.id_io_attribute = a.id and a.id = idAttribute; --attribute should be included in gived category!
    if(refTable isnull) then
        return NULL;
    end if;

    mainAttr := 'id_' || mainTable;
    childAttr := 'id_' || refTable;

    refTable := mainTable || '_' || refTable;
    if (refTable = 'organization_work_mode' or 
        refTable = 'table_notifies_io_objects' or
        refTable = 'position_work_mode' or 
        refTable = 'units_work_mode' or 
        refTable = 'io_objects_organization') 
    then                         
        refTable := refTable;
    else
        refTable := refTable || '_ref_' || idAttribute;
    end if;
    

    iValues = getExValuesId(idRecord, refTable, mainAttr, childAttr);

    if(array_upper(iValues, 1) = 0) then
        return NULL;
    end if;
    
    return iValues;

end
$BODY$
language 'plpgsql' security definer;


create or replace function getExValuesId(int4, int8, int4) returns setof int8 as
$BODY$
declare
    idObject alias for $1;
    idRecord alias for $2;
    idAttribute alias for $3;

    r record;
    iValues int8[];
begin

    iValues := NULL;--'{}'::int8[];

    iValues = getExValuesIdArray(idObject, idRecord, idAttribute);

    if(array_upper(iValues, 1) = 0) then
        return;
    end if;

    for r in select unnest(iValues)
    loop
        return next r.unnest;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql' security definer;
