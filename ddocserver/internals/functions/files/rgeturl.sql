create or replace function rGetUrl(int4) returns varchar as
$BODY$
declare
    idUrl alias for $1;

    theUrl varchar;     
begin
    if(idUrl isnull) then
        return NULL;
    end if;

    select url into theUrl from io_urls where id = idUrl;
  
    if(theUrl = 'not assigned' or theUrl isnull) then
        return NULL;
    end if;
	
    return theUrl;
end
$BODY$
language 'plpgsql';
