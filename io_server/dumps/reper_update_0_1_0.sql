INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-305', '2016-03-15 16:36:00.0', 305, 10, NULL, NULL, 1, NULL, false, 'Table of RLI Parameters', 'CATEGORY_305', NULL, false, false, 1, false);
INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-306', '2016-03-15 16:36:44.610661', 306, 1, 305, NULL, 1, NULL, true, 'RLI Parameters', 'CATEGORY_306', NULL, false, false, 1, false);

select setval ('io_categories_id_seq', 306, true);
select csetglobal (305);
select csetglobal (306);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2139', '2016-03-15 19:00:00.0', 2139, 305, 1, 'Идентификатор', NULL, false, false, 1);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2140', '2016-03-15 19:00:00.0', 2140, 305, 1009, 'resolution', NULL, false, false, 2);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2141', '2016-03-15 19:00:00.0', 2141, 305, 3, 'Описание', NULL, false, false, 3);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2142', '2016-03-15 19:00:00.0', 2142, 305, 6, 'Код', NULL, false, false, 4);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2143', '2016-03-15 19:00:00.0', 2143, 305, 1002, 'Azimuth', NULL, false, false, 5);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2144', '2016-03-15 19:00:00.0', 2144, 305, 1003, 'elevation_angle', NULL, false, false, 6);

alter table tbl_rli_image_raws add resolution float8;

drop view rli_image_raws ;

create view rli_image_raws as SELECT f_sel_rli_image_raws.unique_id, f_sel_rli_image_raws.last_update, 
    f_sel_rli_image_raws.id, f_sel_rli_image_raws.id_io_state, 
    f_sel_rli_image_raws.uuid_t, f_sel_rli_image_raws.rr_name, 
    f_sel_rli_image_raws.r_icon, f_sel_rli_image_raws.record_fill_color, 
    f_sel_rli_image_raws.record_text_color, f_sel_rli_image_raws.id_type_ship, 
    f_sel_rli_image_raws.azimuth, f_sel_rli_image_raws.elevation_angle, 
    f_sel_rli_image_raws.image_raw, f_sel_rli_image_raws.image_jpg, 
    f_sel_rli_image_raws.depth, f_sel_rli_image_raws.resolution, f_sel_rli_image_raws."Goodness of fit" FROM f_sel_rli_image_raws() f_sel_rli_image_raws(unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, id_type_ship, azimuth, elevation_angle, image_raw, image_jpg, depth, "Goodness of fit", resolution);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2145', '2016-03-16 15:49:12.0', 2145, 303, 1009, 'resolution', NULL, false, false, 9);
select setval ('attrs_categories_id_seq', 2145, true);

INSERT INTO tbl_io_objects (unique_id, last_update, id, id_io_category, id_io_state, id_io_type, id_maclabel, author, id_sync_type, id_owner_org, name, table_name, description, information, is_system, insert_time, is_completed, is_global, record_fill_color, record_text_color, id_search_template, ref_table_name, r_icon, uuid_t) VALUES ('localorg_prefix-io_objects-304', '2016-03-15 21:20:05.647943', 304, 306, 1, 2, 1, 1, 1, 1, 'КРСА параметры', 'rli_sky', NULL, 'Администратор системы', false, '2016-03-15 21:20:20', 2, true, NULL, NULL, NULL, NULL, NULL, '0e9b6c6e-eadb-11e5-81b2-7b6df462f3b9');

INSERT INTO io_views (unique_id, last_update, id, id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) VALUES ('localorg_prefix-io_views-335', '2016-03-16 16:31:10.048484', 335, 2144, 54, 112, false, 4, NULL);
update io_views set "order"=5 where id=334;
update attrs_categories set "order"=9 where id=2136;
update attrs_categories set "order"=8 where id=2144;
--SELECT pg_catalog.setval('io_views_id_seq', 335, true);
INSERT INTO io_views (unique_id, last_update, id, id_attr_category, id_io_template, id_a_group, is_read_only, "order", def_value) VALUES ('localorg_prefix-io_views-336', '2016-03-16 16:31:10.048484', 336, 2145, 54, 112, false, 5, NULL);
SELECT pg_catalog.setval('io_views_id_seq', 336, true);

INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1010', '2016-03-16 17:35:08.557668', 1010, 2, NULL, 4, 'id_sky', 'RLI parameters', 'Параметры объекта', 'rli_sky', 'id', 'id', 50, true, 9);

select setval ('attributes_id_seq', 1010, true);
INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-307', '2016-03-16 18:02:00.0', 307, 10, NULL, NULL, 1, NULL, false, 'Table of Object Passports', 'CATEGORY_307', NULL, false, false, 1, false);
INSERT INTO io_categories (unique_id, last_update, id, id_io_category_type, id_child, id_child2, id_io_state, id_life_cycle, is_main, name, code, description, is_system, is_global, is_completed, is_archived) VALUES ('localorg_prefix-io_categories-308', '2016-03-16 18:03:44.610661', 308, 1, 307, NULL, 1, NULL, true, 'Object Passports', 'CATEGORY_308', NULL, false, false, 1, false);

