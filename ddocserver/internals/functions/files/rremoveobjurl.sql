create or replace function rRemoveObjUrl(int4, int4, boolean) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idUrl alias for $2;
    bFile alias for $3;

    cnt int4;
begin

    select count(*) into cnt from urls_objects where id_io_object = idObject and id_url = idUrl;
    if(cnt = 0) then
        return 1;
    end if;

    delete from urls_objects where id_io_object = idObject and id_url = idUrl;

    if(cnt = 1 and bFile = true) then
        select rDeleteFile(idUrl) into cnt;
        return cnt;
    end if;

    return 1;

end

$BODY$
language 'plpgsql' security definer;

create or replace function rRemoveObjUrl(int4, boolean) returns int4 as
$BODY$
declare
    idObject alias for $1;
    bFile alias for $2;

    cnt int4;
    r RECORD;
begin

    if(bFile = true) then
        for r in 
            select id_url from urls_objects where id_io_object = idObject
        loop
            select count(*) into cnt from urls_objects where id_io_object = idObject and id_url = r.id_url;
            if(cnt <= 1) then
                select rDeleteFile(r.id_url) into cnt;
                if(cnt = 0) then
                    return 0;
                end if;
            end if;
        end loop;
    end if;

    delete from urls_objects where id_io_object = idObject;

    return 1;

end

$BODY$
language 'plpgsql' security definer;

create or replace function rRemoveObjUrl(int4, boolean, int4[]) returns int4 as
$BODY$
declare
    idObject alias for $1;
    bFile alias for $2;
    excludes alias for $3;

    cnt int4;
    r RECORD;
begin

    if(bFile = true) then
        for r in 
            select id_url 
            from urls_objects 
            where 
                id_io_object = idObject 
                and id_url not in (select id from io_urls where id = ANY(excludes))
        loop
            select count(*) into cnt from urls_objects where id_io_object = idObject and id_url = r.id_url;
            if(cnt <= 1) then
                select rDeleteFile(r.id_url) into cnt;
                if(cnt = 0) then
                    return 0;
                end if;
            end if;
        end loop;
    end if;

    delete from urls_objects 
    where 
        id_io_object = idObject 
        and id_url not in (select id from io_urls where id = ANY(excludes));

    return 1;

end

$BODY$
language 'plpgsql' security definer;
