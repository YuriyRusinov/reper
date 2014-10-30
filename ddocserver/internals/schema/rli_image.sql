create table type_craft (id serial not null primary key,
                         description varchar,
                         code varchar not null);

create table radio_image (id serial not null primary key,
                          image_width integer not null default 0,
                          image_height integer not null default 0,
                          id_type_craft integer not null,
                          description varchar);

create table radio_image_raw (id serial not null primary key,
                              azimuth float not null,
                              image_raw bytea,
                              id_radio_image integer not null);

create table object_passport (id serial not null primary key,
                              id_radio_image integer not null,
                              id_type_craft integer not null,
                              description varchar,
                              radio_image_location POINT);

alter table radio_image
    add constraint fk_image_type_ref foreign key (id_type_craft)
        references type_craft (id)
        on delete restrict on update restrict;

alter table radio_image_raw
    add constraint fk_radio_image_ref foreign key (id_radio_image)
        references radio_image (id)
        on delete restrict on update restrict;

alter table object_passport
    add constraint fk_radio_image_ref foreign key (id_radio_image)
        references radio_image (id)
        on delete restrict on update restrict;

