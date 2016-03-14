/*==============================================================*/
/* DBMS name:      PostgreSQL 8                                 */
/* Created on:     14.03.2016 17:21:43                          */
/*==============================================================*/


drop table tbl_object_passport;

drop table tbl_rli_image_raws;

drop table tbl_rli_sky;

drop table tbl_type_ship;

/*==============================================================*/
/* Table: tbl_object_passport                                   */
/*==============================================================*/
create table tbl_object_passport (
   id                   INT4                 not null,
   id_rli_sky           INT4                 null,
   description          VARCHAR              null,
   id_type_ship         INT4                 not null,
   constraint PK_TBL_OBJECT_PASSPORT primary key (id)
);

/*==============================================================*/
/* Table: tbl_rli_image_raws                                    */
/*==============================================================*/
create table tbl_rli_image_raws (
   id                   serial               not null,
   elevation_angle      FLOAT8               null,
   azimuth              FLOAT8               null,
   image_raw            BYTEA                not null,
   image_jpg            VARCHAR              null,
   id_object            INT4                 null,
   constraint PK_TBL_RLI_IMAGE_RAWS primary key (id)
);

/*==============================================================*/
/* Table: tbl_rli_sky                                           */
/*==============================================================*/
create table tbl_rli_sky (
   id                   SERIAL               not null,
   azimuth              FLOAT8               null default NULL
      constraint CKC_AZIMUTH_TBL_RLI_ check (azimuth is null or (azimuth between 0 and 360)),
   elevation_angle      FLOAT8               null default NULL
      constraint CKC_ELEVATION_ANGLE_TBL_RLI_ check (elevation_angle is null or (elevation_angle between -90 and 90)),
   resolution           FLOAT8               null,
   description          VARCHAR              null,
   code                 VARCHAR              null,
   constraint PK_TBL_RLI_SKY primary key (id)
);

/*==============================================================*/
/* Table: tbl_type_ship                                         */
/*==============================================================*/
create table tbl_type_ship (
   id                   SERIAL               not null,
   name                 VARCHAR              not null default '',
   description          VARCHAR              null,
   code                 VARCHAR              not null default '',
   constraint PK_TBL_TYPE_SHIP primary key (id)
);

alter table tbl_object_passport
   add constraint FK_TBL_OBJE_REFERENCE_TBL_RLI_ foreign key (id_rli_sky)
      references tbl_rli_sky (id)
      on delete restrict on update restrict;

alter table tbl_object_passport
   add constraint FK_TBL_OBJE_REFERENCE_TBL_TYPE foreign key (id_type_ship)
      references tbl_type_ship (id)
      on delete restrict on update restrict;

alter table tbl_rli_image_raws
   add constraint FK_TBL_RLI__REFERENCE_TBL_OBJE foreign key (id_object)
      references tbl_object_passport (id)
      on delete restrict on update restrict;

