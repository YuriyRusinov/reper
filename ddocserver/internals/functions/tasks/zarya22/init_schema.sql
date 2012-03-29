/*==============================================================*/
/* DBMS name:      PostgreSQL 8                                 */
/* Created on:     09.09.2011 10:40:59                          */
/*==============================================================*/


/*==============================================================*/
/* User: public                                                 */
/*==============================================================*/
/*==============================================================*/
/* Table: shu_acs                                               */
/*==============================================================*/
create table shu_acs (
   id                   SERIAL               not null,
   code                 VARCHAR              not null,
   name                 VARCHAR              not null,
   uri                  VARCHAR              not null,
   constraint PK_SHU_ACS primary key (id)
);

comment on table shu_acs is
'Реестр цнии эису АСУ';

/*==============================================================*/
/* Table: shu_addressee                                         */
/*==============================================================*/
create table shu_addressee (
   id                   SERIAL               not null,
   id_acs               INT4                 not null,
   code                 VARCHAR              not null,
   name                 VARCHAR              not null,
   type                 VARCHAR              not null,
   value                VARCHAR              not null,
   uri                  VARCHAR              not null,
   constraint PK_SHU_ADDRESSEE primary key (id)
);

comment on table shu_addressee is
'реестр цнии эису адресов сопрягаемых систем';

/*==============================================================*/
/* Table: shu_chksum                                            */
/*==============================================================*/
create table shu_chksum (
   id                   SERIAL               not null,
   code                 VARCHAR              not null,
   name                 VARCHAR              not null,
   constraint PK_SHU_CHKSUM primary key (id)
);

comment on table shu_chksum is
'реестр цнии эису методов подсчета контрольной суммы файла';

/*==============================================================*/
/* Table: shu_dls                                               */
/*==============================================================*/
create table shu_dls (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   uri                  VARCHAR              not null,
   id_acs               INT4                 not null,
   id_org               INT4                 not null,
   id_pos               INT4                 not null,
   constraint PK_SHU_DLS primary key (id)
);

comment on table shu_dls is
'реестр цнии эису должностных лиц';

/*==============================================================*/
/* Table: shu_dls_position                                      */
/*==============================================================*/
create table shu_dls_position (
   id_shu_dls           INT4                 not null,
   id_position          INT4                 not null,
   constraint PK_SHU_DLS_POSITION primary key (id_shu_dls, id_position)
);

comment on table shu_dls_position is
'Соответствие должностных лиц в системе DynamicDocs и должностных лиц в системе Заря22';

/*==============================================================*/
/* Table: shu_domains                                           */
/*==============================================================*/
create table shu_domains (
   id                   SERIAL               not null,
   code                 VARCHAR              not null,
   name                 VARCHAR              not null,
   constraint PK_SHU_DOMAINS primary key (id)
);

comment on table shu_domains is
'Реестр ЦНИИ ЭИСУ предметных областей';

/*==============================================================*/
/* Table: shu_orgs                                              */
/*==============================================================*/
create table shu_orgs (
   id                   SERIAL               not null,
   code                 VARCHAR              not null,
   name                 VARCHAR              not null,
   uri                  VARCHAR              not null,
   constraint PK_SHU_ORGS primary key (id)
);

comment on table shu_orgs is
'реестр цнии эису организационных единиц';

/*==============================================================*/
/* Table: shu_positions                                         */
/*==============================================================*/
create table shu_positions (
   id                   SERIAL               not null,
   code                 VARCHAR              not null,
   name                 VARCHAR              not null,
   constraint PK_SHU_POSITIONS primary key (id)
);

comment on table shu_positions is
'реестр цнии эису должностных  единиц';

alter table shu_addressee
   add constraint FK_SHU_ADDR_REFERENCE_SHU_ACS foreign key (id_acs)
      references shu_acs (id)
      on delete restrict on update restrict;

alter table shu_dls
   add constraint FK_SHU_DLS_REFERENCE_SHU_ACS foreign key (id_acs)
      references shu_acs (id)
      on delete restrict on update restrict;

alter table shu_dls
   add constraint FK_SHU_DLS_REFERENCE_SHU_ORGS foreign key (id_org)
      references shu_orgs (id)
      on delete restrict on update restrict;

alter table shu_dls
   add constraint FK_SHU_DLS_REFERENCE_SHU_POSI foreign key (id_pos)
      references shu_positions (id)
      on delete restrict on update restrict;

alter table shu_dls_position
   add constraint FK_SHU_DLS__REFERENCE_SHU_DLS foreign key (id_shu_dls)
      references shu_dls (id)
      on delete restrict on update restrict;

alter table shu_dls_position
   add constraint FK_SHU_DLS__REFERENCE_POSITION foreign key (id_position)
      references "position" (id)
      on delete restrict on update restrict;

