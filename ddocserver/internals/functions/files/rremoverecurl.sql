create or replace function rRemoveRecUrl(int8, int4, boolean) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    idUrl alias for $2;
    bFile alias for $3;

    cnt int4;
begin

    select count(*) into cnt from urls_records where id_record = idRecord and id_url = idUrl;
    if(cnt = 0) then
        return 1;
    end if;

    delete from urls_records where id_record = idRecord and id_url = idUrl;

    if(cnt = 1 and bFile = true) then
        select rDeleteFile(idUrl) into cnt;
        return cnt;
    end if;

    return 1;

end

$BODY$
language 'plpgsql' security definer;

create or replace function rRemoveRecUrl(int8, boolean) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    bFile alias for $2;

    cnt int4;
    r RECORD;
begin

    if(bFile = true) then
        for r in 
            select id_url from urls_records where id_records = idRecord
        loop
            select count(*) into cnt from urls_records where id_record = idRecord and id_url = r.id_url;
            if(cnt <= 1) then
                select rDeleteFile(r.id_url) into cnt;
                if(cnt = 0) then
                    return 0;
                end if;
            end if;
        end loop;
    end if;

    delete from urls_records where id_record = idRecord;

    return 1;

end

$BODY$
language 'plpgsql' security definer;

create or replace function rRemoveGISRecUrl(int8, boolean) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    bFile alias for $2;

    cnt int4;
    r RECORD;
begin

    if(bFile = true) then
        for r in 
            select ur.id_url 
            from urls_records ur, io_urls iu
            where 
                ur.id_record = idRecord
                and ur.id_url = iu.id
                and iu.id_url_type = 11
        loop
            select count(*) into cnt from urls_records where id_records = idRecord and id_url = r.id_url;
            if(cnt <= 1) then
                select rDeleteFile(r.id_url) into cnt;
                if(cnt = 0) then
                    return 0;
                end if;
            end if;
        end loop;
    end if;

    delete from urls_records
    where 
        id_record = idRecord
        and id_url in (select id from io_urls where id_url_type = 11);

    return 1;

end

$BODY$
language 'plpgsql' security definer;


--remove all but GIS files
create or replace function rRemoveRecUrl(int8, boolean, int4[]) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    bFile alias for $2;
    excludes alias for $3;

    cnt int4;
    r RECORD;
begin

    if(bFile = true) then
        for r in 
            select id_url 
            from urls_records 
            where 
                id_record = idRecord 
                and id_url not in (select id from io_urls where id_url_type = 11 or id = ANY(excludes))
        loop
            select count(*) into cnt from urls_records where id_record = idRecord and id_url = r.id_url;
            if(cnt <= 1) then
                select rDeleteFile(r.id_url) into cnt;
                if(cnt = 0) then
                    return 0;
                end if;
            end if;
        end loop;
    end if;

    delete from urls_records 
    where 
        id_record = idRecord 
        and id_url not in (select id from io_urls where  id_url_type = 11 or id = ANY(excludes));

    return 1;

end
$BODY$
language 'plpgsql' security definer;
