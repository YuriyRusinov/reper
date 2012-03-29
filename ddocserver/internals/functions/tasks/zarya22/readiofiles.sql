create or replace function readIOFiles (int4) returns varchar as
$BODY$
declare
    idObject alias for $1;

    query varchar;
    str_res varchar;
    cnt int4;
    r record;
    fdata bytea;
    url varchar;
    fdata_enc text;
    i int4;
    ii int8;
    blockdata bytea;
    limit_file_size bigint;
    fileSize int8;
begin
    select count(*) into cnt from urls_objects where id_io_object=idObject;
    limit_file_size := 2*1024;
    limit_file_size := limit_file_size*1024;
    limit_file_size := limit_file_size*1024;
    if (cnt is not null and cnt > 0) then
        str_res :=  E'\t\t<attached_files_set where_files_amnt="' || cnt || E'">\n';
        i := 1;
        for r in 
            select id_url from urls_objects uo where uo.id_io_object=idObject
        loop
            str_res := str_res || E'\t\t\t<attached_file number="' || i || E'">\n';
            str_res := str_res || E'\t\t\t\t<![CDATA[\n';
            select into url rGetAbsUrl(r.id_url);
            select into fileSize rGetFileSize(r.id_url);
            if (fileSize > limit_file_size) then
                raise warning 'Too big file, size is %', fileSize;
                return null;
            end if;
            raise warning 'absolute url is %', url;
            select into fdata rGetFileByUrl(url, 102400, 0); --1024*1024*1024);
            ii := 1;
            blockdata := fdata;
            loop
                select into blockdata rGetFileByUrl(url, 102400, 102400*ii);
                if (blockdata is null or length (blockdata) = 0) then
                    exit;
                end if;
                fdata := fdata || blockdata;
                ii := ii+1;
                --raise warning '%', blockdata;
            end loop;

            --raise warning 'row data is %', fdata;
            --select into fdata encode (fdata, 'escape');
            select into fdata_enc encode (fdata, 'base64');
            raise warning 'encoding data is %', fdata_enc;
            if (length (fdata_enc) > limit_file_size) then
                return NULL;
            end if;
            str_res := str_res || fdata_enc;
            str_res := str_res || E'\t\t\t\t]]>\n';
            str_res := str_res || E'\t\t\t</attached_file>\n';
            i := i+1;
        end loop;
    else
        str_res :=  E'\t\t<attached_files_set where_files_amnt="0">\n';
    end if;

    str_res := str_res || E'\t\t</attached_files_set>\n';
    return str_res;
end
$BODY$
language 'plpgsql';
