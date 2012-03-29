\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql


--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql



select setAsLogging(1);
select setAsLogging(2);

drop function cGetCategoryAttrs(int4);
drop function cGetSystemAttrs();


\i ./functions/readd_functions.sql
\i ./functions/security/readd_security.sql

\i ./functions/tasks/zarya22/readd_zarya_func.sql

select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');

create or replace function aaa() returns int4 as
$BODY$
declare
    cnt int4;
begin

    select count(*) into cnt from pg_attribute where attname = 'id_ref_attr_type';
    if(cnt > 0) then
        return 1;
    end if;

    alter table attributes add column id_ref_attr_type int4;
    alter table attributes
       add constraint FK_ATTRIBUT_REFERENCE_A_TYPES1 foreign key (id_ref_attr_type)
          references a_types (id)
          on delete restrict on update restrict;

    update attributes set id_ref_attr_type = (select id from atGetAttrType(column_name, table_name) limit 1);

    return 2;
end
$BODY$
language 'plpgsql';

select aaa();

drop function aaa();



insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-107', 107, 10, NULL, false, 'MIME-типы', 'SYSCATEGORY_107', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(107, -10, true, true, false, false, true);
insert into io_categories (unique_id, id, id_io_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values ('localorg-categories-108', 108, 8, 107, true, 'Справочник MIME-типов', 'SYSCATEGORY_108', NULL::varchar, true, true, 1);
--insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use)
--values(108, -10, true, true, false, false, true);

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (107, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (107, 2, 1);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (108, 1, 2);
insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (108, 2, 1);

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-46', 46, 9, 'type', 'Тип', 'Тип', NULL, NULL, 150, TRUE);
insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-69', 69, 9, 'subtype', 'Подтип', 'Подтип', NULL, NULL, 150, TRUE);

--таблица MIME-типов
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (107, 1, NULL, true, true); --id
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (107, 2, NULL, true, false); --name
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (107, 46, NULL, true, false); --type
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (107, 69, NULL, true, false); --subtype

select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

insert into tbl_io_objects (unique_id, id, id_io_category, author, id_io_state, name, table_name, description, information, is_system, is_global, id_sync_type, insert_time, id_maclabel, id_owner_org)
values ('localorg-io_objects-53', 53, 108, 1, 1, 'Справочник MIME-типов', 'mimetypes', NULL, 'Системный объект', true, true, 5, current_timestamp, 1, NULL);
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (53, -10, TRUE, TRUE, FALSE, FALSE);


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');


create table mimetypes (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   type                 VARCHAR              not null,
   subtype              VARCHAR              not null,
   constraint PK_MIMETYPES primary key (id)
)
inherits (root_table);

comment on column mimetypes.name is
'Объединение type + subtype';

select setMacToNULL('mimetypes');
select createTriggerUID('mimetypes');


insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-30', 30, 2, 'id_mimetype', 'MIME-тип', 'MIME-тип', 'mimetypes', 'name', 200, TRUE);
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (43, 30, '1', true, false);  --id_mimetype

alter table url_extentions add column id_mimetype int4;
alter table url_extentions
   add constraint FK_URL_EXTE_REFERENCE_MIMETYPE foreign key (id_mimetype)
      references mimetypes (id)
      on delete restrict on update restrict;

insert into mimetypes (name, "type", "subtype") values('undef/undef', 'undef', 'undef');
insert into mimetypes (name, "type", "subtype") values('text/plain', 'text', 'plain');
insert into mimetypes (name, "type", "subtype") values('text/csv', 'text', 'csv');
insert into mimetypes (name, "type", "subtype") values('text/css', 'text', 'css');
insert into mimetypes (name, "type", "subtype") values('text/html', 'text', 'html');
insert into mimetypes (name, "type", "subtype") values('text/xml', 'text', 'xml');

insert into mimetypes (name, "type", "subtype") values('application/octet-stream', 'application', 'octet-stream');
insert into mimetypes (name, "type", "subtype") values('application/msword', 'application', 'msword');
insert into mimetypes (name, "type", "subtype") values('application/vnd.ms-powerpoint', 'application', 'vnd.ms-powerpoint');
insert into mimetypes (name, "type", "subtype") values('application/vnd.ms-excel', 'application', 'vnd.ms-excel');
insert into mimetypes (name, "type", "subtype") values('application/x-visio', 'application', 'x-visio');

insert into mimetypes (name, "type", "subtype") values('image/png', 'image', 'png');
insert into mimetypes (name, "type", "subtype") values('image/jpeg', 'image', 'jpeg');
insert into mimetypes (name, "type", "subtype") values('image/bmp', 'image', 'bmp');
insert into mimetypes (name, "type", "subtype") values('image/gif', 'image', 'gif');
insert into mimetypes (name, "type", "subtype") values('image/tiff', 'image', 'tiff');
insert into mimetypes (name, "type", "subtype") values('image/pcx', 'image', 'pcx');

update url_extentions set id_mimetype = 1;
alter table url_extentions alter column id_mimetype set not null;
alter table url_extentions alter column id_mimetype set default 1;

commit;
