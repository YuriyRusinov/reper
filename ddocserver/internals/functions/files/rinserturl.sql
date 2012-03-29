create or replace function rInsertUrl(varchar, varchar, int4, varchar) returns int4 as 
$BODY$
declare
    iName alias for $1;
    iUrl alias for $2;
    idType alias for $3;
    srcExt alias for $4;

    idUrl int4;
begin

    select pg_catalog.nextval('io_urls_id_seq') into idUrl;

    insert into io_urls (id, name, url, id_url_type, src_ext) values (idUrl, iName, iUrl, idType, srcExt);

    return idUrl;

end
$BODY$
language 'plpgsql' security definer;