select setval ('io_categories_id_seq', 308, true);
select csetglobal (307);
select csetglobal (308);

INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2148', '2016-03-16 18:32:12.0', 2148, 307, 1, 'Идентификатор', NULL, false, false, 1);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2149', '2016-03-16 18:32:12.0', 2149, 307, 2, 'Название', NULL, true, false, 2);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2150', '2016-03-16 18:32:12.0', 2150, 307, 3, 'description', NULL, false, false, 3);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2151', '2016-03-16 18:32:44.581934', 2151, 307, 1001, 'Ship type', NULL, true, false, 4);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2152', '2016-03-16 18:32:44.581934', 2152, 307, 1010, 'RLI Parameters', NULL, false, false, 5);
select setval ('attrs_categories_id_seq', 2152, true);
INSERT INTO tbl_io_objects (unique_id, last_update, id, id_io_category, id_io_state, id_io_type, id_maclabel, author, id_sync_type, id_owner_org, name, table_name, description, information, is_system, insert_time, is_completed, is_global, record_fill_color, record_text_color, id_search_template, ref_table_name, r_icon, uuid_t) VALUES ('localorg_prefix-io_objects-305', '2016-03-16 19:20:05.647943', 305, 308, 1, 2, 1, 1, 1, 1, 'Экспериментальные изображения объектов', 'object_passports', NULL, 'Администратор системы', false, '2016-03-16 19:20:20', 2, true, NULL, NULL, NULL, NULL, NULL, '83b3ffc8-eb91-11e5-988e-b3422cd447de');
select setval ('tbl_io_objects_id_seq', 305, true);

