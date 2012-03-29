create or replace function getUrlMimeType (int4) returns varchar as
$BODY$
declare
    idUrl alias for $1;

    idUrlType int4;
    url_extentions int4[];
    query varchar;
    i int4;
    n_ext int4;

    res varchar;
begin
    select into idUrlType id_url_type from io_urls where id=idUrl;
    if (idUrlType is null or idUrlType < 0) then
        raise warning 'Incorrect type';
        return NULL::int4;
    end if;
    select into url_extentions extentions from url_types where id=idUrlType;
    n_ext := array_upper (url_extentions, 1);
    query := 'select mt.name from mimetypes mt inner join url_extentions ue on (ue.id in (';
    for i in 1..n_ext
    loop
        query := query || url_extentions[i];
        if (i < n_ext) then
            query := query || ',';
        else
            query := query || ')';
        end if;
    end loop;
    query := query || 'and ue.name ilike (select src_ext from io_urls where id=' || idUrl || ') and mt.id=ue.id_mimetype);';
    execute query into res;
    if (res is null) then
        return 'application/octet-stream';
    else
        return res;
    end if;
end
$BODY$
language 'plpgsql';
