/*==============================================================*/
/* DBMS name:      PostgreSQL 8                                 */
/* Created on:     29.01.2015 17:18:15                          */
/*==============================================================*/

/*==============================================================*/
/* Table: root_table                                            */
/*==============================================================*/
create table root_table (
   unique_id            VARCHAR              not null,
   last_update          TIMESTAMP            not null default CURRENT_TIMESTAMP
);

comment on column root_table.last_update is
'���� � ����� ��������� ����������� (��������) ������';

select setMacToNULL('root_table');

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create unique index Index_1 on root_table using BTREE (
unique_id
);

/*==============================================================*/
/* User: public                                                 */
/*==============================================================*/
/*==============================================================*/
/* Table: ERROR                                                 */
/*==============================================================*/
create table ERROR (
   SQLSTATE             CHAR(5)              not null,
   ID_GROUP             INT4                 not null,
   NAME                 VARCHAR              not null,
   constraint PK_ERROR primary key (SQLSTATE)
);

select setMacToNULL('ERROR');

/*==============================================================*/
/* Table: GROUP_ERROR                                           */
/*==============================================================*/
create table GROUP_ERROR (
   ID_GROUP             INT4                 not null,
   NAME                 VARCHAR              not null,
   constraint PK_GROUP_ERROR primary key (ID_GROUP)
);

select setMacToNULL('GROUP_ERROR');

/*==============================================================*/
/* Table: a_groups                                              */
/*==============================================================*/
create table a_groups (
   id                   SERIAL not null,
   id_io_template       INT4                 null,
   id_parent            INT4                 null,
   name                 VARCHAR              not null,
   "order"              INT4                 not null default 0,
   constraint PK_A_GROUPS primary key (id)
)
inherits (root_table);

comment on table a_groups is
'������ ���������.
������������ � �������� ��� ����������� ��������� ��� ������������';

comment on column a_groups."order" is
'������� ���������� ������ � ������� ��� �����������';

select setMacToNULL('a_groups');
select createTriggerUID('a_groups');

/*==============================================================*/
/* Index: g_opt_index                                           */
/*==============================================================*/
create unique index g_opt_index on a_groups using BTREE (
id_io_template,
id_parent,
"order"
);

/*==============================================================*/
/* Index: g_id_template                                         */
/*==============================================================*/
create  index g_id_template on a_groups using BTREE (
id_io_template
);

/*==============================================================*/
/* Table: a_types                                               */
/*==============================================================*/
create table a_types (
   id                   SERIAL not null,
   id_a_view            INT4                 not null,
   name                 VARCHAR              not null,
   code                 VARCHAR              not null,
   constraint PK_A_TYPES primary key (id)
)
inherits (root_table);

comment on table a_types is
'���������� ����� ���������.
��������� ����:
- ����������;
- ������� �����������;
- �������� (������ �� ������� ����� �� �����������)
- ����;
- ����/�����;
- ������� ��������;
- ����;
- ������������� ��������;
- ������
- ��������� �������� (���, �������, ����, �����, �����);
- �����������.
';

comment on column a_types.id is
'������������� ���� ��������';

comment on column a_types.id_a_view is
'������������� ������������� ��������� ������� ����';

comment on column a_types.name is
'�������� ���� ��������';

comment on column a_types.code is
'��� ���� ��������
� �������� ��������������� ����
INT4
VARCHAR
BOOL
TIMESTAMP
� �.�.';

select setMacToNULL('a_types');
select createTriggerUID('a_types');

/*==============================================================*/
/* Table: a_views                                               */
/*==============================================================*/
create table a_views (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   constraint PK_A_VIEWS primary key (id)
)
inherits (root_table);

comment on table a_views is
'������� �������� �������� ��������� ����������, ��� ������ ������� ������ ������������ �������� ��������� �����.
���, ��������, �������� ���� ������� ����������� ������ ������������ ��� ������ �������� ���������� ���������� ������ � �.�.';

select setMacToNULL('a_views');
select createTriggerUID('a_views');

/*==============================================================*/
/* Table: access_cards                                          */
/*==============================================================*/
create table access_cards (
   id                   SERIAL               not null,
   id_card_type         INT4                 not null,
   id_card_status       INT4                 not null,
   id_person            INT4                 null,
   comment              VARCHAR              null,
   code                 VARCHAR              null,
   extra_id             INT4                 null,
   is_completed         BOOL                 not null default FALSE,
   constraint PK_ACCESS_CARDS primary key (id)
)
inherits (root_table);

comment on table access_cards is
'���������� ���� ������� (�� ���)';

select setMacToNULL('access_cards');
select createTriggerUID('access_cards');

/*==============================================================*/
/* Table: access_cards_access_plan                              */
/*==============================================================*/
create table access_cards_access_plan (
   id_access_plan       INT4                 not null,
   id_access_cards      INT4                 not null,
   constraint PK_ACCESS_CARDS_ACCESS_PLAN primary key (id_access_plan, id_access_cards)
);

select setMacToNULL('access_cards_access_plan');

/*==============================================================*/
/* Table: access_cards_q                                        */
/*==============================================================*/
create table access_cards_q (
   id                   SERIAL               not null,
   code                 VARCHAR              null,
   comment              VARCHAR              null,
   id_card_type_p       INT4                 null,
   card_type_name       VARCHAR              null,
   id_card_status_p     INT4                 null,
   card_status_name     VARCHAR              null,
   access_plans         INT4[]               null,
   description          VARCHAR              null,
   org_name             VARCHAR              null,
   org_uid              VARCHAR              null,
   id_ac                INT4                 null,
   constraint PK_ACCESS_CARDS_Q primary key (id)
)
inherits (root_table);

comment on table access_cards_q is
'������������ ���������� ���� �������';

select setMacToNULL('access_cards_q');
select createTriggerUID('access_cards_q');

/*==============================================================*/
/* Table: access_categories_table                               */
/*==============================================================*/
create table access_categories_table (
   id_io_category       INT4                 not null,
   id_role              INT4                 not null,
   allow_read           BOOL                 not null default FALSE,
   allow_readlist       BOOL                 not null default FALSE,
   allow_delete         BOOL                 not null default FALSE,
   allow_update         BOOL                 not null default FALSE,
   allow_use            BOOL                 not null default FALSE,
   constraint PK_ACCESS_CATEGORIES_TABLE primary key (id_io_category, id_role)
);

comment on table access_categories_table is
'������ ������� ������ ���������� � ������ ������� ����� � ����������';

/*==============================================================*/
/* Table: access_plan                                           */
/*==============================================================*/
create table access_plan (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   org_name             VARCHAR              null,
   org_uid              VARCHAR              null,
   is_completed         BOOL                 not null default FALSE,
   constraint PK_ACCESS_PLAN primary key (id)
)
inherits (root_table);

comment on table access_plan is
'����� ������� (�� ���)';

select setMacToNULL('access_plan');
select createTriggerUID('access_plan');

/*==============================================================*/
/* Table: access_rubric_table                                   */
/*==============================================================*/
create table access_rubric_table (
   id_rubric            INT4                 not null,
   id_role              INT4                 not null,
   allow_read           BOOL                 not null default true,
   allow_readlist       BOOL                 not null default true,
   allow_delete         BOOL                 not null default true,
   allow_update         BOOL                 not null default true,
   constraint PK_ACCESS_RUBRIC_TABLE primary key (id_role, id_rubric)
);

select setMacToNULL('access_rubric_table');

/*==============================================================*/
/* Table: access_table                                          */
/*==============================================================*/
create table access_table (
   id_io_object         INT4                 not null,
   id_role              INT4                 not null,
   allow_read           BOOL                 not null default false,
   allow_readlist       BOOL                 not null default false,
   allow_delete         BOOL                 not null default false,
   allow_update         BOOL                 not null default false,
   allow_send           BOOL                 not null default FALSE,
   constraint PK_ACCESS_TABLE primary key (id_io_object, id_role)
);

comment on column access_table.allow_read is
'������ ������������� ��������������� ������� (��� ���������, ��������� ���������, ������ ������������� ������, ��������������� � ��� ������, ������� ����������� (��� ������������)) ������� ������� ����� ������ �� ����� ������� ������� ����� ������ ������� ��';

comment on column access_table.allow_readlist is
'������ ������� ��������������� �������, �.�. ������ ��� ��������� ��������� ��� ������ �� ������� ��������� ��. ������ ����� ���������� ����������� �� � ��������������� ������';

comment on column access_table.allow_delete is
'�������� ��������������� �������. ������� ������� ����� ������ �� ����� ������� ������� ����� ������ ������� ��';

comment on column access_table.allow_update is
'�������������� ������������� ��������������� ������� (��� ���������, ��������� ���������, ������ ������������� ������, ��������������� � ��� ������, ������� ����������� (��� ������������)). ������� ������� ����� ������ �� ����� ������� ������� ����� ������ �������������� ��';

select setMacToNULL('access_table');

/*==============================================================*/
/* Index: i_io_object                                           */
/*==============================================================*/
create  index i_io_object on access_table using BTREE (
id_io_object
);

/*==============================================================*/
/* Index: i_role                                                */
/*==============================================================*/
create  index i_role on access_table using BTREE (
id_role
);

/*==============================================================*/
/* Index: i_object_role                                         */
/*==============================================================*/
create unique index i_object_role on access_table using BTREE (
id_io_object,
id_role
);

/*==============================================================*/
/* Table: acl_template_exceptions                               */
/*==============================================================*/
create table acl_template_exceptions (
   id_template          INT4                 not null,
   id_role              INT4                 not null,
   allow_readlist       BOOL                 not null default false,
   allow_read           BOOL                 not null default false,
   allow_delete         BOOL                 not null default false,
   allow_update         BOOL                 not null default false,
   allow_use1           BOOL                 not null default FALSE
);

comment on table acl_template_exceptions is
'������� �������������� ���� ������������ ���, ������� ������� � �������.';

select setMacToNULL('acl_template_exceptions');

/*==============================================================*/
/* Table: acl_template_group_types                              */
/*==============================================================*/
create table acl_template_group_types (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   constraint PK_ACL_TEMPLATE_GROUP_TYPES primary key (id)
);

comment on table acl_template_group_types is
'������� ��������� ����� � �������� �������������� ������������� ������� ����������� ��� � �������������� ��������
�������� ��������� ������
1) ����� ���������� ������ ��
2) ����� ������ ������������� ������ (��� ������������ ����)
3) ����� ��������� ������������� �� ��� ���� ';

select setMacToNULL('acl_template_group_types');

/*==============================================================*/
/* Table: acl_template_groups                                   */
/*==============================================================*/
create table acl_template_groups (
   id_group_type        INT4                 not null,
   id_template          INT4                 not null,
   allow_readlist       BOOL                 not null default false,
   allow_read           BOOL                 not null default false,
   allow_delete         BOOL                 not null default false,
   allow_update         BOOL                 not null default false,
   allow_use1           BOOL                 not null default FALSE
);

comment on table acl_template_groups is
'������, ����� ������� ������ � �������';

select setMacToNULL('acl_template_groups');

/*==============================================================*/
/* Table: actions                                               */
/*==============================================================*/
create table actions (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   constraint PK_ACTIONS primary key (id)
);

select setMacToNULL('actions');

/*==============================================================*/
/* Table: alarm_events                                          */
/*==============================================================*/
create table alarm_events (
   id                   SERIAL               not null,
   id_person            INT4                 null,
   id_device            INT4                 null,
   event_date           TIMESTAMP            not null,
   message              VARCHAR              not null,
   source               VARCHAR              not null,
   alarm                INT4                 not null,
   card_code            VARCHAR              null,
   pass                 BOOL                 null,
   direction            VARCHAR              null,
   video                bytea                null,
   extra_id             INT4                 not null,
   is_completed         BOOL                 not null default FALSE,
   video_url            VARCHAR              null,
   constraint PK_ALARM_EVENTS primary key (id)
)
inherits (root_table);

comment on table alarm_events is
'������� ������� �� ������� � ��������� ��������
(� �� ���)';

select setMacToNULL('alarm_events');
select createTriggerUID('alarm_events');

/*==============================================================*/
/* Table: alarm_events_q                                        */
/*==============================================================*/
create table alarm_events_q (
   id                   SERIAL               not null,
   id_person_p          INT4                 null,
   event_date           TIMESTAMP            null,
   message              VARCHAR              null,
   source               VARCHAR              null,
   alarm                INT4                 null,
   card_code            VARCHAR              null,
   pass                 BOOL                 null,
   direction            VARCHAR              null,
   lastname             VARCHAR              null,
   firstname            VARCHAR              null,
   sirname              VARCHAR              null,
   id_device_p          INT4                 null,
   dev_name             VARCHAR              null,
   id_unit_p            INT4                 null,
   unit_name            VARCHAR              null,
   id_dolzh_p           INT4                 null,
   dolzh_name           VARCHAR              null,
   org_name             VARCHAR              null,
   org_uid              VARCHAR              null,
   id_ae                INT4                 null,
   video_url            VARCHAR              null,
   for_report           BOOL                 null,
   is_deleted           BOOL                 not null default FALSE,
   constraint PK_ALARM_EVENTS_Q primary key (id)
)
inherits (root_table);

comment on table alarm_events_q is
'������������ ���������� ������� �� ���';

select setMacToNULL('alarm_events_q');
select createTriggerUID('alarm_events_q');

/*==============================================================*/
/* Table: attributes                                            */
/*==============================================================*/
create table attributes (
   id                   SERIAL not null,
   id_a_type            INT4                 not null,
   id_search_template   INT4                 null,
   id_attr_group        INT4                 not null default 2,
   code                 VARCHAR(256)         not null,
   name                 VARCHAR              not null,
   title                VARCHAR              not null,
   table_name           VARCHAR(256)         null,
   column_name          VARCHAR(256)         null,
   ref_column_name      VARCHAR(256)         null,
   def_width            INT4                 not null default 100,
   is_system            BOOL                 not null default false,
   id_ref_attr_type     INT4                 null,
   constraint PK_ATTRIBUTES primary key (id)
)
inherits (root_table);

comment on table attributes is
'�������� ��������.
������ ��������� ������� ����������� �� �������
���� table_name � column_name �������� ��� ��������� ���� ������� �����������. � �������� �������� ����� �������� �������, ������� �������� ���������� ����������� � �������� ������� �������, ���������� ������� ���� �������� � ��������������� �������� ����������
��������� ��������� ������ �������� ����� �������� �������������� ������ � ������� table_name

���� code �������� ����������. ������������� ������������� ���� ��������� � ���������� �����

�������� ����� ���� ���������� ��� �����������������. ��������� �������� ������������ � ������ ��� ������������� ������� � �� ��������� ���������� ��� �������������.
������� �� ��������� � ���������������� �������� ������ ��� ����, ����� �� ������ ��������� �����������. ��������� �� ��������� ��������� ��, ������� ��� � ������� ����� ����������� ����� �� �����.

���� ref_column_name �������� �������� ������� (�����) � �������, �� ������� ��������� ������ �������. � ����� ������ ��� ���� ����� ����� ��������, �������� �� id (id ������������ �� ���������). ����� �������� ��������, ���� ��� ������������ ��������� ������ ���� "���������" � ����� DynamicDocs.';

comment on column attributes.id is
'������������� ��������';

comment on column attributes.id_a_type is
'������������� ���� ��������';

comment on column attributes.id_search_template is
'������������� ���������� �������';

comment on column attributes.id_attr_group is
'������������� ������, � ������� ������ ������ �������';

comment on column attributes.code is
'������� ����� ��������� ���� �������. � ���� ������ ������ ��������� ������� �������� � �������� ������� �������. ������ ���� ������ �������� ������� �������';

comment on column attributes.name is
'�������� ��������';

comment on column attributes.title is
'��������� �������� (��������� ��� human-readable �������� ������� �������)';

comment on column attributes.table_name is
'�������� ������� ��, � ������� ������������ �������� ��������, ���� ������� ������� �������� ������� �� ������� �����������';

comment on column attributes.column_name is
'�������� ������� � �������, ������� �������� �������� ��������, ���� ������� �������� ������� �� ������� �����������';

comment on column attributes.ref_column_name is
'�������� ������� � �������, �� ������� ��������� ������ �������, ���������� ��������� ������. �� ��������� ��� id';

comment on column attributes.def_width is
'����������� ������  ������� � KKSView ��� ������������ ������� ��������. ���������� � ��������.
�������� ���������� ������ ��������, ��������� ������ ��������, ��� ����������� �� �� ���� ����� ����� ��������� ������ ��� �� ������������ � KKSView
��� ����� ��������� � ���������������� 13 � 14. �������������� ���� ������������� ����� ���������. 
��� ���� 13 (������������� �����) ������ ���� �������� ����� ��������� oX ������ � ������ ������, ����� ���������� ������� ���������� ������� �� ��������� ������.
��� ���� 14 ��� ���� �������� ���������� ��������, ������� ����������� ������ ���� ������� � ���� ������ ����. CHAR(3), ��������.';

comment on column attributes.is_system is
'���� �����������';

comment on column attributes.id_ref_attr_type is
'��������� ����.
������ ���������� � ���� ��������, �� ������� ��������� ������ ������� (���� �� �������� ���������, � ��������� ������ NULL).
������ ���� "�� �����" ��� ������������, �.�. ��� �� ������ � ����� ��������� ����������� ���������.
��� ������������ � ��������, � ��������� � cGetCategoryAttrs().
�������� ������� ���� ������������� ������������� ��� �������� ��������.

������� ��� ����������� ������� cGetCategoryAttrs (����� �� ������������ ���������)';

select setMacToNULL('attributes');
select createTriggerUID('attributes');

/*==============================================================*/
/* Table: attrs_attrs                                           */
/*==============================================================*/
create table attrs_attrs (
   id                   SERIAL               not null,
   id_attr_parent       INT4                 null,
   id_attr_child        INT4                 null,
   name                 VARCHAR              null,
   def_value            VARCHAR              null,
   is_mandatory         BOOL                 null,
   is_read_only         BOOL                 null,
   "order"              INT4                 not null default 1,
   constraint PK_ATTRS_ATTRS primary key (id)
)
inherits (root_table);

comment on table attrs_attrs is
'������� ��������� ��������� ��������
������� ����� ����������� ������� ������ ���������, ������� ����� ���� ��������� ��������.
� ��� ����� ���� �� ��������.
����� �������, ������� ����� ���������� ���������, �� ������ � ������ ����������� �������� �������������� ������������� ��� �������� �������� (��� ��� �� ������ ��������� ��� �� ������ ����� ���������, �� � ����� ���� �������).
������ ����������� ��������� ������� ����������� ���������.
���� name ��������� �������� ������������ ��������.';

comment on column attrs_attrs.id_attr_parent is
'������������� ������������� ��������. �.�. �������� ������� ����������� ��� ������ ������� ��������.
�.�. ������������ ������� � ������ ������ �������������� �����������';

comment on column attrs_attrs.id_attr_child is
'������������� ������������ ���������� ��������';

comment on column attrs_attrs.name is
'�������� ������������ ���������� ��������';

comment on column attrs_attrs.def_value is
'�������� �� ���������';

comment on column attrs_attrs.is_mandatory is
'���� �������������� ������� �������� � ������� ������������ ���������� ��������';

comment on column attrs_attrs.is_read_only is
'���� "������ ��� ������"';

comment on column attrs_attrs."order" is
'������� ����������� ��������� ��� ������������ ��� ��������.';

select setMacToNULL('attrs_attrs');
select createTriggerUID('attrs_attrs');

/*==============================================================*/
/* Index: i_aav_parent                                          */
/*==============================================================*/
create  index i_aav_parent on attrs_attrs (
id_attr_parent
);

/*==============================================================*/
/* Index: i_aav_parent_child                                    */
/*==============================================================*/
create unique index i_aav_parent_child on attrs_attrs (
id_attr_parent,
id_attr_child
);

/*==============================================================*/
/* Table: attrs_attrs_values                                    */
/*==============================================================*/
create table attrs_attrs_values (
   id                   SERIAL               not null,
   id_attr_value        INT4                 not null,
   id_attr_attr         INT4                 not null,
   value                VARCHAR              not null,
   constraint PK_ATTRS_ATTRS_VALUES primary key (id)
)
inherits (root_table);

comment on table attrs_attrs_values is
'�������� ��������� ��������� (�����������) ��� ��.
������ � ������ ������� ���������� ������������� ��� �������������� �������� �������� � �������������� �������';

comment on column attrs_attrs_values.id is
'������������� ������';

comment on column attrs_attrs_values.id_attr_value is
'������������� �������� ����������, � �������� ��������� ������ �������� ��������';

comment on column attrs_attrs_values.id_attr_attr is
'������������� ������������ ���������� ��������';

comment on column attrs_attrs_values.value is
'�������� (��������� �������������) ������������ ���������� ��������';

select setMacToNULL('attrs_attrs_values');
select createTriggerUID('attrs_attrs_values');

/*==============================================================*/
/* Table: attrs_categories                                      */
/*==============================================================*/
create table attrs_categories (
   id                   SERIAL               not null,
   id_io_category       INT4                 not null,
   id_io_attribute      INT4                 not null,
   name                 VARCHAR              not null,
   def_value            VARCHAR              null,
   is_mandatory         BOOL                 not null,
   is_read_only         BOOL                 not null,
   "order"              INT4                 not null default 0,
   constraint PK_ATTRS_CATEGORIES primary key (id)
)
inherits (root_table);

comment on column attrs_categories.is_mandatory is
'����, ������������, �������� �� ������ ������� ������������ ��� ���������� ��� �� ������ ���������';

comment on column attrs_categories.is_read_only is
'����, ������������, �������� �� ������ ������� ��������� ������ ��� ������';

select setMacToNULL('attrs_categories');
select createTriggerUID('attrs_categories');

/*==============================================================*/
/* Index: i_ac_attr_category                                    */
/*==============================================================*/
create unique index i_ac_attr_category on attrs_categories (
id_io_category,
id_io_attribute
);

/*==============================================================*/
/* Index: i_ac_category                                         */
/*==============================================================*/
create  index i_ac_category on attrs_categories (
id_io_category
);

/*==============================================================*/
/* Table: attrs_groups                                          */
/*==============================================================*/
create table attrs_groups (
   id                   SERIAL               not null,
   id_parent            INT4                 null,
   name                 VARCHAR              not null,
   constraint PK_ATTRS_GROUPS primary key (id)
)
inherits (root_table);

comment on table attrs_groups is
'������ ���������. ������ ���������� ������ ������, ��� � �������� ������� ��������� ����� ������������ ����� �����.
������������ ����� ��������� ���� ������ � �������� �������� � ���. ����� �������� ����� ���������� � ���� ������, ����������� �� �������.';

comment on column attrs_groups.id is
'������������� ������';

comment on column attrs_groups.id_parent is
'������������ ������';

comment on column attrs_groups.name is
'�������� ������';

select setMacToNULL('attrs_groups');
select createTriggerUID('attrs_groups');

/*==============================================================*/
/* Table: attrs_values                                          */
/*==============================================================*/
create table attrs_values (
   id                   SERIAL               not null,
   id_io_object         INT4                 not null,
   id_attr_category     INT4                 not null,
   value                VARCHAR              not null,
   start_time           TIMESTAMP            not null default CURRENT_TIMESTAMP,
   stop_time            TIMESTAMP            null,
   insert_time          TIMESTAMP            not null default CURRENT_TIMESTAMP,
   id_io_object_src     INT4                 null,
   id_io_object_src1    INT4                 null,
   description          VARCHAR              null,
   is_actual            BOOL                 not null default TRUE,
   constraint PK_ATTRS_VALUES primary key (id)
)
inherits (root_table);

comment on table attrs_values is
'������� �������� ��������� � �������������� ��������.
������ ���� �������� ��������� � �� �������������� ��������������.
������������� � ������ ������� �������������� ������������ ������.
������ �������� �������� ����� �������� ������������.

��� ��������� �������� �������� ���������� ���������� �������� ����� ������ � ���� �������. ��� ���� ���� value �������� ����� ���������� ��������. ���� start_time �� �������� ��������������� � current_timestamp, ���� stop_time - � NULL, ���� meas_time - � current_timestamp, ���� insert_time - � current_timestamp, is_actual - � true.
��������� ���� ������������ ����� ������ ��������������, ������ start_time ��� ����� ����� ���������� � current_timestamp, � ���� stop_time - � NULL.
������ (��������) ������ ��� ���� ��������. ������ �� ���� is_actual ��������������� � false, stop_time - � current_timestamp.

��� �������� ������ �� ������ ������� ��������� �������� �� ����������. �������� ������ ��������. ������ �� ���� is_actual ��������������� � false, stop_time - � current_timestamp.
';

comment on column attrs_values.id is
'������������� ������';

comment on column attrs_values.id_io_object is
'������������� ���������������� �������';

comment on column attrs_values.id_attr_category is
'������������� ����������';

comment on column attrs_values.value is
'�������� �������� (����������) � ��������� �������������';

comment on column attrs_values.start_time is
'������ ������ ������������ �������� �������� (����������)';

comment on column attrs_values.stop_time is
'������ ����� ������������ �������� �������� (����������)';

comment on column attrs_values.insert_time is
'���� � ����� ��������� �������� ������� (����������) � � �������';

comment on column attrs_values.id_io_object_src is
'������������� ��, ������� ������ ���������� ���������� � �������� �������� (����������).';

comment on column attrs_values.id_io_object_src1 is
'������������� �������, �� ���� �������� ����������. ���� �������� �������� ���������� ��� �� ����, �� �������� � ���� ���� ��������� �� ��������� � ���� <��������>';

comment on column attrs_values.description is
'��������� �������� �������� ����������. ��� ������� ����������� ������� ���������� ������� ��������';

