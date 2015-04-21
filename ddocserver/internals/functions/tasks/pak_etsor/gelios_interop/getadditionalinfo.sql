
/*
select * from getAdditionalInfo(1073); --hpost
select * from getAdditionalInfo(1094); --modeling
select * from getAdditionalInfo(927); --camera


select * from io_objects where id = 490;
select * from io_categories where id = 487

drop view eio_table_490 cascade;
alter table tbl_eio_table_490 rename to eio_table_490
select acl_secureTable('eio_table_490');

alter table tbl_eio_table_490 add column incident int8;
alter table tbl_eio_table_490 alter column incident set default 2017;
update tbl_eio_table_490 set incident = 2017
alter table tbl_eio_table_490 alter column incident set not null;
alter table tbl_eio_table_490 add  CONSTRAINT "FK_eio_table_490_REF_incident" FOREIGN KEY (incident)
      REFERENCES "tbl_eio_table_424" (id) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT;

insert into attrs_categories (id_io_category, id_io_attribute, name, is_mandatory, is_read_only, def_value) values (486, 1293, 'Ассоциированные события датчиков (аргус-спектр)', false, false, NULL);

select * from eio_table_436
select * from position where id = 28
select * from units where id = 18
update units set code_name = 'MONITORING' where id = 18
update eio_table_436 set "medical-attributes-1198" = 62
alter table tbl_eio_table_436 alter column "medical-attributes-1198" set default 28;
alter table tbl_eio_table_436 alter column "medical-attributes-1198" set not null;

update eio_table_436 set incident = 2017

alter table tbl_eio_table_436 alter column incident set default 2017;
alter table tbl_eio_table_436 alter column incident set not null;

--update eio_table_439 set incident = 2017
alter table tbl_eio_table_439 alter column incident set default 1059;
alter table tbl_eio_table_439 alter column incident set not null;

--drop view eio_table_438 cascade
alter table tbl_eio_table_438 add column "medical-attributes-1198" int8;
alter table tbl_eio_table_438 add  CONSTRAINT "FK_eio_table_438_REF_medical-attributes-1198" FOREIGN KEY ("medical-attributes-1198")
      REFERENCES "position" (id) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT;


--alter table tbl_eio_table_438 alter column "medical-attributes-1198" set default 28 
--alter table tbl_eio_table_438 alter column "medical-attributes-1198" set not null;
--update tbl_eio_table_438 set "medical-attributes-1198" = 28

select * from attrs_categories where id_io_category = 426

insert into attrs_categories (id_io_category, id_io_attribute, name, is_mandatory, is_read_only, def_value) values (426, 1198, 'Система', true, false, '28');

alter table tbl_eio_table_438 rename to eio_table_438;
select acl_secureTable('eio_table_438');
select * from eio_table_438
select * from io_objects where id = 438;
select * from io_categories where id = 427

select * from eio_table_422
select * from io_objects where id = 422;
select * from io_categories where id = 403


select * from eio_table_439
select * from io_objects where id = 439;
select * from io_categories where id = 429
select a.* from attributes a, attrs_categories ac where ac.id_io_category = 428 and ac.id_io_attribute = a.id

select * from io_categories where id_child in (select id_io_category from attrs_categories where id_io_attribute = 1206)
update attributes set code = 'source_incident_rec', name = 'Сетевой ресурс (запись события)', title = 'Сетевой ресурс (запись события)' where id = 1206;
update attrs_categories set name = 'Сетевой ресурс (запись события)' where id_io_attribute = 1206

select table_name from io_objects where id_io_category in (417, 421, 429)

alter table tbl_eio_table_439 rename column source to source_incident_rec;
alter table tbl_eio_table_433 rename column source to source_incident_rec;
alter table tbl_eio_table_436 rename column source to source_incident_rec;

drop view eio_table_439 cascade;
drop view eio_table_433 cascade;
drop view eio_table_436 cascade;

alter table tbl_eio_table_439 rename to eio_table_439;
alter table tbl_eio_table_433 rename to eio_table_433;
alter table tbl_eio_table_436 rename to eio_table_436;

select acl_secureTable('eio_table_439');
select acl_secureTable('eio_table_433');
select acl_secureTable('eio_table_436');


select uuid_t, incident_name from eio_table_424

select * from io_categories order by id
select * from eio_table_490;

select * from io_objects where id = 490;
select * from io_categories where id = 486
select * from attributes where id in (select id_io_attribute from attrs_categories where id_io_category = 486)

select * from attrs_categories where id_io_attribute = 1293
--3280

insert into io_views (id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) values(3309, 66, 161, false, 3, NULL)
select * from io_templates where id_io_category = 486
select * from io_views where id_attr_category = 3269
select * from io_views where id_a_group = 161
select * from io_templates where id = 66
select * from a_groups where id_io_template = 66
update io_views set id_a_group = 161 where id = 522

select * from attributes where id in (select id_io_attribute from attrs_categories where id_io_category = 486)

--3269
*/ 

