create table type_ship ( 
                         name varchar not null,
                         description varchar,
                         code varchar not null) inherits (q_base_table);

create table radio_image (
                          image_width integer not null default 0,
                          image_height integer not null default 0,
                          id_type_ship integer not null,
                          description varchar) inherits (q_base_table);

create table radio_image_raw (
                              azimuth float not null,
                              image_raw bytea,
                              id_radio_image integer not null) inherits (q_base_table);

create table object_passport (
                              id_radio_image integer not null,
                              id_type_ship integer not null,
                              description varchar,
                              radio_image_location POINT) inherits (q_base_table);

alter table radio_image
    add constraint fk_image_type_ref foreign key (id_type_ship)
        references type_ship (id)
        on delete restrict on update restrict;

alter table radio_image_raw
    add constraint fk_radio_image_ref foreign key (id_radio_image)
        references radio_image (id)
        on delete restrict on update restrict;

alter table object_passport
    add constraint fk_radio_image_ref foreign key (id_radio_image)
        references radio_image (id)
        on delete restrict on update restrict;

select f_safe_drop_trigger ('f_type_ships_uid', 'type_ship');
select f_create_trigger ('f_type_ships_uid', 'before', 'insert or update', 'type_ship', 'uidcheck()');

select f_safe_drop_trigger ('f_radio_image_uid', 'radio_image');
select f_create_trigger ('f_radio_image_uid', 'before', 'insert or update', 'radio_image', 'uidcheck()');

select f_safe_drop_trigger ('f_radio_image_raw_uid', 'radio_image_raw');
select f_create_trigger ('f_radio_image_raw_uid', 'before', 'insert or update', 'radio_image_raw', 'uidcheck()');

select f_safe_drop_trigger ('f_object_passport_uid', 'object_passport');
select f_create_trigger ('f_object_passport_uid', 'before', 'insert or update', 'object_passport', 'uidcheck()');

