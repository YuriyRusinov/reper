create or replace function rGetAbsUrl(int4) returns varchar as
$BODY$
declare
    idUrl alias for $1;
	
    url varchar;     
    raPath varchar;
    rPath varchar;
begin
	
    select rGetUrl(idUrl) into url;
    if(url isnull) then
        return NULL;
    end if;
	
    select raGetPath() into raPath;
	
    rPath := raPath || url;
	
    return rPath;
end
$BODY$
language 'plpgsql';
