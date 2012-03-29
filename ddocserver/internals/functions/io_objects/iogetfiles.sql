select f_safe_drop_type('h_io_get_files');
create type h_io_get_files as(id_io_object int4, 
                              id_url int4,
                              uo_name varchar, 
                              iu_name varchar, 
                              ut_id int4, 
                              ut_name varchar,
                              win_app varchar,
                              lin_app varchar,
                              src_ext varchar);


create or replace function ioGetFiles(int4) returns setof h_io_get_files as
$BODY$
declare
    idObject alias for $1;
    r h_io_get_files%rowtype;
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
            iu.src_ext
        from 
            urls_objects uo,
            io_urls iu,
            url_types ut
        where 
            uo.id_io_object= idObject
            and uo.id_url = iu.id
            and iu.id_url_type = ut.id
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