comment on column attrs_values.is_actual is
'����, ������������ ��������� �� ������ �������� ��������, ���� ��� ��������.
������ ���� �������� ������������� ��� �������� � ��������� ��������, ������� �� �������� ���� "������ ����� ������������". ���� ������� ����-����� �������� � �������� (������ ������ ������������) - (������ ����� ������������), �� ���� ��������������� � true, � ��������� ������ �� ����� false.';

select setMacToNULL('attrs_values');
select createTriggerUID('attrs_values');

/*==============================================================*/
/* Index: i_attr_values_o                                       */
/*==============================================================*/
create  index i_attr_values_o on attrs_values using BTREE (
id_io_object
);

/*==============================================================*/
/* Index: i_attr_values                                         */
/*==============================================================*/
create  index i_attr_values on attrs_values using BTREE (
id_io_object,
id_attr_category
);

/*==============================================================*/
/* Index: i_attr_values_a                                       */
/*==============================================================*/
create  index i_attr_values_a on attrs_values (
id_attr_category
);

/*==============================================================*/
/* Table: calendar                                              */
/*==============================================================*/
create table calendar (
   id_calendar          SERIAL not null,
   author_id            text                 not null,
   name                 varchar(250)         null default NULL,
   description          text                 null,
   type                 text                 not null default 'default',
   constraint PK_CALENDAR primary key (id_calendar)
);

/*==============================================================*/
/* Table: calendar_event                                        */
/*==============================================================*/
create table calendar_event (
   id_event             SERIAL not null,
   calendar_id          int                  not null,
   what                 text                 null,
   "where"              text                 null,
   description          text                 null,
   start_date           date                 not null,
   start_time           time                 null default NULL,
   stop_date            date                 not null,
   stop_time            time                 null default NULL,
   completed            text                 not null,
   notice               text                 not null,
   mark                 text                 not null,
   constraint PK_CALENDAR_EVENT primary key (id_event)
);

/*==============================================================*/
/* Index: calendar_id                                           */
/*==============================================================*/
create  index calendar_id on calendar_event (
calendar_id,
start_date,
start_time,
stop_date,
stop_time
);

/*==============================================================*/
/* Table: card_statuses                                         */
/*==============================================================*/
create table card_statuses (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   constraint PK_CARD_STATUSES primary key (id)
)
inherits (root_table);

comment on table card_statuses is
'�������  ���� ������� (�� ���)';

select setMacToNULL('card_statuses');
select createTriggerUID('card_statuses');

/*==============================================================*/
/* Table: card_types                                            */
/*==============================================================*/
create table card_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   constraint PK_CARD_TYPES primary key (id)
)
inherits (root_table);

comment on table card_types is
'���� ���� ������� (�� ���)';

select setMacToNULL('card_types');
select createTriggerUID('card_types');

/*==============================================================*/
/* Table: categories_rubrics                                    */
/*==============================================================*/
create table categories_rubrics (
   id                   SERIAL not null,
   id_parent            INT4                 null,
   id_category          INT4                 null,
   name                 VARCHAR              not null,
   constraint PK_CATEGORIES_RUBRICS primary key (id)
)
inherits (root_table);

comment on table categories_rubrics is
'�������� ������� ���������
������ ������� ����� ����������� ��, ���������� �� ������ ���������
������ �� ���� ������� ����� ���������� � ������� rubricator';

comment on column categories_rubrics.id_parent is
'������������� ������������ �������. �� ����� ���� ����� ������ � ��������������� ���������';

comment on column categories_rubrics.id_category is
'������������� ���������. �� ����� ���� ����� ������ � ��������������� ������������ �������';

comment on column categories_rubrics.name is
'�������� �������';

select setMacToNULL('categories_rubrics');
select createTriggerUID('categories_rubrics');

/*==============================================================*/
/* Table: chains                                                */
/*==============================================================*/
create table chains (
   id                   SERIAL               not null,
   id_handler           INT4                 not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_CHAINS primary key (id)
)
inherits (root_table);

comment on table chains is
'������� �������� ��������,  � ������� ������������ �� �������� ��������� � � �������� ��������� ��� ��������� (������ �� ������� ��������� � ���������).
���� ������� (�� � ��� ��������) ���������� � ������� �������� (chains_data).
��� ��������� �� �������� ��������� � ����������� � �������� ��������� ��������� ������ ���� �������.
������� ���� "���������"  � "���������" ��������� ���������� ��������.
� ����� �� ���������� ������������� ������ ������� "��������� ����" + "���������" �� ������ ������� "�������" + "������ ��������� �������".
';

comment on column chains.id_handler is
'���������� ������� (������ �� ������� ��������-������������)
������ ������� ����� ����� ������ ���� ����������
';

comment on column chains.name is
'�������� �������';

select setMacToNULL('chains');
select createTriggerUID('chains');

/*==============================================================*/
/* Table: chains_data                                           */
/*==============================================================*/
create table chains_data (
   id                   SERIAL               not null,
   id_chain             INT4                 not null,
   id_io_object         INT4                 null,
   id_parent            INT4                 null,
   id_processing_scenario INT4                 not null,
   id_processing_variant INT4                 not null,
   id_record            INT8                 null,
   insert_time          TIMESTAMP            not null default CURRENT_TIMESTAMP,
   is_handled           INT2                 not null default 0
      constraint CKC_IS_HANDLED_CHAINS_D check (is_handled in (0,1)),
   handled_time         TIMESTAMP            null,
   start_service_time   TIMESTAMP            null,
   end_service_time     TIMESTAMP            null,
   return_code          INT4                 null,
   what_happens         INT2                 not null
      constraint CKC_WHAT_HAPPENS_CHAINS_D check (what_happens in (1,2,3,4,5,6,7,8)),
   handler_in_data      VARCHAR              null,
   handler_out_data     VARCHAR              null,
   lc_is_parent         BOOL                 not null default TRUE,
   constraint PK_CHAINS_DATA primary key (id)
)
inherits (root_table);

comment on table chains_data is
'������� ��������.
�������� ������ �� �� ��� ���, ������� ������ � ������� ���������.
������������ ������ ���� ����� ������ �� ��� ���';

comment on column chains_data.id_chain is
'������ �� �������� ������� ���������';

comment on column chains_data.id_io_object is
'������ �� ��, ���������� � ������� (���-��������)';

comment on column chains_data.id_parent is
'��������� �� ������ ����������� �������� ���������, ��������������  ��������� �� � ������� ��������� (� ������ ���� ��� ������� ���� �������� �������-�������� ����������� ����� �������)
���� �������� �������� �� ������, �� ����������� ��������� �� (���) � ������� �������� �� ������-�� ��';

comment on column chains_data.id_processing_scenario is
'�������� ��������� �� � ��������. ��������� �� ��������, ������� ��� ������������ ��� �������� � ������ ������ ������������';

comment on column chains_data.id_processing_variant is
'������� ��������� �� � ��������. ��������� �� ������� ���������, ������� ��� ������������ ��� �������� � ������ ������ ������������';

comment on column chains_data.id_record is
'������������� ���, �������� ������� � ������� (���� �������������� ���)';

comment on column chains_data.insert_time is
'����, ����� �������� ������ � ������� �������� (����������� ������������� ��� �������� ������)';

comment on column chains_data.is_handled is
'������� ��������� ������ �������� ���������� (� �������� ���������=0, ����� ���������=1). 
���������=1 ��������������� � ���, ��� �������� ��������� ��������� ������ ������� 
������� ����������� ������ ��������� � �������� ��� �������� ������ ��� ���������
(����� ������ � ������� ��������)';

comment on column chains_data.handled_time is
'����, ����� ��������� ������ ������ �������� ���������� (���������)';

comment on column chains_data.start_service_time is
'����, ����� ������ ������ ������� ��������� ������� (����������� ��������)';

comment on column chains_data.end_service_time is
'����, ����� ����������  ������ ������� ��������� ������� (����������� ��������)';

comment on column chains_data.return_code is
'��� �������� ������� ���������';

comment on column chains_data.what_happens is
'�������, �� ������� ������ ��������� � �������
1 - ������ ����� ��
2 - ���������� �������� (����������������) ��
3 - ���������� ��������� ��
4 - ������ ����� ���
5 - �������� ��������� �������� ���
6 - �������� ���������� ���
7 - ���������� ��������� ���
8 - ������ ��������� � ���������� ������ ���������� ������� (������� ������ � ������� logistic)';

comment on column chains_data.handler_in_data is
'������� ������ ����������� �������>, � ������� ���������� ������� ������ ��� ����������� �������, � ������ �������-����������� ������� ����������� � ���, ����� ������������ �� ��� ���������';

comment on column chains_data.handler_out_data is
'<�������� ������ ����������� �������>, � ������� ���������� �������� ������ ����������� ������� � ������ �������-����������� ������� ����������� � ���, ����� ������������ �� ��� ���������';

comment on column chains_data.lc_is_parent is
'���������� �������, ������� � ��������� <������> ����������, ��� ��������� ��������� �� � ������ - ��� �� (�.� ������ ������ ��������� ���������� ��������� ��)';

select setMacToNULL('chains_data');
select createTriggerUID('chains_data');

/*==============================================================*/
/* Table: cmd_confirmations                                     */
/*==============================================================*/
create table cmd_confirmations (
   id_cmd               INT4                 not null
);

comment on table cmd_confirmations is
'��������� �������. 
� ��� ��������� �������������� ������, � ������� ������ ���� ���������� ��������� �� ������ �������.
��� ���������, ����� ������� ������ ���� ���������. �������� � ������, ����� �� �� ������ � ����������.
������ ������� ��������� � ������ mail.';

select setMacToNULL('cmd_confirmations');

/*==============================================================*/
/* Table: command_journal                                       */
/*==============================================================*/
create table command_journal (
   id                   SERIAL not null,
   id_dl_from           INT4                 not null,
   id_dl_to             INT4                 not null,
   id_dl_executor       INT4                 not null,
   id_io_category       INT4                 not null,
   id_jr_state          INT4                 not null,
   insert_time          TIMESTAMP            not null default CURRENT_TIMESTAMP,
   is_automated         BOOL                 not null default false,
   exec_datetime        TIMESTAMP            not null,
   exec_period          INT4[2]              null,
   message_body         VARCHAR              null,
   is_archived          BOOL                 not null default false,
   id_tabel             INT4                 null,
   id_urgency_level     INT4                 not null default 1,
   id_io_object_att     INT4                 null,
   extra_id             INT4                 null,
   last_exec_datetime   TIMESTAMP            null,
   input_number         VARCHAR              not null,
   output_number        VARCHAR              not null,
   accepted_datetime    TIMESTAMP            null,
   receive_datetime     TIMESTAMP            null,
   extra_table          VARCHAR              null,
   extra_table_id       INT4                 null,
   read_datetime_e      TIMESTAMP            null,
   read_datetime_c      TIMESTAMP            null,
   constraint PK_COMMAND_JOURNAL primary key (id)
)
inherits (root_table);

comment on table command_journal is
'������ ����� ������������
���������� �������� ���������� �� ���������� ��������.
- ������� ����� �������� ������������
- ������� ����� ��������� ������������';

comment on column command_journal.id_dl_to is
'����������';

comment on column command_journal.id_io_category is
'����������������� ��� ������������ ���������(��� �������);
����������, ����� ��������� �������� ���������';

comment on column command_journal.insert_time is
'���� �������� ������ ��� ���� �������� ������  � ������ ����� �������� ������������';

comment on column command_journal.is_automated is
'��� ����������� ������������-����������������� (TRUE) ��� �������(FALSE)';

comment on column command_journal.exec_datetime is
'���� ���������� ���� � ������� ���������� ���������  ����� ������� �������� ������������ (������ ���� ���������� ������);
����� ����������  ����� � �������� ���������� ����������� ��������� �� ���������� ������ ������� �������� ������������';

comment on column command_journal.exec_period is
'������������� ����������  ������ ���������� ������� ������, ������� � ��������� ���� � ������� (����� ����������� � �����, ������, �������, ���������, ���������� ��� �����;';

comment on column command_journal.message_body is
'���������� ������������';

comment on column command_journal.is_archived is
'����, ������������ ���������� ������� ������� � ������

������� � �������� ��������� ��������, ��� ������ � ��������� ����� �� ��������� � ������������ ������� ������� � ������������ ������� ������� �������� ���������� �������� � ��������� ������������ � ������ ������� ��������� ��� ����

������� � �������� ��������� ��������, ��� ������ � ��������� �������� ��������� � ������������ ������� ������� � ������������ ������� ������� �������� ���������� �������� � ��������� ������������ � ������ ������� ��������� ��� ����';

comment on column command_journal.id_tabel is
'���������� ����� ������ ������ ������� ���������
���� ������ � ��� ��� ��� ��������� �� ���
��������� ����. ���������������';

comment on column command_journal.id_urgency_level is
'��� ��������� ��������� (������������)';

comment on column command_journal.id_io_object_att is
'������������� � ������������ �������������� ������.
�� ����� �� ������ � ���������� "���������".
������������ ��� ���������� ����������� �������� ������ � �������������, ��������, ������� ��� ���������� (����� ���� ��������� ��� ��� � ���� �������������� ��)';

comment on column command_journal.extra_id is
'��������� ����.
���������������

������ ���� ������������ ��� ������������� ������.
������ ������������� ���������������� ������������ � �� ������� ������� (� �������� ��� ���� ��������).';

comment on column command_journal.last_exec_datetime is
'���� ��������� ������ ��� ������������� ������������.
������ ����-����� ���������� ���������������� ���������� ������� ������������.
������������ ����������� ������������� ������ ������������ � ������� cmdAnalyzeJournal()
��������� ����. ���������������';

comment on column command_journal.input_number is
'�������� ����� ������������. ����������� ��� ��������. ��������� �� ��������';

comment on column command_journal.output_number is
'��������� ����� ������������ (����������� ������� � ��������� �������:
1) ��� ������� ������������ �� ������� ����������� (� ���� ������ �������� � ��������� ����� ��������).
2) ��� �������� ������������ �� ������ ������� (� ���� ������ �������� � ��������� ����� �����������).

��������� �� ��������';

comment on column command_journal.extra_table is
'������������ ����� ���������� �� �����-�� ������ ���������� ������� (������� ��). � ���� ������ ��� ���� ������������ ����� ������������ � ������� � ������ ����������� (�������)';

comment on column command_journal.extra_table_id is
'������������ ����� ���������� �� �����-�� ������ ���������� ������� (������� ��). � ���� ������ ��� ���� ������������ ����� ������������ � ������� � ������ ����������� (�������)';

comment on column command_journal.read_datetime_e is
'���� ��������� � �� �������� ��������� ����������� ������������ � ������.
����� ��������� ������ �� �������� �������������. ������ ���������� � ���� � ������� ������������ ��-�����������  � �������������. ����������� �������������. NULL ��������, ��� ������������ �������� ����� (�������������) � � ������ ������������ �� ������� ������ ������������ ������ �������';

comment on column command_journal.read_datetime_c is
'���� ��������� � �� �������� ��������� ����������� ������������ � ������.
����� ��������� ������ � ������������� ���������� � ����� "�� ��������" (�.�. ������ �� ��-�����������). ������ ���������� � ���� � ������� ������������ ��-����������� � ������ �������������. ����������� �������������. NULL ��������, ��� ������������ �������� ����� (�������������) � � ������ ������������ �� ������� ������ ������������ ������ �������';

select setMacToNULL('command_journal');
select createTriggerUID('command_journal');

/*==============================================================*/
/* Table: coworkers                                             */
/*==============================================================*/
create table coworkers (
   id_organization1     INT4                 not null,
   id_organization2     INT4                 not null,
   constraint PK_COWORKERS primary key (id_organization1, id_organization2)
);

comment on table coworkers is
'����������������� �����������, � �������� ������ ����������� ��������������� � ������ �������';

select setMacToNULL('coworkers');

/*==============================================================*/
/* Table: criteria                                              */
/*==============================================================*/
create table criteria (
   id                   SERIAL not null,
   attr_id              INT4                 null,
   value                VARCHAR              null,
   id_type              INT4                 not null,
   id_operation         INT4                 not null,
   case_sensitive       BOOL                 not null default TRUE,
   is_not               BOOL                 not null default FALSE,
   constraint PK_CRITERIA primary key (id)
)
inherits (root_table);

comment on column criteria.attr_id is
'ID  ��������, ���� ����������� �������� ����� ���� 5 ��� 6 (�������� ������� � ������������ ��� �������� ���������)';

comment on column criteria.value is
'�������� ���������������� ��������.
� ����������� �� ����  �������� ���������� � ������� ���� ������ (int4, varchar � �.�.). �������� ������ ����������.
��� ���� �������� 5 ����� �������� NULL
��� ����� ��������� 4 � 6 � ������ ���������, ����������� �� �����������, ��������� ������� ���� ������ �������� ��������, �������� ������������. � ���������� ���������� ������ ����� ���������, ���� �� � ��������� ����������� ������ � ������ ���������.';

comment on column criteria.is_not is
'������ ���� ������, ����������� �� ������� �������� ��������� � ������� ��������';

select setMacToNULL('criteria');
select createTriggerUID('criteria');

/*==============================================================*/
/* Table: criteria_types                                        */
/*==============================================================*/
create table criteria_types (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   constraint PK_CRITERIA_TYPES primary key (id)
)
inherits (root_table);

comment on table criteria_types is
'���� ��������� �������������� �������� �� ��������
1 - ����������� �������� ��������
2 - �������� �����
3 - ������� �������������� ����� ��������� ����
4 - ������� �������� � �������� ���������
5 - ������� ��������� �������� � ����������� ���������
6 - ������� ��������� �������� � �������� ���������';

select setMacToNULL('criteria_types');
select createTriggerUID('criteria_types');

/*==============================================================*/
/* Table: dev_object_q                                          */
/*==============================================================*/
create table dev_object_q (
   id                   SERIAL               not null,
   id_device_p          INT4                 null,
   dev_name             VARCHAR              null,
   id_guard_obj_p       INT4                 null,
   guard_obj_name       VARCHAR              null,
   device_svg           XML                  null,
   plan_svg             XML                  null,
   is_ok                BOOL                 null,
   ok_datetime          TIMESTAMP            null,
   org_name             VARCHAR              null,
   org_uid              VARCHAR              null,
   constraint PK_DEV_OBJECT_Q primary key (id)
)
inherits (root_table);

comment on table dev_object_q is
'������������ ���������� ��������� � ���������� �������� (�� ���)';

select setMacToNULL('dev_object_q');
select createTriggerUID('dev_object_q');

/*==============================================================*/
/* Table: devices                                               */
/*==============================================================*/
create table devices (
   id                   SERIAL               not null,
   id_graph_plan        INT4                 null,
   name                 VARCHAR              null,
   svg_image            XML                  null,
   extra_id             INT4                 not null,
   is_completed         BOOL                 not null default FALSE,
   constraint PK_DEVICES primary key (id)
)
inherits (root_table);

comment on table devices is
'���������� (��������) ����������� ������ (�� ���)';

comment on column devices.id_graph_plan is
'��� ���� �������� ��������������, ��������� ������ ������ ��������, ����� �������� ����������, � ���������� � ����. ����� � ��� ���. ��� ����������, ��������, ��� ������������ ����� ����������� ������� (������� �� �������).';

select setMacToNULL('devices');
select createTriggerUID('devices');

/*==============================================================*/
/* Table: document_kinds                                        */
/*==============================================================*/
create table document_kinds (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_DOCUMENT_KINDS primary key (id)
)
inherits (root_table);

comment on table document_kinds is
'���� ���������� (��� ����������������)';

select setMacToNULL('document_kinds');
select createTriggerUID('document_kinds');

/*==============================================================*/
/* Table: document_subtypes                                     */
/*==============================================================*/
create table document_subtypes (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_DOCUMENT_SUBTYPES primary key (id)
)
inherits (root_table);

comment on table document_subtypes is
'������� ���������� (��� ����������������)';

select setMacToNULL('document_subtypes');
select createTriggerUID('document_subtypes');

/*==============================================================*/
/* Table: document_types                                        */
/*==============================================================*/
create table document_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_DOCUMENT_TYPES primary key (id)
)
inherits (root_table);

comment on table document_types is
'���� ���������� (��� ����������������)';

select setMacToNULL('document_types');
select createTriggerUID('document_types');

/*==============================================================*/
/* Table: element_shapes                                        */
/*==============================================================*/
create table element_shapes (
   id                   SERIAL               not null,
   id_shape_type        INT4                 not null,
   id_device            INT4                 not null,
   id_graph_style       INT4                 null,
   is_closed            BOOL                 not null default TRUE,
   constraint PK_ELEMENT_SHAPES primary key (id)
)
inherits (root_table);

comment on table element_shapes is
'������, �������� ���������� ���������� �� ����. ����� (�� ���)';

select setMacToNULL('element_shapes');
select createTriggerUID('element_shapes');

/*==============================================================*/
/* Table: employee_calendar                                     */
/*==============================================================*/
create table employee_calendar (
   calendar_id          int                  not null,
   employee_id          text                 not null,
   constraint AK_KEY_1_EMPLOYEE_CALENDAR unique (calendar_id, employee_id)
);

/*==============================================================*/
/* Table: employee_day                                          */
/*==============================================================*/
create table employee_day (
   employee_id          text                 not null,
   date                 date                 not null,
   start_work           time                 not null,
   stop_work            time                 not null,
   last_active          time                 not null,
   status               text                 not null,
   description          text                 not null,
   constraint AK_KEY_1_EMPLOYEE_DAY unique (employee_id, date)
);

/*==============================================================*/
/* Index: status                                                */
/*==============================================================*/
create  index status on employee_day (
status
);

/*==============================================================*/
/* Table: fault_devices                                         */
/*==============================================================*/
create table fault_devices (
   id                   SERIAL               not null,
   id_device            INT4                 not null,
   fault_state_name     VARCHAR              not null,
   fault_state_value    VARCHAR              not null,
   reg_datetime         TIMESTAMP            not null,
   is_completed         BOOL                 not null default FALSE,
   constraint PK_FAULT_DEVICES primary key (id)
)
inherits (root_table);

comment on table fault_devices is
'���������� ����������� ��������� (�� ���)';

select setMacToNULL('fault_devices');
select createTriggerUID('fault_devices');

/*==============================================================*/
/* Table: fault_devices_q                                       */
/*==============================================================*/
create table fault_devices_q (
   id                   SERIAL               not null,
   id_device_p          INT4                 null,
   dev_name             VARCHAR              null,
   fault_state_name     VARCHAR              null,
   fault_state_value    VARCHAR              null,
   reg_datetime         TIMESTAMP            null,
   org_name             VARCHAR              null,
   org_uid              VARCHAR              null,
   is_deleted           BOOL                 not null default FALSE,
   constraint PK_FAULT_DEVICES_Q primary key (id)
)
inherits (root_table);

comment on table fault_devices_q is
'������������ ���������� ����������� ���������';

comment on column fault_devices_q.is_deleted is
'�� ������ ������� ������ �� ���������. � ������ ���������� ��� ��������� � ������, ���� � ��������� ������ �� ������ ����������� ��������� �� ����������� ��������.';

select setMacToNULL('fault_devices_q');
select createTriggerUID('fault_devices_q');

/*==============================================================*/
/* Table: graph_plans                                           */
/*==============================================================*/
create table graph_plans (
   id                   SERIAL               not null,
   id_parent            INT4                 null,
   name                 VARCHAR              not null,
   bg_image             VARCHAR              null,
   svg_image            XML                  null,
   extra_id             INT4                 not null,
   is_completed         BOOL                 not null default FALSE,
   constraint PK_GRAPH_PLANS primary key (id)
)
inherits (root_table);

comment on table graph_plans is
'���������� ����. ������ (�� ���)';

select setMacToNULL('graph_plans');
select createTriggerUID('graph_plans');

/*==============================================================*/
/* Table: graph_styles                                          */
/*==============================================================*/
create table graph_styles (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_GRAPH_STYLES primary key (id)
)
inherits (root_table);

comment on table graph_styles is
'���������� ������ ����������� ����������� ���������� (�� ���)';

select setMacToNULL('graph_styles');
select createTriggerUID('graph_styles');

/*==============================================================*/
/* Table: groups                                                */
/*==============================================================*/
create table groups (
   id                   SERIAL not null,
   id_parent            INT4                 null,
   operation            INT4                 not null,
   is_not               BOOL                 not null default FALSE,
   constraint PK_GROUPS primary key (id)
)
inherits (root_table);

comment on column groups.operation is
'0 - ���
1 - �

��������, ����������� � ��������� � ��������� ������� ������� ������ ���������.';

comment on column groups.is_not is
'������ ���� ������, ����������� �� ������� �������� ��������� � ������ ������ ���������';

select setMacToNULL('groups');
select createTriggerUID('groups');

/*==============================================================*/
/* Table: groups_criteria                                       */
/*==============================================================*/
create table groups_criteria (
   id_group             INT4                 not null,
   id_criteria          INT4                 not null,
   constraint PK_GROUPS_CRITERIA primary key (id_group, id_criteria)
);

select setMacToNULL('groups_criteria');

/*==============================================================*/
/* Table: guard_objects                                         */
/*==============================================================*/
create table guard_objects (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   guard_datetime       TIMESTAMP            null,
   extra_id             INT4                 not null,
   is_completed         BOOL                 not null default FALSE,
   constraint PK_GUARD_OBJECTS primary key (id)
)
inherits (root_table);

comment on table guard_objects is
'���������� ���������� �������� (�� ���)';

select setMacToNULL('guard_objects');
select createTriggerUID('guard_objects');

