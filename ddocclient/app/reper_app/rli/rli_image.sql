create table type_ship (id serial not null primary key,
                         name varchar not null,
                         description varchar,
                         code varchar not null) inherits (root_table);

create table radio_image (id serial not null primary key,
                          image_width integer not null default 0,
                          image_height integer not null default 0,
                          id_type_ship integer not null,
                          description varchar) inherits (root_table);

create table radio_image_raw (id serial not null primary key,
                              azimuth float not null,
                              image_raw bytea,
                              id_radio_image integer not null) inherits (root_table);

create table object_passport (id serial not null primary key,
                              id_radio_image integer not null,
                              id_type_ship integer not null,
                              description varchar,
                              radio_image_location POINT) inherits (root_table);

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