select f_safe_drop_type('h_get_additional_info');
create type h_get_additional_info as (param_code varchar, param_value varchar, param_name varchar, param_type varchar);


create or replace function getAdditionalInfo(int8) returns setof h_get_additional_info as
$BODY$
declare
    alarm_id alias for $1;

    alarm_uuid varchar;
    r h_get_additional_info%rowtype;

begin

    select uuid_t::varchar into alarm_uuid from q_base_table where id = alarm_id;

    if(alarm_uuid isnull) then
        return;
    end if;

    for r in select * from getAdditionalInfo(alarm_uuid)
    loop
        return next r;
    end loop;

    return;

end
$BODY$ language 'plpgsql';


create or replace function getAdditionalInfo(varchar) returns setof h_get_additional_info as
$BODY$
declare
    alarm_uuid alias for $1;

    r h_get_additional_info%rowtype;

    tableName varchar;
    sql varchar;
begin

    if(alarm_uuid isnull) then
        return;
    end if;

    tableName = getRecordTableByUuid(alarm_uuid);
    if(tableName = 'eio_table_436') then
        sql = 'select * from getAdditionalInfoHPost(' || asString(alarm_uuid, true) || ')';
    elsif (tableName = 'eio_table_438') then
        sql = 'select * from getAdditionalInfoModeling(' || asString(alarm_uuid, true) || ')';
    elsif(tableName = 'eio_table_439') then
        sql = 'select * from getAdditionalInfoCamera(' || asString(alarm_uuid, true) || ')';
    elsif(tableName = 'eio_table_492') then
        sql = 'select * from getAdditionalInfoFire(' || asString(alarm_uuid, true) || ')';
    elsif(tableName = 'fiks_incidents_1') then
        sql = 'select * from getAdditionalInfoPortal(' || asString(alarm_uuid, true) || ')';
    else
        return;
    end if;


    for r in execute sql
    loop
        return next r;
    end loop;

    return;

end
$BODY$ language 'plpgsql';


/* ********************************** */


create or replace function getAdditionalInfoCamera(varchar) returns setof h_get_additional_info as
$BODY$
declare
    alarm_uuid alias for $1;

    r h_get_additional_info%rowtype;

    tableName varchar;
    tableNameDevice varchar;
    
    params_code varchar[];
    params_type varchar[];

    params_code_device varchar[];
    params_type_device varchar[];

    params_sql_string varchar;
    params_code_string varchar;
    params_type_string varchar;

    params_sql_string_device varchar;
    params_code_string_device varchar;
    params_type_string_device varchar;

    idChildCategory int4;
    idChildCategoryDevice int4;

    sql varchar;
