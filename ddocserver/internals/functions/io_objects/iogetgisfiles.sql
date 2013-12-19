select f_safe_drop_type('h_io_get_gis_files');
create type h_io_get_gis_files as(id_io_object int4, 
                                  id_url int4,
                                  uo_name varchar, 
                                  iu_name varchar, 
                                  ut_id int4, 
                                  ut_name varchar,
                                  win_app varchar,
                                  lin_app varchar,
                                  src_ext varchar,
                                  url_uid varchar);


create or replace function ioGetGISFiles(int4) returns setof h_io_get_gis_files as
$BODY$
declare
    idObject alias for $1;
    r h_io_get_gis_files%rowtype;
begin

    for r in
        select 
            uo.id_io_object, 
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
            urls_objects uo,
            io_urls iu,
            url_types ut
        where 
            uo.id_io_object= idObject
            and uo.id_url = iu.id
            and iu.id_url_type = ut.id
            and iu.uploaded = true
            and ut.id = 11 --system files for embedded GIS
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
