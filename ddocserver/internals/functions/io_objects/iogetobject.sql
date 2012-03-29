select f_safe_drop_type('h_get_object');
create type h_get_object as (id int4,
                             id_io_category int4,
                             author int4,
                             id_io_state int4,
                             io_name varchar,
                             io_code varchar,
                             io_table_name varchar,
                             io_desc varchar,
                             io_info varchar,
                             is_system boolean,
                             insert_time timestamp,
                             s_name varchar,
                             s_desc varchar,
                             id_maclabel int4,
                             unique_id varchar,
                             id_sync_type int4,
                             sync_name varchar,
                             sync_desc varchar,
                             id_owner_org int4,
                             is_global boolean,
                             record_fill_color int8,
                             record_text_color int8,
                             id_search_template int4,
                             ref_table_name varchar,
                             owner_email_prefix varchar,
                             id_io_type int4,
                             t_name varchar,
                             t_desc varchar,
                             uid_io_type varchar);
                             
create or replace function ioGetObject(int4) returns setof h_get_object as
$BODY$
declare
    idObject alias for $1;
    r h_get_object%rowtype;
begin

    for r in 
        select
            io.id,
            io.id_io_category,
            io.author,
            io.id_io_state,
            io.name,
            NULL as io_code,
            io.table_name,
            io.description,
            io.information,
            io.is_system,
            io.insert_time,
            s.name,
            s.description,
            io.id_maclabel,
            io.unique_id,
            io.id_sync_type,
            sy.name,
            sy.description,
            io.id_owner_org,
            io.is_global,
            io.record_fill_color,
            io.record_text_color,
            io.id_search_template,
            io.ref_table_name,
            (case when io.id_owner_org isnull then NULL else o.email_prefix end) as owner_email_prefix,
            t.id,
            t.name,
            t.description,
            t.unique_id

        from
            --io_objects io
            f_sel_io_objects(idObject) io

            left join organization o on io.id_owner_org = o.id
            left join io_states s on io.id_io_state = s.id
            left join io_sync_types sy on io.id_sync_type = sy.id
            left join io_types t on io.id_io_type = t.id
        where
            io.id = idObject
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

select f_safe_drop_type('h_get_object_ref_table');
create type h_get_object_ref_table as (
                             id int4,
                             table_name varchar,
                             id_io_object int4,
                             id_io_category int4,
                             name varchar,
                             unique_id varchar,
                             id_search_template int4,
                             ref_table_name varchar);
                             
create or replace function ioGetObjectTables (int4) returns setof h_get_object_ref_table as
$BODY$
declare
    idObject alias for $1;
    r h_get_object_ref_table%rowtype;
begin

    for r in
        select
            id,
            table_name,
            id_io_object,
            id_io_category,
            title,
            unique_id,
            id_search_template,
            ref_table_name
        from
            object_ref_tables as ort
        where
            ort.id_io_object = idObject
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function ioGetObjectIDByTableName (varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    idObject int4;
begin
    select into idObject io.id from f_sel_io_objects_tbl(tableName) io where io.table_name = tableName;
    --raise notice '%', idObject;

    if (idObject is null) then
        select id_io_object into idObject from object_ref_tables where table_name = tableName;
        if(idObject is null)  then
            idObject := -1;
        end if;
    end if;

    return idObject;
end
$BODY$
language 'plpgsql';

create or replace function ioGetObjectIDByUID (varchar) returns int4 as
$BODY$
declare
    ioUID alias for $1;
    idObject int4;
begin
    select into idObject io.id from f_sel_io_objects(ioUID) io where unique_id = ioUID;
    raise notice '%', idObject;

    if (idObject is null) then
        idObject := -1;
    end if;

    return idObject;
end
$BODY$
language 'plpgsql';

create or replace function ioGetTableNameByUID(varchar) returns varchar as
$BODY$
declare
    theUID alias for $1;
    tableName varchar;
begin
    select table_name into tableName from f_sel_io_objects(theUID) where unique_id = theUID;
    if(tableName is null) then
        select table_name into tableName from object_ref_tables where unique_id = theUID;
    end if;

    return tableName;
end
$BODY$
language 'plpgsql';
