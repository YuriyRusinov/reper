create or replace function f_is_index_exist(varchar) returns bool as
$BODY$
declare
    indexName alias for $1;

    iCount int4;
begin

    select count(pgi.indexname)  into iCount
    from
        pg_indexes pgi
    where
        pgi.indexname = indexName;

    if(iCount >= 1) then
        return TRUE;
    else
        return FALSE;
    end if;

    return FALSE;
    
    exception when others then
        return FALSE;

end
$BODY$
language 'plpgsql';