begin

    /* ********************************/
    return;--Не требуется выдавать в Гелиос доп.параметры по видеокамере
    /* ********************************/
    
    if(alarm_uuid isnull) then
        return;
    end if;
    
    params_type = getCameraAdditionalParamsType();
    params_code = getCameraAdditionalParams();

    params_type_device = getCameraDeviceAdditionalParamsType();
    params_code_device = getCameraDeviceAdditionalParams();

    idChildCategory = 428;
    idChildCategoryDevice = 402;

    tableName = 'eio_table_439';
    tableNameDevice = 'eio_table_422';

    select array_to_string(array_agg('asString(t."' || t.code || '", false)'), ',') into params_sql_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.code, true) ), ',') into params_code_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.type, true) ), ',') into params_type_string
    from (select unnest(params_type) as type) as t;
    

    select array_to_string(array_agg('asString(t."' || t.code || '", false)'), ',') into params_sql_string_device
    from (select unnest(params_code_device) as code) as t;

    select array_to_string(array_agg( asString(t.code, true) ), ',') into params_code_string_device
    from (select unnest(params_code_device) as code) as t;

    select array_to_string(array_agg( asString(t.type, true) ), ',') into params_type_string_device
    from (select unnest(params_type_device) as type) as t;

   sql = 'select t.colname, t.the_value, a.name, t.coltype
          from
              (SELECT 
                   t.id,
                   unnest(ARRAY[' || params_code_string || ']) AS colname,
                   unnest(ARRAY[' || params_type_string || ']) AS coltype,
                   unnest(ARRAY[' || params_sql_string || ']) AS the_value
               FROM ' || tableName || ' t
               WHERE
                   t.uuid_t = ' || asString(alarm_uuid, true) || '
               ORDER BY t.id, colname) as t,
              attributes a,
              attrs_categories ac
          where
              a.id = ac.id_io_attribute
              and ac.id_io_category = ' || idChildCategory || '
              and a.code = t.colname

        union

          select t.colname, t.the_value, a.name, t.coltype
          from
              (SELECT 
                   t.id,
                   unnest(ARRAY[' || params_code_string_device || ']) AS colname,
                   unnest(ARRAY[' || params_type_string_device || ']) AS coltype,
                   unnest(ARRAY[' || params_sql_string_device || ']) AS the_value
               FROM ' || tableNameDevice || ' t, ' || tableName || ' c 
               WHERE
                   c.uuid_t = ' || asString(alarm_uuid, true) || '
                   and c.device = t.id
               ORDER BY t.id, colname) as t,
              attributes a,
              attrs_categories ac
          where
              a.id = ac.id_io_attribute
              and ac.id_io_category = ' || idChildCategoryDevice || '
              and a.code = t.colname ';


    for r in execute sql
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function getCameraAdditionalParams() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['source_incident_rec']; -- из таблицы tbl_eio_table_439
    return res;
end
$BODY$
language 'plpgsql';

create or replace function getCameraAdditionalParamsType() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['CONTENT']; -- из таблицы tbl_eio_table_439
    return res;
end
$BODY$
language 'plpgsql';


create or replace function getCameraDeviceAdditionalParams() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['source']; -- из таблицы tbl_eio_table_422
    return res;
end
$BODY$
language 'plpgsql';

create or replace function getCameraDeviceAdditionalParamsType() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['CONTENT']; -- из таблицы tbl_eio_table_422
    return res;
end
$BODY$
language 'plpgsql';



/* ********************************** */


create or replace function getAdditionalInfoModeling(varchar) returns setof h_get_additional_info as
$BODY$
declare
    alarm_uuid alias for $1;

    r h_get_additional_info%rowtype;

    tableName varchar;
    params_code varchar[];
    params_type varchar[];

    params_sql_string varchar;
    params_code_string varchar;
    params_type_string varchar;

    idChildCategory int4;
    sql varchar;