INSERT INTO attributes (unique_id, last_update, id, id_a_type, id_search_template, id_attr_group, code, name, title, table_name, column_name, ref_column_name, def_width, is_system, id_ref_attr_type) VALUES ('localorg_prefix-attributes-1011', '2016-03-16 17:35:08.557668', 1011, 2, NULL, 4, 'id_rli_object', 'object_passport', 'Параметры объекта', 'object_passports', 'name', 'id', 100, true, 9);
select setval ('attributes_id_seq', 1011, true);
INSERT INTO attrs_categories (unique_id, last_update, id, id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order") VALUES ('localorg_prefix-attrs_categories-2153', '2016-03-18 13:47:44.581934', 2153, 303, 1011, 'id_rli_object', NULL, false, false, 10);
select setval ('attrs_categories_id_seq', 2153, true);

alter table tbl_rli_image_raws add id_rli_object int4;

alter table tbl_rli_image_raws
   add constraint FK_TBL_RLI__REFERENCE_TBL_OBJE foreign key (id_rli_object)
      references tbl_object_passports (id)
      on delete restrict on update restrict;

drop view rli_image_raws ;

create or replace function f_sel_rli_image_raws () returns setof "tbl_rli_image_raws" as
$BODY$
declare
    r "tbl_rli_image_raws"%rowtype;
begin
    for r 
        in select * from "tbl_rli_image_raws"
    loop
        if (getPrivilege(getCurrentUser(), ioGetObjectIDByTableName('rli_image_raws'), 2, true) = false) then
            raise exception 'You have insufficient permissions to do the operation!';
            return;
        end if;
        return next r;
    end loop;
    return;
end
$BODY$
language'plpgsql' security definer;

create or replace function f_ins_rli_image_raws (character varying, timestamp without time zone, bigint, integer, uuid, character varying, character varying, bigint, bigint, bigint, double precision, double precision, bytea, character varying, integer, double precision, double precision, bigint) returns int4 as
$BODY$
declare
"ii_unique_id" alias for $1;
"ii_last_update" alias for $2;
"ii_id" alias for $3;
"ii_id_io_state" alias for $4;                                                                                            
"ii_uuid_t" alias for $5;                                                                                                 
"ii_rr_name" alias for $6;                                                                                                
"ii_r_icon" alias for $7;
"ii_record_fill_color" alias for $8;
"ii_record_text_color" alias for $9;
"ii_id_type_ship" alias for $10;
"ii_azimuth" alias for $11;
"ii_elevation_angle" alias for $12;                                                                                                    
"ii_image_raw" alias for $13;                                                                                                          
"ii_image_jpg" alias for $14;                                                                                                          
"ii_depth" alias for $15;                                                                                                              
"ii_Goodness of fit" alias for $16;
"ii_resolution" alias for $17;
"ii_id_rli_object" alias for $18;
begin
    if(getPrivilege(getCurrentUser(), ioGetObjectIDByTableName('rli_image_raws'), 4, true) = false) then 
        raise exception 'You have insufficient permissions to do the operation!';
        return 0;
    end if;
    insert into "tbl_rli_image_raws" ("unique_id", "last_update", "id", "id_io_state", "uuid_t", "rr_name", "r_icon", "record_fill_color", "record_text_color", "id_type_ship", "azimuth", "elevation_angle", "image_raw", "image_jpg", "depth", "Goodness of fit", "resolution", "id_rli_object") values ("ii_unique_id", "ii_last_update", "ii_id", "ii_id_io_state", "ii_uuid_t", "ii_rr_name", "ii_r_icon", "ii_record_fill_color", "ii_record_text_color", "ii_id_type_ship", "ii_azimuth", "ii_elevation_angle", "ii_image_raw", "ii_image_jpg", "ii_depth", "ii_Goodness of fit", "ii_resolution", "ii_id_rli_object");
    return 1;
end
$BODY$
language'plpgsql' security definer;

create or replace function f_upd_rli_image_raws (character varying, timestamp without time zone, bigint, integer, uuid, character varying, character varying, bigint, bigint, bigint, double precision, double precision, bytea, character varying, integer, double precision, double precision, bigint) returns int4 as
$BODY$
declare
"ii_unique_id" alias for $1;
"ii_last_update" alias for $2;
"ii_id" alias for $3;
"ii_id_io_state" alias for $4;                                                                                            
"ii_uuid_t" alias for $5;                                                                                                 
"ii_rr_name" alias for $6;                                                                                                
"ii_r_icon" alias for $7;
"ii_record_fill_color" alias for $8;
"ii_record_text_color" alias for $9;
"ii_id_type_ship" alias for $10;
"ii_azimuth" alias for $11;
"ii_elevation_angle" alias for $12;                                                                                                    
"ii_image_raw" alias for $13;                                                                                                          
"ii_image_jpg" alias for $14;                                                                                                          
"ii_depth" alias for $15;                                                                                                              
"ii_Goodness of fit" alias for $16;
"ii_resolution" alias for $17;
"ii_id_rli_object" alias for $18;
begin
    if(getPrivilege(getCurrentUser(), ioGetObjectIDByTableName('rli_image_raws'), 4, true) = false) then 
        raise exception 'You have insufficient permissions to do the operation!';
        return 0;
    end if;
    update "tbl_rli_image_raws" set "unique_id" = "ii_unique_id", "last_update" = "ii_last_update", "id_io_state" = "ii_id_io_state", "uuid_t" = "ii_uuid_t", "rr_name" = "ii_rr_name", "r_icon" = "ii_r_icon", "record_fill_color" = "ii_record_fill_color",  "record_text_color" = "ii_record_text_color", "id_type_ship" = "ii_id_type_ship", "azimuth" = "ii_azimuth", "elevation_angle" = "ii_elevation_angle", "image_raw" = "ii_image_raw",  "image_jpg" = "ii_image_jpg", "depth" = "ii_depth", "Goodness of fit" = "ii_Goodness of fit", "resolution" = "ii_resolution", "id_rli_object" = "ii_id_rli_object" where id = ii_id;
    return 1;
end
$BODY$
language'plpgsql' security definer;

create view rli_image_raws as SELECT f_sel_rli_image_raws.unique_id, f_sel_rli_image_raws.last_update, 
    f_sel_rli_image_raws.id, f_sel_rli_image_raws.id_io_state, 
    f_sel_rli_image_raws.uuid_t, f_sel_rli_image_raws.rr_name, 
    f_sel_rli_image_raws.r_icon, f_sel_rli_image_raws.record_fill_color, 
    f_sel_rli_image_raws.record_text_color, f_sel_rli_image_raws.id_type_ship, 
    f_sel_rli_image_raws.azimuth, f_sel_rli_image_raws.elevation_angle, 
    f_sel_rli_image_raws.image_raw, f_sel_rli_image_raws.image_jpg, 
    f_sel_rli_image_raws.depth, f_sel_rli_image_raws.resolution, f_sel_rli_image_raws.id_rli_object, f_sel_rli_image_raws."Goodness of fit" FROM f_sel_rli_image_raws() f_sel_rli_image_raws(unique_id, last_update, id, id_io_state, uuid_t, rr_name, r_icon, record_fill_color, record_text_color, id_type_ship, azimuth, elevation_angle, image_raw, image_jpg, depth, "Goodness of fit", resolution, id_rli_object);

create or replace rule r_ins_rli_image_raws AS ON INSERT TO rli_image_raws DO INSTEAD SELECT f_ins_rli_image_raws(new.unique_id, new.last_update, new.id, new.id_io_state, new.uuid_t, new.rr_name, new.r_icon, new.record_fill_color, new.record_text_color, new.id_type_ship, new.azimuth, new.elevation_angle, new.image_raw, new.image_jpg, new.depth, new."Goodness of fit", new.resolution, new.id_rli_object) AS f_ins_rli_image_raws;

create or replace rule r_upd_rli_image_raws AS ON UPDATE TO rli_image_raws DO INSTEAD SELECT f_upd_rli_image_raws(new.unique_id, new.last_update, new.id, new.id_io_state, new.uuid_t, new.rr_name, new.r_icon, new.record_fill_color, new.record_text_color, new.id_type_ship, new.azimuth, new.elevation_angle, new.image_raw, new.image_jpg, new.depth, new."Goodness of fit", new.resolution, new.id_rli_object) AS f_upd_rli_image_raws;

create or replace rule r_del_rli_image_raws AS ON DELETE TO rli_image_raws DO INSTEAD SELECT f_del_rli_image_raws(old.id) AS f_del_rli_image_raws;

update attributes set title='Разрешение' where id=1009;
