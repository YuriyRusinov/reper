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
        delete from io_urls where id = idUrl;
        return cnt;
    end if;

    return 1;

end

$BODY$
language 'plpgsql' security definer;

--remove with GIS files!!!
--should be used when delete IO from database
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
            select uo.id_url 
            from urls_objects uo, io_urls iu
            where 
                uo.id_io_object = idObject
                and uo.id_url = iu.id
--                and iu.id_url_type <> 11
        loop
            select count(*) into cnt from urls_objects where id_io_object = idObject and id_url = r.id_url;
            if(cnt <= 1) then
                select rDeleteFile(r.id_url) into cnt;
                if(cnt = 0) then
                    return 0;
                end if;
                delete from urls_objects where id_io_object = idObject and id_url = r.id_url;
                delete from io_urls where id = r.id_url;
            end if;
        end loop;

        return 1;
    end if;

    --in other case just remove relation between IO and files (NOT GIS files!!!)
    delete from urls_objects 
    where 
        id_io_object = idObject;
--        and id_url in (select id from io_urls where id_url_type <> 11);

    return 1;

end

$BODY$
language 'plpgsql' security definer;

create or replace function rRemoveGISObjUrl(int4, boolean) returns int4 as
$BODY$
declare
    idObject alias for $1;
    bFile alias for $2;

    cnt int4;
    r RECORD;
begin

    if(bFile = true) then
        for r in 
            select uo.id_url 
            from urls_objects uo, io_urls iu
            where 
                uo.id_io_object = idObject
                and uo.id_url = iu.id
                and iu.id_url_type = 11
        loop
            select count(*) into cnt from urls_objects where id_io_object = idObject and id_url = r.id_url;
            if(cnt <= 1) then
                select rDeleteFile(r.id_url) into cnt;
                if(cnt = 0) then
                    return 0;
                end if;
                delete from urls_objects where id_io_object = idObject and id_url = r.id_url;
                delete from io_urls where id = r.id_url;
            end if;
        end loop;

        return 1;
    end if;

    --in other case just remove relation between IO and GIS files
    delete from urls_objects 
    where 
        id_io_object = idObject
        and id_url in (select id from io_urls where id_url_type = 11);

    return 1;

end

$BODY$
language 'plpgsql' security definer;

--remove all but GIS files
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
                and id_url not in (select id from io_urls where id_url_type = 11 or id = ANY(excludes))
        loop
            select count(*) into cnt from urls_objects where id_io_object = idObject and id_url = r.id_url;
            if(cnt <= 1) then
                select rDeleteFile(r.id_url) into cnt;
                if(cnt = 0) then
                    return 0;
                end if;
                delete from urls_objects where id_io_object = idObject and id_url = r.id_url;
                delete from io_urls where id = r.id_url;
            end if;
        end loop;

        return 1;
    end if;

    delete from urls_objects 
    where 
        id_io_object = idObject 
        and id_url not in (select id from io_urls where id_url_type = 11 or id = ANY(excludes));

    return 1;

end
$BODY$
language 'plpgsql' security definer;
