create or replace function getNextSeq(varchar, varchar) returns int8 as
$BODY$
declare
    tableName alias for $1;
    pkName alias for $2;

    id int8;
    seqName varchar;
    seqTableName varchar;
    isExist int4;
begin
    perform clearLastError();

    if (tableName = 'units' or
        tableName = 'position' or
        tableName = 'users'
       ) then
        --seqName := 'kks_roles' || '_' || pkName || '_seq';
        seqTableName := 'kks_roles'; -- || '_' || pkName || '_seq';
    else
        if(substr(tableName, 1, 4) = 'tbl_') then
            --seqName := tableName || '_' || pkName || '_seq';
            seqTableName := tableName;-- || '_' || pkName || '_seq';
        else

            select f_is_view_exist(tableName) into isExist; --≈сли представление с таким названием существует, то это означает, что реальна€ таблица имеет название с префиксом tbl_
            if(isExist = 1) then
                --seqName := 'tbl_' || tableName || '_' || pkName || '_seq';
                seqTableName := 'tbl_' || tableName;-- || '_' || pkName || '_seq';
            else
                --seqName := tableName || '_' || pkName || '_seq';                
                seqTableName := tableName;-- || '_' || pkName || '_seq';                
            end if;

        end if;
    end if;

    if(f_is_table_has_parent(seqTableName, 'q_base_table') = true) then
        seqTableName = 'q_base_table';
    end if;

    seqName = seqTableName || '_' || pkName || '_seq';
    select pg_catalog.nextval(seqName) into id;

    if(id isnull) then
        perform updateLastError('UNK0', 'Unknown error was occured');
        return -1;
    end if;
    
    return id;

--    exception when OTHERS
--
--              then 
--                   perform updateLastError(SQLSTATE, SQLERRM);
--                   return -1;
end
$BODY$
language 'plpgsql';