/*==============================================================*/
/* Table: guard_objects_devices                                 */
/*==============================================================*/
create table guard_objects_devices (
   id_guard_objects     INT4                 not null,
   id_device            INT4                 not null,
   constraint PK_GUARD_OBJECTS_DEVICES primary key (id_guard_objects, id_device)
);

comment on table guard_objects_devices is
'����� ��������� ����������� ���������, �������� � ������ ������ (�� ���)';

select setMacToNULL('guard_objects_devices');

/*==============================================================*/
/* Table: handler_params                                        */
/*==============================================================*/
create table handler_params (
   id                   SERIAL               not null,
   name                 VARCHAR              null,
   handler_version      VARCHAR              not null,
   h_host               VARCHAR              null,
   h_port               int4                 null,
   service              VARCHAR              not null,
   extra_params         VARCHAR              null,
   handler_in_data      VARCHAR              null,
   constraint PK_HANDLER_PARAMS primary key (id)
)
inherits (root_table);

comment on table handler_params is
'��������� ��������-������������
������������ ������������. 
��������� ���������� ������� � ��������� ������� ��������� �������� ��� ��������� ������� ��������� ������������� ������, � ����� ������������ ������� ��� ��������� ������';

comment on column handler_params.name is
'�������� �������';

comment on column handler_params.handler_version is
'������ �������';

comment on column handler_params.h_host is
'IP-����� �����, �� ������� ������������� ������-����������';

comment on column handler_params.h_port is
'���� �����, �� ������� ������������� ������-����������';

comment on column handler_params.service is
'��������� ������� (��������� �������)
������ ������ ������ ����� ��������� ��. ���������:
-������������� ������ ������� ��������, �������������� ����������
������ ������ ��� ��� ������ ������:
-���������� ������� ������ � ������� ��������, ����� �� ����� ���������
-���������� ������� ������ � ������� ��������, ����� �� �������� ���������
-������ ��������� ��������� � �������� ������� (� ����� ������- � �������)
';

comment on column handler_params.extra_params is
'�������������� ��������� ��� ������� (���� ���������)';

comment on column handler_params.handler_in_data is
'XML-������� �������';

select setMacToNULL('handler_params');
select createTriggerUID('handler_params');

/*==============================================================*/
/* Table: handlers                                              */
/*==============================================================*/
create table handlers (
   id                   SERIAL               not null,
   id_handler_params    INT4                 null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   is_external          BOOL                 not null default FALSE,
   constraint PK_HANDLERS primary key (id)
)
inherits (root_table);

comment on table handlers is
'�������-����������� ��, ������� ������ � ������������ ������� ���������.
��� ������ ������� ����� ���� ��������� ������ ���� ������-����������.
������-���������� ������������ ����� ���������� ������������, ������� �������� �� ���� ������������� ������ � ������� �������� � ���������� � ���������� ��, ������������� ������ ���������������.';

comment on column handlers.name is
'��������';

comment on column handlers.description is
'��������';

comment on column handlers.is_external is
'���� ����������, �������� �� ������-���������� ������� ������� �� ��������� � �� ���������� ��� �� ������� �������� ���������� ��. ';

select setMacToNULL('handlers');
select createTriggerUID('handlers');

/*==============================================================*/
/* Table: histogram_graphics_chains                             */
/*==============================================================*/
create table histogram_graphics_chains (
   id                   SERIAL               not null,
   id_histogram_params_chains INT4                 not null,
   h_order              INT4                 not null,
   h_x                  float8               not null,
   h_y                  float8               not null,
   constraint PK_HISTOGRAM_GRAPHICS_CHAINS primary key (id)
)
inherits (root_table);

comment on table histogram_graphics_chains is
'������� �������� ���������� ��� ��������';

select setMacToNULL('histogram_graphics_chains');
select createTriggerUID('histogram_graphics_chains');

/*==============================================================*/
/* Table: histogram_graphics_streams                            */
/*==============================================================*/
create table histogram_graphics_streams (
   id                   SERIAL               not null,
   id_histogram_params_streams INT4                 not null,
   h_order              INT4                 not null,
   h_x                  float8               not null,
   h_y                  float8               not null,
   constraint PK_HISTOGRAM_GRAPHICS_STREAMS primary key (id)
)
inherits (root_table);

comment on table histogram_graphics_streams is
'������� �������� ����������';

select setMacToNULL('histogram_graphics_streams');
select createTriggerUID('histogram_graphics_streams');

/*==============================================================*/
/* Table: histogram_params_chains                               */
/*==============================================================*/
create table histogram_params_chains (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   h_min                float8               null,
   h_max                float8               null,
   h_count              int4                 not null,
   scenarios            INT4[]               null,
   variants             INT4[]               null,
   io_categories        INT4[]               null,
   services             INT4[]               null,
   constraint PK_HISTOGRAM_PARAMS_CHAINS primary key (id)
)
inherits (root_table);

comment on table histogram_params_chains is
'������� �������� ��� ���������� ���������� �� ��������';

select setMacToNULL('histogram_params_chains');
select createTriggerUID('histogram_params_chains');

/*==============================================================*/
/* Table: histogram_params_streams                              */
/*==============================================================*/
create table histogram_params_streams (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   h_min                float8               null,
   h_max                float8               null,
   h_count              int4                 not null,
   scenarios            INT4[]               null,
   variants             INT4[]               null,
   io_categories        INT4[]               null,
   io_objects           INT4[]               null,
   partition_lows       INT4[]               null,
   constraint PK_HISTOGRAM_PARAMS_STREAMS primary key (id)
)
inherits (root_table);

comment on table histogram_params_streams is
'������� �������� ��� ���������� ���������� �� ������� ���������';

select setMacToNULL('histogram_params_streams');
select createTriggerUID('histogram_params_streams');

/*==============================================================*/
/* Table: in_external_queue                                     */
/*==============================================================*/
create table in_external_queue (
   id                   BIGSERIAL            not null,
   id_organization      INT4                 not null,
   id_format            INT4                 not null,
   in_data              VARCHAR              not null,
   interaction_type     int4                 not null,
   interaction_result   int4                 not null,
   constraint PK_IN_EXTERNAL_QUEUE primary key (id)
)
inherits (root_table);

comment on table in_external_queue is
'�������� ������� ��� ������������� ��������� �������������� �������� (������� ������������) �� ������� ����������������� ������ (�� DynamicDocs)';

comment on column in_external_queue.id_organization is
'�����������, �� ������� ��������� ������';

comment on column in_external_queue.id_format is
'������ ������������� ������';

comment on column in_external_queue.in_data is
'�������� ������������� �������� ������';

comment on column in_external_queue.interaction_type is
'��� ��������������
1 - ������� ����� �������� �� ������� �������
2 - �������� �������� �� ������ �� ������� ������� (���� ��� �� ����������, ������� �����)
3 - ������� �������� �� ������� �������
';

comment on column in_external_queue.interaction_result is
'���������� ���������� ��������������� ������� � ��
1 - ������ ������ ���������, �� ��������� ��� �� ���������� (�������� �� ���������).
2 - �������� ��������������� ������� ����������� �������
3 - ��������� ������ ��� �������� ��������������� ������� � ��';

select setMacToNULL('in_external_queue');
select createTriggerUID('in_external_queue');

/*==============================================================*/
/* Table: indicator                                             */
/*==============================================================*/
create table indicator (
   id                   SERIAL               not null,
   id_indicator_type    INT4                 not null,
   id_parent            INT4                 null,
   id_a_type            INT4                 not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   table_name           VARCHAR              null,
   column_name          VARCHAR              null,
   id_ref_attr_type     INT4                 null,
   constraint PK_INDICATOR primary key (id)
)
inherits (root_table);

comment on table indicator is
'������� ����������� �������������� ��������.
��� ������������ ������ ����22 ������ ����� ��� ���������� ��� ���������� �������������� ��������.
���������� �� ������� ��������� ���, ��� ����� ����� ����� ����������� �������������. � ����� ��������� ������� ��������';

comment on column indicator.table_name is
'����������� ��� �����������, ������� �������� �� ������������. 
��� ������ ����� ����������� - ������ ';

comment on column indicator.column_name is
'����������� ��� �����������, ������� �������� �� ������������. 
��� ������ ����� ����������� - ������ ��� ��������/�������.
� ��������� ������ - table_name �� �����������';

select setMacToNULL('indicator');
select createTriggerUID('indicator');

/*==============================================================*/
/* Table: indicator_type                                        */
/*==============================================================*/
create table indicator_type (
   id                   SERIAL               not null,
   name                 VARCHAR              null,
   constraint PK_INDICATOR_TYPE primary key (id)
)
inherits (root_table);

select setMacToNULL('indicator_type');
select createTriggerUID('indicator_type');

/*==============================================================*/
/* Table: indicators_values                                     */
/*==============================================================*/
create table indicators_values (
   id                   SERIAL               not null,
   id_io_object         INT4                 not null,
   id_indicator         INT4                 not null,
   id_io_object_src     INT4                 null,
   id_io_object_src1    INT4                 null,
   meas_time            TIMESTAMP            not null,
   insert_time          TIMESTAMP            not null,
   the_value            VARCHAR              not null,
   start_time           TIMESTAMP            not null,
   stop_time            TIMESTAMP            null,
   constraint PK_INDICATORS_VALUES primary key (id)
)
inherits (root_table);

comment on table indicators_values is
'�������� ����������� ��� �������������� ��������';

comment on column indicators_values.id_io_object_src is
'������������� ��, ������� ������ ���������� ���������� � �������� ����������.';

comment on column indicators_values.id_io_object_src1 is
'������������� �������, �� ���� �������� ����������. ���� �������� �������� ���������� ��� �� ����, �� �������� � ���� ���� ��������� �� ��������� � ���� <��������>';

comment on column indicators_values.meas_time is
'���� � ����� ��������� �������� ����������';

comment on column indicators_values.insert_time is
'���� � ����� ��������� �������� ���������� � �������';

comment on column indicators_values.the_value is
'�������� ����������';

comment on column indicators_values.start_time is
'���� � ����� ������ ������������ ����������';

comment on column indicators_values.stop_time is
'���� � ����� ��������� ������������ ����������';

select setMacToNULL('indicators_values');
select createTriggerUID('indicators_values');

/*==============================================================*/
/* Table: interaction_formats                                   */
/*==============================================================*/
create table interaction_formats (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_INTERACTION_FORMATS primary key (id)
)
inherits (root_table);

comment on table interaction_formats is
'������� ������������� ������ (�������������� ��������, ������� ������������), ������������ ��� �������������� � �������� ���������
��������:
1) unified XML format (��������������� ������ �������� �������������� �������� IRL)
2) param-value representation (�������������� ������, ������ ����������� ��������� � ���� ������ ���������, ����������� � ���� �������-��������� ������������� ��������). ������������ ��� �������� �������� ��������������. �������� �� ������������ ���������� ������ �� ��������������
3) shushun (������ ������������� ������ ���� ���� � ��� "���� 22")
4) internal (���������� ������ ������������� ������ ��� �������������� ����� ��������� DynamicDocs)
';

select setMacToNULL('interaction_formats');
select createTriggerUID('interaction_formats');

/*==============================================================*/
/* Table: io_categories                                         */
/*==============================================================*/
create table io_categories (
   id                   SERIAL not null,
   id_io_category_type  INT4                 not null,
   id_child             INT4                 null,
   id_child2            INT4                 null,
   id_io_state          INT4                 not null default 1,
   id_life_cycle        INT4                 null,
   is_main              BOOL                 not null default true,
   name                 VARCHAR              not null,
   code                 VARCHAR              not null,
   description          VARCHAR              null,
   is_system            BOOL                 not null default false,
   is_global            BOOL                 not null default FALSE,
   is_completed         INT4                 not null default 0
      constraint CKC_IS_COMPLETED_IO_CATEG check (is_completed in (0,1,2)),
   is_archived          BOOL                 not null default FALSE,
   constraint PK_IO_CATEGORIES primary key (id)
)
inherits (root_table);

comment on table io_categories is
'������� ��������� �������������� ��������. ������ ��������� �������� ������� ���������, ������� ������ ����� ������� ������ ���������. ����� ����, ��������� ����� ����� �������� ���������, ������� ���������� ��������� �������, ������� ����� ��������� ������ ������� ������ ���������, ���� ������ �������� ������������ (�.�. �������� ���������� ��������������� �������). �������� ������ ������� �������� ������� � �����������.
���� �� �� �������� ������������, �� ������ ���� (id_child) ������ ���� ����� (������ ���� ������ ���� �����, �� ������� ������ ��������� �� �������� �������������)
����� ���� ��������� ����� ����� ������ �������� ���������, ������� ��������� ����� ����������� ������������� ��������������� �������. ���������� ���������� �� ��������� �������� ������������ ������ � ����������� ������� ����������';

comment on column io_categories.id_child is
'� ���������, ����������� ���������� ����� ���� 2 ����������� ���������, ������ ��������� ������� (����� �������), ������ - ���������� ����� ���������������� ��������� (�����������), �������� ����� �������� ������ ������������.
������ ���� ��������� ��������� ��� ������� �������';

comment on column io_categories.id_child2 is
'� ���������, ����������� ���������� ����� ���� 2 ����������� ���������, ������ ��������� ������� (����� �������), ������ - ���������� ����� ���������������� ��������� (�����������), �������� ����� �������� ������ ������������.
������ ���� ��������� ��������� ��� �����������';

comment on column io_categories.id_io_state is
'��������� ��������� � ����� ������ �������������';

comment on column io_categories.id_life_cycle is
'��������� ���� �������������� �������� ������ ���������';

comment on column io_categories.is_global is
'������� ������������ ���������';

comment on column io_categories.is_completed is
'0 - ��������� ��������� � �������� ��������� (��������)
1 - ������� ��������. ����������� �������� ��������� ���������
2 - ������� ��������. ����������� �������� �������� ����� ���������';

comment on column io_categories.is_archived is
'�������� ��������� �� ������������ � ������. ��������� ����������.';

select setMacToNULL('io_categories');
select createTriggerUID('io_categories');

/*==============================================================*/
/* Index: i_cat_code                                            */
/*==============================================================*/
create unique index i_cat_code on io_categories using BTREE (
code
);

/*==============================================================*/
/* Table: io_category_types                                     */
/*==============================================================*/
create table io_category_types (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   r_name               VARCHAR              not null,
   is_qualifier         BOOL                 not null default FALSE,
   description          VARCHAR              null,
   constraint PK_IO_CATEGORY_TYPES primary key (id)
)
inherits (root_table);

comment on table io_category_types is
'���������� ����� ���������.
- ����������;
- ������;
- ��������;
- ��������;
- ���������;
- ������ ���������.
��� ������.


';

comment on column io_category_types.r_name is
'�������� ���� ��������� � ����������� ������';

select setMacToNULL('io_category_types');
select createTriggerUID('io_category_types');

/*==============================================================*/
/* Table: io_last_sync                                          */
/*==============================================================*/
create table io_last_sync (
   id                   SERIAL               not null,
   id_io_object         INT4                 not null,
   id_organization      INT4                 not null,
   min_period           INT4[2]              not null,
   last_sync            TIMESTAMP            not null,
   constraint PK_IO_LAST_SYNC primary key (id)
)
inherits (root_table);

comment on table io_last_sync is
'��������� ���������� � ����������� � ���� � ������� ��������� ������������� � ����������� ������� �������������';

comment on column io_last_sync.min_period is
'����������� ������ �������������';

comment on column io_last_sync.last_sync is
'���� � ����� ��������� �������������';

select setMacToNULL('io_last_sync');
select createTriggerUID('io_last_sync');

/*==============================================================*/
/* Table: io_objects                                            */
/*==============================================================*/
create table io_objects (
   id                   SERIAL not null,
   id_io_category       INT4                 not null,
   id_io_state          INT4                 not null default 1,
   id_io_type           INT4                 not null default 1,
   id_maclabel          INT4                 not null default 1,
   author               INT4                 null,
   id_sync_type         INT4                 not null default 1,
   id_owner_org         INT4                 null,
   name                 VARCHAR              not null,
   table_name           VARCHAR(256)         null,
   description          VARCHAR              null,
   information          TEXT                 not null,
   is_system            BOOL                 not null default false,
   insert_time          TIMESTAMP            not null default current_timestamp,
   is_completed         INT4                 not null default 0
      constraint CKC_IS_COMPLETED_IO_OBJEC check (is_completed in (0,1,2)),
   is_global            BOOL                 not null default FALSE,
   record_fill_color    INT8                 null,
   record_text_color    INT8                 null,
   id_search_template   INT4                 null,
   ref_table_name       VARCHAR              null,
   r_icon               VARCHAR              null,
   uuid_t               UUID                 not null,
   constraint PK_IO_OBJECTS primary key (id)
)
inherits (root_table);

comment on table io_objects is
'������� ���� �������������� �������� (���������, �����������, �������, ������ � ��.). ������ �� �������� � ��� ������� � � ��� �������� ���������� �������������. �� ����� ���� ������������ (��������, �������, ������, �����������), � ���� ������ �� ����� �������, ������� �������� ������. ������ �� �������� ��, ��� �������� ���������� ������������� ��. ����� �� ����� ����� ���������, ������� ��������� �� ����� ���������.
����� �� ����� ����� ��� ���� �� ����� ���������, ������� ���������� � ������ ������� (����-����� ��������, �����, ������� ��������� � �.�.)
������ ��������� �� �������� ���� �����������. �� �������� ��������� ����� "MACLABEL", ������� ����������� ��� �������� ������� io_objects.';

comment on column io_objects.id_io_state is
'������� ��������� ��������������� �������';

comment on column io_objects.id_maclabel is
'���� ����������� ��������������� �������';

comment on column io_objects.id_sync_type is
'��� �� � ����� ������ �������������';

comment on column io_objects.id_owner_org is
'�����������-�������� ��������������� �������. �������� ��� �������������� ������.
������ �����������-�������� ����� ���� ����������� ��������������� ������.';

comment on column io_objects.table_name is
'�������� �������, ������� �������� ������ ��������������� �������, ���� ������ �������� ������������. ��������, ������ ��� ����������. ���� �������� �� �������� ������������ ������ ���� ������ ���� NULL';

comment on column io_objects.description is
'������� �������������� �������';

comment on column io_objects.information is
'����������� �������������� �������';

comment on column io_objects.is_system is
'���� ���������� �������� �� ������ �� ���������, �.�. ���������� ������ � ��.
��� ������� ��������� �� �������� �������������. ��������, 
- ���������� ����� ��������
- ���������� ����� ��������� ��������
- ���������� ��������� ����������
- ���������� ��������� ����������
- � �.�.
';

comment on column io_objects.is_completed is
'������ ���� �������� ��������� � ������������ ������ ����� ������ ��� ���������� ��������� ���������� ����������������� ������������� � ������������� ������������� � ����������� �������������� ��������.
����������� ����� ����������� ����������� � ���, ��� �������� �� �������� � ��������� ������� � �������� ��� ������ ��������� �������. � �����, ��������� ����� ������ � ������� io_objects ��� �� �������� ��������� ������ �� � ��. ����� �������� � ���. ��� �� �������� � ��, ����� ��� ������ �������� ���� ��������� ���������.

����� �������� ������� ���� � 0 �� 1 ������� � ���, ��� ��� �������� ��������� ������ � �� �������� � ��. 
����� ������� ���� � 0 �� 2 ������� � ���, ��� ������������ �� ��������� ��������� � ��� ��������� ���������. 
������ ������ �������� ������ ������� ioSetCompleted().
��� ������ ���������� ������ ���, ����� ���������� �������� ��.';

comment on column io_objects.ref_table_name is
'���� ������� ��������������� ������� ������������ �� ������ �������, �� � ���� ���� ����������� �������� ���������';

comment on column io_objects.r_icon is
'������ ��� ����������� ��������������� ������� � ������ ��� � �����������. ���� �� ������, �� ������������ �������� �� ��������� (������������ ���������� �����������). ��� ����������� � ����������� ������������, ���� �� ������ ������ � ��������������� ������ io_rubricator';

comment on column io_objects.uuid_t is
'���������� ������������� ��';

select createTriggerUID('io_objects');
alter table io_objects alter column uuid_t set default uuid_generate_v1();

/*==============================================================*/
/* Index: i_category                                            */
/*==============================================================*/
create  index i_category on io_objects using BTREE (
id_io_category
);

/*==============================================================*/
/* Index: i_author                                              */
/*==============================================================*/
create  index i_author on io_objects using BTREE (
author
);

/*==============================================================*/
/* Index: i_io_table                                            */
/*==============================================================*/
create unique index i_io_table on io_objects using BTREE (
table_name
);

/*==============================================================*/
/* Table: io_objects_organization                               */
/*==============================================================*/
create table io_objects_organization (
   id_organization      INT4                 not null,
   id_io_objects        INT4                 not null,
   constraint PK_IO_OBJECTS_ORGANIZATION primary key (id_organization, id_io_objects)
);

comment on table io_objects_organization is
'�����������, � �������� �������������� ������������� ������� ��';

select setMacToNULL('io_objects_organization');

/*==============================================================*/
/* Index: i_io_obj_org_io                                       */
/*==============================================================*/
create  index i_io_obj_org_io on io_objects_organization using BTREE (
id_io_objects
);

/*==============================================================*/
/* Table: io_processing_order                                   */
/*==============================================================*/
create table io_processing_order (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   id_state_src         INT4                 null,
   id_state_dest        INT4                 not null,
   id_io_category       INT4                 not null,
   constraint PK_IO_PROCESSING_ORDER primary key (id)
)
inherits (root_table);

comment on table io_processing_order is
'���������� ������� ��������� �������������� �������� � ��������� ���������� ���������� �����';

comment on column io_processing_order.name is
'��������';

comment on column io_processing_order.id_state_src is
'�� ������ (id_state_src) ��������� � ����� (id_state_dest) ��������� ��, ������� ������ �������������� ������ ��������';

comment on column io_processing_order.id_state_dest is
'�� ������ (id_state_src) ��������� � ����� (id_state_dest) ��������� ��, ������� ������ �������������� ������ ��������';

comment on column io_processing_order.id_io_category is
'�� ����� ��������� �������������� ������ ��������';

select setMacToNULL('io_processing_order');
select createTriggerUID('io_processing_order');

/*==============================================================*/
/* Table: io_processing_order_chains                            */
/*==============================================================*/
create table io_processing_order_chains (
   id_io_processing_order INT4                 not null,
   id_chains            INT4                 not null,
   constraint PK_IO_PROCESSING_ORDER_CHAINS primary key (id_io_processing_order, id_chains)
);

comment on table io_processing_order_chains is
'�������� ��������, � ������� ������ ���� �������� �� � ��� ��� ��������� �� ���������';

comment on column io_processing_order_chains.id_io_processing_order is
'������� �������� �� � �������';

comment on column io_processing_order_chains.id_chains is
'�������, � ������� ������ ���� ������� ��, ��������������� ��������, ��������� � ������� ���������';

select setMacToNULL('io_processing_order_chains');

/*==============================================================*/
/* Table: io_rubricator                                         */
/*==============================================================*/
create table io_rubricator (
   id_io_object         INT4                 not null,
   id_rubric            INT4                 not null,
   is_automated         BOOL                 null default FALSE,
   r_icon               VARCHAR              null,
   constraint PK_IO_RUBRICATOR primary key (id_io_object, id_rubric)
);

comment on column io_rubricator.is_automated is
'����, ������������, ����� ������� �������� �� � �������.
TRUE - ��� ������ ������ ���������� �������
FALSE - �������.
������������ ��� ������ ��������� �������������� ����������. ��� ��������� ���������� ������� �� ��������� �� � ������ ������� �� ��, ������� �������� ������������� ����� �������������� �������.';

comment on column io_rubricator.r_icon is
'������ ��� ����������� �������� � ������ ������. ���� �� ������, �� ������������ ������ �� ��������� (������������ ���������� �����������)';

select setMacToNULL('io_rubricator');

/*==============================================================*/
/* Table: io_states                                             */
/*==============================================================*/
create table io_states (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   is_system            BOOL                 not null default FALSE,
   constraint PK_IO_STATES primary key (id)
)
inherits (root_table);

comment on table io_states is
'��������� �������������� �������� (���������)
�� � �������� ��������� � �������� ��������� ����� ��������� �� ��������� � ��������� �������.
�������� ������ ������� �������� � ������� chains
��� ���� ���� ��������� ��������� ���������, ������� ������ ���� ��� ���������� �����.
-��������
-��������
-�������������� �������������� �������������
-�������������� �������������';

comment on column io_states.is_system is
'���� ���������� ���������. ��������� � ���������������� 2-5 �������� ���������� � �� ����� ���� ������������ ��� ���������� �������������� ��������';

select setMacToNULL('io_states');
select createTriggerUID('io_states');

/*==============================================================*/
/* Table: io_sync_archive                                       */
/*==============================================================*/
create table io_sync_archive (
   id                   SERIAL               not null,
   id_organization      INT4                 not null,
   id_io_object         INT4                 not null,
   sync_datetime        TIMESTAMP            not null,
   sync_result          INT4                 not null,
   constraint PK_IO_SYNC_ARCHIVE primary key (id)
)
inherits (root_table);

comment on table io_sync_archive is
'��������� ������� ����������� ������������� �������������� ��������';

comment on column io_sync_archive.sync_datetime is
'���� � ����� ���������� �������� �������������';

comment on column io_sync_archive.sync_result is
'��������� �������������';

select setMacToNULL('io_sync_archive');
select createTriggerUID('io_sync_archive');

/*==============================================================*/
/* Table: io_sync_types                                         */
/*==============================================================*/
create table io_sync_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_IO_SYNC_TYPES primary key (id)
)
inherits (root_table);

