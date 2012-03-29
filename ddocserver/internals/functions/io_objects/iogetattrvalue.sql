create or replace function ioGetAttrValue(int4, int4, int4) returns varchar as
$BODY$
declare
    idAttribute alias for $1;
    iValue alias for $2;
    idObject alias for $3;

    r record;
    q varchar;
    ret varchar;
    tableName varchar;
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
language 'plpgsql';
