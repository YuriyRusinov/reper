\encoding win1251

begin;

drop table organization_transport;
create table organization_transport (
   id                   SERIAL               not null,
   id_organization      INT4                 not null,
   id_transport         INT4                 not null,
   address              VARCHAR              not null,
   is_active            bool                 not null default TRUE,
   constraint PK_ORGANIZATION_TRANSPORT primary key (id)
);

comment on table organization_transport is
'используемые организацией транспорты межобъектового обмена';

comment on column organization_transport.address is
'адрес организации в данном транспорте';

alter table organization_transport set mac TO NULL;

create unique index ot_unique_index on organization_transport using BTREE (
id_organization,
id_transport
);

alter table organization_transport
   add constraint FK_ORGANIZA_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table organization_transport
   add constraint FK_ORGANIZA_REFERENCE_TRANSPOR foreign key (id_transport)
      references transport (id)
      on delete restrict on update restrict;

create table out_sync_queue (
   id                   SERIAL               not null,
   id_organization      INT4                 not null,
   id_entity            INT4                 not null,
   entity_table         VARCHAR              not null,
   entity_type          INT4                 not null,
   sync_type            INT4                 not null,
   sync_result          INT4                 not null,
   constraint PK_OUT_SYNC_QUEUE primary key (id)
);

create  index i_sync_result on out_sync_queue using BTREE (
sync_result
);

alter table out_sync_queue
   add constraint FK_OUT_SYNC_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;


create table io_objects_organization (
   id_organization      INT4                 not null,
   id_io_objects        INT4                 not null,
   constraint PK_IO_OBJECTS_ORGANIZATION primary key (id_organization, id_io_objects)
);

alter table io_objects_organization
   add constraint FK_IO_OBJEC_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table io_objects_organization
   add constraint FK_IO_OBJEC_REFERENCE_IO_OBJEC foreign key (id_io_objects)
      references io_objects (id)
      on delete restrict on update restrict;


create table io_categories_organization (
   id_organization      INT4                 not null,
   id_io_categories     INT4                 not null,
   constraint PK_IO_CATEGORIES_ORGANIZATION primary key (id_organization, id_io_categories)
);

alter table io_categories_organization
   add constraint FK_IO_CATEG_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table io_categories_organization
   add constraint FK_IO_CATEG_REFERENCE_IO_CATEG foreign key (id_io_categories)
      references io_categories (id)
      on delete restrict on update restrict;

create or replace function aa11() returns int4 as'
declare
    idAttr1 int4;
    idAttr2 int4;
    idAttr3 int4;
    idAttr4 int4;

    idAttr5 int4; --inf_exchange
    idAttr6 int4; --inf_exchange

    idCat1 int4;
    idCat11 int4;
    idCat2 int4;
    idCat21 int4;

    idIO1 int4;
    idIO2 int4;