comment on table io_sync_types is
'���� �������������� �������� � ����� ������ �������������
���������� 4 ����:
- ���������� ����������������� �������
- ���������� ���������
- ���������� ����������� �������, �������������
- ���������� ����������� �������, ����������';

select setMacToNULL('io_sync_types');
select createTriggerUID('io_sync_types');

/*==============================================================*/
/* Table: io_templates                                          */
/*==============================================================*/
create table io_templates (
   id                   SERIAL not null,
   id_io_category       INT4                 not null,
   name                 VARCHAR              not null,
   code                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_IO_TEMPLATES primary key (id)
)
inherits (root_table);

comment on table io_templates is
'������� ��������, ������� ������ ��� ��� ��� ���� ���������. 
������ ���������� ������������� � ��������� ��������� ��� �������� ���������. 
� �������� "��������-�������������" ��������� - ��� ��������, � ������ - ��� �������������';

select setMacToNULL('io_templates');
select createTriggerUID('io_templates');

/*==============================================================*/
/* Index: i_templ_code                                          */
/*==============================================================*/
create unique index i_templ_code on io_templates using BTREE (
code
);

/*==============================================================*/
/* Table: io_types                                              */
/*==============================================================*/
create table io_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_IO_TYPES primary key (id)
)
inherits (root_table);

comment on table io_types is
'���� �������������� ��������';

select setMacToNULL('io_types');
select createTriggerUID('io_types');

/*==============================================================*/
/* Table: io_urls                                               */
/*==============================================================*/
create table io_urls (
   id                   SERIAL not null,
   id_url_type          INT4                 not null,
   url                  VARCHAR              not null default 'not assigned',
   name                 VARCHAR              not null,
   src_ext              VARCHAR              null,
   uploaded             BOOL                 not null default FALSE,
   hash_sum             VARCHAR              null,
   constraint PK_IO_URLS primary key (id)
)
inherits (root_table);

comment on table io_urls is
'���� �� �������� ���������� � � ���� ����������� �����, �� ������ ������� �������� ������ ����� ��� ������� ��
URL �������� ������������ �������� ������� ������� ��
��� ���� url ������ PARTIAL UNIQUE ������ (�� ��������� ����� ������ � ���������� URL, ������ ����� ������������ ����� ������� � ��� �� �������� URL, ������� ������������ ��������� ''not assigned'')';

comment on column io_urls.src_ext is
'�������� ���������� �����, ������� � ���� ���� ����� ���, ��� �� ��� ������������ �� ������.
��������. ���� �������������� ���� file.doc, �� �� ������� �� ����� ������� � ��������� � ����� ��������� (kks_file_1, ��������, �.�. ��� ����������), � � ��� ���� ��������� �������� ''doc''
��� ����� ��� ����, ����� ��������� ����� �� ������ � �����������.';

comment on column io_urls.uploaded is
'������ ���� ����������, �������� �� ���� �� ������ ��� ���.
��� ������������� ������� ���, ��� ����� �� ������ ����������� ������� (�������� ��� �������� ��� �������������� ������). � ��������� ������� ������ ����� ���� ����� ��������, �� ������� �� �������� �� ������ ����� �������� ���������� �����, � ������� �������� ����, ����������, ��� �� ����� � ������������� (� ��� ��� �� ��� ������). � ��� �� ����� �������� ���� ��������� � ������� �����, � ��� ���� ����� �������, ������������, ����� �� ���� ��� ������������ ��� ���.
�����, ������� ������������ ��� ������, � ������� ������ ��������������� ������� ���������� �� ������.';

comment on column io_urls.hash_sum is
'���-����� �����';

select setMacToNULL('io_urls');
select createTriggerUID('io_urls');

/*==============================================================*/
/* Index: i_url                                                 */
/*==============================================================*/
create unique index i_url on io_urls using BTREE (
url
)
where url <> '' and url <> 'not assigned';

/*==============================================================*/
/* Table: io_views                                              */
/*==============================================================*/
create table io_views (
   id                   SERIAL               not null,
   id_attr_category     INT4                 not null,
   id_io_template       INT4                 not null,
   id_a_group           INT4                 not null,
   is_read_only         BOOL                 not null default false,
   "order"              INT4                 not null,
   def_value            VARCHAR              null,
   constraint PK_IO_VIEWS primary key (id)
)
inherits (root_table);

comment on table io_views is
'�������� ��������� � ������� ��������� �� �������
� �������� ������� ������� ����� ���� read_only, ���� ���� �� �� read_only � ������� attrs_categories.
���� � ������� attrs_categories ������� �������� ��� read_only, �� �������� ������� ���� ������������';

comment on column io_views."order" is
'������� ���������� �������� � ������';

select setMacToNULL('io_views');
select createTriggerUID('io_views');

/*==============================================================*/
/* Index: i_iv_cat_template                                     */
/*==============================================================*/
create unique index i_iv_cat_template on io_views (
id_attr_category,
id_io_template
);

/*==============================================================*/
/* Index: i_iv_template                                         */
/*==============================================================*/
create  index i_iv_template on io_views (
id_io_template
);

/*==============================================================*/
/* Table: jr_states                                             */
/*==============================================================*/
create table jr_states (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_JR_STATES primary key (id)
)
inherits (root_table);

comment on table jr_states is
'������� ��������� ��������� ������� � ������� ������� ���������, � ����� �������� ������������
������ ����� ������� � �������� ����� ����:
- ������������
- ������ �������
- �����������
- ��������
- ���������
- �������
';

select setMacToNULL('jr_states');
select createTriggerUID('jr_states');

/*==============================================================*/
/* Table: kks_roles                                             */
/*==============================================================*/
create table kks_roles (
   id                   SERIAL not null,
   role_name            VARCHAR              not null,
   with_inheritance     BOOL                 not null default true,
   constraint PK_KKS_ROLES primary key (id)
);

comment on table kks_roles is
'�������� ������������ �������� ��� ��������� ������:
1) users
2) position
3) units

������ ������ � ��� ������� ���������';

comment on column kks_roles.with_inheritance is
'������ ������� ���������� ������������� ���� �� ������������ ����.
��������, ������������ ��������� ����� ������������ ����, � �������� �������� �� �������� � ��.
��� ����������� ���� ��������� ����� �������������, � ������� ��� ������.';

select setMacToNULL('kks_roles');

/*==============================================================*/
/* Table: life_cycle                                            */
/*==============================================================*/
create table life_cycle (
   id                   SERIAL               not null,
   id_start_state       INT4                 null,
   id_auto_state_attr   INT4                 null,
   id_auto_state_ind    INT4                 null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_LIFE_CYCLE primary key (id)
)
inherits (root_table);

comment on table life_cycle is
'��������� ����
���������� ����� ��������� ��, � ����� ������������������ ��������� ����� ����
�� �������� ��������������� ���������. �������������� �� ����� ����� ������ �� ���������, ������� ���������� � �� ��� ���������.';

comment on column life_cycle.id_start_state is
'��������� ���������, � ������� ��������� �� �������� ���������, ������� ������ ��������� ����';

comment on column life_cycle.id_auto_state_attr is
'����������� ��������� ��� ��������� ���������';

comment on column life_cycle.id_auto_state_ind is
'����������� ��������� ��� ��������� �����������';

select setMacToNULL('life_cycle');
select createTriggerUID('life_cycle');

/*==============================================================*/
/* Index: i_lc_start_state                                      */
/*==============================================================*/
create  index i_lc_start_state on life_cycle (
id_start_state
);

/*==============================================================*/
/* Table: life_cycle_io_states                                  */
/*==============================================================*/
create table life_cycle_io_states (
   id_life_cycle        INT4                 not null,
   id_io_states         INT4                 not null,
   constraint PK_LIFE_CYCLE_IO_STATES primary key (id_life_cycle, id_io_states)
);

comment on table life_cycle_io_states is
'����� ���������� ��������� � ������ ��������� �����.
����� ���������, ��� ��������� id_start_state � ������� ��������� ������ ����� ������ ����������� � ����� ���������� ���������, ���� ��� �� ������������ � ���� �������. ��� ���� ��� �������� ��� ����� id_start_state �� ������ ������� �� �� ���������.
������ ������� ���������� ���� ����� ���������� ���������.
������������������ ��������� ����� ����������� ������������ �������� ���������';

comment on column life_cycle_io_states.id_life_cycle is
'��������� ����';

comment on column life_cycle_io_states.id_io_states is
'���������, � ���� ��������';

select setMacToNULL('life_cycle_io_states');

/*==============================================================*/
/* Index: i_lc_s_key                                            */
/*==============================================================*/
create unique index i_lc_s_key on life_cycle_io_states (
id_life_cycle,
id_io_states
);

/*==============================================================*/
/* Table: log                                                   */
/*==============================================================*/
create table log (
   id                   SERIAL not null,
   id_io_object         INT4                 not null,
   id_user              INT4                 null,
   id_operation_type    INT4                 not null
      constraint CKC_ID_OPERATION_TYPE_LOG check (id_operation_type in (1,2,3,4,5)),
   datetime             TIMESTAMP            not null,
   constraint PK_LOG primary key (id)
);

comment on table log is
'������ ����� ������ ������������� � ��.
� ������ ������� �������� ���������, ������� ���� ������� �������������� � ��������������� ���������.
��������� ����� ��������� ����� �������:
- ���������� ����� ������ � ������ ����� �������� ������������
- ���������� ����� ������ � ������ ������� ���������
- �������� ���������
- �������� �����������
- �������� ����� ������ �����������
- �������������� ������ �����������/������/�������
- � �.�.';

select setMacToNULL('log');

/*==============================================================*/
/* Table: logistic                                              */
/*==============================================================*/
create table logistic (
   id                   SERIAL               not null,
   name                 VARCHAR              null,
   id_processing_scenario INT4                 not null,
   id_processing_variant INT4                 not null,
   id_io_object         INT4                 not null,
   id_handler           INT4                 not null,
   return_code          INT4                 not null,
   handler_in_data      VARCHAR              null,
   handler_out_data     VARCHAR              null,
   description          VARCHAR              null,
   constraint PK_LOGISTIC primary key (id)
)
inherits (root_table);

comment on table logistic is
'����������� �������, �������������� ���������� ������ ��������� �� ��������� ��������� �������� ��������������� �������� (�.�. �������������� ����� ����������� ��� ���������� ��������� �������� � ����������� �� ���������� ��������� ������� ��������)

������ ������� ���������� ����� ��������� �����:
���� �������� �������� ��������� ��������������� �������� ��������� � � chains_data ��������� �������� ��������� �������� �� (���), � ��� ���� ��� �������� ����� ���������, �� ���������� ������ �� (���) ��������� � ��������� ������� (����� ��������)';

comment on column logistic.id_processing_scenario is
'�������� ��������� (��� ��������� ��������� ����� ���� ��������� ������������������)
';

comment on column logistic.id_processing_variant is
'������� ���������';

comment on column logistic.id_io_object is
'��';

comment on column logistic.id_handler is
'������ (������ �� ������� �������� ��������)
';

comment on column logistic.handler_in_data is
'XML-��. ������� ������� (������� ��������� �������)';

comment on column logistic.handler_out_data is
'XML-������� ������� (������������ ���������)
';

select setMacToNULL('logistic');
select createTriggerUID('logistic');

/*==============================================================*/
/* Index: i_main_index                                          */
/*==============================================================*/
create unique index i_main_index on logistic (
id_processing_scenario,
id_processing_variant,
id_io_object,
id_handler,
return_code
);

/*==============================================================*/
/* Table: logistic_chains                                       */
/*==============================================================*/
create table logistic_chains (
   id_chains            INT4                 not null,
   id_logistic          INT4                 not null,
   constraint PK_LOGISTIC_CHAINS primary key (id_chains, id_logistic)
);

comment on table logistic_chains is
'�������� �������� ���
����������� ��������� ��.
(��� ������ � ����-�� ���� �������� ����� ���� ��������� �������� ���������)
';

select setMacToNULL('logistic_chains');

/*==============================================================*/
/* Table: maclabels                                             */
/*==============================================================*/
create or replace function checkForMaclabelType() returns int4 as
$BODY$
declare
    isExist int4;
begin

    select f_is_type_exist('maclabel') into isExist;
    if(isExist > 0) then
        return 1;
    end if;
    
    create type maclabel as(m1 int4, m2 int4);
    
    return 2;

end
$BODY$
language 'plpgsql';

select checkForMaclabelType();
create table maclabels (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   mac_value            MACLABEL             not null,
   constraint PK_MACLABELS primary key (id)
)
inherits (root_table);

comment on table maclabels is
'������ ���������� �������� ������ ������ ����������� �������������� ��������.';

select setMacToNULL('maclabels');
select createTriggerUID('maclabels');

/*==============================================================*/
/* Table: mail_lists                                            */
/*==============================================================*/
create table mail_lists (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_MAIL_LISTS primary key (id)
)
inherits (root_table);

comment on table mail_lists is
'������ �������� ����������� ����� ��������� � ������������.
� ��� ����� ������ ������ ������������ ��� ��������� ������� ���� ������� � ��. 
������ ��������� ���������� ������������ ������ � ����� ��������� ������ �������������';

select setMacToNULL('mail_lists');
select createTriggerUID('mail_lists');

/*==============================================================*/
/* Table: mail_lists_position                                   */
/*==============================================================*/
create table mail_lists_position (
   id_position          INT4                 not null,
   id_mail_lists        INT4                 not null,
   constraint PK_MAIL_LISTS_POSITION primary key (id_position, id_mail_lists)
);

comment on table mail_lists_position is
'�������� ��, �������� � ������ ��������';

/*==============================================================*/
/* Table: message_journal                                       */
/*==============================================================*/
create table message_journal (
   id                   SERIAL not null,
   id_io_object         INT4                 null,
   id_dl_sender         INT4                 not null,
   id_dl_receiver       INT4                 not null,
   id_urgency_level     INT4                 not null default 1,
   insert_time          TIMESTAMP            not null,
   sent_datetime        TIMESTAMP            null,
   receive_datetime     TIMESTAMP            null,
   read_datetime        TIMESTAMP            null,
   message_body         VARCHAR              null,
   is_outed             int4                 not null default 3
      constraint CKC_IS_OUTED_MESSAGE_ check (is_outed in (0,1,2,3)),
   extra_id             int4                 null,
   input_number         VARCHAR              not null,
   output_number        VARCHAR              not null,
   constraint PK_MESSAGE_JOURNAL primary key (id)
)
inherits (root_table);

comment on table message_journal is
'������ ����� ���������
���������� ������� �� ���� ��������:
- ���
- ���
';

comment on column message_journal.id_io_object is
'������ �� ��, ������� ������������ ����� ���� ��������� (��������)';

comment on column message_journal.id_urgency_level is
'��� ���������';

comment on column message_journal.sent_datetime is
'����� �������� ���������';

comment on column message_journal.receive_datetime is
'����� ��������� ��������� �� ������� ����������';

comment on column message_journal.read_datetime is
'����� ��������� ��������� �����������';

comment on column message_journal.message_body is
'���� ���������. �� ������������ ����.
����� ��������� ����������������� �����';

comment on column message_journal.is_outed is
'����, ����������� �� ��, ��� ��������� ���������� ����������.
���� ������������ ��������� �������
0 - �� ����������
1 - ���������� ���������� � ���������
2 - ���������� ���������� � ���������
3 - �������� �� ���������

���� ��������� ��������� (�.�. ������������ �������������� ������� �� jupiter) 
������������� ���������� �� ������ ��, �� ��������� ��� �������� �� ��������� is_outed = 3

���� ��� ������������� ���������� ��, �� ��� ��������� �� ������������. (is_outed = 0)
� ���� ������ kksinteractor ��� �������� ���������� � ����� ��� �������� �������� �������� ����� ���� �������� 3

���� ��������� �������� �������� � ������������ �������� ��������� ��, �� is_outed = 0
� ���� ������ kksinteractor �������� ��������� ����������� � ������� �������� ���������.
����� �������� �������� ��������� is_outed ����� ���������� � 1.

����  ��������� �������� �������� � ������������ �������� ��������� �� � ���������� ������� ��������� ��� �����������, �� 
� ���� ������ kksinteractor �������� ��������� ����������� � ������� ��������� ���������.
����� �������� �������� ��������� is_outed ����� ���������� � 2.

�� ���� ��������� ������� is_outed = 3. ';

comment on column message_journal.extra_id is
'��������� ����.
���������������

������ ���� ������������ ��� ������������� ������.
������ ������������� ���������������� ��������� � �� ������� ������� (� �������� ��� ���� ��������).';

comment on column message_journal.input_number is
'�������� ����� ���������. ����������� ��� ��������. ��������� �� ��������';

comment on column message_journal.output_number is
'��������� ����� ��������� (����������� ������� � ��������� �������:
1) ��� ������� ��������� �� ������� ����������� (� ���� ������ �������� � ��������� ����� ��������).
2) ��� �������� �������� �� ������ ������� (� ���� ������ �������� � ��������� ����� �����������).

��������� �� ��������';

select setMacToNULL('message_journal');
select createTriggerUID('message_journal');

/*==============================================================*/
/* Table: message_series                                        */
/*==============================================================*/
create table message_series (
   id                   SERIAL               not null,
   id_message_stream    INT4                 not null,
   "time"               TIMESTAMP            not null,
   time_step            FLOAT8               not null default 1.0,
   constraint PK_MESSAGE_SERIES primary key (id)
)
inherits (root_table);

select setMacToNULL('message_series');
select createTriggerUID('message_series');

/*==============================================================*/
/* Table: message_streams                                       */
/*==============================================================*/
create table message_streams (
   id                   SERIAL               not null,
   id_partition_low     INT4                 not null,
   id_io_object         INT4                 not null,
   id_dl_sender         INT4                 not null,
   id_dl_receiver       INT4                 not null,
   id_time_unit         INT4                 not null default 1,
   id_processing_scenario INT4                 null,
   id_processing_variant INT4                 null,
   name                 VARCHAR              not null,
   lambda               FLOAT8               null default 0,
   sigma                FLOAT8               null default 0,
   moda                 FLOAT8               null default 0,
   min_p                FLOAT8               null default 0,
   max_p                FLOAT8               null default 0,
   start_time           TIMESTAMP            not null,
   stop_time            TIMESTAMP            not null,
   constraint PK_MESSAGE_STREAMS primary key (id)
)
inherits (root_table);

comment on table message_streams is
'����
���������� ��������� ������� ��������� (������������� ��������)';

comment on column message_streams.id_io_object is
'������ �� �������������� ������ ���� ����������, �� ���������  �������� <���������> ��������� ������ ����� ���������������� ������� ������� �����������, ��� ���������� �������� ������ ����������� ������������ ������������ �� ������ ������ � �.�.';

comment on column message_streams.id_dl_sender is
'�� ���� ��� ���� ������ SERIAL! ��� ����������� �� kks_roles. ����� �������� ������ ������� ������������ ������, ������� ������ ��� �������� �� ��������� � nextval(''kks_roles_id_seq'');';

comment on column message_streams.id_dl_receiver is
'����������� ����-�������  ������';

comment on column message_streams.id_processing_scenario is
'�������� ������� �������� �������� (�� ������������ ��)';

comment on column message_streams.id_processing_variant is
'�������� ������� �������� ������� (�� ������������ ��)';

comment on column message_streams.lambda is
'������ � ������ ������� ���������� ������ �������������';

comment on column message_streams.sigma is
'������ � ������ ������� ���������� ������ �������������';

comment on column message_streams.moda is
'������ � ������ ������� ���������� ������ �������������';

comment on column message_streams.min_p is
'������ � ������ ������� ���������� ������ �������������';

comment on column message_streams.max_p is
'������ � ������ ������� ���������� ������ �������������';

comment on column message_streams.start_time is
'���� � ����� ������ ��������� ��������� ������';

comment on column message_streams.stop_time is
'���� � ����� ���������� ��������� ������';

select setMacToNULL('message_streams');
select createTriggerUID('message_streams');

/*==============================================================*/
/* Table: mimetypes                                             */
/*==============================================================*/
create table mimetypes (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   type                 VARCHAR              not null,
   subtype              VARCHAR              not null,
   constraint PK_MIMETYPES primary key (id)
)
inherits (root_table);

comment on column mimetypes.name is
'����������� type + subtype';

select setMacToNULL('mimetypes');
select createTriggerUID('mimetypes');

/*==============================================================*/
/* Table: module                                                */
/*==============================================================*/
create table module (
   id_module            int                  not null,
   module               text                 not null,
   name                 text                 not null,
   constraint PK_MODULE primary key (id_module),
   constraint AK_KEY_2_MODULE unique (module)
);

/*==============================================================*/
/* Index: name                                                  */
/*==============================================================*/
create  index name on module (
name
);

/*==============================================================*/
/* Table: module_action                                         */
/*==============================================================*/
create table module_action (
   id_module_action     int                  not null,
   module_id            int                  not null,
   action               varchar(50)          not null,
   name                 varchar(50)          not null,
   public_flag          text                 not null,
   constraint PK_MODULE_ACTION primary key (id_module_action)
);

/*==============================================================*/
/* Index: module_id                                             */
/*==============================================================*/
create  index module_id on module_action (
module_id
);

/*==============================================================*/
/* Index: action                                                */
/*==============================================================*/
create  index action on module_action (
action
);

/*==============================================================*/
/* Index: name_module_action                                    */
/*==============================================================*/
create  index name_module_action on module_action (
name
);

/*==============================================================*/
/* Index: public_flag                                           */
/*==============================================================*/
create  index public_flag on module_action (
public_flag
);

/*==============================================================*/
/* Table: module_auto_action                                    */
/*==============================================================*/
create table module_auto_action (
   module_action_id     int                  not null,
   auto_enabled_action_id int                  not null,
   constraint AK_KEY_1_MODULE_AUTO_ACTION unique (module_action_id, auto_enabled_action_id)
);

/*==============================================================*/
/* Index: module_action_id                                      */
/*==============================================================*/
create  index module_action_id on module_auto_action (
module_action_id
);

/*==============================================================*/
/* Index: auto_enabled_action_id                                */
/*==============================================================*/
create  index auto_enabled_action_id on module_auto_action (
auto_enabled_action_id
);

/*==============================================================*/
/* Table: news                                                  */
/*==============================================================*/
create table news (
   news_id              SERIAL not null,
   news_time            varchar(5)           not null default '00:00',
   news_date            date                 not null default '2010-01-01',
   news_title           text                 not null,
   news_content         text                 not null,
   news_img             char(1)              null default 'n',
   news_exp             varchar(5)           not null default '.jpg',
   news_stamp           int                  not null default '0',
   constraint PK_NEWS primary key (news_id)
);

/*==============================================================*/
/* Table: news_config                                           */
/*==============================================================*/
create table news_config (
   id                   SERIAL not null,
   am_news              int                  not null,
   PreLengthC           int                  not null,
   tab_width            varchar(5)           not null,
   p_time               int                  not null,
   p_date               int                  not null,
   img_height           int                  not null,
   c_top                int                  not null,
   c_bottom             int                  not null,
   constraint PK_NEWS_CONFIG primary key (id)
);

/*==============================================================*/
/* Table: notify_where                                          */
/*==============================================================*/
create table notify_where (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_NOTIFY_WHERE primary key (id)
)
inherits (root_table);

comment on table notify_where is
'����� ��������� ������ ��������������
1 - INSERT
2 - UPDATE
3 - DELETE
������������ � ����������� ����������� ��������� � ���� �������� ���� ������ ��������';

select setMacToNULL('notify_where');
select createTriggerUID('notify_where');

/*==============================================================*/
/* Table: object_ref_tables                                     */
/*==============================================================*/
create table object_ref_tables (
   id                   SERIAL not null,
   table_name           varchar              not null,
   id_io_object         INT4                 not null,
   id_io_category       INT4                 not null,
   title                VARCHAR              not null,
   id_search_template   INT4                 null,
   ref_table_name       VARCHAR              null,
   constraint PK_OBJECT_REF_TABLES primary key (id)
)
inherits (root_table);

comment on table object_ref_tables is
'�������������� ������� ��������������� �������.
����������� ������ ��� �������������� �������� ���� "����������"';

comment on column object_ref_tables.table_name is
'�������� ���������� ������� ��, �������� ������ ������� �����������';

comment on column object_ref_tables.id_io_object is
'������������� ���������������� ��������������� �������';

comment on column object_ref_tables.id_io_category is
'���������, ������� ������ ��������������� ��������� �������������� �������';

comment on column object_ref_tables.title is
'�������� (���������) �������������� �������';

comment on column object_ref_tables.id_search_template is
'������������� ���������� �������, ������� ������������ ��� ������������ ������� ������ �������������� �������, ���� ���������� �����������';

comment on column object_ref_tables.ref_table_name is
'�������� ���������� ������� �� (�������� �����������), ������� ������������ ��� ������������ ������� ������ ���. ������� (��� ����������� ������������)';

select setMacToNULL('object_ref_tables');
select createTriggerUID('object_ref_tables');

/*==============================================================*/
/* Table: operations                                            */
/*==============================================================*/
create table operations (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   the_value            VARCHAR              not null,
   constraint PK_OPERATIONS primary key (id)
)
inherits (root_table);

comment on table operations is
'�������� ��� �������������� �������� �� ��������
=
>
<
>=
<=
like
!=';

select setMacToNULL('operations');
select createTriggerUID('operations');

