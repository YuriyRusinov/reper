create or replace function f_safe_drop_trigger(varchar, varchar) returns int4 as
$BODY$
declare
    trgName alias for $1;
    tableName alias for $2;
    
    cnt int4;
    query varchar;
    isExist int4;
    rTableName varchar;
begin

    if(trgName isnull or tableName isnull) then
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
            raise exception 'Table % is corrupted! Cannot drop trigger on it!', tableName;
            return 0;
        end if;
    end if;


    select 
        count(c.relname) into cnt 
    from 
        pg_trigger t, 
	pg_class c 
    where 
        t.tgname = trgName 
	and t.tgrelid = c.oid
	and c.relname = rTableName;
	
    if(cnt > 0) then
        query := 'drop trigger ' || trgName || ' on ' || rTableName;
        execute query;
    end if;

    return 1;    

end
$BODY$
language 'plpgsql';

create or replace function f_safe_drop_trigger(varchar, varchar, varchar) returns int4 as
$BODY$
declare
    trgName alias for $1;
    tableName alias for $2;
    schemaName alias for $3;
    
    cnt int4;
    query varchar;
    isExist int4;
    rTableName varchar;
begin

    if(trgName isnull or tableName isnull) then
        return 0;
    end if;

    select f_is_table_exist(tableName, schemaName) into isExist; 
    if(isExist = 1) then
        rTableName := tableName;
    else
        select f_is_table_exist('tbl_' || tableName, schemaName) into isExist; 
        if(isExist = 1) then
            rTableName := 'tbl_' || tableName;
        else
            raise exception 'Table % is corrupted! Cannot drop trigger on it!', tableName;
            return 0;
        end if;
    end if;


    select 
        count(c.relname) into cnt 
    from 
        pg_trigger t, 
	pg_class c 
    where 
        t.tgname = trgName 
	and t.tgrelid = c.oid
	and c.relname = rTableName;
	
    if(cnt > 0) then
        query := 'drop trigger ' || trgName || ' on ' || rTableName;
        execute query;
    end if;

    return 1;    

end
$BODY$
language 'plpgsql';
