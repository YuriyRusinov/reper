create or replace function rUpdateUrl(int4, varchar, int4, varchar) returns int4 as 
$BODY$
declare
    idUrl alias for $1;
    iName alias for $2;
    idType alias for $3;
    srcExt alias for $4;

begin

    update io_urls set name = iName, id_url_type = idType, src_ext = srcExt where id = idUrl;
    update urls_objects set name = iName where id_url = idUrl;
 
    return idUrl;

end
$BODY$
language 'plpgsql' security definer;