/*==============================================================*/
/* Table: organization                                          */
/*==============================================================*/
create table organization (
   id                   SERIAL               not null,
   id_type_org          INT4                 not null,
   id_parent            INT4                 null,
   id_parent1           INT4                 null,
   id_substitutor       INT4                 null,
   id_curr_mode         INT4                 not null default 1,
   id_prev_mode         INT4                 not null default 1,
   id_boss              INT4                 null,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   code_name            VARCHAR              not null,
   email_prefix         VARCHAR              not null,
   latitude             FLOAT8               not null default 0,
   longitude            FLOAT8               not null default 0,
   map_symbol           VARCHAR              not null,
   tree_symbol          VARCHAR              not null,
   dt_prev_mode         TIMESTAMP            not null default CURRENT_TIMESTAMP,
   dt_curr_mode         TIMESTAMP            not null default CURRENT_TIMESTAMP,
   is_completed         BOOL                 not null default FALSE,
   is_created           INT4                 not null default 0
      constraint CKC_IS_CREATED_ORGANIZA check (is_created in (0,1,2)),
   is_main              bool                 not null default false,
   constraint PK_ORGANIZATION primary key (id)
)
inherits (root_table);

comment on table organization is
'�����������';

comment on column organization.id_parent is
'�������� (����������� ����������� - ��� ������������ ������������';

comment on column organization.id_parent1 is
'�������� (�����������) ����������� - �� ������ ������';

comment on column organization.id_substitutor is
'����������� ������� ���������� ������� ������ ����������� ��� ������ �� �� �����';

comment on column organization.id_curr_mode is
'������� ����� ���������������� �������(��������� �������)';

comment on column organization.id_boss is
'������������ �����������.
������ ���� �� �������� ������������, ��������� � ��������� ������ ����� ���������� ������� �����������, �� ������ ������������ (����������� ����).
������ ������ ������� �� ���� ����������� ���� ��� ������������� � �������������� ��� �����������';

comment on column organization.code_name is
'�������� ������������ (� ������������ �������� ����������� ������ ���� �������)';

comment on column organization.email_prefix is
'������� ����������� ����� �����������.
������ ���� ����������. �������� ���������� ��������� ���������� ������ �������� �� ��� ��������, ��� ������ ���� �������� ����������.
���������� ���������� ������ �� ������ ����';

comment on column organization.map_symbol is
'�������� ��� ����� �������, ���������� �� ����� (XPM)';

comment on column organization.tree_symbol is
'���� ����������� (�������) � ������ ���(JPG)';

comment on column organization.dt_curr_mode is
'�����  � ���� ����� �������� ������';

comment on column organization.is_completed is
'��������� ����, ������� ����������, ��� �������� � ����������� ����� ����������� ���������. ��� ����� �������� ������� ����� � FALSE �� TRUE ����������� �������� �������� ����� � ��������� ������������.
���� �������� ��� ������� ��� ������ ����������� ���� �� ������ ���������� � ���������� ������ (� ����������� organization_transport)';

comment on column organization.is_created is
'������ ���� ������ ������� (� ���������) �������� ����� � ������ ������������.
��������� ��������:
0 - ��������� �������� ����� (�������� �� ���������)
1 - �������� ����� ��������� �������
2 - �������� ����� ����������� � �������
�������� ����� ������ � �������������, ������� ����� 1 � �������� �������� ������� ����';

select setMacToNULL('organization');
select createTriggerUID('organization');

/*==============================================================*/
/* Index: i_email_prefix                                        */
/*==============================================================*/
create unique index i_email_prefix on organization using BTREE (
email_prefix
);

/*==============================================================*/
/* Table: organization_transport                                */
/*==============================================================*/
create table organization_transport (
   id                   SERIAL               not null,
   id_organization      INT4                 not null,
   id_transport         INT4                 not null,
   address              VARCHAR              not null,
   port                 INT4                 null,
   is_active            bool                 not null default TRUE,
   use_gateway          BOOL                 not null default FALSE,
   constraint PK_ORGANIZATION_TRANSPORT primary key (id)
)
inherits (root_table);

comment on table organization_transport is
'������������ ������������ ���������� �������������� ������

�����!!!
� ������, ���� ������� �� �������� ����������, � ������� replaceLocalOrg() ��������� ���������� ������ �� ���� address, port
� ������� �������� �� ��� ��������� ������, ��������� ��� �������������� ������������� �������� ������������ ������.

���� ������� �� �������, �� ���������� ������ �� ������ ���� ��������� � ������������';

comment on column organization_transport.address is
'����� ����������� � ������ ����������';

comment on column organization_transport.port is
'���� ������������ ������, ������� ������������ �� ������ �����������.
���� ������������ ������ � IP-�����, ���� ������������ ��������� http_connector';

comment on column organization_transport.use_gateway is
'������������ �� ���� ��� �������������� (��� � ��������� �����)';

select setMacToNULL('organization_transport');
select createTriggerUID('organization_transport');

/*==============================================================*/
/* Index: ot_unique_index                                       */
/*==============================================================*/
create unique index ot_unique_index on organization_transport using BTREE (
id_organization,
id_transport
);

/*==============================================================*/
/* Table: organization_type                                     */
/*==============================================================*/
create table organization_type (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   constraint PK_ORGANIZATION_TYPE primary key (id)
)
inherits (root_table);

comment on table organization_type is
'��� ����������� (������������, ���������, ������� �������������� �������)';

select setMacToNULL('organization_type');
select createTriggerUID('organization_type');

/*==============================================================*/
/* Table: organization_work_mode                                */
/*==============================================================*/
create table organization_work_mode (
   id_work_mode         INT4                 not null,
   id_organization      INT4                 not null,
   constraint PK_ORGANIZATION_WORK_MODE primary key (id_work_mode, id_organization)
);

comment on table organization_work_mode is
'������ ���������������� ����������� � ��������� ���� (�������������)';

select setMacToNULL('organization_work_mode');

/*==============================================================*/
/* Table: out_external_queue                                    */
/*==============================================================*/
create table out_external_queue (
   id                   BIGSERIAL            not null,
   id_organization      INT4                 not null,
   id_format            INT4                 not null,
   id_io_object         int8                 not null,
   id_entity            int8                 null,
   interaction_type     int4                 not null,
   interaction_result   int4                 not null,
   out_data             VARCHAR              not null,
   constraint PK_OUT_EXTERNAL_QUEUE primary key (id)
)
inherits (root_table);

comment on table out_external_queue is
'��������� ������� ��� ������������ �������� �������������� �������� (������� ������������) �� ������� ����������������� ������� (�� DynamicDocs)';

comment on column out_external_queue.id_organization is
'� ����� ����������� ����������';

comment on column out_external_queue.id_format is
'��� ������ ������ ������� ������������ ������';

comment on column out_external_queue.id_io_object is
'����� �������������� ������ ����������.

��� ���� ���� ���������� ��������� �� message_journal, 
�� ���� ������ ���� ������� ������������� �� "������ ���������" (��=20), 
� � ���� id_entity ������ ���� ������� ������������� ���������. 
�� ��� � ��� ������, ���� � ��������� �� ���������� ��';

comment on column out_external_queue.id_entity is
'���� ������� ��������, �� ���������� �� ��� �������������� ������, � ������ ��� ����������� (��� �������������� ��������, ���������� �������������)';

comment on column out_external_queue.interaction_type is
'��� ��������������
1 - ������� ����� �������� �� ������� �������
2 - �������� �������� �� ������ �� ������� ������� (���� ��� �� ����������, ������� �����)
3 - ������� �������� �� ������� �������
';

comment on column out_external_queue.interaction_result is
'���������� ��������������� �������������� (�������� ������)
1 - ��������� �������� (����� � ������� �� ��������). ��� �������� �� ���������.
2 - �������� �������������� (�������������� ����� ��� �� ��������)
3 - �������� ����������� �������
4 - ��������� ������';

comment on column out_external_queue.out_data is
'�������������� XML-�������� ������������ ��������';

select setMacToNULL('out_external_queue');
select createTriggerUID('out_external_queue');

/*==============================================================*/
/* Table: out_sync_queue                                        */
/*==============================================================*/
create table out_sync_queue (
   id                   BIGSERIAL            not null,
   id_organization      INT4                 not null,
   id_entity            INT8                 not null,
   entity_table         VARCHAR              not null,
   entity_type          INT4                 not null,
   sync_type            INT4                 not null,
   sync_result          INT4                 not null,
   entity_uid           varchar              not null,
   entity_io_uid        varchar              not null,
   constraint PK_OUT_SYNC_QUEUE primary key (id)
)
inherits (root_table);

comment on table out_sync_queue is
'������� ��������� ��������� ��������� ��� �������������.
�������� ��� ������������ jupiter

������� ������������ �� root_table. ��� ���� ���� last_update ������������ ��� ���� � ����� ��������� ������ � �������. ���������� ���� ���� � ���� � ������� �������� ��������.';

comment on column out_sync_queue.id_organization is
'�����������, � ������� �������������� �������������';

comment on column out_sync_queue.id_entity is
'������������� ������������ ��������.
������� �� ��������������� �������, �������� ������� �������� ����� entity_table.';

comment on column out_sync_queue.entity_table is
'�������� �������, � ������� ���������� ����������� ��������
- io_objects - ��� ��
- io_categories - ��� ���������
- ���� �������� ������� - ��� ���';

comment on column out_sync_queue.entity_type is
'��� ������������ ��������
1 - ��������� (���������). ������������ ������ � ����������.
2 - �� (���������).
3 - ��� (������ �����������). � ���� ������ ���������� ��������� �������� ������� ��, � ������� �������� ������ �����������.
4 - ��� (������ �������������� ������� �����������). � ���� ������ ���������� ��������� �������� ������� ��, � ������� �������� ������ �����������.
5 - �������������� ������� ����������� (���������� ������ � ����������, �� ������ ������� ��� ������� � UID ��������������� �������, � �������� ��� ���������)
6 - �������� ������� �� �������� �����
7 - ���������������
8 - �������� ������� ����������� ����������� �� ����� ������
9 - �������� ������ ����������� ����������� �� ��� ����������� � ���� ������ �������
10 - ��������� �� (��������, ����������)
11 - ��������� �� (��������)
12 - ����, ������������ ������� (��������, ����������)
13 - ����, ������������ ������� (��������)
';

comment on column out_sync_queue.sync_type is
'��� �������������
1 - ������� ����� �������� �� ������� �������
2 - �������� �������� �� ������ �� ������� ������� (���� ��� �� ����������, ������� �����)
3 - ������� �������� �� ������� �������';

comment on column out_sync_queue.sync_result is
'���������� ������������� ��� ������� ��������
1 - ��������� ������������� (����� � ������� �� ��������). ��� �������� �� ���������.
2 - ���������������� (�������������� ����� ��� �� ��������)
3 - ������������� ����������� �������
4 - ��������� ������';

comment on column out_sync_queue.entity_uid is
'���������� ������������� ������������ �������� (unique_id)';

comment on column out_sync_queue.entity_io_uid is
'���������� ������������� ��������������� �������, � ������� ��������� ������������ ��������.
��� ������� ������������ - ��� unique_id ��������������� �������, � �������� ��������� �������, ���������� ������������ ��������
��� ��������� - ��� unique_id ��������� (��� ��������� ������� ������ �� �����, �� �������� ��� ����������)
��� �� - ��� unique_id ��������������� ������� (��� ��������� ������� ������ �� �����, �� �������� ��� ����������)';

select setMacToNULL('out_sync_queue');
select createTriggerUID('out_sync_queue');

/*==============================================================*/
/* Index: i_sync_result                                         */
/*==============================================================*/
create  index i_sync_result on out_sync_queue using BTREE (
sync_result
);

/*==============================================================*/
/* Index: i_io_uid_sync_res                                     */
/*==============================================================*/
create  index i_io_uid_sync_res on out_sync_queue using BTREE (
sync_result,
entity_io_uid
);

/*==============================================================*/
/* Table: partition_lows                                        */
/*==============================================================*/
create table partition_lows (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_PARTITION_LOWS primary key (id)
)
inherits (root_table);

comment on table partition_lows is
'����
���������� ������� �������������';

select setMacToNULL('partition_lows');
select createTriggerUID('partition_lows');

/*==============================================================*/
/* Table: personal_q                                            */
/*==============================================================*/
create table personal_q (
   id                   SERIAL               not null,
   lastname             VARCHAR              null,
   firstname            VARCHAR              null,
   sirname              VARCHAR              null,
   access_cards         INT4[]               null,
   id_dolzh_p           INT4                 null,
   dolzh_name           VARCHAR              null,
   id_unit_p            INT4                 null,
   unit_name            VARCHAR              null,
   comment              VARCHAR              null,
   org_name             VARCHAR              null,
   org_uid              VARCHAR              null,
   id_p                 INT4                 null,
   constraint PK_PERSONAL_Q primary key (id)
)
inherits (root_table);

comment on table personal_q is
'������������ ���������� �  ��������� �������, ������������������� � �� ��� ������� �� ������ ������� � ������ ������� (�� ������ ������ � ���������)';

select setMacToNULL('personal_q');
select createTriggerUID('personal_q');

/*==============================================================*/
/* Table: persons                                               */
/*==============================================================*/
create table persons (
   id                   SERIAL               not null,
   id_tso_position      INT4                 null,
   id_tso_unit          INT4                 null,
   lastname             VARCHAR              not null,
   firstname            VARCHAR              null,
   sirname              VARCHAR              null,
   extra_id             INT4                 not null,
   is_completed         BOOL                 not null default FALSE,
   constraint PK_PERSONS primary key (id)
)
inherits (root_table);

comment on table persons is
'���������� � ��������� (�� ���)';

select setMacToNULL('persons');
select createTriggerUID('persons');

/*==============================================================*/
/* Table: "position"                                            */
/*==============================================================*/
create table "position" (
--   id                   INT4                 not null,
   id_unit              INT4                 not null,
   id_maclabel          INT4                 not null default 1,
   id_user_vrio         INT4                 null,
   id_user              INT4                 null,
   id_position_type     INT4                 not null default 1,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   is_io                bool                 not null default FALSE,
   email_prefix         VARCHAR              not null,
   phone                VARCHAR              null,
   is_public            BOOL                 not null default FALSE,
   is_archived          BOOL                 not null default FALSE,
   constraint PK_POSITION primary key (id)
)
inherits
(
     kks_roles,
     root_table
);

comment on table "position" is
'���������� ����������';

comment on column "position".id is
'�� ���� ��� ���� ������ SERIAL! ��� ����������� �� kks_roles. ����� �������� ������ ������� ������������ ������, ������� ������ ��� �������� �� ��������� � nextval(''kks_roles_id_seq'');';

comment on column "position".email_prefix is
'������ ���� ������������ ��� ����������� ������������� ������������ ���� ������ �����������.
� ��������� ��� ����� ���� ������� ��� ������ ����������� � �������������� ����� �������������.
������������ �� ������� ����������� ���������� ����� ���� ����� �������� ����� ����, ��������������� ���� ������������ ����, �������� �� ����� ��������� ���������/������������. ������� �����������, ��� � ���� ������ ��� ���������� ��������';

comment on column "position".is_public is
'�������� �� �� ���������';

comment on column "position".is_archived is
'���� ���������� ��������� � ������.
���������� � ������ ���������� ��������, ��� ��������� �������';

select setMacToNULL('"position"');
select createTriggerUID('"position"');
alter table "position" alter column id set default pg_catalog.nextval('kks_roles_id_seq');

/*==============================================================*/
/* Index: i_dl_email                                            */
/*==============================================================*/
create unique index i_dl_email on "position" using BTREE (
email_prefix
);

/*==============================================================*/
/* Table: position_types                                        */
/*==============================================================*/
create table position_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_POSITION_TYPES primary key (id)
)
inherits (root_table);

comment on table position_types is
'��� ������������ ����.
- �� � ������� DynamicDocs
- �� �� ������� ����������� �������';

select setMacToNULL('position_types');
select createTriggerUID('position_types');

/*==============================================================*/
/* Table: position_work_mode                                    */
/*==============================================================*/
create table position_work_mode (
   id_work_mode         INT4                 not null,
   id_position          INT4                 not null,
   constraint PK_POSITION_WORK_MODE primary key (id_work_mode, id_position)
);

comment on table position_work_mode is
'������, ��� ������� ��������� ����������';

select setMacToNULL('position_work_mode');

/*==============================================================*/
/* Table: processing_scenario                                   */
/*==============================================================*/
create table processing_scenario (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_PROCESSING_SCENARIO primary key (id)
)
inherits (root_table);

comment on table processing_scenario is
'�������� ��������� ��
(����)';

select setMacToNULL('processing_scenario');
select createTriggerUID('processing_scenario');

/*==============================================================*/
/* Table: processing_variant                                    */
/*==============================================================*/
create table processing_variant (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_PROCESSING_VARIANT primary key (id)
)
inherits (root_table);

comment on table processing_variant is
'�������� ��������� �� � ��������';

select setMacToNULL('processing_variant');
select createTriggerUID('processing_variant');

/*==============================================================*/
/* Table: q_base_table                                          */
/*==============================================================*/
create table q_base_table (
   id                   BIGSERIAL            not null,
   id_io_state          INT4                 not null default 1,
   uuid_t               UUID                 not null,
   rr_name              VARCHAR              not null,
   r_icon               VARCHAR              null,
   record_fill_color    INT8                 null,
   record_text_color    INT8                 null,
   constraint PK_Q_BASE_TABLE primary key (id)
)
inherits (root_table);

comment on column q_base_table.rr_name is
'�������� ������.';

alter table q_base_table alter column uuid_t set default uuid_generate_v1();

/*==============================================================*/
/* Table: queue_results                                         */
/*==============================================================*/
create table queue_results (
   id                   BIGSERIAL            not null,
   id_transport         INT4                 not null,
   id_external_queue    INT8                 not null,
   sync_result          INT4                 not null default 4,
   address              varchar              not null,
   is_read              INT4                 not null default 1,
   port                 INT4                 null,
   org_uid              VARCHAR              not null,
   use_gateway          BOOL                 not null default FALSE,
   constraint PK_QUEUE_RESULTS primary key (id)
)
inherits (root_table);

comment on table queue_results is
'������� � ����������� � ����������� ��������� �������������';

comment on column queue_results.id_external_queue is
'������������� � ����������� ��������� ������� �� �������-�����������
���� ��������� ��������� �������� ������� �� ���� (atPingResponse), 
�� � ���� ���� ����������  (����������� �� -1) ������������� �����������-���������� ����� � �� ����������� �����

���� � �������� �������� ���������� 0 - ��� ��������, ��� ���������� ��������� �������������� �������������.
������ ������������ ����� ����� �� ����, �� � ��-����������� ����� ��� ��� ������ � ����������������� ������������ (��� ��� �� ������). � ���� ������ ����������� ����� � �������� ��������������� idOrgTo �������� 0.
';

comment on column queue_results.sync_result is
'��������� ��������� ��������� (����� ������������ � ���������)
3 - �������
4 - � �������';

comment on column queue_results.address is
'����� ���������� ���������';

comment on column queue_results.is_read is
'1 - �� �����������, ��������� ��������
2 - ����������� � �� ��������� ��������';

comment on column queue_results.port is
'���� ���������� ���������';

comment on column queue_results.org_uid is
'email_prefix �����������, ������� ���� �������� ���������. � ��� ���� ��������� ��������������� ���������� �� ��������� ���������. �������� ������������ ����������� ���';

comment on column queue_results.use_gateway is
'���� ���������� ������������� �������� ��������� ����� ���� (���)
�.�. ��������� �� ������, �������� ���� ��������� ���������, � �����';

select setMacToNULL('queue_results');
select createTriggerUID('queue_results');

/*==============================================================*/
/* Table: rand_state                                            */
/*==============================================================*/
create table rand_state (
   id                   SERIAL               not null,
   state_rand           BYTEA                null,
   rand_seed            INT8                 null,
   constraint PK_RAND_STATE primary key (id)
)
inherits (root_table);

comment on table rand_state is
'��������� ������� ��� �������� ��������� ������� ��������� �����';

select setMacToNULL('rand_state');
select createTriggerUID('rand_state');

/*==============================================================*/
/* Table: ranks                                                 */
/*==============================================================*/
create table ranks (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   code                 VARCHAR              null,
   constraint PK_RANKS primary key (id)
)
inherits (root_table);

comment on table ranks is
'���������� �������� ������';

comment on column ranks.code is
'������������ ��� ���������� � �������� ���������, ������ ��� 7�1';

select setMacToNULL('ranks');
select createTriggerUID('ranks');

/*==============================================================*/
/* Table: rec_attrs_attrs_values                                */
/*==============================================================*/
create table rec_attrs_attrs_values (
   id                   BIGSERIAL            not null,
   id_rec_attr_value    INT8                 not null,
   id_attr_attr         INT4                 not null,
   value                VARCHAR              not null
)
inherits (root_table);

comment on table rec_attrs_attrs_values is
'�������� ��������� ��������� (�����������) ��� ������� ������������.
������ � ������ ������� ���������� ������������� ��� �������������� �������� �������� ������ �����������';

comment on column rec_attrs_attrs_values.value is
'�������� (��������� �������������) ������������ ���������� ��������';

select setMacToNULL('rec_attrs_attrs_values');
select createTriggerUID('rec_attrs_attrs_values');

/*==============================================================*/
/* Table: rec_attrs_values                                      */
/*==============================================================*/
create table rec_attrs_values (
   id                   BIGSERIAL            not null,
   id_attr_category     INT4                 not null,
   id_record            INT8                 not null,
   value                VARCHAR              not null,
   start_time           TIMESTAMP            not null default CURRENT_TIMESTAMP,
   stop_time            TIMESTAMP            null,
   insert_time          TIMESTAMP            not null default CURRENT_TIMESTAMP,
   id_io_object_src     INT4                 null,
   id_io_object_src1    INT4                 null,
   description          VARCHAR              null,
   is_actual            BOOL                 not null default TRUE,
   constraint PK_REC_ATTRS_VALUES primary key (id)
)
inherits (root_table);

comment on table rec_attrs_values is
'�������� ��������� (�����������) ��� ������� ������������
������ ���� �������� ��������� � �� �������������� ��������������.
������������� � ������ ������� �������������� ������������ ������.
������ �������� �������� ����� �������� ������������.

��� ��������� �������� �������� ���������� ���������� �������� ����� ������ � ���� �������. ��� ���� ���� value �������� ����� ���������� ��������. ���� start_time �� �������� ��������������� � current_timestamp, ���� stop_time - � NULL, ���� meas_time - � current_timestamp, ���� insert_time - � current_timestamp, is_actual - � true.
��������� ���� ������������ ����� ������ ��������������, ������ start_time ��� ����� ����� ���������� � current_timestamp, � ���� stop_time - � NULL.
������ (��������) ������ ��� ���� ��������. ������ �� ���� is_actual ��������������� � false, stop_time - � current_timestamp.

��� �������� ������ �� ������ ������� ��������� �������� �� ����������. �������� ������ ��������. ������ �� ���� is_actual ��������������� � false, stop_time - � current_timestamp.

�����!!!
� ��������� ����� POSTGRESQL  �� ������������ ������� ����� �� �������� ������������ ������. �� ���� ������� ������ ������� ���� �� REC_ATTRS_VALUES �� Q_BASE_TABLE ������. � ��������� ���������� ��������� ������� ������ ��������, ������ �� �����������������, � �������� �������������� �������� ��������. �� ���� ������� ��������������� ������� ���� �� ������������ �������� �������� ��.
��� ����������� ��������� ����������� ������� �������, ������� ��������� ���� ������������� ������, �� ������� ��������� ����������� (����������) ������ � REC_ATTRS_VALUES. 
';

comment on column rec_attrs_values.value is
'�������� �������� � ��������� �������������';

comment on column rec_attrs_values.start_time is
'������ ������ ������������ �������� ��������';

comment on column rec_attrs_values.stop_time is
'������ ����� ������������ �������� ��������';

comment on column rec_attrs_values.insert_time is
'���� � ����� ��������� �������� �������� � �������';

comment on column rec_attrs_values.id_io_object_src is
'������������� ��, ������� ������ ���������� ���������� � �������� ��������.';

comment on column rec_attrs_values.id_io_object_src1 is
'������������� �������, �� ���� �������� ����������. ���� �������� �������� ���������� ��� �� ����, �� �������� � ���� ���� ��������� �� ��������� � ���� <��������>';

comment on column rec_attrs_values.is_actual is
'����, ������������ ��������� �� ������ �������� ��������, ���� ��� ��������.
������ ���� �������� ������������� ��� �������� � ��������� ��������, ������� �� �������� ���� "������ ����� ������������". ���� ������� ����-����� �������� � �������� (������ ������ ������������) - (������ ����� ������������), �� ���� ��������������� � true, � ��������� ������ �� ����� false.';

select setMacToNULL('rec_attrs_values');
select createTriggerUID('rec_attrs_values');

/*==============================================================*/
/* Index: i_rav_id_record                                       */
/*==============================================================*/
create  index i_rav_id_record on rec_attrs_values using BTREE (
id_record
);

/*==============================================================*/
/* Table: receive_order                                         */
/*==============================================================*/
create table receive_order (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   constraint PK_RECEIVE_ORDER primary key (id)
)
inherits (root_table);

comment on table receive_order is
'������� �������� �������� ���������.
������� 1, ���� 1 ���, ����� ������� 2-��, ���� � ��� ��� - 3-�� � �.�.
 ������� 1
 ������� 2
 ������� 3
 ������� 4';

select setMacToNULL('receive_order');
select createTriggerUID('receive_order');

/*==============================================================*/
/* Table: receivers                                             */
/*==============================================================*/
create table receivers (
   id_order             INT4                 not null,
   id_organization      INT4                 not null,
   id_work_mode         INT4                 not null,
   id_position          INT4                 not null,
   constraint PK_RECEIVERS primary key (id_order, id_organization, id_work_mode, id_position)
);