begin

    if(alarm_uuid isnull) then
        return;
    end if;
    
    params_type = getModelingAdditionalParamsType();
    params_code = getModelingAdditionalParams();

    idChildCategory = 426;
    tableName = 'eio_table_438';

    select array_to_string(array_agg('asString(t."' || t.code || '", false)'), ',') into params_sql_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.code, true) ), ',') into params_code_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.type, true) ), ',') into params_type_string
    from (select unnest(params_type) as type) as t;
    
   sql = 'select t.colname, t.the_value, a.name, t.coltype
          from
              (SELECT 
                   t.id,
                   unnest(ARRAY[' || params_code_string || ']) AS colname,
                   unnest(ARRAY[' || params_type_string || ']) AS coltype,
                   unnest(ARRAY[' || params_sql_string || ']) AS the_value
               FROM ' || tableName || ' t
               WHERE
                   t.uuid_t = ' || asString(alarm_uuid, true) || '
               ORDER BY t.id, colname) as t,
              attributes a,
              attrs_categories ac
          where
              a.id = ac.id_io_attribute
              and ac.id_io_category = ' || idChildCategory || '
              and a.code = t.colname';


    for r in execute sql
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function getModelingAdditionalParams() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['affected_people', 'damage_building', 'death', 'spread_wms', 'spread_wfs', 'notification_wms', 'notification_wfs']; -- из таблицы tbl_eio_table_438
    return res;
end
$BODY$
language 'plpgsql';

create or replace function getModelingAdditionalParamsType() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['STRING', 'STRING', 'STRING', 'CONTENT', 'CONTENT', 'CONTENT', 'CONTENT']; -- из таблицы tbl_eio_table_438
    return res;
end
$BODY$
language 'plpgsql';


/* ********************************** */

create or replace function getAdditionalInfoPortal(varchar) returns setof h_get_additional_info as
$BODY$
declare
    alarm_uuid alias for $1;

    r h_get_additional_info%rowtype;

    tableName varchar;
    params_code varchar[];
    params_type varchar[];

    params_sql_string varchar;
    params_code_string varchar;
    params_type_string varchar;

    idChildCategory int4;
    sql varchar;
begin

    /* ********************************/
    return;--Не требуется выдавать в Гелиос доп.параметры по порталу
    /* ********************************/
    
    if(alarm_uuid isnull) then
        return;
    end if;
    
    params_type = getPortalAdditionalParamsType();
    params_code = getPortalAdditionalParams();

    idChildCategory = 524;
    tableName = 'fiks_incidents_1';


    select array_to_string(array_agg('asString(t."' || t.code || '", false)'), ',') into params_sql_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.code, true) ), ',') into params_code_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.type, true) ), ',') into params_type_string
    from (select unnest(params_type) as type) as t;
    
   sql = 'select t.colname, t.the_value, a.name, t.coltype
          from
              (SELECT 
                   t.id,
                   unnest(ARRAY[' || params_code_string || ']) AS colname,
                   unnest(ARRAY[' || params_type_string || ']) AS coltype,
                   unnest(ARRAY[' || params_sql_string || ']) AS the_value
               FROM ' || tableName || ' t
               WHERE
                   t.uuid_t = ' || asString(alarm_uuid, true) || '
               ORDER BY t.id, colname) as t,
              attributes a,
              attrs_categories ac
          where
              a.id = ac.id_io_attribute
              and ac.id_io_category = ' || idChildCategory || '
              and a.code = t.colname';


    for r in execute sql
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function getPortalAdditionalParams() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['guuid_chs3']; -- из таблицы tbl_fiks_incidents_1
    return res;
end
$BODY$
language 'plpgsql';

create or replace function getPortalAdditionalParamsType() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['STRING']; -- из таблицы tbl_fiks_incidents_1
    return res;
end
$BODY$
language 'plpgsql';


/* ********************************** */

create or replace function getAdditionalInfoHPost(varchar) returns setof h_get_additional_info as
$BODY$
declare
    alarm_uuid alias for $1;

    r h_get_additional_info%rowtype;

    tableName varchar;
    params_code varchar[];
    params_type varchar[];

    params_sql_string varchar;
    params_code_string varchar;
    params_type_string varchar;

    idChildCategory int4;
    sql varchar;
