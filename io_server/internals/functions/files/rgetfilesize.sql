create or replace function rGetFileSize(int4) returns int8 as
$BODY$
declare
    idUrl alias for $1;
    url varchar;
    fSize int8;
begin

    url = rGetAbsUrl(idUrl);
    if(url is null) then
        return -1;
    end if;

    fSize = rGetFileSizeByUrl(url);

    return fSize;

end
$BODY$
language 'plpgsql';