comment on table receivers is
'�������� ��� ��������� �������� � �������� �� ';

select setMacToNULL('receivers');

/*==============================================================*/
/* Table: record_rubricator                                     */
/*==============================================================*/
create table record_rubricator (
   id                   BIGSERIAL            not null,
   id_parent            INT8                 null,
   id_record            INT8                 null,
   id_io_object         INT4                 null,
   id_io_category       INT4                 null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   r_icon               VARCHAR              null,
   constraint PK_RECORD_RUBRICATOR primary key (id)
)
inherits (root_table);

comment on table record_rubricator is
'���������� ��� ������� ������������

�����!!!
� ��������� ����� POSTGRESQL  �� ������������ ������� ����� �� �������� ������������ ������. �� ���� ������� ������ ������� ���� �� record_rubricator �� Q_BASE_TABLE ������. � ��������� ���������� ��������� ������� ������ ��������, ������ �� �����������������, � �������� �������������� �������� ��������. �� ���� ������� ��������������� ������� ���� �� ������������ �������� �������� ��.
��� ����������� ��������� ����������� ������� �������, ������� ��������� ���� ������������� ������, �� ������� ��������� ����������� (����������) ������ � record_rubricator. 
';

comment on column record_rubricator.id_parent is
'������������ �������';

comment on column record_rubricator.id_record is
'������ �� ������, � ������� ��������� ���������';

comment on column record_rubricator.id_io_object is
'������ �� ����������. ���� ������, �� � ������� ����� ���� �������� ������ ������ �� ������� �����������';

comment on column record_rubricator.id_io_category is
'������ �� ���������. ���� �������, ��  � ������� ����� ���� �������� ������ ������ �� ������������ �������� ���������';

comment on column record_rubricator.name is
'�������� �������';

comment on column record_rubricator.description is
'�������� �������';

comment on column record_rubricator.r_icon is
'������, ������������ ��� ����������� �������� � ������ ������. ���� �� ������, �� ������������ �������� �� ��������� (������������ ���������� �����������)';

select setMacToNULL('record_rubricator');
select createTriggerUID('record_rubricator');

/*==============================================================*/
/* Table: report                                                */
/*==============================================================*/
create table report (
--   id                   SERIAL               not null,
   report_id            INT4                 not null,
   name                 VARCHAR              not null,
   report_name          VARCHAR              not null,
   report_descrip       VARCHAR              null,
   report_grade         INT4                 not null,
   report_source        VARCHAR              not null,
   constraint PK_REPORT primary key (id)
)
inherits (q_base_table);

comment on table report is
'��������� ���������� "���������� �������� �������".
������������ ����������� ������� openRPT.

��������� ������� OpenRPT, ������� �������� � ������ ��������, �������, ����� � ������ ����������� ���� ���� report_id � ������� ��� ��������� ������. ������� ����� ����� ������� �� ������ ������� �������� ������� ������� ��������� �������� ���� id � ��� ����.

������ ���������� �������� ���������� ����������� �������, �������������- �.�. �� ������ ������� ������� ���� �������, �� ��� ����� ����������� ������� ����������� ������� ������� �������.';

comment on column report.report_id is
'��������� ������� OpenRPT, ������� �������� � ������ ��������, �������, ����� ��� ���� ���� � ������� ��� ��������� ������. ������� ����� ����� ������� �� ������ ������� �������� ������� ������� ��������� �������� ���� id � ��� ����';

select setMacToNULL('report');
select createTriggerUID('report');

create or replace function check_report_id() returns trigger as
$BODY$
declare
    
begin
    if(new.report_id isnull) then
        new.report_id = new.id;
    end if;

    if(new.report_name isnull and new.name is not null) then
        new.report_name = new.name;
    end if;

    if(new.report_name is not null and new.name isnull) then
        new.name = new.report_name;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

--����������, ����� ������ ������� ����������� ������, ��� ������� trgsetuid, 
--������� ������ �������� ������� rr_name, ��������� �������� ������� name
--������� ���������� �� ������ ���, ����� �� �������� ������ ���.
create trigger trg_check_report_id
before insert or update
on report
for each row 
execute procedure check_report_id();



create or replace function fkQBaseTableCheck1() returns trigger as
$BODY$
declare
    theId int8;
begin
    if(TG_OP = 'UPDATE') then
        if(old.id <> new.id) then
            raise exception E'Change of primary keys on DynamicDocs tables is unsupported!';
            return NULL;
        end if;
        return new;
    end if;

    delete from rec_attrs_attrs_values where id_rec_attr_value in (select id from rec_attrs_values where id_record = old.id);
    delete from rec_attrs_values where id_record = old.id;
    delete from rubric_records where id_record = old.id;
    delete from rubric_records where id_rubric in (select id from recGetRubrics(old.id) where type in (0, 1));
    delete from record_rubricator where id in (select id from recGetRubrics(old.id) where type in (0, 1));
    delete from urls_records where id_record = old.id;

    return old;
end
$BODY$
language 'plpgsql';

create trigger trg_fk_q_base_table_check1
before update or delete
on report
for each row 
execute procedure fkQBaseTableCheck1();


create or replace function generateUUID() returns uuid as
$BODY$
declare
    
begin
    return uuid_generate_v1();
end
$BODY$
language 'plpgsql';


create or replace function uuidCheck() returns trigger as
$BODY$
declare
    tableName varchar;
    uuid_t uuid;
begin

    if(new.uuid_t isnull) then
        new.uuid_t = generateUUID();
    end if;

    return new;
end
$BODY$
language 'plpgsql';


create trigger trgSetUUID
before insert or update
on report
for each row 
execute procedure uuidCheck();

/*==============================================================*/
/* Table: report_organization                                   */
/*==============================================================*/
create table report_organization (
   id_organization      INT4                 not null,
   id_report            INT4                 not null,
   constraint PK_REPORT_ORGANIZATION primary key (id_organization, id_report)
);

comment on table report_organization is
'�������� �����������, � �������� �������������� ������������� ����������� �������� �������';

select setMacToNULL('report_organization');

/*==============================================================*/
/* Table: role                                                  */
/*==============================================================*/
create table role (
   id_role              int                  not null,
   name                 text                 not null,
   super_user           text                 not null,
   constraint PK_ROLE primary key (id_role),
   constraint AK_KEY_2_ROLE unique (name)
);

/*==============================================================*/
/* Index: super_user                                            */
/*==============================================================*/
create  index super_user on role (
super_user
);

/*==============================================================*/
/* Table: role_action                                           */
/*==============================================================*/
create table role_action (
   role_id              int                  not null,
   module_action_id     int                  not null,
   constraint AK_KEY_1_ROLE_ACTION unique (role_id, module_action_id)
);

/*==============================================================*/
/* Index: role_id                                               */
/*==============================================================*/
create  index role_id on role_action (
role_id
);

/*==============================================================*/
/* Index: role_action_id                                        */
/*==============================================================*/
create  index role_action_id on role_action (
module_action_id
);

/*==============================================================*/
/* Table: roles_actions                                         */
/*==============================================================*/
create table roles_actions (
   id_service           INT4                 not null,
   id_role              INT4                 not null,
   constraint PK_ROLES_ACTIONS primary key (id_service, id_role)
);

select setMacToNULL('roles_actions');


/*==============================================================*/
/* Table: rubric_records                                        */
/*==============================================================*/
create table rubric_records (
   id_rubric            INT8                 not null,
   id_record            INT8                 not null,
   r_icon               VARCHAR              null,
   constraint PK_RUBRIC_RECORDS primary key (id_rubric, id_record)
);

comment on table rubric_records is
'������ ������������, ������� ��������� � ��������������� ��������

�����!!!
� ��������� ����� POSTGRESQL  �� ������������ ������� ����� �� �������� ������������ ������. �� ���� ������� ������ ������� ���� �� rubric_records �� Q_BASE_TABLE ������. � ��������� ���������� ��������� ������� ������ ��������, ������ �� �����������������, � �������� �������������� �������� ��������. �� ���� ������� ��������������� ������� ���� �� ������������ �������� �������� ��.
��� ����������� ��������� ����������� ������� �������, ������� ��������� ���� ������������� ������, �� ������� ��������� ����������� (����������) ������ � rubric_records. ';

comment on column rubric_records.r_icon is
'������, ������������ ��� ����������� �������� � ������ ������. ���� �� ������, �� ������������ �������� �� ��������� (������������ ���������� �����������)';

/*==============================================================*/
/* Table: rubricator                                            */
/*==============================================================*/
create table rubricator (
   id                   SERIAL not null,
   id_parent            INT4                 null,
   id_io_object         INT4                 null,
   id_search_template   INT4                 null,
   id_io_category       INT4                 null,
   name                 VARCHAR              not null,
   code                 VARCHAR              null,
   description          VARCHAR              null,
   r_icon               VARCHAR              null,
   constraint PK_RUBRICATOR primary key (id)
)
inherits (root_table);

comment on table rubricator is
'������ ���������� �������������� ��������
�������� ����� ������, ������� ����� ����� ��������-������� ��� ��������-�������������� ������.
������������� ������� ��������-������� � ��������-�� �� �����������. 
�� ����������� ����� � �������� ���� ������ � ����� ��������-���������. ��� ������� � ���, ��� ����� ������� �������� ��� ������� ��������. � ������������� ���� � ���, ��� � ������� ������� ��������

����� ������� � ����� ������� �������� ����� ���������� ��� ����������-���������� ����������, � ����� ������� ����������� ��� ����������, ������� ��������� �������� ����������, � ����� ���������������� �����������';

comment on column rubricator.id_search_template is
'��������� ������ (������) �������)';

comment on column rubricator.id_io_category is
'���������� ��������� ������������ � ������� �������������� ��������
���� ������, �� ��� ��������, ��� ������ �� �������� ��������� ����� ���������� � ������ �������.';

comment on column rubricator.r_icon is
'������ ��� ����������� �������� � ������ ������. ���� �� ������, �� ������������ ������ �� ��������� (������������ ���������� �����������)';

select setMacToNULL('rubricator');
select createTriggerUID('rubricator');

/*==============================================================*/
/* Index: i_rub_object                                          */
/*==============================================================*/
create unique index i_rub_object on rubricator using BTREE (
id_io_object
);

/*==============================================================*/
/* Table: search_template_types                                 */
/*==============================================================*/
create table search_template_types (
   id                   SERIAL               not null,
   id_parent            INT4                 null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_SEARCH_TEMPLATE_TYPES primary key (id)
)
inherits (root_table);

comment on table search_template_types is
'���� ��������� ��������
���������� ������ ��� ����������� ��������� � ���������� ������ ��������� �������� � ����� �������� �� ����������� ��� ������������';

select setMacToNULL('search_template_types');
select createTriggerUID('search_template_types');

/*==============================================================*/
/* Table: search_templates                                      */
/*==============================================================*/
create table search_templates (
   id                   SERIAL not null,
   id_group             INT4                 not null,
   id_search_template_type INT4                 not null default 1,
   author               INT4                 null,
   id_io_category       INT4                 not null default 13,
   name                 VARCHAR              not null,
   creation_datetime    TIMESTAMP            not null default CURRENT_TIMESTAMP,
   description          VARCHAR              null,
   constraint PK_SEARCH_TEMPLATES primary key (id)
)
inherits (root_table);

comment on table search_templates is
'������� ������, ����������� ��� ������������� ������ �������������� �������� � � ������������. ';

comment on column search_templates.id_io_category is
'���������� ���������, � ����������� ������� �������� ������ ��������� ������.
����������� ��������� �������� ��������� ��� �������������� ��������, �.�. ��� ����������� ��������� "���������� �������������� ��������". ��� �� ����� ����� ��������� �������, ����������� � ��������� ������������. 

� ���������� ���������� ������ ��������, ��� ��������� ������ ����� ���� �������� ��� ���������� ��������� (���� ��� �������� ����� ��������� ��, ������� ������� � ��������� �������)';

comment on column search_templates.creation_datetime is
'���� � ����� �������� ���������� �������';

select setMacToNULL('search_templates');
select createTriggerUID('search_templates');

/*==============================================================*/
/* Table: segment_types                                         */
/*==============================================================*/
create table segment_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_SEGMENT_TYPES primary key (id)
)
inherits (root_table);

comment on table segment_types is
'���� �������� ��� ��������� (�� ���)';

select setMacToNULL('segment_types');
select createTriggerUID('segment_types');

/*==============================================================*/
/* Table: shape_segments                                        */
/*==============================================================*/
create table shape_segments (
   id                   SERIAL               not null,
   id_segment_type      INT4                 not null,
   id_element_shape     INT4                 not null,
   points               POINT[]              not null,
   p_order              INT4                 not null default 0,
   constraint PK_SHAPE_SEGMENTS primary key (id)
)
inherits (root_table);

comment on table shape_segments is
'��������� ���������� ������� ������������ �������� (�� ���)';

select setMacToNULL('shape_segments');
select createTriggerUID('shape_segments');

/*==============================================================*/
/* Table: shape_types                                           */
/*==============================================================*/
create table shape_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_SHAPE_TYPES primary key (id)
)
inherits (root_table);

comment on table shape_types is
'���� ����� ����������� ��������� (�� ���):
- �������������;
- ������;
- ����� ���������.';

select setMacToNULL('shape_types');
select createTriggerUID('shape_types');

/*==============================================================*/
/* Table: sheduled_handlers                                     */
/*==============================================================*/
create table sheduled_handlers (
   id                   SERIAL               not null,
   id_handler           INT4                 not null,
   name                 VARCHAR              not null,
   exec_period          INT4[2]              not null,
   last_exec            TIMESTAMP            null,
   constraint PK_SHEDULED_HANDLERS primary key (id)
)
inherits (root_table);

comment on table sheduled_handlers is
'�������� ��������, ����������� ������������';

comment on column sheduled_handlers.name is
'�������� ������������ ������� (���� �� ������ - ��� �������� �� ��������� ��������� �� ����������� ��������)';

comment on column sheduled_handlers.exec_period is
'������������� ���������� ������� �������
';

comment on column sheduled_handlers.last_exec is
'���� � ����� ���������� ������';

select setMacToNULL('sheduled_handlers');
select createTriggerUID('sheduled_handlers');

/*==============================================================*/
/* Table: state_crosses                                         */
/*==============================================================*/
create table state_crosses (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   id_life_cycle        INT4                 not null,
   id_state_src         INT4                 not null,
   id_state_dest        INT4                 not null,
   constraint PK_STATE_CROSSES primary key (id)
)
inherits (root_table);

comment on table state_crosses is
'������� �������� ��������� �������� ��������� �� � ������ ��������� �����
��������� ������ ������� � ��������� ���� ���������! ��� ����������� �������.
�� ����� ���������� �� ������ ��������� � �����. ������� ��������������� ������ � ������ ������� �������� ���� ��, ��� ����� ��������� � �������� ��������� �� ������ ������� � �������� ��������� (� �������������� ��������� � ������� ���������, ���� ������� ��������� ��� �������� ���������)';

comment on column state_crosses.id_life_cycle is
'� ����� ��������� ����� ������� ��������';

comment on column state_crosses.id_state_src is
'�������� ���������';

comment on column state_crosses.id_state_dest is
'���������, � ������� ����� ������� �� ���������';

select setMacToNULL('state_crosses');
select createTriggerUID('state_crosses');

/*==============================================================*/
/* Index: i_lc_states                                           */
/*==============================================================*/
create unique index i_lc_states on state_crosses (
id_life_cycle,
id_state_src,
id_state_dest
);

/*==============================================================*/
/* Index: i_lc_life_cycle                                       */
/*==============================================================*/
create  index i_lc_life_cycle on state_crosses (
id_life_cycle
);

/*==============================================================*/
/* Table: system_table                                          */
/*==============================================================*/
create table system_table (
   id_system            SERIAL not null,
   int_parameter        INT4                 null,
   str_parameter        VARCHAR              null,
   description          VARCHAR              null,
   constraint PK_SYSTEM_TABLE primary key (id_system)
);

select setMacToNULL('system_table');

/*==============================================================*/
/* Table: table_notifies                                        */
/*==============================================================*/
create table table_notifies (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   notify_where         int4[]               not null,
   description          VARCHAR              null,
   constraint PK_TABLE_NOTIFIES primary key (id)
)
inherits (root_table);

comment on table table_notifies is
'���������� ����������� ���������, ������������ �������� ��� ��������, ��������� � �������� ��� ��� ��� ������� � ������������ (notify).
�������� ����������� ��������� ������ ���� ����������, ��� ���� ��������� ������������ ��������� � ����� ������ ��� ������ ������������.
�������� ������������, ��� ������� ������������ ���������, �������� � ������� table_notifies_io_objects (������-��-������).
����������� ��������� ������������ ��������� �������
pg_notify(notify_name, payload)
payload ����������� ��������� �������:
tableName + ''~_~_~'' + idRecord + ''~_~_~'' + recordUniqueId + ''~_~_~'' +whatHappens
whatHappens ����������� ��������� �������:
1 - ��������� �������� �������� ����� ������
2 - ��������� �������� ��������� ������
3 - ��������� �������� �������� ������';

comment on column table_notifies.name is
'�������� ���������. ������ ���� ����������';

comment on column table_notifies.notify_where is
'����� ������������ ��������� (������ ��������)';

select setMacToNULL('table_notifies');
select createTriggerUID('table_notifies');

/*==============================================================*/
/* Index: i_u_table_notify_name                                 */
/*==============================================================*/
create unique index i_u_table_notify_name on table_notifies (
name
);

/*==============================================================*/
/* Table: table_notifies_io_objects                             */
/*==============================================================*/
create table table_notifies_io_objects (
   id_table_notifies    INT4                 not null,
   id_io_objects        INT4                 not null,
   constraint PK_TABLE_NOTIFIES_IO_OBJECTS primary key (id_table_notifies, id_io_objects)
);

/*==============================================================*/
/* Table: time_units                                            */
/*==============================================================*/
create table time_units (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   constraint PK_TIME_UNITS primary key (id)
)
inherits (root_table);

comment on table time_units is
'������� ��������� �������';

select setMacToNULL('time_units');
select createTriggerUID('time_units');

/*==============================================================*/
/* Table: transport                                             */
/*==============================================================*/
create table transport (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   local_address        VARCHAR              not null,
   local_port           INT4                 null,
   is_active            bool                 not null default TRUE,
   use_gateway          BOOL                 null,
   constraint PK_TRANSPORT primary key (id)
)
inherits (root_table);

comment on table transport is
'���������� �������������� ������';

comment on column transport.local_address is
'��������� ����� ������� ����������';

comment on column transport.local_port is
'���� ������������ ������, ������� ������������ �� ���������  �����������.
���� ������������ ������ � IP-�����, ���� ������������ ��������� http_connector';

comment on column transport.use_gateway is
'������ �� ��������� ���� ��������� � ����� (���) ��� �������������� � ������� ��������� ';

select setMacToNULL('transport');
select createTriggerUID('transport');

/*==============================================================*/
/* Table: tsd                                                   */
/*==============================================================*/
create table tsd (
   id                   SERIAL not null,
   id_io_category       INT4                 not null,
   id_jr_state          INT4                 not null,
   id_dl_to             INT4                 not null,
   id_dl_executor       INT4                 not null,
   id_dl_controller     INT4                 not null,
   insert_time          TIMESTAMP            not null default CURRENT_TIMESTAMP,
   exec_datetime        TIMESTAMP            not null,
   exec_period          INT4[2]              null,
   osnovanie            VARCHAR              not null,
   is_archived          BOOL                 not null default false,
   constraint PK_TSD primary key (id)
)
inherits (root_table);

comment on table tsd is
'������ ������� ���������
��� �������� ����� ������ � ������ ������� ��������� ������ ������������� ����������� ������ � ������� �������� ����������';

comment on column tsd.id is
'���������� ����� ������ ������ ������� ���������';

comment on column tsd.id_io_category is
'���� ����������������� ��� ����� ��������� (��� ������� ���������) � ������� ���������� ������������ ��������� �� ������� ������ ( ����������� �� ������� ��������� ��������);';

comment on column tsd.insert_time is
'���� � ����� �������� ������ � ������ ������� ���������';

comment on column tsd.exec_datetime is
'���� � ������� ���������� ���������  ����� ������ (������ ���� ���������� ������ ������ ������� ���������-(���� � �������:��.��.��)
����� � �������� ���������� ����������� ��������� �� ���������� ������ ������ ������� ��������� (����� � �������-��.��);';

comment on column tsd.exec_period is
'������ ���������� ������� ������ ������� � ��������� ���� � ������� (����� ����������� � �����, ������, �������, ���������, ����� (��.���.��.�.�; ��������-00.001.00.0.0-�������������� ����������);';

comment on column tsd.osnovanie is
'��������� ��� �������� ������- �������� ����� �� ������� �������� ������������ ��� �������� ����� ��������� ��� ��� �������������������� ���������';

comment on column tsd.is_archived is
'����, ������������ ���������� ������ ������ � ������

������� � �������� ��������� ��������, ��� ������ � ��������� ����� �� ��������� � ������������ ������� ������� � ������������ ������� ������� �������� ���������� �������� � ��������� ������������ � ������ ������� ��������� ��� ����

������� � �������� ��������� ��������, ��� ������ � ��������� �������� ��������� � ������������ ������� ������� � ������������ ������� ������� �������� ���������� �������� � ��������� ������������ � ������ ������� ��������� ��� ����';

select setMacToNULL('tsd');
select createTriggerUID('tsd');

/*==============================================================*/
/* Table: tsd_control_journal                                   */
/*==============================================================*/
create table tsd_control_journal (
   id                   SERIAL not null,
   id_journal           INT4                 null,
   id_io_object         INT4                 null,
   insert_time          TIMESTAMP            not null,
   planned_exec         TIMESTAMP            not null,
   real_exec            TIMESTAMP            not null,
   is_archived          BOOL                 not null default false,
   is_outed             BOOL                 not null default true,
   constraint PK_TSD_CONTROL_JOURNAL primary key (id)
)
inherits (root_table);

comment on table tsd_control_journal is
'������ ����� � �������� ����������  ���������, ��������  ������������ � ������ ������� ���������';

comment on column tsd_control_journal.id is
'���������� ����� ������ ������� ';

comment on column tsd_control_journal.id_journal is
'����� ������� ����� ������������, ���� ����������� ������ ������� ����� ������������';

comment on column tsd_control_journal.id_io_object is
'������ �� �������� ��������';

comment on column tsd_control_journal.insert_time is
'���� �������� ������ - ���� �������� ������ � ������ (���� � �������: ��.��.��);';

comment on column tsd_control_journal.planned_exec is
'����������� ���� � ����� ���������� - ���� � ����� ����������, ��������� � �������������� ���������;';

comment on column tsd_control_journal.real_exec is
'�������� ���� � ����� ���������� - �������� ���� �  ����� ����������;';

comment on column tsd_control_journal.is_archived is
'����, ������������ ���������� ������� ������� �������� � ������';

comment on column tsd_control_journal.is_outed is
'���� ������ ���� ������ ��� TRUE, �� ��������, ��������� ��� ����� �� ������������, ��������� ��������.
��� ��������� ������������ ������ ���� ������ ������������ ��� TRUE';

select setMacToNULL('tsd_control_journal');
select createTriggerUID('tsd_control_journal');

/*==============================================================*/
/* Table: tso_params                                            */
/*==============================================================*/
create table tso_params (
   id                   SERIAL               not null,
   tso_max_level        INT4                 not null,
   tso_dl_from          VARCHAR              not null,
   tso_dl_exec          VARCHAR              not null,
   tso_dl_to            VARCHAR              not null,
   tso_category         VARCHAR              not null,
   tso_exectime         VARCHAR              not null,
   constraint PK_TSO_PARAMS primary key (id)
)
inherits (root_table);

comment on table tso_params is
'��������� ������� ���';

select setMacToNULL('tso_params');
select createTriggerUID('tso_params');

/*==============================================================*/
/* Table: tso_positions                                         */
/*==============================================================*/
create table tso_positions (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   constraint PK_TSO_POSITIONS primary key (id)
)
inherits (root_table);

comment on table tso_positions is
'���������� ���������� (�� ���)';

select setMacToNULL('tso_positions');
select createTriggerUID('tso_positions');

/*==============================================================*/
/* Table: tso_units                                             */
/*==============================================================*/
create table tso_units (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   extra_id             INT4                 not null,
   constraint PK_TSO_UNITS primary key (id)
)
inherits (root_table);

comment on table tso_units is
'���������� ������� (�� ���)';

select setMacToNULL('tso_units');
select createTriggerUID('tso_units');

/*==============================================================*/
/* Table: units                                                 */
/*==============================================================*/
create table units (
--   id                   SERIAL not null,
   id_organization      INT4                 null,
   id_parent            INT4                 null,
   id_curr_mode         INT4                 not null,
   id_boss              INT4                 null,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   code_name            VARCHAR              not null,
   tree_symbol          VARCHAR              null,
   is_fgroup            bool                 not null default FALSE,
   time_start           TIMESTAMP            null,
   time_elapsed         TIMESTAMP            null,
   is_archived          BOOL                 not null default FALSE,
   constraint PK_UNITS primary key (id)
)
inherits
(
     kks_roles,
     root_table
);

comment on table units is
'���������� �������������';

comment on column units.id is
'�� ���� ��� ���� ������ SERIAL! ��� ����������� �� kks_roles. ����� �������� ������ ������� ������������ ������, ������� ������ ��� �������� �� ��������� � nextval(''kks_roles_id_seq'');';