begin

    select getNextSeq(\'attributes\', \'id\') into idAttr1;
    insert into attributes (id, id_a_type, code, name, title, table_name, column_name, def_width) values(idAttr1, 1, \'is_active\', \'Активен\', \'Транспорт активен\', NULL, NULL, 120);

    select getNextSeq(\'attributes\', \'id\') into idAttr2;
    insert into attributes (id, id_a_type, code, name, title, table_name, column_name, def_width) values(idAttr2, 2, \'id_transport\', \'Транспорт\', \'Транспорт межобъектового обмена\', \'transport\', \'name\', 150);

    select getNextSeq(\'attributes\', \'id\') into idAttr3;
    insert into attributes (id, id_a_type, code, name, title, table_name, column_name, def_width) values(idAttr3, 9, \'local_address\', \'Локальный адрес\', \'Локальный адрес транспорта\', NULL, NULL, 150);



    select getNextSeq(\'io_categories\', \'id\') into idCat1;
    insert into io_categories (id, id_io_type, id_child, is_main, name, code, description, is_system) values (idCat1, 10, NULL, false, \'Таблицы справочников транспортов межобъектового обмена (новая ОШС)\', \'SYSCATEGORY_67_\', NULL::varchar, true);

    select getNextSeq(\'io_categories\', \'id\') into idCat11;
    insert into io_categories (id, id_io_type, id_child, is_main, name, code, description, is_system) values (idCat11, 8, idCat1, true, \'Справочники транспортов межобъектового обмена (новая ОШС)\', \'SYSCATEGORY_68_\', NULL::varchar, true);

    select getNextSeq(\'io_categories\', \'id\') into idCat2;
    insert into io_categories (id, id_io_type, id_child, is_main, name, code, description, is_system) values (idCat2, 10, NULL, false, \'Таблицы справочников адресов организаций (новая ОШС)\', \'SYSCATEGORY_69_\', NULL::varchar, true);

    select getNextSeq(\'io_categories\', \'id\') into idCat21;
    insert into io_categories (id, id_io_type, id_child, is_main, name, code, description, is_system) values (idCat21, 8, idCat2, true, \'Справочники адресов организаций (новая ОШС)\', \'SYSCATEGORY_70_\', NULL::varchar, true);


    insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (idCat1, 1, 2);
    insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (idCat1, 2, 1);

    insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (idCat11, 1, 2);
    insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (idCat11, 2, 1);

    insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (idCat2, 1, 2);
    insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (idCat2, 2, 1);

    insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (idCat21, 1, 2);
    insert into io_life_cycle (id_io_category, id_state_src, id_state_dest) values (idCat21, 2, 1);

    --таблица транспортов
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (idCat1, 1, NULL, true, true); --id
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (idCat1, 2, NULL, true, false); --name
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (idCat1, idAttr3, NULL, true, false); --local_address
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (idCat1, idAttr1, \'true\', true, false); --is_active

    --таблица адресов организаций
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (idCat2, 1, NULL, true, true); --id
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (idCat2, 74, NULL, true, false); --id_organization
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (idCat2, idAttr2, NULL, true, false); --id_transport
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (idCat2, 28, NULL, true, false); --address
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (idCat2, idAttr1, \'true\', true, false); --is_active

    select getNextSeq(\'io_objects\', \'id\') into idIO1;
    insert into io_objects (id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel)
    values (idIO1, idCat11, 1, 1, \'Справочник транспортов межобъектового обмена\', \'q_transport\', \'transport\', NULL, NULL, true, current_timestamp, 1);
    insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
    values (idIO1, -10, TRUE, TRUE, FALSE, FALSE);

    select getNextSeq(\'io_objects\', \'id\') into idIO2;
    insert into io_objects (id, id_io_category, author, id_io_state, name, code, table_name, description, information, is_system, insert_time, id_maclabel)
    values (idIO2, idCat21, 1, 1, \'Справочник адресов организаций\', \'q_organization_transport\', \'organization_transport\', NULL, NULL, true, current_timestamp, 1);
    insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
    values (idIO2, -10, TRUE, TRUE, FALSE, FALSE);

    select getNextSeq(\'attributes\', \'id\') into idAttr4;
    insert into attributes (id, id_a_type, code, name, title, table_name, column_name, def_width) values( idAttr4, 5, \'time_start\', \'Начало периода существования функциональной группы\', \'Начало периода существования ФГ\', NULL, NULL, 120);
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (55, idAttr4, \'true\', true, false);--time_start (for units)
    alter table units add column time_start timestamp;
    update attributes set name = \'Конец периода существования функциональной группы\', title = \'Конец периода существования ФГ\' where id = 80;

    select getNextSeq(\'attributes\', \'id\') into idAttr5;
    insert into attributes (id, id_a_type, code, name, title, table_name, column_name, def_width) values(idAttr5, 17, 'io_objects_organization', \'Синхронизируемые организации\', \'Синхронизируемые организации\', \'organization\', \'name\', 200);
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (13, idAttr5, NULL, false, false);--io_objects_organization (separate table)
    insert into io_views (id_io_category, id_io_attribute, id_io_template, id_a_group, is_read_only, "order", def_value) values(13, idAttr5, 1, 2, true, 2, NULL);
    
    select getNextSeq(\'attributes\', \'id\') into idAttr6;
    insert into attributes (id, id_a_type, code, name, title, table_name, column_name, def_width) values(idAttr6, 17, \'io_categories_organization\', \'Синхронизируемые организации\', \'Синхронизируемые организации\', \'organization\', \'name\', 200);
    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (11, idAttr6, NULL, false, false);--io_categories_organization (separate table)

end' 
language 'plpgsql';

select aa11();

drop function aa11();

alter table transport add column local_address varchar;
update transport set local_address = 'local';
alter table transport alter column local_address set not null;

alter table transport add column is_active bool;
update transport set is_active = TRUE;
alter table transport alter column lis_active set not null;
alter table transport alter column is_active set default TRUE;


alter table position drop column address;
alter table units drop column address;
alter table organization drop column address;

delete from attrs_categories where id_io_category = 51 and id_io_attribute = 28; --position address
delete from attrs_categories where id_io_category = 53 and id_io_attribute = 28; --organization address
delete from attrs_categories where id_io_category = 55 and id_io_attribute = 28; --unit address


update attributes set title = 'Кому представить', name = 'Кому представить доклад об исполнении' where id = 34;


\i ./functions/readd_functions.sql

commit;

begin;
--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql


commit;