begin

    if(alarm_uuid isnull) then
        return;
    end if;
    
    params_type = getHPostAdditionalParamsType();
    params_code = getHPostAdditionalParams();

    idChildCategory = 420;
    tableName = 'eio_table_436';

    select array_to_string(array_agg('asString(t."' || t.code || '", false)'), ',') into params_sql_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.code, true) ), ',') into params_code_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.type, true) ), ',') into params_type_string
    from (select unnest(params_type) as type) as t;
    
   sql = 'select t.colname, t.the_value, a.name, t.coltype
          from
              (SELECT 
                   t.id,
                   unnest(ARRAY[' || params_code_string || ']) AS colname,
                   unnest(ARRAY[' || params_type_string || ']) AS coltype,
                   unnest(ARRAY[' || params_sql_string || ']) AS the_value
               FROM ' || tableName || ' t
               WHERE
                   t.uuid_t = ' || asString(alarm_uuid, true) || '
               ORDER BY t.id, colname) as t,
              attributes a,
              attrs_categories ac
          where
              a.id = ac.id_io_attribute
              and ac.id_io_category = ' || idChildCategory || '
              and a.code = t.colname';


    for r in execute sql
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function getHPostAdditionalParams() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    --res = ARRAY['real_id', 'source_incident_rec', 'value', 'reference_bad', 'reference_critical', 'is_critical']; -- из таблицы tbl_eio_table_436
    res = ARRAY['real_id', 'value', 'reference_bad', 'reference_critical', 'is_critical']; -- из таблицы tbl_eio_table_436
    return res;
end
$BODY$
language 'plpgsql';

create or replace function getHPostAdditionalParamsType() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['STRING', 'STRING', 'STRING', 'STRING', 'STRING']; -- из таблицы tbl_eio_table_436
    return res;
end
$BODY$
language 'plpgsql';


/* ********************************** */
--select * from io_objects where id = 492

create or replace function getAdditionalInfoFire(varchar) returns setof h_get_additional_info as
$BODY$
declare
    alarm_uuid alias for $1;

    r h_get_additional_info%rowtype;

    tableName varchar;
    params_code varchar[];
    params_type varchar[];

    params_sql_string varchar;
    params_code_string varchar;
    params_type_string varchar;

    idChildCategory int4;
    sql varchar;
begin

    if(alarm_uuid isnull) then
        return;
    end if;
    
    params_type = getFireAdditionalParamsType();
    params_code = getFireAdditionalParams();

    idChildCategory = 420;
    tableName = 'eio_table_492';

    select array_to_string(array_agg('asString(t."' || t.code || '", false)'), ',') into params_sql_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.code, true) ), ',') into params_code_string
    from (select unnest(params_code) as code) as t;

    select array_to_string(array_agg( asString(t.type, true) ), ',') into params_type_string
    from (select unnest(params_type) as type) as t;
    
   sql = 'select t.colname, t.the_value, a.name, t.coltype
          from
              (SELECT 
                   t.id,
                   unnest(ARRAY[' || params_code_string || ']) AS colname,
                   unnest(ARRAY[' || params_type_string || ']) AS coltype,
                   unnest(ARRAY[' || params_sql_string || ']) AS the_value
               FROM ' || tableName || ' t
               WHERE
                   t.uuid_t = ' || asString(alarm_uuid, true) || '
               ORDER BY t.id, colname) as t,
              attributes a,
              attrs_categories ac
          where
              a.id = ac.id_io_attribute
              and ac.id_io_category = ' || idChildCategory || '
              and a.code = t.colname';


    for r in execute sql
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function getFireAdditionalParams() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['real_id']; -- из таблицы tbl_eio_table_492
    return res;
end
$BODY$
language 'plpgsql';

create or replace function getFireAdditionalParamsType() returns varchar[] as
$BODY$
declare
    res varchar[];
begin

    res = ARRAY['STRING']; -- из таблицы tbl_eio_table_492
    return res;
end
$BODY$
language 'plpgsql';