comment on column units.id_organization is
'�����������, � ������� ��������� ������ �������������.
������ ���� �� �������� ������������, ��������� � ������� ���������� ��������� ������������ (����������� ����) ����� ��� admin, jupiter, ������� �� ����������� �� ����� �����������';

comment on column units.id_curr_mode is
'������� ����� �������������, ����������� �� ������������ ������������� (��� ������� ��� �������, � ��������� ������ - �� �����������, � ������� ������������� ������)';

comment on column units.tree_symbol is
'������ ������������� � ������ ��� (XPM)';

comment on column units.time_elapsed is
'��������� ������, �� ������� ��������� ��, ���� ������ ������������� �������� �������������� �������';

comment on column units.is_archived is
'���� ���������� ������������� � ������.
���������� � ������ ���������� ��������, ��� ������������� �������.
���� ������������� ��������� � ������, �� ��� ��������� ��� ����� ��������� � ������ � ��������� ����� � ��� ������';

select setMacToNULL('units');
select createTriggerUID('units');
alter table units alter column id set default pg_catalog.nextval('kks_roles_id_seq');

/*==============================================================*/
/* Table: units_work_mode                                       */
/*==============================================================*/
create table units_work_mode (
   id_units             INT4                 not null,
   id_work_mode         INT4                 not null,
   constraint PK_UNITS_WORK_MODE primary key (id_units, id_work_mode)
);

comment on table units_work_mode is
'������, ��� ������� ������������� ����������';

select setMacToNULL('units_work_mode');

/*==============================================================*/
/* Table: urgency_levels                                        */
/*==============================================================*/
create table urgency_levels (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   code                 VARCHAR              not null,
   urgency_symbol       VARCHAR              not null,
   constraint PK_URGENCY_LEVELS primary key (id)
)
inherits (root_table);

comment on table urgency_levels is
'������ ��������� ��������� (������������)';

select setMacToNULL('urgency_levels');
select createTriggerUID('urgency_levels');

/*==============================================================*/
/* Table: url_extentions                                        */
/*==============================================================*/
create table url_extentions (
   id                   SERIAL not null,
   id_mimetype          INT4                 not null default 1,
   name                 VARCHAR              not null,
   constraint PK_URL_EXTENTIONS primary key (id)
)
inherits (root_table);

comment on table url_extentions is
'��������� ���������� ��������� ���������� ������������� ������';

select setMacToNULL('url_extentions');
select createTriggerUID('url_extentions');

/*==============================================================*/
/* Table: url_types                                             */
/*==============================================================*/
create table url_types (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   win_app              VARCHAR              null,
   lin_app              VARCHAR              null,
   extentions           INT4[]               not null,
   constraint PK_URL_TYPES primary key (id)
)
inherits (root_table);

select setMacToNULL('url_types');
select createTriggerUID('url_types');

/*==============================================================*/
/* Table: urls_objects                                          */
/*==============================================================*/
create table urls_objects (
   id_url               INT4                 not null,
   id_io_object         INT4                 not null,
   name                 VARCHAR              not null,
   constraint PK_URLS_OBJECTS primary key (id_url, id_io_object)
);

select setMacToNULL('urls_objects');

/*==============================================================*/
/* Table: urls_records                                          */
/*==============================================================*/
create table urls_records (
   id_record            INT8                 not null,
   id_url               INT4                 not null,
   name                 VARCHAR              null,
   constraint PK_URLS_RECORDS primary key (id_record, id_url)
);

comment on table urls_records is
'����������� ������� ������������ � �������������� � ��� �������.
����������� ����������� ����������� � ������� ������������ DynamicDocs ��������� �����';

/*==============================================================*/
/* Table: user_acl_templates                                    */
/*==============================================================*/
create table user_acl_templates (
   id                   SERIAL not null,
   id_user              INT4                 null,
   name                 VARCHAR              not null,
   constraint PK_USER_ACL_TEMPLATES primary key (id)
);

comment on table user_acl_templates is
'���������������� ������ ������������� ���� ������� ������������� (����������� ���) � �������������� ��������';

select setMacToNULL('user_acl_templates');

/*==============================================================*/
/* Table: user_chains                                           */
/*==============================================================*/
create table user_chains (
   id                   SERIAL               not null,
   id_search_template   INT4                 null,
   id_io_category       INT4                 not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   trig_level           INT4                 not null default 1,
   proc_name            VARCHAR              not null,
   constraint PK_USER_CHAINS primary key (id)
)
inherits (root_table);

comment on table user_chains is
'��������� ���������� ���������������� ��������
������ ������� ����� ���������� ����������� �������, ������������� �.�. �� ������ ������� ������� ���� �������, �� ��� ����� ����������� ������� ����������� ������� ������� �������
';

comment on column user_chains.id_search_template is
'������ �� ��������� ������, ��������������� � ���������������� ��������';

comment on column user_chains.id_io_category is
'���������, �� ������ ������� ������� �����������, ���������� ��������� ���������������� ������������.
����������� �������� ���������';

select setMacToNULL('user_chains');
select createTriggerUID('user_chains');

/*==============================================================*/
/* Table: user_chains_organization                              */
/*==============================================================*/
create table user_chains_organization (
   id_organization      INT4                 not null,
   id_user_chains       INT4                 not null,
   constraint PK_USER_CHAINS_ORGANIZATION primary key (id_organization, id_user_chains)
);

comment on table user_chains_organization is
'�������� �����������, � �������� �������������� ������������� ����������� ���������������� ��������';

select setMacToNULL('user_chains_organization');

/*==============================================================*/
/* Table: user_handlers_data                                    */
/*==============================================================*/
create table user_handlers_data (
   id                   INT4                 not null,
   id_io_category       INT4                 not null,
   table_name           VARCHAR              not null,
   rec_type             INT4                 not null,
   constraint PK_USER_HANDLERS_DATA primary key (id, id_io_category, table_name, rec_type)
)
inherits (root_table);

comment on table user_handlers_data is
'��������� �������, � ������� ���������� ���������� �� ����������, �����������, ��������� ������� ������������, ������� �������������� ����������������� �������������';

comment on column user_handlers_data.rec_type is
'��� �������
1 - ������ ���� �������
2 - ������ ���� ��������';

select setMacToNULL('user_handlers_data');
select createTriggerUID('user_handlers_data');

/*==============================================================*/
/* Table: user_rubricator                                       */
/*==============================================================*/
create table user_rubricator (
   id_rubricator        INT4                 not null,
   id_user              INT4                 not null,
   constraint PK_USER_RUBRICATOR primary key (id_user)
);

comment on table user_rubricator is
'���������������� �����������
�.�. ������ "��� ���������" ��� ������� ������������.
������������ ����� ����� ������ ���� ������ "��� ���������". � ��� ��� � ���� ������� �������� ������ ������ � ���������';

select setMacToNULL('user_rubricator');

/*==============================================================*/
/* Table: user_state                                            */
/*==============================================================*/
create table user_state (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   constraint PK_USER_STATE primary key (id)
)
inherits (root_table);

comment on table user_state is
'��������� ������������ (������, ������������ � �.�.)';

select setMacToNULL('user_state');
select createTriggerUID('user_state');

/*==============================================================*/
/* Table: user_templates                                        */
/*==============================================================*/
create table user_templates (
   id_user              INT4                 not null,
   id_template          INT4                 not null,
   id_io_object         INT4                 not null,
   type                 INT4                 not null
      constraint CKC_TYPE_USER_TEM check (type in (0,1)),
   constraint PK_USER_TEMPLATES primary key (id_io_object, type, id_user)
);

comment on table user_templates is
'������� �������� ���������� � �������� ������� ����������� ��������� ��������������� ������� � ������������.
���������� ���� ���� � ��������� ����������� ����������� �������, � ����� ��������� (��������� � ��� �����) ��������������� �������

��������������� �� ������������ ������� ��������������� ���������� ����� ������� (������� �������). ';

comment on column user_templates.type is
'��� ����������.
0 - ������ ����������� � ����������� �������
1 - ������ ����������� � ���������������� ���������';

select setMacToNULL('user_templates');

/*==============================================================*/
/* Table: users                                                 */
/*==============================================================*/
create table users (
--   id                   SERIAL not null,
   id_rank              INT4                 not null,
   id_state             INT4                 not null,
   id_maclabel          INT4                 not null default 1,
   lastname             VARCHAR              not null,
   firstname            VARCHAR              not null,
   sirname              VARCHAR              not null,
   fio                  VARCHAR              not null,
   insert_time          TIMESTAMP            not null default current_timestamp,
   email                VARCHAR              null,
   acc_right_num        VARCHAR              null,
   acc_right_date       DATE                 null,
   is_connected         bool                 not null default FALSE,
   constraint PK_USERS primary key (id)
)
inherits
(
     kks_roles,
     root_table
);

comment on table users is
'������� (����������) �������������, ������������������ � �������';

comment on column users.id is
'�� ���� ��� ���� ������ SERIAL! ��� ����������� �� kks_roles. ����� �������� ������ ������� ������������ ������, ������� ������ ��� �������� �� ��������� � nextval(''kks_roles_id_seq'');';

comment on column users.lastname is
'�������';

comment on column users.firstname is
'���';

comment on column users.sirname is
'��������';

comment on column users.fio is
'����������� ����. ��� �������� �������� ������� � ��
������ �.�.
����������� �� lastname, firstname, sirname';

comment on column users.insert_time is
'����� �������� ������';

comment on column users.acc_right_num is
'����� �������';

comment on column users.acc_right_date is
'���� ������ �������';

select setMacToNULL('users');
select createTriggerUID('users');
alter table users alter column id set default pg_catalog.nextval('kks_roles_id_seq');

/*==============================================================*/
/* Table: work_mode                                             */
/*==============================================================*/
create table work_mode (
   id                   SERIAL not null,
   id_mode_type         INT4                 not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   symbol               VARCHAR              null,
   audio_file           bytea                null,
   constraint PK_WORK_MODE primary key (id)
)
inherits (root_table);

comment on table work_mode is
'����� ���������������� ����������� (�������������)';

comment on column work_mode.symbol is
'�������� ���� (� ������� XPM), ������� ������������ ������� ����� ����������������';

comment on column work_mode.audio_file is
'��������� ���������� � ����� ������� ������ ����������������';

select setMacToNULL('work_mode');
select createTriggerUID('work_mode');

/*==============================================================*/
/* Table: work_mode_type                                        */
/*==============================================================*/
create table work_mode_type (
   id                   SERIAL not null,
   name                 VARCHAR              not null,
   short_name           VARCHAR              not null,
   constraint PK_WORK_MODE_TYPE primary key (id)
)
inherits (root_table);

comment on table work_mode_type is
'��� ������ ���������������� ����������� (������� ��� ������).
���������, ��� ����� ����� ���������������� ����� ������� � ������ �� ���� (� ������� ������) �����:
- �������� 
- �������';

select setMacToNULL('work_mode_type');
select createTriggerUID('work_mode_type');

/*==============================================================*/
/* Table: xml_formats                                           */
/*==============================================================*/
create table xml_formats (
   id                   SERIAL               not null,
   id_xml_query_type    INT4                 not null,
   xml_level            INT4                 not null,
   xpath                VARCHAR              not null,
   node_name            VARCHAR              not null,
   id_name              VARCHAR              not null,
   constraint PK_XML_FORMATS primary key (id)
)
inherits (root_table);

select setMacToNULL('xml_formats');
select createTriggerUID('xml_formats');

/*==============================================================*/
/* Index: i_xml_level                                           */
/*==============================================================*/
create unique index i_xml_level on xml_formats using BTREE (
id_xml_query_type,
xml_level
);

/*==============================================================*/
/* Table: xml_paths                                             */
/*==============================================================*/
create table xml_paths (
   id                   SERIAL               not null,
   id_xml_query_type    INT4                 not null,
   xpath                VARCHAR              not null,
   xcode                VARCHAR              not null,
   ref_table            VARCHAR              null,
   is_mandatory         BOOL                 not null default FALSE,
   data_type            VARCHAR              not null,
   xml_level            INT4                 not null default 0,
   constraint PK_XML_PATHS primary key (id)
)
inherits (root_table);

comment on table xml_paths is
'���������� XML-����� � XML-�������� ���� ����������� ���
(��� ������� �� ���)';

comment on column xml_paths.data_type is
'��� ������';

comment on column xml_paths.xml_level is
'������� ����������� ���� � xml-�������';

select setMacToNULL('xml_paths');
select createTriggerUID('xml_paths');

/*==============================================================*/
/* Table: xml_queries                                           */
/*==============================================================*/
create table xml_queries (
   id                   SERIAL               not null,
   id_xml_query_type    INT4                 not null,
   id_xml_query_kind    INT4                 not null,
   name                 VARCHAR              not null,
   xml_data             XML                  not null,
   insert_time          TIMESTAMP            not null default CURRENT_TIMESTAMP,
   exec_period_h        INT4[3]              null,
   description          VARCHAR              null,
   constraint PK_XML_QUERIES primary key (id)
)
inherits (root_table);

comment on table xml_queries is
'��� ��������� ������� � �� ���';

select setMacToNULL('xml_queries');
select createTriggerUID('xml_queries');

/*==============================================================*/
/* Table: xml_query_kinds                                       */
/*==============================================================*/
create table xml_query_kinds (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   constraint PK_XML_QUERY_KINDS primary key (id)
)
inherits (root_table);

comment on table xml_query_kinds is
'��� XML-������� � �� ���: 
- ������������������;
- ��������������������.';

select setMacToNULL('xml_query_kinds');
select createTriggerUID('xml_query_kinds');

/*==============================================================*/
/* Table: xml_query_responses                                   */
/*==============================================================*/
create table xml_query_responses (
   id                   SERIAL               not null,
   id_xml_query         INT4                 not null,
   name                 VARCHAR              not null,
   xml_data             XML                  not null,
   exec_time            TIMESTAMP            not null,
   constraint PK_XML_QUERY_RESPONSES primary key (id)
)
inherits (root_table);

comment on table xml_query_responses is
'���������� ������� �� ������� � �� ���';

select setMacToNULL('xml_query_responses');
select createTriggerUID('xml_query_responses');

/*==============================================================*/
/* Table: xml_query_types                                       */
/*==============================================================*/
create table xml_query_types (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   code                 VARCHAR              not null,
   constraint PK_XML_QUERY_TYPES primary key (id)
)
inherits (root_table);

comment on table xml_query_types is
'���� �������� �� XML (�������� ������������������ �������� � �� ���)';

select setMacToNULL('xml_query_types');
select createTriggerUID('xml_query_types');

alter table ERROR
   add constraint FK_ERROR_REFERENCE_GROUP_ER foreign key (ID_GROUP)
      references GROUP_ERROR (ID_GROUP)
      on delete restrict on update restrict;

alter table a_groups
   add constraint FK_A_GROUPS_REFERENCE_IO_TEMPL foreign key (id_io_template)
      references io_templates (id)
      on delete restrict on update restrict;

alter table a_groups
   add constraint FK_A_GROUPS_REFERENCE_A_GROUPS foreign key (id_parent)
      references a_groups (id)
      on delete restrict on update restrict;

alter table a_types
   add constraint FK_A_TYPES_REFERENCE_A_VIEWS foreign key (id_a_view)
      references a_views (id)
      on delete restrict on update restrict;

alter table access_cards
   add constraint FK_ACCESS_C_REFERENCE_CARD_TYP foreign key (id_card_type)
      references card_types (id)
      on delete restrict on update restrict;

alter table access_cards
   add constraint FK_ACCESS_C_REFERENCE_CARD_STA foreign key (id_card_status)
      references card_statuses (id)
      on delete restrict on update restrict;

alter table access_cards
   add constraint FK_ACCESS_C_REFERENCE_PERSONS foreign key (id_person)
      references persons (id)
      on delete restrict on update restrict;

alter table access_cards_access_plan
   add constraint FK_ACCESS_C_REFERENCE_ACCESS_P foreign key (id_access_plan)
      references access_plan (id)
      on delete restrict on update restrict;

alter table access_cards_access_plan
   add constraint FK_ACCESS_C_REFERENCE_ACCESS_C foreign key (id_access_cards)
      references access_cards (id)
      on delete restrict on update restrict;

alter table access_categories_table
   add constraint FK_ACCESS_C_REFERENCE_KKS_ROLE foreign key (id_role)
      references kks_roles (id)
      on delete restrict on update restrict;

alter table access_categories_table
   add constraint FK_ACCESS_C_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table access_rubric_table
   add constraint FK_ACCESS_R_REFERENCE_KKS_ROLE foreign key (id_role)
      references kks_roles (id)
      on delete restrict on update restrict;

alter table access_rubric_table
   add constraint FK_ACCESS_R_REFERENCE_RUBRICAT foreign key (id_rubric)
      references rubricator (id)
      on delete restrict on update restrict;

alter table access_table
   add constraint FK_ACCESS_T_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table access_table
   add constraint FK_ACCESS_T_REFERENCE_KKS_ROLE foreign key (id_role)
      references kks_roles (id)
      on delete restrict on update restrict;

alter table acl_template_exceptions
   add constraint FK_ACL_TEMP_REFERENCE_USER_ACL foreign key (id_template)
      references user_acl_templates (id)
      on delete restrict on update restrict;

alter table acl_template_exceptions
   add constraint FK_ACL_TEMP_REFERENCE_KKS_ROLE foreign key (id_role)
      references kks_roles (id)
      on delete restrict on update restrict;

alter table acl_template_groups
   add constraint FK_ACL_TEMP_REFERENCE_USER_ACL foreign key (id_template)
      references user_acl_templates (id)
      on delete restrict on update restrict;

alter table acl_template_groups
   add constraint FK_ACL_TEMP_REFERENCE_ACL_TEMP foreign key (id_group_type)
      references acl_template_group_types (id)
      on delete restrict on update restrict;

alter table alarm_events
   add constraint FK_ALARM_EV_REFERENCE_DEVICES foreign key (id_device)
      references devices (id)
      on delete restrict on update restrict;

alter table alarm_events
   add constraint FK_ALARM_EV_REFERENCE_PERSONS foreign key (id_person)
      references persons (id)
      on delete restrict on update restrict;

alter table attributes
   add constraint FK_ATTRIBUT_REFERENCE_SEARCH_T foreign key (id_search_template)
      references search_templates (id)
      on delete restrict on update restrict;

alter table attributes
   add constraint FK_ATTRIBUT_REFERENCE_ATTRS_GR foreign key (id_attr_group)
      references attrs_groups (id)
      on delete restrict on update restrict;

alter table attributes
   add constraint FK_ATTRIBUT_REF_A_TYPES1 foreign key (id_ref_attr_type)
      references a_types (id)
      on delete restrict on update restrict;

alter table attributes
   add constraint FK_ATTRIBUT_REFERENCE_A_TYPES foreign key (id_a_type)
      references a_types (id)
      on delete restrict on update restrict;

alter table attrs_attrs
   add constraint FK_ATTRS_ATTRS_PARENT foreign key (id_attr_parent)
      references attributes (id)
      on delete restrict on update restrict;

alter table attrs_attrs
   add constraint FK_ATTRS_ATTRS_CHILD foreign key (id_attr_child)
      references attributes (id)
      on delete restrict on update restrict;

alter table attrs_attrs_values
   add constraint FK_ATTRS_AT_REFERENCE_ATTRS_VA foreign key (id_attr_value)
      references attrs_values (id)
      on delete restrict on update restrict;

alter table attrs_attrs_values
   add constraint FK_ATTRS_AT_REFERENCE_ATTRS_AT foreign key (id_attr_attr)
      references attrs_attrs (id)
      on delete restrict on update restrict;

alter table attrs_categories
   add constraint FK_ATTRS_CA_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table attrs_categories
   add constraint FK_ATTRS_CA_REFERENCE_ATTRIBUT foreign key (id_io_attribute)
      references attributes (id)
      on delete restrict on update restrict;

alter table attrs_groups
   add constraint FK_ATTRS_GR_REFERENCE_ATTRS_GR foreign key (id_parent)
      references attrs_groups (id)
      on delete restrict on update restrict;

alter table attrs_values
   add constraint FK_ATTRS_VA_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table attrs_values
   add constraint FK_ATTRS_VA_REFERENCE_ATTRS_CA foreign key (id_attr_category)
      references attrs_categories (id)
      on delete restrict on update restrict;

alter table categories_rubrics
   add constraint FK_CATEGORI_REFERENCE_CATEGORI foreign key (id_parent)
      references categories_rubrics (id)
      on delete restrict on update restrict;

