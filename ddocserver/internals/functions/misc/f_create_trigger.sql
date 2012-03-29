create or replace function f_create_trigger(varchar, varchar, varchar, varchar, varchar) returns int4 as
$BODY$
declare
    trgName alias for $1;
    trgWhen alias for $2; --before after
    trgActions alias for $3; --insert or update or delete
    tableName alias for $4;
    trgProcName alias for $5;

    query varchar;
    rTableName varchar;
    isExist int4;
begin
    if(trgName isnull or trgWhen isnull or trgActions isnull or tableName isnull or trgProcName isnull) then
        return 0;
    end if;

    select f_is_table_exist(tableName, NULL) into isExist; 
    if(isExist = 1) then
        rTableName := tableName;
    else
        select f_is_table_exist('tbl_' || tableName, NULL) into isExist; 
        if(isExist = 1) then
            rTableName := 'tbl_' || tableName;
        else
            raise exception 'Table % is corrupted! Cannot create trigger on it!', tableName;
            return 0;
        end if;
    end if;


    query = 'create trigger ' || trgName || ' ' || trgWhen || ' ' || trgActions || ' on ' || rTableName || ' for each row execute procedure ' || trgProcName;
    execute query;
    
    return 1;

end
$BODY$
language 'plpgsql';
