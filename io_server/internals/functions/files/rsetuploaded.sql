create or replace function rSetUploaded(int4, BOOL) returns int4 as
$BODY$
declare
    iid_file alias for $1;
    bUploaded alias for $2;
	
    r RECORD;
    tableName varchar;
    dyn_query varchar;
	
    idUrl int4;	
begin

    if(iid_file isnull or bUploaded isnull) then
        return 0;
    end if;

    select id into idUrl from io_urls where id = iid_file;
    if(idUrl isnull) then
        return -2;
    end if;
  
    update io_urls set uploaded = bUploaded where id = idUrl;
	
    return  1;
  
end
$BODY$
language 'plpgsql' security definer;