alter table categories_rubrics
   add constraint FK_CATEGORI_REFERENCE_IO_CATEG foreign key (id_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table chains
   add constraint FK_CHAINS_REFERENCE_HANDLERS foreign key (id_handler)
      references handlers (id)
      on delete restrict on update restrict;

alter table chains_data
   add constraint FK_CHAINS_D_REFERENCE_CHAINS foreign key (id_chain)
      references chains (id)
      on delete restrict on update restrict;

alter table chains_data
   add constraint FK_CHAINS_D_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table chains_data
   add constraint FK_CHAINS_D_REFERENCE_CHAINS_D foreign key (id_parent)
      references chains_data (id)
      on delete restrict on update restrict;

alter table chains_data
   add constraint FK_CHAINS_D_PROC_VARIANT foreign key (id_processing_variant)
      references processing_variant (id)
      on delete restrict on update restrict;

alter table chains_data
   add constraint FK_CHAINS_D_REFERENCE_PROCESSI foreign key (id_processing_scenario)
      references processing_scenario (id)
      on delete restrict on update restrict;

alter table cmd_confirmations
   add constraint FK_CMD_CONF_REFERENCE_COMMAND_ foreign key (id_cmd)
      references command_journal (id)
      on delete restrict on update restrict;

alter table command_journal
   add constraint FK_CMD_REF_POSITION_FROM foreign key (id_dl_from)
      references "position" (id)
      on delete restrict on update restrict;

alter table command_journal
   add constraint FK_CMD_REF_POSITION_TO foreign key (id_dl_to)
      references "position" (id)
      on delete restrict on update restrict;

alter table command_journal
   add constraint FK_CMD_REF_POSITION_EXEC foreign key (id_dl_executor)
      references "position" (id)
      on delete restrict on update restrict;

alter table command_journal
   add constraint FK_COMMAND__REFERENCE_URGENCY_ foreign key (id_urgency_level)
      references urgency_levels (id)
      on delete restrict on update restrict;

alter table command_journal
   add constraint FK_COMMAND__REFERENCE_IO_OBJEC foreign key (id_io_object_att)
      references io_objects (id)
      on delete restrict on update restrict;

alter table command_journal
   add constraint FK_COMMAND__REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table command_journal
   add constraint FK_COMMAND__REFERENCE_JR_STATE foreign key (id_jr_state)
      references jr_states (id)
      on delete restrict on update restrict;

alter table command_journal
   add constraint FK_COMMAND__REFERENCE_TSD foreign key (id_tabel)
      references tsd (id)
      on delete restrict on update restrict;

alter table coworkers
   add constraint FK_COWORKER_REF_ORG_1 foreign key (id_organization1)
      references organization (id)
      on delete restrict on update restrict;

alter table coworkers
   add constraint FK_COWORKER_REF_ORG_2 foreign key (id_organization2)
      references organization (id)
      on delete restrict on update restrict;

alter table criteria
   add constraint FK_CRITERIA_REFERENCE_CRITERIA foreign key (id_type)
      references criteria_types (id)
      on delete restrict on update restrict;

alter table criteria
   add constraint FK_CRITERIA_REFERENCE_OPERATIO foreign key (id_operation)
      references operations (id)
      on delete restrict on update restrict;

alter table devices
   add constraint FK_DEVICES_REFERENCE_GRAPH_PL foreign key (id_graph_plan)
      references graph_plans (id)
      on delete restrict on update restrict;

alter table element_shapes
   add constraint FK_ELEMENT__REFERENCE_GRAPH_ST foreign key (id_graph_style)
      references graph_styles (id)
      on delete restrict on update restrict;

alter table element_shapes
   add constraint FK_ELEMENT__REFERENCE_SHAPE_TY foreign key (id_shape_type)
      references shape_types (id)
      on delete restrict on update restrict;

alter table element_shapes
   add constraint FK_ELEMENT__REFERENCE_DEVICES foreign key (id_device)
      references devices (id)
      on delete restrict on update restrict;

alter table fault_devices
   add constraint FK_FAULT_DE_REFERENCE_DEVICES foreign key (id_device)
      references devices (id)
      on delete restrict on update restrict;

alter table graph_plans
   add constraint FK_GRAPH_PL_REFERENCE_GRAPH_PL foreign key (id_parent)
      references graph_plans (id)
      on delete restrict on update restrict;

alter table groups
   add constraint FK_GROUPS_REFERENCE_GROUPS foreign key (id_parent)
      references groups (id)
      on delete restrict on update restrict;

alter table groups_criteria
   add constraint FK_GROUPS_C_REFERENCE_GROUPS foreign key (id_group)
      references groups (id)
      on delete restrict on update restrict;

alter table groups_criteria
   add constraint FK_GROUPS_C_REFERENCE_CRITERIA foreign key (id_criteria)
      references criteria (id)
      on delete restrict on update restrict;

alter table guard_objects_devices
   add constraint FK_GUARD_OB_REFERENCE_DEVICES foreign key (id_device)
      references devices (id)
      on delete restrict on update restrict;

alter table guard_objects_devices
   add constraint FK_GUARD_OB_REFERENCE_GUARD_OB foreign key (id_guard_objects)
      references guard_objects (id)
      on delete restrict on update restrict;

alter table handlers
   add constraint FK_HANDLERS_REFERENCE_HANDLER_ foreign key (id_handler_params)
      references handler_params (id)
      on delete restrict on update restrict;

alter table histogram_graphics_chains
   add constraint FK_HISTOGRA_REFERENCE_HISTOGRA foreign key (id_histogram_params_chains)
      references histogram_params_chains (id)
      on delete restrict on update restrict;

alter table histogram_graphics_streams
   add constraint FK_HISTOGRAM_REF_HIST_STREAMS foreign key (id_histogram_params_streams)
      references histogram_params_streams (id)
      on delete restrict on update restrict;

alter table in_external_queue
   add constraint FK_IN_EXTER_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table in_external_queue
   add constraint FK_IN_EXTER_REFERENCE_INTERACT foreign key (id_format)
      references interaction_formats (id)
      on delete restrict on update restrict;

alter table indicator
   add constraint FK_INDICATO_REFERENCE_INDICATO foreign key (id_indicator_type)
      references indicator_type (id)
      on delete restrict on update restrict;

alter table indicator
   add constraint FK_INDICATO_REFERENCE_PARENT foreign key (id_parent)
      references indicator (id)
      on delete restrict on update restrict;

alter table indicator
   add constraint FK_INDICATO_REFERENCE_A_TYPES foreign key (id_a_type)
      references a_types (id)
      on delete restrict on update restrict;

alter table indicator
   add constraint FK_INDICATOR_REF_A_TYPE foreign key (id_ref_attr_type)
      references a_types (id)
      on delete restrict on update restrict;

alter table indicators_values
   add constraint FK_INDICATO_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table indicators_values
   add constraint FK_INDICATOR_IO_OBJECT foreign key (id_indicator)
      references indicator (id)
      on delete restrict on update restrict;

alter table indicators_values
   add constraint FK_INDICATO_IO_OBJECT_SRC foreign key (id_io_object_src)
      references io_objects (id)
      on delete restrict on update restrict;

alter table indicators_values
   add constraint FK_INDICATO_IO_OBJECT_SRC1 foreign key (id_io_object_src1)
      references io_objects (id)
      on delete restrict on update restrict;

alter table io_categories
   add constraint FK_IO_CATEG_REF_CATEG_TYPE foreign key (id_io_category_type)
      references io_category_types (id)
      on delete restrict on update restrict;

alter table io_categories
   add constraint FK_IO_CATEG_REFERENCE_IO_STATE foreign key (id_io_state)
      references io_states (id)
      on delete restrict on update restrict;

alter table io_categories
   add constraint FK_IO_CATEG_CHILD1 foreign key (id_child)
      references io_categories (id)
      on delete restrict on update restrict;

alter table io_categories
   add constraint FK_IO_CATEG_CHILD2 foreign key (id_child2)
      references io_categories (id)
      on delete restrict on update restrict;

alter table io_categories
   add constraint FK_IO_CATEG_REFERENCE_LIFE_CYC foreign key (id_life_cycle)
      references life_cycle (id)
      on delete restrict on update restrict;

alter table io_last_sync
   add constraint FK_IO_LAST__REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table io_last_sync
   add constraint FK_IO_LAST__REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table io_objects
   add constraint FK_IO_OBJEC_REFERENCE_USERS foreign key (author)
      references users (id)
      on delete restrict on update restrict;

alter table io_objects
   add constraint FK_IO_OBJEC_REFERENCE_IO_SYNC_ foreign key (id_sync_type)
      references io_sync_types (id)
      on delete restrict on update restrict;

alter table io_objects
   add constraint FK_IO_OBJEC_REFERENCE_ORGANIZA foreign key (id_owner_org)
      references organization (id)
      on delete restrict on update restrict;

alter table io_objects
   add constraint FK_IO_OBJEC_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table io_objects
   add constraint FK_IO_OBJEC_REFERENCE_SEARCH_T foreign key (id_search_template)
      references search_templates (id)
      on delete restrict on update restrict;

alter table io_objects
   add constraint FK_IO_OBJEC_REFERENCE_IO_TYPES foreign key (id_io_type)
      references io_types (id)
      on delete restrict on update restrict;

alter table io_objects
   add constraint FK_IO_OBJEC_REFERENCE_IO_STATE foreign key (id_io_state)
      references io_states (id)
      on delete restrict on update restrict;

alter table io_objects
   add constraint FK_IO_OBJEC_REFERENCE_MACLABEL foreign key (id_maclabel)
      references maclabels (id)
      on delete restrict on update restrict;

alter table io_objects_organization
   add constraint FK_IO_OBJEC_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table io_objects_organization
   add constraint FK_IO_OBJEC_REFERENCE_IO_OBJEC foreign key (id_io_objects)
      references io_objects (id)
      on delete restrict on update restrict;

alter table io_processing_order
   add constraint FK_IO_PROC_REF_IO_STATE_DEST foreign key (id_state_dest)
      references io_states (id)
      on delete restrict on update restrict;

alter table io_processing_order
   add constraint FK_IO_PROC_REF_IO_STATE_SRC foreign key (id_state_src)
      references io_states (id)
      on delete restrict on update restrict;

alter table io_processing_order
   add constraint FK_IO_PROCE_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table io_processing_order_chains
   add constraint FK_IO_PROCE_REFERENCE_IO_PROCE foreign key (id_io_processing_order)
      references io_processing_order (id)
      on delete restrict on update restrict;

alter table io_processing_order_chains
   add constraint FK_IO_PROCE_REFERENCE_CHAINS foreign key (id_chains)
      references chains (id)
      on delete restrict on update restrict;

alter table io_rubricator
   add constraint FK_IO_RUBRI_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table io_rubricator
   add constraint FK_IO_RUBRI_REFERENCE_RUBRICAT foreign key (id_rubric)
      references rubricator (id)
      on delete restrict on update restrict;

alter table io_sync_archive
   add constraint FK_IO_SYNC__REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table io_sync_archive
   add constraint FK_IO_SYNC__REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table io_templates
   add constraint FK_IO_TEMPL_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table io_urls
   add constraint FK_IO_URLS_REFERENCE_URL_TYPE foreign key (id_url_type)
      references url_types (id)
      on delete restrict on update restrict;

alter table io_views
   add constraint FK_IO_VIEWS_REFERENCE_ATTRS_CA foreign key (id_attr_category)
      references attrs_categories (id)
      on delete restrict on update restrict;

alter table io_views
   add constraint FK_IO_VIEWS_REFERENCE_IO_TEMPL foreign key (id_io_template)
      references io_templates (id)
      on delete restrict on update restrict;

alter table io_views
   add constraint FK_IO_VIEWS_REFERENCE_A_GROUPS foreign key (id_a_group)
      references a_groups (id)
      on delete restrict on update restrict;

alter table life_cycle
   add constraint FK_LIFE_CYC_REF_IO_STATE_ATTR foreign key (id_auto_state_attr)
      references io_states (id)
      on delete restrict on update restrict;

alter table life_cycle
   add constraint FK_LIFE_CYC_REFERENCE_IO_STATE foreign key (id_start_state)
      references io_states (id)
      on delete restrict on update restrict;

alter table life_cycle
   add constraint FK_LIFE_CYC_REF_IO_STATE_IND foreign key (id_auto_state_ind)
      references io_states (id)
      on delete restrict on update restrict;

alter table life_cycle_io_states
   add constraint FK_LIFE_CYC_REFERENCE_LIFE_CYC foreign key (id_life_cycle)
      references life_cycle (id)
      on delete restrict on update restrict;

alter table life_cycle_io_states
   add constraint FK_LIFE_CYC_REFERENCE_IO_STATE foreign key (id_io_states)
      references io_states (id)
      on delete restrict on update restrict;

alter table log
   add constraint FK_LOG_REFERENCE_USERS foreign key (id_user)
      references users (id)
      on delete restrict on update restrict;

alter table log
   add constraint FK_LOG_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table logistic
   add constraint FK_LOGICTIC_PROC_SCENARIO foreign key (id_processing_scenario)
      references processing_scenario (id)
      on delete restrict on update restrict;

alter table logistic
   add constraint FK_LOGICTIC_PROC_VARIANT foreign key (id_processing_variant)
      references processing_variant (id)
      on delete restrict on update restrict;

alter table logistic
   add constraint FK_LOGISTIC_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table logistic
   add constraint FK_LOGISTIC_REFERENCE_HANDLERS foreign key (id_handler)
      references handlers (id)
      on delete restrict on update restrict;

alter table logistic_chains
   add constraint FK_LOGISTIC_REFERENCE_CHAINS foreign key (id_chains)
      references chains (id)
      on delete restrict on update restrict;

alter table logistic_chains
   add constraint FK_LOGISTIC_REFERENCE_LOGISTIC foreign key (id_logistic)
      references logistic (id)
      on delete restrict on update restrict;

alter table mail_lists_position
   add constraint FK_MAIL_LIS_REFERENCE_POSITION foreign key (id_position)
      references "position" (id)
      on delete restrict on update restrict;

alter table mail_lists_position
   add constraint FK_MAIL_LIS_REFERENCE_MAIL_LIS foreign key (id_mail_lists)
      references mail_lists (id)
      on delete restrict on update restrict;

alter table message_journal
   add constraint FK_MSG_REF_POSITION_SEND foreign key (id_dl_sender)
      references "position" (id)
      on delete restrict on update restrict;

alter table message_journal
   add constraint FK_MSG_REF_POSITION_RECV foreign key (id_dl_receiver)
      references "position" (id)
      on delete restrict on update restrict;

alter table message_journal
   add constraint FK_MESSAGE__REFERENCE_URGENCY_ foreign key (id_urgency_level)
      references urgency_levels (id)
      on delete restrict on update restrict;

alter table message_journal
   add constraint FK_MESSAGE__REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table message_series
   add constraint FK_MESSAGE__REFERENCE_MESSAGE_ foreign key (id_message_stream)
      references message_streams (id)
      on delete restrict on update restrict;

alter table message_streams
   add constraint FK_MESSAGE_STREAMS_PART_LOWS foreign key (id_partition_low)
      references partition_lows (id)
      on delete restrict on update restrict;

alter table message_streams
   add constraint FK_MESSAGE_STREAMS_IO_OBJECTS foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table message_streams
   add constraint FK_MESSAGE_STREAMS_POSITION foreign key (id_dl_receiver)
      references "position" (id)
      on delete restrict on update restrict;

alter table message_streams
   add constraint FK_MESSAGE__REFERENCE_TIME_UNI foreign key (id_time_unit)
      references time_units (id)
      on delete restrict on update restrict;

alter table message_streams
   add constraint FK_MESSAGE__REF_POS_SENDER foreign key (id_dl_sender)
      references "position" (id)
      on delete restrict on update restrict;

alter table message_streams
   add constraint FK_MESSAGE__REFERENCE_PROCESSI foreign key (id_processing_scenario)
      references processing_scenario (id)
      on delete restrict on update restrict;

alter table message_streams
   add constraint FK_MESSAGE__REF_VARIANT foreign key (id_processing_variant)
      references processing_variant (id)
      on delete restrict on update restrict;

alter table object_ref_tables
   add constraint FK_OBJECT_R_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table object_ref_tables
   add constraint FK_OBJECT_R_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table object_ref_tables
   add constraint FK_OBJECT_R_REFERENCE_SEARCH_T foreign key (id_search_template)
      references search_templates (id)
      on delete restrict on update restrict;

alter table organization
   add constraint FK_ORGANIZA_REFERENCE_ORGANIZA foreign key (id_type_org)
      references organization_type (id)
      on delete restrict on update restrict;

alter table organization
   add constraint FK_ORG_REF_ORG_PARENT foreign key (id_parent)
      references organization (id)
      on delete restrict on update restrict;

alter table organization
   add constraint FK_ORG_REF_ORG_SUBST foreign key (id_substitutor)
      references organization (id)
      on delete restrict on update restrict;

alter table organization
   add constraint FK_ORG_REF_ORG_PARENT1 foreign key (id_parent1)
      references organization (id)
      on delete restrict on update restrict;

alter table organization
   add constraint FK_ORG_REF_WORK_MODE_CURR foreign key (id_curr_mode)
      references work_mode (id)
      on delete restrict on update restrict;

alter table organization
   add constraint FK_ORG_REF_WORK_MODE_PREV foreign key (id_prev_mode)
      references work_mode (id)
      on delete restrict on update restrict;

alter table organization
   add constraint FK_ORGANIZA_REFERENCE_POSITION foreign key (id_boss)
      references "position" (id)
      on delete restrict on update restrict;

alter table organization_transport
   add constraint FK_ORGANIZA_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table organization_transport
   add constraint FK_ORGANIZA_REFERENCE_TRANSPOR foreign key (id_transport)
      references transport (id)
      on delete restrict on update restrict;

alter table organization_work_mode
   add constraint FK_ORGANIZA_REFERENCE_WORK_MOD foreign key (id_work_mode)
      references work_mode (id)
      on delete cascade on update cascade;

alter table organization_work_mode
   add constraint FK_ORG_REF_WORK_MODE foreign key (id_organization)
      references organization (id)
      on delete cascade on update cascade;

alter table out_external_queue
   add constraint FK_OUT_EXTE_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table out_external_queue
   add constraint FK_OUT_EXTE_REFERENCE_INTERACT foreign key (id_format)
      references interaction_formats (id)
      on delete restrict on update restrict;

alter table out_sync_queue
   add constraint FK_OUT_SYNC_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table persons
   add constraint FK_PERSONS_REFERENCE_TSO_POSI foreign key (id_tso_position)
      references tso_positions (id)
      on delete restrict on update restrict;

alter table persons
   add constraint FK_PERSONS_REFERENCE_TSO_UNIT foreign key (id_tso_unit)
      references tso_units (id)
      on delete restrict on update restrict;

alter table "position"
   add constraint FK_POSITION_REFERENCE_UNITS foreign key (id_unit)
      references units (id)
      on delete restrict on update restrict;

alter table "position"
   add constraint FK_POSITION_REFERENCE_MACLABEL foreign key (id_maclabel)
      references maclabels (id)
      on delete restrict on update restrict;

alter table "position"
   add constraint FK_POSITION_REFERENCE_USER foreign key (id_user)
      references users (id)
      on delete restrict on update restrict;

alter table "position"
   add constraint FK_POSITION_REFERENCE_VRIO foreign key (id_user_vrio)
      references users (id)
      on delete restrict on update restrict;

alter table "position"
   add constraint FK_POSITION_REFERENCE_POSITION foreign key (id_position_type)
      references position_types (id)
      on delete restrict on update restrict;

alter table position_work_mode
   add constraint FK_POSITION_REFERENCE_WORK_MOD foreign key (id_work_mode)
      references work_mode (id)
      on delete cascade on update cascade;

alter table position_work_mode
   add constraint FK_POSITION_REFERENCE_POSITION foreign key (id_position)
      references "position" (id)
      on delete cascade on update cascade;

alter table q_base_table
   add constraint FK_Q_BASE_T_REFERENCE_IO_STATE foreign key (id_io_state)
      references io_states (id)
      on delete restrict on update restrict;

alter table queue_results
   add constraint FK_QUEUE_RE_REFERENCE_TRANSPOR foreign key (id_transport)
      references transport (id)
      on delete restrict on update restrict;

alter table rec_attrs_attrs_values
   add constraint FK_REC_ATTR_REFERENCE_REC_ATTR foreign key (id_rec_attr_value)
      references rec_attrs_values (id)
      on delete restrict on update restrict;

alter table rec_attrs_attrs_values
   add constraint FK_REC_ATTR_REFERENCE_ATTRS_AT foreign key (id_attr_attr)
      references attrs_attrs (id)
      on delete restrict on update restrict;

alter table rec_attrs_values
   add constraint FK_REC_ATTR_REFERENCE_ATTRS_CA foreign key (id_attr_category)
      references attrs_categories (id)
      on delete restrict on update restrict;

alter table receivers
   add constraint FK_RECEIVER_REFERENCE_RECEIVE_ foreign key (id_order)
      references receive_order (id)
      on delete restrict on update restrict;

alter table receivers
   add constraint FK_RECEIVER_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table receivers
   add constraint FK_RECEIVER_REFERENCE_WORK_MOD foreign key (id_work_mode)
      references work_mode (id)
      on delete restrict on update restrict;

alter table receivers
   add constraint FK_RECEIVER_REFERENCE_POSITION foreign key (id_position)
      references "position" (id)
      on delete restrict on update restrict;

alter table record_rubricator
   add constraint FK_RECORD_R_REFERENCE_RECORD_R foreign key (id_parent)
      references record_rubricator (id)
      on delete restrict on update restrict;

alter table record_rubricator
   add constraint FK_RECORD_R_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table record_rubricator
   add constraint FK_RECORD_R_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table report_organization
   add constraint FK_REPORT_O_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table report_organization
   add constraint FK_REPORT_O_REFERENCE_REPORT foreign key (id_report)
      references report (id)
      on delete restrict on update restrict;

alter table roles_actions
   add constraint FK_ROLES_AC_REFERENCE_KKS_ROLE foreign key (id_role)
      references kks_roles (id)
      on delete restrict on update restrict;

alter table roles_actions
   add constraint FK_ROLES_AC_REFERENCE_ACTIONS foreign key (id_service)
      references actions (id)
      on delete restrict on update restrict;

alter table rubric_records
   add constraint FK_RUBRIC_R_REFERENCE_RECORD_R foreign key (id_rubric)
      references record_rubricator (id)
      on delete restrict on update restrict;

alter table rubricator
   add constraint FK_RUBRICAT_REFERENCE_SEARCH_T foreign key (id_search_template)
      references search_templates (id)
      on delete restrict on update restrict;

alter table rubricator
   add constraint FK_RUBRICAT_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table rubricator
   add constraint FK_RUBRICAT_REFERENCE_RUBRICAT foreign key (id_parent)
      references rubricator (id)
      on delete cascade on update restrict;

alter table rubricator
   add constraint FK_RUBRICAT_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table search_template_types
   add constraint FK_SEARCH_T_REFERENCE_SEARCH_T foreign key (id_parent)
      references search_template_types (id)
      on delete restrict on update restrict;

alter table search_templates
   add constraint FK_SEARCH_T_REFERENCE_USERS foreign key (author)
      references users (id)
      on delete restrict on update restrict;

alter table search_templates
   add constraint FK_SEARCH_T_REFERENCE_SEARCH_T foreign key (id_search_template_type)
      references search_template_types (id)
      on delete restrict on update restrict;

alter table search_templates
   add constraint FK_SEARCH_T_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table search_templates
   add constraint FK_SEARCH_T_REFERENCE_GROUPS foreign key (id_group)
      references groups (id)
      on delete restrict on update restrict;

alter table shape_segments
   add constraint FK_SHAPE_SE_REFERENCE_SEGMENT_ foreign key (id_segment_type)
      references segment_types (id)
      on delete restrict on update restrict;

alter table shape_segments
   add constraint FK_SHAPE_SE_REFERENCE_ELEMENT_ foreign key (id_element_shape)
      references element_shapes (id)
      on delete restrict on update restrict;

alter table sheduled_handlers
   add constraint FK_SHEDULED_REFERENCE_HANDLERS foreign key (id_handler)
      references handlers (id)
      on delete restrict on update restrict;

alter table state_crosses
   add constraint FK_IO_LIFE__REF_IO_STATE_SRC foreign key (id_state_src)
      references io_states (id)
      on delete restrict on update restrict;

alter table state_crosses
   add constraint FK_IO_LIFE__REF_IO_STATE_DST foreign key (id_state_dest)
      references io_states (id)
      on delete restrict on update restrict;

alter table state_crosses
   add constraint FK_STATE_CR_REFERENCE_LIFE_CYC foreign key (id_life_cycle)
      references life_cycle (id)
      on delete restrict on update restrict;

alter table table_notifies_io_objects
   add constraint FK_TABLE_NO_REFERENCE_TABLE_NO foreign key (id_table_notifies)
      references table_notifies (id)
      on delete restrict on update restrict;

alter table table_notifies_io_objects
   add constraint FK_TABLE_NO_REFERENCE_IO_OBJEC foreign key (id_io_objects)
      references io_objects (id)
      on delete restrict on update restrict;

alter table tsd
   add constraint FK_TSD_REF_POSITION_TO foreign key (id_dl_to)
      references "position" (id)
      on delete restrict on update restrict;

alter table tsd
   add constraint FK_TSD_REF_POSITION_EXEC foreign key (id_dl_executor)
      references "position" (id)
      on delete restrict on update restrict;

alter table tsd
   add constraint FK_TSD_REF_POSITION_CONTROLLER foreign key (id_dl_controller)
      references "position" (id)
      on delete restrict on update restrict;

alter table tsd
   add constraint FK_TSD_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table tsd
   add constraint FK_TSD_REFERENCE_JR_STATE foreign key (id_jr_state)
      references jr_states (id)
      on delete restrict on update restrict;

alter table tsd_control_journal
   add constraint FK_TSD_CONT_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table tsd_control_journal
   add constraint FK_TSD_CONT_REFERENCE_COMMAND_ foreign key (id_journal)
      references command_journal (id)
      on delete restrict on update restrict;

alter table units
   add constraint FK_UNITS_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table units
   add constraint FK_UNITS_REFERENCE_UNITS foreign key (id_parent)
      references units (id)
      on delete restrict on update restrict;

alter table units
   add constraint FK_UNITS_REFERENCE_WORK_MOD foreign key (id_curr_mode)
      references work_mode (id)
      on delete restrict on update restrict;

alter table units
   add constraint FK_UNITS_REFERENCE_POSITION foreign key (id_boss)
      references "position" (id)
      on delete restrict on update restrict;

alter table units_work_mode
   add constraint FK_UNITS_WO_REFERENCE_WORK_MOD foreign key (id_work_mode)
      references work_mode (id)
      on delete cascade on update cascade;

alter table units_work_mode
   add constraint FK_UNITS_WO_REFERENCE_UNITS foreign key (id_units)
      references units (id)
      on delete cascade on update cascade;

alter table url_extentions
   add constraint FK_URL_EXTE_REFERENCE_MIMETYPE foreign key (id_mimetype)
      references mimetypes (id)
      on delete restrict on update restrict;

alter table urls_objects
   add constraint FK_URLS_OBJ_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table urls_objects
   add constraint FK_URLS_OBJ_REFERENCE_IO_URLS foreign key (id_url)
      references io_urls (id)
      on delete restrict on update restrict;

alter table urls_records
   add constraint FK_URLS_REC_REFERENCE_IO_URLS foreign key (id_url)
      references io_urls (id)
      on delete restrict on update restrict;

alter table user_acl_templates
   add constraint FK_USER_ACL_REFERENCE_USERS foreign key (id_user)
      references users (id)
      on delete restrict on update restrict;

alter table user_chains
   add constraint FK_USER_CHA_REFERENCE_SEARCH_T foreign key (id_search_template)
      references search_templates (id)
      on delete restrict on update restrict;

alter table user_chains
   add constraint FK_USER_CHA_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table user_chains_organization
   add constraint FK_USER_CHA_REFERENCE_ORGANIZA foreign key (id_organization)
      references organization (id)
      on delete restrict on update restrict;

alter table user_chains_organization
   add constraint FK_USER_CHA_REFERENCE_USER_CHA foreign key (id_user_chains)
      references user_chains (id)
      on delete restrict on update restrict;

alter table user_handlers_data
   add constraint FK_USER_HAN_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table user_rubricator
   add constraint FK_USER_RUB_REFERENCE_USERS foreign key (id_user)
      references users (id)
      on delete restrict on update restrict;

alter table user_rubricator
   add constraint FK_USER_RUB_REFERENCE_RUBRICAT foreign key (id_rubricator)
      references rubricator (id)
      on delete restrict on update restrict;

alter table user_templates
   add constraint FK_USER_TEM_REFERENCE_USERS foreign key (id_user)
      references users (id)
      on delete restrict on update restrict;

alter table user_templates
   add constraint FK_USER_TEM_REFERENCE_IO_TEMPL foreign key (id_template)
      references io_templates (id)
      on delete restrict on update restrict;

alter table user_templates
   add constraint FK_USER_TEM_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references io_objects (id)
      on delete restrict on update restrict;

alter table users
   add constraint FK_USERS_REFERENCE_MACLABEL foreign key (id_maclabel)
      references maclabels (id)
      on delete restrict on update restrict;

alter table users
   add constraint FK_USERS_REFERENCE_RANKS foreign key (id_rank)
      references ranks (id)
      on delete restrict on update restrict;

alter table users
   add constraint FK_USERS_REFERENCE_USER_STA foreign key (id_state)
      references user_state (id)
      on delete restrict on update restrict;

alter table work_mode
   add constraint FK_WORK_MOD_REFERENCE_WORK_MOD foreign key (id_mode_type)
      references work_mode_type (id)
      on delete restrict on update restrict;

alter table xml_formats
   add constraint FK_XML_FORM_REFERENCE_XML_QUER foreign key (id_xml_query_type)
      references xml_query_types (id)
      on delete restrict on update restrict;

alter table xml_paths
   add constraint FK_XML_PATH_REFERENCE_XML_QUER foreign key (id_xml_query_type)
      references xml_query_types (id)
      on delete restrict on update restrict;

alter table xml_queries
   add constraint FK_XML_QUERY_TYPES foreign key (id_xml_query_type)
      references xml_query_types (id)
      on delete restrict on update restrict;

alter table xml_queries
   add constraint FK_XML_QUERY_KINDS foreign key (id_xml_query_kind)
      references xml_query_kinds (id)
      on delete restrict on update restrict;

alter table xml_query_responses
   add constraint FK_XML_QUER_REFERENCE_XML_QUER foreign key (id_xml_query)
      references xml_queries (id)
      on delete restrict on update restrict;

