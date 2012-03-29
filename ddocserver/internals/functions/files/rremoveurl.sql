create or replace function rRemoveUrl(int4) returns int4 as 
$BODY$
declare
    id_Url alias for $1;
    idUrl int4;
    rmResult int4;
begin

    select id into idUrl from io_urls where id = id_Url;
    if(idUrl isnull) then
        return -1;
    end if;

    select rDeleteFile(idUrl) into rmResult;

    delete from io_urls where id = idUrl;
    
    return rmResult;

end
$BODY$
language 'plpgsql' security definer;
