select f_safe_drop_type('h_rec_get_files');
create type h_rec_get_files as(id_record int8, 
                              id_url int4,
                              uo_name varchar, 
                              iu_name varchar, 
                              ut_id int4, 
                              ut_name varchar,
                              win_app varchar,
                              lin_app varchar,
                              src_ext varchar,
                              record_uid varchar);


create or replace function recGetFiles(int8) returns setof h_rec_get_files as
$BODY$
declare
    idRecord alias for $1;
    r h_rec_get_files%rowtype;
begin

    for r in
        select 
            uo.id_record, 
            uo.id_url, 
            uo.name, 
            iu.name,
            ut.id, 
            ut.name, 
            ut.win_app, 
            ut.lin_app,
            iu.src_ext,
            iu.unique_id
        from 
            urls_records uo,
            io_urls iu,
            url_types ut
        where 
            uo.id_record = idRecord
            and uo.id_url = iu.id
            and iu.id_url_type = ut.id
            and iu.uploaded = true
            and ut.id <> 11 --system files for embedded GIS (not included in IO files list!!!)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function recGetFilesByUUID(varchar) returns setof h_rec_get_files as
$BODY$
declare
    uuidRecord alias for $1;
    r h_rec_get_files%rowtype;
    idRecord int8;
begin
    select id into idRecord from q_base_table where uuid_t::varchar = uuidRecord;
    if(idRecord isnull) then
        return;
    end if;

    for r in
        select 
            uo.id_record, 
            uo.id_url, 
            uo.name, 
            iu.name,
            ut.id, 
            ut.name, 
            ut.win_app, 
            ut.lin_app,
            iu.src_ext,
            iu.unique_id
        from 
            urls_records uo,
            io_urls iu,
            url_types ut
        where 
            uo.id_record = idRecord
            and uo.id_url = iu.id
            and iu.id_url_type = ut.id
            and iu.uploaded = true
            and ut.id <> 11 --system files for embedded GIS (not included in IO files list!!!)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function recGetFilesByUUID(uuid) returns setof h_rec_get_files as
$BODY$
declare
    uuidRecord alias for $1;
    r h_rec_get_files%rowtype;
    idRecord int8;
begin
    select id into idRecord from q_base_table where uuid_t = uuidRecord;
    if(idRecord isnull) then
        return;
    end if;

    for r in
        select 
            uo.id_record, 
            uo.id_url, 
            uo.name, 
            iu.name,
            ut.id, 
            ut.name, 
            ut.win_app, 
            ut.lin_app,
            iu.src_ext,
            iu.unique_id
        from 
            urls_records uo,
            io_urls iu,
            url_types ut
        where 
            uo.id_record = idRecord
            and uo.id_url = iu.id
            and iu.id_url_type = ut.id
            and iu.uploaded = true
            and ut.id <> 11 --system files for embedded GIS (not included in IO files list!!!)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
