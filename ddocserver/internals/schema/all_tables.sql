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
'Дата и время последней модификации (создания) записи';

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
'Группы атрибутов.
используются в шаблонах для группировки атрибутов при визуализации';

comment on column a_groups."order" is
'Порядок следования группы в шаблоне при отображении';

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
'Справочник типов атрибутов.
Возможные типы:
- логический;
- элемент справочника;
- родитель (ссылка на элемент этого же справочника)
- дата;
- дата/время;
- дробное значение;
- файл;
- целочисленное значение;
- строка
- временной интервал (лет, месяцев, дней, часов, минут);
- гиперссылка.
';

comment on column a_types.id is
'Идентификатор типа атрибута';

comment on column a_types.id_a_view is
'Идентификатор представления атрибутов данного типа';

comment on column a_types.name is
'Название типа атрибута';

comment on column a_types.code is
'Код типа атрибута
В терминах соответствующей СУБД
INT4
VARCHAR
BOOL
TIMESTAMP
и т.д.';

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
'Таблица содержит перечень элементов управления, при помощи которых должны отображаться атрибуты различных типов.
Так, например, атрибуты типа элемент справочника должны отображаться при помощи элемента управления выпадающий список и т.д.';

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
'справочник карт доступа (ИК ТСО)';

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
'Интегральный справочник карт доступа';

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
'Данная таблица хранит информацию о правах доступа ролей к категориям';

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
'Планы доступа (ИК ТСО)';

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
'Чтение характеристик информационного объекта (его атрибутов, системных атрибутов, списка прикрепленных файлов, ассоциированных с ним рубрик, записей справочника (для справочников)) Задание данного права влечет за собой неявное задание права чтения наличия ИО';

comment on column access_table.allow_readlist is
'Чтение наличия информационного объекта, т.е. чтение его системных атрибутов при работе со списком доступных ИО. Данное право определяет присутствие ИО в соответствующем списке';

comment on column access_table.allow_delete is
'Удаление информационного объекта. Задание данного права влечет за собой неявное задание права чтения наличия ИО';

comment on column access_table.allow_update is
'Редактирование характеристик информационного объекта (его атрибутов, системных атрибутов, списка прикрепленных файлов, ассоциированных с ним рубрик, записей справочника (для справочников)). Задание данного права влечет за собой неявное задание права чтения характеристики ИО';

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
'Таблица исключительных прав должносьтных лиц, которые описаны в шаблоне.';

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
'таблица доступных групп в шаблонах дискреционного разграничения доступа должностных лиц к информационным объектам
Доступны следующие группы
1) Права начальника автора ИО
2) Права членов подразделения автора (как должностного лица)
3) Права остальных пользователей БД ККС СИТО ';

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
'группы, права которых заданы в шаблоне';

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
'Таблица ответов на запросы о тревожных событиях
(в ИК ТСО)';

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
'Интегральный справочник событий ИК ТСО';

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
'Атрибуты объектов.
Список атрибутов объекта наследуется из шаблона
Поля table_name и column_name задаются для атрибутов типа элемент справочника. В качестве значения имеют название таблицы, которая содержит содержимое справочника и название колонки таблицы, содержимое которой надо выводить в соответствующем элементе управления
возможным значением такого атрибута будет значение идентификатора записи в таблице table_name

Поле code является уникальным. Недопускается существование двух атрибутов с одинаковым кодом

Атрибуты могут быть системными или пользовательскими. Системные атрибуты используются и заданы при инициализации системы и по умолчанию недоступны для использования.
Деление на системные и пользовательские атрибцты ведено для того, чтобы из списка доступных пользоватед. атрибутов по умолчанию исключить те, которые ему с большой долей вероятности будут не нужны.

Поле ref_column_name содержит название колонки (ключа) в таблице, на которую ссылается данный атрибут. В общем случае это поле может иметь название, отличное от id (id используется по умолчанию). Такая ситуация возможна, если уже существующая структура таблиц была "погружена" в среду DynamicDocs.';

comment on column attributes.id is
'Идентификатор атрибута';

comment on column attributes.id_a_type is
'Идентификатор типа атрибута';

comment on column attributes.id_search_template is
'Идентификатор поискового запроса';

comment on column attributes.id_attr_group is
'Идентификатор группы, в которую входит данный атрибут';

comment on column attributes.code is
'Атрибут может описывать поле таблицы. В этом случае удобно разделить понятие Название и Название Колонки Таблицы. Данное поле хранит Название Колонки Таблицы';

comment on column attributes.name is
'Название атрибута';

comment on column attributes.title is
'Заголовок атрибута (выводится как human-readable название колонки таблицы)';

comment on column attributes.table_name is
'название таблицы БД, в которой присутствует значение атрибута, если текущий атрибут является ссылкой на элемент справочника';

comment on column attributes.column_name is
'Название колонки в таблице, которая содержит значение атрибута, если атрибут является ссылкой на элемент справочника';

comment on column attributes.ref_column_name is
'Название колонки в таблице, на которую ссылается данный атрибут, являющейся первичным ключом. По умолчанию это id';

comment on column attributes.def_width is
'Изначальный размер  колонки в KKSView при визуализации данного атрибута. Измеряется в пикселях.
Является параметром именно атрибута, поскольку разные атрибуты, вне зависимости от их типа могут иметь различный размер при их визуализации в KKSView
Для типов атрибутов с идентификаторами 13 и 14. Предполагается иное использование этого параметра. 
Для типа 13 (многострочный текст) данное поле означает длину измерения oX текста в каждой строке, после превышения которой происходит переход на следующую строку.
Для типа 14 это поле означает количество символов, которое максимально должно быть введено в поле такого типа. CHAR(3), например.';

comment on column attributes.is_system is
'Флаг системности';

comment on column attributes.id_ref_attr_type is
'Служебное поле.
хранит информацию о типе атрибута, на который ссылается данный атрибут (если он является ссылочным, в противном случае NULL).
Данное поле "не видно" при визуализации, т.к. оно не входит в набор атрибутов справочника атрибутов.
Оно используется в запросах, в частности в cGetCategoryAttrs().
Значение данному полю присваивается автоматически при создании атрибута.

Введено для оптимизации запроса cGetCategoryAttrs (чтобы не использовать подзапрос)';

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
'Таблица описывает составные атрибуты
Атрибут может описываться набором других атрибутов, которым могут быть присвоены значения.
И так далее вниз по иерархии.
Иными словами, атрибут может напоминать категорию, но ТОЛЬКО в смысле возможности описания дополнительных характеристик для значения атрибута (ибо как мы помним категория это не только набор атрибутов, но и много чего другого).
Данная возможность описывает принцип вложенности атрибутов.
Поле name описывает название описывающего атрибута.';

comment on column attrs_attrs.id_attr_parent is
'Идентификатор родительского атрибута. Т.е. атрибута который описывается при помощи данного атрибута.
Т.е. Родительский атрибут в данном смысле представляется показателем';

comment on column attrs_attrs.id_attr_child is
'Идентификатор описывающего показатель атрибута';

comment on column attrs_attrs.name is
'Название описыввющего показатель атрибута';

comment on column attrs_attrs.def_value is
'Значение по умолчанию';

comment on column attrs_attrs.is_mandatory is
'Флаг обязательности наличия значения у данного описывающего показатель атрибута';

comment on column attrs_attrs.is_read_only is
'Флаг "только для чтения"';

comment on column attrs_attrs."order" is
'Порядок отображения атрибутов при визуализации его родителя.';

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
'Значения составных атрибутов (показателей) для ИО.
Записи в данной таблице появляются автоматически при редактировании Значения атрибута в информационном объекте';

comment on column attrs_attrs_values.id is
'Идентификатор записи';

comment on column attrs_attrs_values.id_attr_value is
'Идентификатор значения показателя, к которому относится данное значение атрибута';

comment on column attrs_attrs_values.id_attr_attr is
'Идентификатор описывающего показатель атрибута';

comment on column attrs_attrs_values.value is
'Значение (строковое представление) описывающего показатель атрибута';

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
'Флаг, определяющий, является ли данный атрибут обязательным для заполнения для ИО данной категории';

comment on column attrs_categories.is_read_only is
'Флаг, определяющий, является ли данный атрибут доступным только для чтения';

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
'Группы атрибутов. Данный справочник введен потому, что в реальной системе атрибутов может существовать очень много.
Пользователь может создавать свои группы и помещать атрибуты в них. Потом атрибуты будут выводиться в виде дерева, группируясь по группам.';

comment on column attrs_groups.id is
'Идентификатор группы';

comment on column attrs_groups.id_parent is
'Родительская группа';

comment on column attrs_groups.name is
'Название группы';

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
'Таблица значений атрибутов в информационных объектах.
Хранит сами значения атрибутов и их дополнительные характеристики.
Применительно к данной таблице поддерживается темпоральная модель.
Каждое значение атрибута имеет диапазон актуальности.

При изменении значения атрибута фактически происходит создание новой записи в этой таблице. При этом поле value получает новое актуальное значение. Поле start_time по умлчанию устанавливается в current_timestamp, поле stop_time - в NULL, поле meas_time - в current_timestamp, поле insert_time - в current_timestamp, is_actual - в true.
Указанные поля пользователь может задать самостоятельно, однако start_time все равно будет выставлено в current_timestamp, а поле stop_time - в NULL.
Старая (исходная) запись при этом остается. Однако ее поле is_actual устанавливается в false, stop_time - в current_timestamp.

При удалении строки из данной таблицы реального удаления не происходит. Исходная запись остается. Однако ее поле is_actual устанавливается в false, stop_time - в current_timestamp.
';

comment on column attrs_values.id is
'Идентификатор записи';

comment on column attrs_values.id_io_object is
'Идентификатор информауционного объекта';

comment on column attrs_values.id_attr_category is
'Идентификатор показателя';

comment on column attrs_values.value is
'Значение атрибута (показателя) в текстовом представлении';

comment on column attrs_values.start_time is
'Момент начала актуальности значения атрибута (показателя)';

comment on column attrs_values.stop_time is
'Момент утери актуальности значения атрибута (показателя)';

comment on column attrs_values.insert_time is
'Дата и время доведения значения атрибут (показателя) а в систему';

comment on column attrs_values.id_io_object_src is
'Идентификатор ИО, который явился источником информации о значении атрибута (показателя).';

comment on column attrs_values.id_io_object_src1 is
'Идентификатор объекта, за кого передали информацию. Если источник передаёт информацию сам за себя, то значение в этом поле совпадает со значением в поле <Источник>';

comment on column attrs_values.description is
'Подробное описание значения показателя. Как правило указывается причина присвоения данного значения';

comment on column attrs_values.is_actual is
'Флаг, определяющий актуально ли данное значение атрибута, било оно архивное.
Данный флаг задается автоматически при создании и изменении атрибута, зависит от значения поля "момент утери актуальности". Если текущая дата-время попадает в диапазон (момент начала актуальности) - (момент утери актуальности), то флаг устанавливается в true, в противном случае он равен false.';

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
'Статусы  карт доступа (ИК ТСО)';

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
'Типы карт доступа (ИК ТСО)';

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
'Содержит рубрики категории
Данные рубрики затем наследуются ИО, созданными на основе категории
Данные из этой таблицы затем копируются в таблицу rubricator';

comment on column categories_rubrics.id_parent is
'идентификатор родительской рубрики. Не может быть задан вместе с идентификатором категории';

comment on column categories_rubrics.id_category is
'идентификатор категории. Не может быть задан вместе с идентификатором родительской рубрики';

comment on column categories_rubrics.name is
'Название рубрики';

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
'Таблица описаний очередей,  в которые направляются ИО заданной категории и в заданном состоянии для обработки (ссылка на таблицу состояний и категорий).
Сами очереди (ИО в них попавшие) содержатся в таблице очередей (chains_data).
Для обработки ИО заданной категории и находящихся в заданном состоянии допустима только одна очередь.
Поэтому поля "категория"  и "состояние" объявлены уникальным индексом.
В итоге мы добиваемся независимости группы понятий "жизненный цикл" + "состояние" от группы понятий "очередь" + "сервис обработки очереди".
';

comment on column chains.id_handler is
'обработчик очереди (ссылка на таблицу сервисов-обработчиков)
Каждая очередь может иметь только один обработчик
';

comment on column chains.name is
'название очереди';

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
'Таблица очередей.
Содержит ссылки на ИО или ЭИО, которые попали в очередь обработки.
Одновременно должен быть задан только ИО или ЭИО';

comment on column chains_data.id_chain is
'Ссылка на описание очереди обработки';

comment on column chains_data.id_io_object is
'ссылка на ИО, записанный в очередь (тип-документ)';

comment on column chains_data.id_parent is
'ссылается на запись справочника очередей обработки, инициировавшей  помещение ИО в очередь обработки (в случае если при анализе кода возврата сервиса-родителя порождаются новые сервисы)
Если значение атрибуту не задано, то инициатором помещения ИО (ЭИО) в очередь является ЖЦ какого-то ИО';

comment on column chains_data.id_processing_scenario is
'Сценарий обработки ИО в очередях. Указывает на сценарий, который был зафиксирован как активный в момент начала эксперимента';

comment on column chains_data.id_processing_variant is
'Вариант обработки ИО в очередях. Указывает на вариант обработки, который был зафиксирован как активный в момент начала эксперимента';

comment on column chains_data.id_record is
'ижентификатор ЭИО, которыей помещен в очередь (если обрабатывается ЭИО)';

comment on column chains_data.insert_time is
'дата, время создания записи в таблице очередей (формируется автоматически при создании записи)';

comment on column chains_data.is_handled is
'признак обработки записи хранимой процедурой (в исходном состоянии=0, после обработки=1). 
Состояние=1 свидетельствует о том, что хранимая процедура обработки данной таблицы 
вызвала необходимый сервис обработки и передала ему исходные данные для обработки
(номер записи в таблице очередей)';

comment on column chains_data.handled_time is
'дата, время обработки данной записи хранимой процедурой (триггером)';

comment on column chains_data.start_service_time is
'дата, время начала работы сервиса обработки очереди (формируется сервисом)';

comment on column chains_data.end_service_time is
'дата, время завершения  работы сервиса обработки очереди (формируется сервисом)';

comment on column chains_data.return_code is
'код возврата сервиса обработки';

comment on column chains_data.what_happens is
'Причина, по которой запись добавлена в очередь
1 - создан новый ИО
2 - изменились атрибуты (пользовательские) ИО
3 - изменилось состояние ИО
4 - создан новый ЭИО
5 - изменены табличные атрибуты ЭИО
6 - изменены показатели ЭИО
7 - изменилось состояние ЭИО
8 - запись порождена в результате работы некоторого сервиса (имеется запись в таблице logistic)';

comment on column chains_data.handler_in_data is
'Входные данные обработчика очереди>, в которые помещаются входные данные для обработчика очереди, и задача сервиса-обработчика очереди заключается в том, чтобы использовать их при обработке';

comment on column chains_data.handler_out_data is
'<Выходные данные обработчика очереди>, в которые помещаются выходные данные обработчика очереди и задача сервиса-обработчика очереди заключается в том, чтобы сформировать их при обработке';

comment on column chains_data.lc_is_parent is
'логический атрибут, который в состоянии <истина> показывает, что инициатор помещения ИО в очеред - его ЖЦ (т.е данная запись появилась вследствие обработки ЖЦ)';

select setMacToNULL('chains_data');
select createTriggerUID('chains_data');

/*==============================================================*/
/* Table: cmd_confirmations                                     */
/*==============================================================*/
create table cmd_confirmations (
   id_cmd               INT4                 not null
);

comment on table cmd_confirmations is
'Служебная таблица. 
В нее заносятся идентификаторы команд, о которых должны быть отправлены квитанции на другие объекты.
Это возникает, когда команда меняет свое состояние. Например в случае, когда ДЛ ее принял к выполнению.
Данная таблица очищается в задаче mail.';

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
'Журнал учета распоряжений
фактически содержит информацию из нескольких журналов.
- журнала учета входящих распоряжений
- журнала учета исходящих распоряжений';

comment on column command_journal.id_dl_to is
'контроллер';

comment on column command_journal.id_io_category is
'идентификационный код исполняемого документа(код шаблона);
Определяет, какой категории документ исполнять';

comment on column command_journal.insert_time is
'дата создания записи это дата внесения записи  в журнал учета входящих распоряжений';

comment on column command_journal.is_automated is
'вид полученного распоряжения-автоматизированно (TRUE) или вручную(FALSE)';

comment on column command_journal.exec_datetime is
'дата исполнения дата к которой необходимо исполнить  пункт журнала входящих распоряжений (первая дата исполнения пункта);
время исполнения  время к которому необходимо представить донесение об исполнении пункта журнала входящих распоряжений';

comment on column command_journal.exec_period is
'периодичность исполнения  период исполнения данного пункта, начиная с указанной даты и времени (может указываться в часах, сутках, месяцах, кварталах, полугодиях или годах;';

comment on column command_journal.message_body is
'Содержание распоряжения';

comment on column command_journal.is_archived is
'Флаг, определяющий нахождение записей журнала в архиве

Перевод в архивное состояние означает, что записи в состоянии архив не участвуют в визуализации записей журнала и формировании записей журнала контроля исполнения входящих и исходящих распоряжений и табеля срочных донесений ККС СИТО

Перевод в активное состояние означает, что записи в состоянии активное участвуют в визуализации записей журнала и формировании записей журнала контроля исполнения входящих и исходящих распоряжений и табеля срочных донесений ККС СИТО';

comment on column command_journal.id_tabel is
'порядковый номер записи табеля срочных донесений
Если запись в ЖВР или ЖИР ссылается на ТСД
служебное поле. зарезервировано';

comment on column command_journal.id_urgency_level is
'код срочности сообщения (распоряжения)';

comment on column command_journal.id_io_object_att is
'Приаттаченный к распоряжению информационный объект.
Он никак не связан с параметром "категория".
Используется для реализации возможности передачи вместе с распоряжением, например, примера его исполнения (может быть оформлено как раз в виде приаттаченного ИО)';

comment on column command_journal.extra_id is
'служебное поле.
Зарезервировано

Данное поле используется при межобъектовом обмене.
Хранит идентификатор соответствующего распоряжения в БД другого объекта (с которого оно было передано).';

comment on column command_journal.last_exec_datetime is
'Поле актуально только для периодических распоряжений.
Хранит дату-время последнего запланированного выполнения данного распоряжения.
Используется подсистемой периодической выдачи распоряжений в функции cmdAnalyzeJournal()
служебное поле. зарезервировано';

comment on column command_journal.input_number is
'Входящий номер распоряжения. Формируется при создании. Изменению не подлежит';

comment on column command_journal.output_number is
'Исходящий номер распоряжения (формируется однажды в следующих случаях:
1) при приходе распоряжения из внешней организации (в этом случае входящий и исходящий будут различны).
2) при создании распоряжения на данном объекте (в этом случае входящий и исходящий будут одинаковыми).

Изменению не подлежит';

comment on column command_journal.extra_table is
'Распоряжение может выдаваться по какой-то записи некоторого журнала (таблицы БД). В этом случае это поле обеспечивает связь распоряжения с записью в другом справочнике (журнале)';

comment on column command_journal.extra_table_id is
'Распоряжение может выдаваться по какой-то записи некоторого журнала (таблицы БД). В этом случае это поле обеспечивает связь распоряжения с записью в другом справочнике (журнале)';

comment on column command_journal.read_datetime_e is
'Поле системное и не является атрибутом справочника распоряжений и команд.
Имеет отношение только ко входящим распоряжениям. Хранит информацию о дате и времени ознакомления ДЛ-исполнителя  с распоряжением. Заполняется автоматически. NULL означает, что распоряжение является новым (непрочитанным) и в дереве распоряжений на клиенте должно отображаться жирным шрифтом';

comment on column command_journal.read_datetime_c is
'Поле системное и не является атрибутом справочника распоряжений и команд.
Имеет отношение только к распоряжениям попадающим в папку "на контроле" (т.е. влияет на ДЛ-контроллера). Хранит информацию о дате и времени ознакомления ДЛ-контроллера с данным распоряжением. Заполняется автоматически. NULL означает, что распоряжение является новым (непрочитанным) и в дереве распоряжений на клиенте должно отображаться жирным шрифтом';

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
'Взаимодействующие организации, с которыми данная организация взаимодействует в рамках системы';

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
'ID  атрибута, если описываемый критерий имеет типы 5 или 6 (заданный атрибут с произвольным или заданным значением)';

comment on column criteria.value is
'Значение рассматриваемого критерия.
В зависимости от типа  критерия приводится к нужному типу данных (int4, varchar и т.д.). Значение должно приводится.
Для типа критерия 5 имеет значение NULL
Для типов критериев 4 и 6 в случае атрибутов, ссылающихся на справочники, значением данного поля должно являться значение, понятное пользователю. В дальнейшем подсистема поиска будет проверять, есть ли в указанном справочнике строка с данным значением.';

comment on column criteria.is_not is
'Данный флаг задает, применяется ли унарная операция отрицания к данному критерию';

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
'Типы критериев автоматической разводки по рубрикам
1 - принадлежит заданной категори
2 - заданный автор
3 - наличие прикрепленного файла заданного типа
4 - наличие атрибута с заданным значением
5 - наличие заданного атрибута с произольным значением
6 - наличие заданного атрибута с заданным значением';

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
'Интегральный справочник устройств и охраняемых объектов (ИК ТСО)';

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
'Устройства (элементы) графических планов (ИК ТСО)';

comment on column devices.id_graph_plan is
'Это поле является необязательным, поскольку иногда бывает ситуация, когда приходит устройство, а информации о граф. плане в нем нет. Так происходит, например, при некорректной смене очередности пакетов (ответов на запросы).';

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
'Виды документов (для документооборота)';

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
'Подтипы документов (для документооборота)';

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
'Типы документов (для документооборота)';

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
'Фигуры, которыми отображено устройство на граф. плане (ИК ТСО)';

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
'Справочник неисправных устройств (ИК ТСО)';

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
'Интегральный справочник неисправных устройств';

comment on column fault_devices_q.is_deleted is
'Из данной таблицы записи не удаляются. А только помечаются как удаленные в случае, если в последнем ответе на запрос неисправных устройств не упоминается заданное.';

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
'Справочник граф. планов (ИК ТСО)';

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
'Справочник стилей отображения графических примитивов (ИК ТСО)';

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
'0 - ИЛИ
1 - И

Операция, применяемая к критериям и вложенным группам текущей группы критериев.';

comment on column groups.is_not is
'Данный флаг задает, применяется ли унарная операция отрицания к данной группе критериев';

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
'Справочник охраняемых объектов (ИК ТСО)';

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
'Набор элементов справочника устройств, входящих в объект охраны (ИК ТСО)';

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
'Параметры сервисов-обработчиков
Обеспечивают версионность. 
Вынесение параметров сервиса в отдельную таблицу позволяет задавать для обработки очереди некоторый фиксированный сервис, а потом осуществлять ведение его различных версий';

comment on column handler_params.name is
'название сервиса';

comment on column handler_params.handler_version is
'версия сервиса';

comment on column handler_params.h_host is
'IP-адрес хоста, на котором функционирует сервис-обработчик';

comment on column handler_params.h_port is
'Порт хоста, на котором функционирует сервис-обработчик';

comment on column handler_params.service is
'параметры сервиса (сигнатура сервиса)
Каждый сервис должен иметь следующие вх. параметры:
-Идентификатор записи таблицы очередей, инициировавшей обработчик
Каждый сервис при его вызове обязан:
-произвести отметку записи в таблице очередей, когда он начал обработку
-произвести отметку записи в таблице очередей, когда он завершил обработку
-выдать результат обработки в выходную очередь (в общем случае- в очереди)
';

comment on column handler_params.extra_params is
'Дополнительные параметры для запуска (если требуются)';

comment on column handler_params.handler_in_data is
'XML-атрибут сервиса';

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
'Сервисы-обработчики ИО, которые попали в определенную очередь обработки.
Для каждой очереди может быть определен только один сервис-обработчик.
Сервис-обработчик представляет собой автономную подпрограмму, которая получает на вход идентификатор записи в таблице очередей и приступает к ообработке ИО, определяемого данным идентификатором.';

comment on column handlers.name is
'Название';

comment on column handlers.description is
'Описание';

comment on column handlers.is_external is
'Поле определяет, является ли сервис-обработчик очереди внешней по отношению к БД программой или же явлется хранимой процедурой БД. ';

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
'Таблица графиков гистограмм для сервисов';

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
'Таблица графиков гистограмм';

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
'Таблица запросов для построения гистограмм по сервисам';

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
'Таблица запросов для построения гистограмм по потокам сообщений';

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
'Входящая очередь для единоразового получения информационных ресурсов (записей справочников) из внешних взаимодействующих систем (не DynamicDocs)';

comment on column in_external_queue.id_organization is
'Организация, из которой поступили данные';

comment on column in_external_queue.id_format is
'формат представления данных';

comment on column in_external_queue.in_data is
'Строкове представление входящих данных';

comment on column in_external_queue.interaction_type is
'Тип взаимодействия
1 - создать новую сущность на целевом объекте
2 - заменить сущность на данную на целевом объекте (если она не существует, создать новую)
3 - удалить сущность на целевом объекте
';

comment on column in_external_queue.interaction_result is
'Результаты сохранения информационного ресурса в БД
1 - Данные только поступили, их обработка еще не начиналась (значение по умолчанию).
2 - создание информационного ресурса завершилась успешно
3 - произошла ошибка при создании информационного ресурса в БД';

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
'Таблица показателей информационных объектов.
Для соответствия модели Заря22 введен такой тип информации как показатели информационных объектов.
Отличаются от обычных атрибутов тем, что имеют набор более расширенных характеристик. А также сохраняют историю значений';

comment on column indicator.table_name is
'Заполняется для индикаторов, берущих значения из справочников. 
Тип данных таких индикаторов - ссылка ';

comment on column indicator.column_name is
'Заполняется для индикаторов, берущих значения из справочников. 
Тип данных таких индикаторов - ссылка или родитель/потомок.
В последнем случае - table_name не заполняется';

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
'Значения показателей для информационных объектов';

comment on column indicators_values.id_io_object_src is
'Идентификатор ИО, который явился источником информации о значении показателя.';

comment on column indicators_values.id_io_object_src1 is
'Идентификатор объекта, за кого передали информацию. Если источник передаёт информацию сам за себя, то значение в этом поле совпадает со значением в поле <Источник>';

comment on column indicators_values.meas_time is
'Дата и время измерения значения показателя';

comment on column indicators_values.insert_time is
'Дата и время доведения значения показателя в систему';

comment on column indicators_values.the_value is
'Значение показателя';

comment on column indicators_values.start_time is
'Дата и время начала актуальности показателя';

comment on column indicators_values.stop_time is
'Дата и время окончания актуальности показателя';

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
'Форматы представления данных (информационных объектов, записей справочников), используемые для взаимодействия с внешними системами
Варианты:
1) unified XML format (унифицированный формат описания информационных объектов IRL)
2) param-value representation (информационный объект, запись справочника предстают в виде набора атрибутов, описываемых в виде атрибут-строковое представление значения). Используется для простого базового взаимодействия. Проверка на соответствие структурам данных не осуществляется
3) shushun (формат представления данных ЦНИИ ЭИСУ в ОКР "Заря 22")
4) internal (внутренний формат представления данных при взаимодействии между системами DynamicDocs)
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
'таблица категорий информационных объектов. Каждая категория обладает набором атрибутов, которые должны иметь объекты данной категории. Кроме того, категория может иметь дочернюю категорию, которая определяет структуру таблицы, которая будет содержать записи объекта данной категории, если объект является контейнерным (т.е. содержит экземпляры информационного объекта). Примером такого объекта являются журналы и справочники.
Если ИО не является конткйнерным, то данное поле (id_child) должно быть пусто (точнее если данное поле пусто, то объекты данной категории не являются контейнерными)
Кроме того категория может иметь вторую дочернюю категорию, которая описывает набор показателей контейнерного информационного объекта. Показатели отличаются от атрибутов наличием темпоральной модели и расширенным набором параметров';

comment on column io_categories.id_child is
'У категории, описывающей справочник может быть 2 подчиненных категории, первая описывает таблицу (набор колонок), вторая - допустимый набор пользовательских атрибутов (показателей), которыми могут обладать записи справочников.
Данное поле описывает категорию для колонок таблицы';

comment on column io_categories.id_child2 is
'У категории, описывающей справочник может быть 2 подчиненных категории, первая описывает таблицу (набор колонок), вторая - допустимый набор пользовательских атрибутов (показателей), которыми могут обладать записи справочников.
Данное поле описывает категорию для показателей';

comment on column io_categories.id_io_state is
'состояние категории с точки зрения синхронизации';

comment on column io_categories.id_life_cycle is
'Жизненный цикл информационных объектов данной категории';

comment on column io_categories.is_global is
'признак глобальности категории';

comment on column io_categories.is_completed is
'0 - категория находится в процессе изменения (создания)
1 - процесс завершен. Происходила операция изменения категории
2 - процесс завершен. Происходила операция создания новой категории';

comment on column io_categories.is_archived is
'Архивные категории не используются в работе. Считаются удаленными.';

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
'Справочник типов категорий.
- справочник;
- журнал;
- документ;
- формуляр;
- сообщение;
- шаблон документа.
Ряд других.


';

comment on column io_category_types.r_name is
'Название типа категории в родительном падеже';

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
'Системный справочник с информацией о дате и времени последней синхронизации и минимальном периоде синхронизации';

comment on column io_last_sync.min_period is
'минимальный период синхронизации';

comment on column io_last_sync.last_sync is
'дата и время последней синхронизации';

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
'таблица всех информационных объектов (документы, справочники, журналы, табели и пр.). Каждый ИО попадает в эту таблицу и в ней получает уникальный идентификатор. ИО может быть контейнерным (например, журналы, табели, справочники), в этом случае он имеет таблицу, которая содержит записи. Записи не являются ИО, они являются содержимым контейнерного ИО. Также ИО имеет набор атрибутов, который наследует из своей категории.
Также ИО имеет общий для всех ИО набор атрибутов, который перечислен в данной таблице (дата-время создания, автор, текущее состояние и т.п.)
Важным атрибутом ИО является гриф секретности. Он задается системным полем "MACLABEL", которое добавляется при создании таблицы io_objects.';

comment on column io_objects.id_io_state is
'текущее состояние информационного объекта';

comment on column io_objects.id_maclabel is
'Гриф секретности информационного объекта';

comment on column io_objects.id_sync_type is
'вид ИО с точки зрения синхронизации';

comment on column io_objects.id_owner_org is
'Организация-владелец информационного объекта. Критично при информационном обмене.
Только организация-владелец может быть инициатором информационного обмена.';

comment on column io_objects.table_name is
'название таблицы, которая содержит записи информационного объекта, если объект является контейнерным. Например, журнал или справочник. Если документ не является контейнерным данное поле должно быть NULL';

comment on column io_objects.description is
'Краткая характеристика объекта';

comment on column io_objects.information is
'Развернутая характеристика объекта';

comment on column io_objects.is_system is
'Флаг определяет является ли данный ОИ системным, т.е. существует всегда в БД.
Как правило системные ИО являются контейнерными. Например, 
- справочник типов объектов
- справочник типов атрибутов объектов
- справочник категорий документов
- справочник состояний документов
- и т.п.
';

comment on column io_objects.is_completed is
'Данное поле является системным и используется только базой данных для реализации механизма управления пользовательскими обработчиками и синхронизации применительно к справочнику информационных объектов.
Особенность этого справочника заключается в том, что атрибуты ИО хранятся в отдельной таблице и задаются при помощи отдельной функции. В итоге, появление новой записи в таблице io_objects еще не означает появление нового ИО в БД. Можно говорить о том. что ИО появился в БД, когда ему заданы значения всех требуемых атрибутов.

Смена значения данного поля с 0 на 1 говорит о том, что все значения атрибутов заданы и ИО появился в БД. 
Смена данного поля с 0 на 2 говорит о том, что сушествуюший ИО подвергся изменению и все изменения завершены. 
Менять данное значение должна функция ioSetCompleted().
Она должна вызываться всякиц раз, когда происходит создание ИО.';

comment on column io_objects.ref_table_name is
'Если таблица информационного объекта унаследована от другой таблицы, то в этом поле указывается название последней';

comment on column io_objects.r_icon is
'иконка при отображении информационного объекта в списке или в рубрикаторе. Если не задана, то используется значение по умолчанию (определяется клиентским приложением). При отображении в рубрикаторе используется, если не задана иконка в соответствующей записи io_rubricator';

comment on column io_objects.uuid_t is
'Уникальный идентификатор ИО';

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
'Организации, с которыми осуществляется синхронизация данного ИО';

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
'Справочник подярка обработки информационных объектов в различных состояниях жизненного цикла';

comment on column io_processing_order.name is
'название';

comment on column io_processing_order.id_state_src is
'Из какого (id_state_src) состояния в какое (id_state_dest) переходит ИО, которые должны обрабатываться данной очередью';

comment on column io_processing_order.id_state_dest is
'Из какого (id_state_src) состояния в какое (id_state_dest) переходит ИО, которые должны обрабатываться данной очередью';

comment on column io_processing_order.id_io_category is
'ИО какой категории обрабатываются данной очередью';

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
'Перечень очередей, в которые должны быть помещены ИО и ЭИО при изменении их состояния';

comment on column io_processing_order_chains.id_io_processing_order is
'порядок обрботки ИО в очереди';

comment on column io_processing_order_chains.id_chains is
'очередь, в которую должен быть помещен ИО, удовлетворяющий условиям, указанным в порядке обработки';

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
'Флаг, определяющий, каким образом добавлен ИО в рубрику.
TRUE - при помощи вызова поискового запроса
FALSE - вручную.
Используется при работе механизма автоматической рубрикации. При отработке поискового запроса на помещение ИО в данную рубрику те ИО, которые помещены автоматически будут предварительно удалены.';

comment on column io_rubricator.r_icon is
'Иконка при отображении элемента в дереве рубрик. Если не задана, то используется иконка по умолчанию (определяется клиентским приложением)';

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
'состояния информационных объектов (категорий)
ИО в заданном состоянии и заданной категории могут поступать на обработку в некоторую очередь.
Описание данной очереди задается в таблице chains
При этом есть некоторые системные состояния, которые служат лишь для внутренних целей.
-активный
-архивный
-осуществляется первоначальная синхронизация
-осуществляется синхронизация';

comment on column io_states.is_system is
'флаг системного состояния. Состояния с идентификаторами 2-5 являются системными и не могут быть использованы для назначения информационным объектам';

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
'Системная таблица результатов синхронизации информационных объектов';

comment on column io_sync_archive.sync_datetime is
'дата и время проведения операции синхронизации';

comment on column io_sync_archive.sync_result is
'результат синхронизации';

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
'Типы информационных объектов с точки зрения синхронизации
Существуют 4 типа:
- Глобальные централизованного ведения
- глобальные формуляры
- глобальные объектового ведения, интегрируемые
- глобальные объектового ведения, автономные';

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
'Таблица шаблонов, которые заданы для той или иной категории. 
Шаблон определяет группирование и видимость атрибутов для заданной категории. 
В терминах "документ-представление" категория - это документ, а шаблон - это представление';

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
'Типы информационных объектов';

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
'Если ИО является документом и к нему прикреплены файлы, то данная таблица содержит список урлов для данного ИО
URL задается относительно файловой системы сервера БД
Для поля url создан PARTIAL UNIQUE индекс (не разрешаем иметь записи с одинаковым URL, однако может существовать много записей с еще не заданным URL, который определяется значением ''not assigned'')';

comment on column io_urls.src_ext is
'Исходное расширение файла, которое у него было перед тем, как он был импортирован на сервер.
Например. Если импортировался файл file.doc, то на сервере он будет записан в хранилище с новым названием (kks_file_1, например, т.е. без расширения), а в это поле запишется значение ''doc''
Это нужно для того, чтобы выгружать файлы на клиент с расширением.';

comment on column io_urls.uploaded is
'Данный флаг определяет, загружен ли файл на сервер или нет.
Его необходимость вызвана тем, что файлы на сервер загружаются блоками (особенно это критично при информационном обмене). А поскольку размеры файлов могут быть очень большими, то процесс их загрузки на сервер может занимать длительное время, в течение которого файл, фактически, еще не готов к использованию (в нем еще не все данные). В это же время возможен факт обращения к данному файлу, и нам надо иметь атрибут, определяющий, можно ли файл уже использовать или нет.
Файлы, которые использовать еще нельзя, в перечне файлов информационного объекта выводиться не должны.';

comment on column io_urls.hash_sum is
'Хэш-сумма файла';

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
'разбивка атрибутов в шаблоне категории по группам
В заданном шаблоне атрибут может быть read_only, даже если он не read_only в таблице attrs_categories.
Если в таблице attrs_categories атрибут объявлен как read_only, то значение данного поля игнорируется';

comment on column io_views."order" is
'Порядок следования атрибута в группе';

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
'Таблица возможных состояний записей в табелях срочных донечений, а также журналов распоряжений
Каждый пункт табелей и журналов может быть:
- запланирован
- выдано задание
- исполняется
- исполнен
- просрочен
- отменен
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
'Является родительской таблицей для следующих таблиц:
1) users
2) position
3) units

Прямая запись в эту таблицу запрещена';

comment on column kks_roles.with_inheritance is
'Данный атрибут определяет наследуемость прав от родительской роли.
Например, пользователь наследует права должностного лица, в качестве которого он работает в БД.
Или должностное лицо наследует права подразделения, в которое оно входит.';

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
'Жизненный цикл
определяет набор состояний ИО, а также последовательность переходов между ними
ЖЦ является характеристикой категории. Соответственно ИО может иметь только те состояния, которые определены в ЖЦ его категории.';

comment on column life_cycle.id_start_state is
'Начальное состояние, в котором создаются ИО заданной категории, имеющей данный жизненный цикл';

comment on column life_cycle.id_auto_state_attr is
'Автовозврат состояния при изменении атрибутов';

comment on column life_cycle.id_auto_state_ind is
'Автовозврат состояния при изменении показателей';

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
'Набор допустимых состояний в данном жизненном цикле.
Важно учитывать, что состояние id_start_state в таблице жизненных циклов также неявно добавляется в набор допустимых состояний, если оно не присутствует в этой таблице. При этом при удалении или смене id_start_state из данной таблицы он не удаляется.
Данная таблица определяет лишь набор допустимых состояний.
Последовательность переходов между состояниями определяется таблицей переходов';

comment on column life_cycle_io_states.id_life_cycle is
'Жизненный цикл';

comment on column life_cycle_io_states.id_io_states is
'состояние, в него входящее';

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
'журнал учета работы пользователей с БД.
В данную таблицу вносятся изменения, которые были сделаны пользователями с информационными объектами.
Примерами таких изменений могут служить:
- добавление новой записи в журнал учета входящих распоряжений
- добавление новой записи в табель срочных донесений
- создание документа
- создание справочника
- создание новой записи справочника
- редактирование записи справочника/табеля/журнала
- и т.п.';

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
'Управляющая таблица, обеспечивающая реализацию логики обработки ИО различных категорий цепочкой взаимосвязанных сервисов (т.е. обеспечивающая вызов необходимых для дальнейшей обработки сервисов в зависимости от результата обработки текущим сервисом)

данная таблица фактически несет следующий смысл:
Если Активным является некоторый зафиксированный сценарий обработки и в chains_data некоторым сервисом обработан заданный ИО (ЭИО), и при этом код возврата равен заданному, то необходимо данный ИО (ЭИО) поместить в указанную очередь (набор очередей)';

comment on column logistic.id_processing_scenario is
'Сценарий обработки (для различных сценариев могут быть различные последовательности)
';

comment on column logistic.id_processing_variant is
'Вариант обработки';

comment on column logistic.id_io_object is
'ИО';

comment on column logistic.id_handler is
'Сервис (ссылка на таблицу каталога сервисов)
';

comment on column logistic.handler_in_data is
'XML-вх. атрибут сервиса (входные параметры сервиса)';

comment on column logistic.handler_out_data is
'XML-атрибут сервиса (возвращаемые параметры)
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
'Перечень очередей для
последующей обработки ИО.
(для одного и того-же кода возврата может быть несколько очередей обработки)
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
'Данный справочник содержит список грифов секретности информационных объектов.';

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
'Списки рассылки должностным лицам сообщений и распоряжений.
В том числе данные списки используются для упрощения задания прав доступа к ИО. 
Данный системный справочник используется только в целях упрощения работы пользователей';

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
'перечень ДЛ, входящих в список рассылки';

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
'журнал учета сообщений
Фактически состоит из двух журналов:
- ЖИС
- ЖВС
';

comment on column message_journal.id_io_object is
'ссылка на ИО, который представляет собой суть сообщения (документ)';

comment on column message_journal.id_urgency_level is
'Код срочности';

comment on column message_journal.sent_datetime is
'время отправки сообщения';

comment on column message_journal.receive_datetime is
'Время получения сообщения на сервере получателя';

comment on column message_journal.read_datetime is
'Время прочтения сообщения получателем';

comment on column message_journal.message_body is
'Тело сообщения. Не обязательное поле.
Может содержать неформализованный текст';

comment on column message_journal.is_outed is
'Флаг, указывающий на то, что сообщение отправлено получателю.
Поле используется следующим образом
0 - не отправлено
1 - отправлена информация о получении
2 - отправлена информация о прочтении
3 - отправка не требуется

Если исходящее сообщение (т.е. пользователь инициировавший вставку не jupiter) 
предназначено локальному ДЛ данной БД, то считается что отправка не требуется is_outed = 3

Если оно предназначено удаленному ДЛ, то оно считается не отправленным. (is_outed = 0)
В этом случае kksinteractor его отправит получателю и после его успешной доставки выставит этому полю значение 3

Если сообщение является входящим и отправителем является удаленное ДЛ, то is_outed = 0
В этом случае kksinteractor отправит квитанцию отправителю о времени доставки сообщения.
После успешной доставки квитанции is_outed будет выставлено в 1.

Если  сообщение является входящим и отправителем является удаленное ДЛ и получатель отметил сообщение как прочитанное, то 
в этом случае kksinteractor отправит квитанцию отправителю о времени прочтения сообщения.
После успешной доставки квитанции is_outed будет выставлено в 2.

Во всех остальных случаях is_outed = 3. ';

comment on column message_journal.extra_id is
'служебное поле.
Зарезервировано

Данное поле используется при межобъектовом обмене.
Хранит идентификатор соответствующего сообщения в БД другого объекта (с которого оно было передано).';

comment on column message_journal.input_number is
'Входящий номер сообщения. Формируется при создании. Изменению не подлежит';

comment on column message_journal.output_number is
'Исходящий номер сообщения (формируется однажды в следующих случаях:
1) при приходе сообщения из внешней организации (в этом случае входящий и исходящий будут различны).
2) при создании собщения на данном объекте (в этом случае входящий и исходящий будут одинаковыми).

Изменению не подлежит';

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
'Пядь
Справочник генерации потоков сообщений (информационых объектов)';

comment on column message_streams.id_io_object is
'ссылка на информационный объект типа справочник, на основании  которого <генерятся> сообщения потока путем последовательной выборки записей справочника, при достижении конечной записи справочника производится переключение на первую запись и т.д.';

comment on column message_streams.id_dl_sender is
'НЕ НАДО ЭТО ПОЛЕ ДЕЛАТЬ SERIAL! Оно наследуется от kks_roles. После создания данной таблицы отрабатывает скрипт, который задает ему значение по умолчанию в nextval(''kks_roles_id_seq'');';

comment on column message_streams.id_dl_receiver is
'Должностное лицо-адресат  потока';

comment on column message_streams.id_processing_scenario is
'задается текущий активный сценарий (из настроечного ИО)';

comment on column message_streams.id_processing_variant is
'задается текущий активный вариант (из настроечного ИО)';

comment on column message_streams.lambda is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.sigma is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.moda is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.min_p is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.max_p is
'Входит в группу задания параметров закона распределения';

comment on column message_streams.start_time is
'Дата и время начала интервала генерации потока';

comment on column message_streams.stop_time is
'Дата и время завершения генерации потока';

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
'Объединение type + subtype';

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
'Когда квитанция должна генерироваться
1 - INSERT
2 - UPDATE
3 - DELETE
Используется в справочнике асинхронных квитанций в виде атрибута типа старые чекбоксы';

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
'Дополнительные таблицы информационного объекта.
Допускаются только для информационных объектов типа "справочник"';

comment on column object_ref_tables.table_name is
'название физической таблицы БД, хранящей записи данного справочника';

comment on column object_ref_tables.id_io_object is
'идентификатор соответствующего информационного объекта';

comment on column object_ref_tables.id_io_category is
'категория, которой должна соответствовать структура дополнительной таблицы';

comment on column object_ref_tables.title is
'название (заголовок) дополнительной таблицы';

comment on column object_ref_tables.id_search_template is
'идентификатор поискового запроса, который используется для формирования записей данной дополнительной таблицы, если справочник виртуальный';

comment on column object_ref_tables.ref_table_name is
'название физической таблицы БД (базового справочника), который используется для формирования записей данной доп. таблицы (для виртуальных справочников)';

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
'Операции при автоматической разводке по рубрикам
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
'Организации';

comment on column organization.id_parent is
'Головная (вышестоящая организация - для повседневной деятельности';

comment on column organization.id_parent1 is
'Головная (вышестоящая) организация - на особый период';

comment on column organization.id_substitutor is
'Организация которой передаются функции данной организации при выходе ее из строя';

comment on column organization.id_curr_mode is
'Текущий режим функционирования объекта(состояние объекта)';

comment on column organization.id_boss is
'руководитель организации.
Данное поле не является обязательным, поскольку в противном случае будет невозможно создать организацию, не указав руководителя (должностное лицо).
Однако нельзя создать ни одно должностное лицо вне подразделения и соответственно вне организации';

comment on column organization.code_name is
'Условное наименование (в используемых словарях применяется именно этот атрибут)';

comment on column organization.email_prefix is
'Префикс электронной почты организации.
Должен быть уникальным. Текущаяя подсистема генерации уникальных ключей основана на том принципе, что данное поле является уникальным.
Существует уникальный индекс на данное поле';

comment on column organization.map_symbol is
'Условный код знака объекта, наносимого на карту (XPM)';

comment on column organization.tree_symbol is
'Знак организации (объекта) в дереве ОШС(JPG)';

comment on column organization.dt_curr_mode is
'Время  и дата ввода текущего режима';

comment on column organization.is_completed is
'Служебный флаг, который определяет, что создание в справочнике новой организации завершено. При смене значения данного флага с FALSE на TRUE запускается механизм проверки связи с введенной организацией.
Флаг меняется при задании для данной организации хотя бы одного транспорта и назначения адреса (в справочнике organization_transport)';

comment on column organization.is_created is
'Данное поле задает условие (и результат) проверки связи с данной организацией.
Возможные значения:
0 - ожидается проверка связи (значение по умолчанию)
1 - проверка связи завершена успешно
2 - проверка связи завершилась с ошибкой
Работать можно только с организациями, которые имеют 1 в качестве значения данного поля';

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
'используемые организацией транспорты межобъектового обмена

ВАЖНО!!!
В случае, если текущая БД является подчиненой, в функции replaceLocalOrg() создается уникальный индекс на поля address, port
В скрипте создания БД его создавать нельзя, поскольку при первоначальной синхронизации возможно дублирование адреса.

Если текущая БД главная, то уникальный индекс на данные поля создается в инсталляторе';

comment on column organization_transport.address is
'адрес организации в данном транспорте';

comment on column organization_transport.port is
'Порт транспортной задачи, которая обрабатывает БД данной организации.
Порт используется только в IP-сетях, если используется транспорт http_connector';

comment on column organization_transport.use_gateway is
'Использовать ли шлюз для взаимодействия (ТПС в настоящее время)';

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
'Тип организации (стационарный, мобильный, внешняя унаследованная система)';

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
'Режимы функционирования организации с указанием типа (боеготовности)';

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
'Исходящая очередь для единоразовой отправки информационных объектов (записей справочников) во внешние взаимодействующие системы (не DynamicDocs)';

comment on column out_external_queue.id_organization is
'В какую организацию отправляем';

comment on column out_external_queue.id_format is
'При помощи какого формата представлять данные';

comment on column out_external_queue.id_io_object is
'Какой информационный ресурс передавать.

При этом если передается сообщение из message_journal, 
то сюда должен быть записан идентификатор ИО "Журнал сообщений" (ИД=20), 
а в поле id_entity должен быть записан идентификатор сообщения. 
Но это в том случае, если к сообщению не прикреплен ИО';

comment on column out_external_queue.id_entity is
'Если указано значение, то передается не сам информационный ресурс, а запись его справочника (для информационных ресурсов, являющихся справочниками)';

comment on column out_external_queue.interaction_type is
'Тип взаимодействия
1 - создать новую сущность на целевом объекте
2 - заменить сущность на данную на целевом объекте (если она не существует, создать новую)
3 - удалить сущность на целевом объекте
';

comment on column out_external_queue.interaction_result is
'Результаты информационного взаимодействия (передачи данных)
1 - ожидается передача (стоит в очереди на отправку). Это значение по умолчанию.
2 - передача осуществляется (информационный обмен еще не завершен)
3 - передача завершилась успешно
4 - произошла ошибка';

comment on column out_external_queue.out_data is
'Сформированное XML-описание пересылаемой сущности';

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
'Очередь системных исходящих сообщений для синхронизации.
Доступна для пользователя jupiter

Таблица унаследована от root_table. При этом поле last_update используется как дата и время помещения записи в очередь. Фактически речь идет о дате и времени отправки адресату.';

comment on column out_sync_queue.id_organization is
'Организация, с которой осуществляется синхронизация';

comment on column out_sync_queue.id_entity is
'идентификатор пересылаемой сущности.
Берется из соответствующей таблицы, назавние которой задается полем entity_table.';

comment on column out_sync_queue.entity_table is
'Название таблицы, в которой содержится пресылаемая сущность
- io_objects - для ИО
- io_categories - для категорий
- иное название таблицы - для ЭИО';

comment on column out_sync_queue.entity_type is
'Тип пересылаемой сущности
1 - категория (системная). Пересылается вместе с атрибутами.
2 - ИО (системный).
3 - ЭИО (запись справочника). В этом случае необходимо указывать название таблицы БД, в которой хранятся записи справочника.
4 - ЭИО (запись дополнительной таблицы справочника). В этом случае необходимо указывать название таблицы БД, в которой хранятся записи справочника.
5 - Дополнительная таблица справочника (передается вместе с категорией, на основе которой она создана и UID информационного объекта, к которому она относится)
6 - Передача запроса на проверку связи
7 - Зарезервировано
8 - передача полного справочника организаций на новый объект
9 - передача записи справочника организаций на все участвующие в инфю обмене объекты
10 - публичное ДЛ (создание, обновление)
11 - публичное ДЛ (удаление)
12 - файл, передаваемый блоками (создание, обновление)
13 - файл, передаваемый блоками (удаление)
';

comment on column out_sync_queue.sync_type is
'Тип синхронизации
1 - создать новую сущность на целевом объекте
2 - заменить сущность на данную на целевом объекте (если она не существует, создать новую)
3 - удалить сущность на целевом объекте';

comment on column out_sync_queue.sync_result is
'Результаты синхронизации для каждого адресата
1 - ожидается синхронизация (стоит в очереди на отправку). Это значение по умолчанию.
2 - синхронизируется (информационный обмен еще не завершен)
3 - синхронизация завершилась успешно
4 - произошла ошибка';

comment on column out_sync_queue.entity_uid is
'уникальный идентификатор пересылаемой сущности (unique_id)';

comment on column out_sync_queue.entity_io_uid is
'уникальный идентификатор информационного объекта, в котором находится пересылаемая сущность.
Для записей справочников - это unique_id информационного объекта, к которому относится таблица, содержащая пересылаемую сущность
Для категорий - это unique_id категории (при пересылке особого смысла не имеет, но задается для унификации)
Для ИО - это unique_id информационного объекта (при пересылке особого смысла не имеет, но задается для унификации)';

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
'Пядь
Справочник законов распределения';

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
'Интегральный справочник о  персонале объекта, зарегистрированного в ИК ТСО объекта их картах доступа и планах доступа (на основе отчета о персонале)';

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
'Информация о персонале (ИК ТСО)';

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
'Справочник должностей';

comment on column "position".id is
'НЕ НАДО ЭТО ПОЛЕ ДЕЛАТЬ SERIAL! Оно наследуется от kks_roles. После создания данной таблицы отрабатывает скрипт, который задает ему значение по умолчанию в nextval(''kks_roles_id_seq'');';

comment on column "position".email_prefix is
'Данное поле используется для однозначной идентификации должностного лица внутри организации.
В частности это может быть полезно при обмене сообщениями и распоряжениями между организациями.
Пользователю во внешней организации достаточно будет лишь знать значение этого поля, соответствующее тому должностному лицу, которому он хочет отправить сообщение/распоряжение. Система гарантирует, что в этом случае оно доставится адресату';

comment on column "position".is_public is
'Является ли ДЛ публичным';

comment on column "position".is_archived is
'Флаг нахождения должности в архиве.
Нахождение в архиве фактически означает, что должность удалена';

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
'Тип должнгстного лица.
- ДЛ в системе DynamicDocs
- ДЛ во внешней сопрягаемой системе';

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
'Режимы, при которых должность существует';

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
'Сценарии обработки ИО
(пядь)';

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
'варианты обработки ИО в очередях';

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
'Название записи.';

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
'таблица с кватанциями о результатах обработки синхронизации';

comment on column queue_results.id_external_queue is
'идентификатор в справочнике исходящей очереди на объекте-отправителе
Если результат обработки является ответом на пинг (atPingResponse), 
то в этом поле содержится  (помноженный на -1) идентификатор организации-получателя пинга в БД отправителя пинга

Если в качестве значения содержится 0 - это означает, что происходит процедура первоначальной синхронизации.
Строка представляет собой ответ на пинг, но в БД-отправителе пинга еще нет данных о взаимодействующих организациях (они еще не пришли). В этом случае отправитель пинга в качестве идентификуатора idOrgTo поставил 0.
';

comment on column queue_results.sync_result is
'результат обработки сообщения (будет возвращаться в квитанции)
3 - успешно
4 - с ошибкой';

comment on column queue_results.address is
'адрес получателя квитанции';

comment on column queue_results.is_read is
'1 - не прочитанное, требующее отправки
2 - прочитанное и не требующее отправки';

comment on column queue_results.port is
'порт получателя квитанции';

comment on column queue_results.org_uid is
'email_prefix организации, которой надо отослать квитанцию. В это поле заносится соответствующая информация из входящего сообщения. Значение используется транспортом ТПС';

comment on column queue_results.use_gateway is
'флаг определяет необходимость отправки квитанции через шлюз (ТПС)
Т.е. подключен ли объект, которому надо отправить квитанцию, к шлюзу';

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
'Служебная таблица для хранения состояний датчика случайных чисел';

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
'Справочник воинских званий';

comment on column ranks.code is
'Используется для сопряжения с внешними системами, такими как 7т1';

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
'Значения составных атрибутов (показателей) для записей справочников.
Записи в данной таблице появляются автоматически при редактировании Значения атрибута записи справочника';

comment on column rec_attrs_attrs_values.value is
'Значение (строковое представление) описывающего показатель атрибута';

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
'Значения атрибутов (показателей) для записей справочников
Хранит сами значения атрибутов и их дополнительные характеристики.
Применительно к данной таблице поддерживается темпоральная модель.
Каждое значение атрибута имеет диапазон актуальности.

При изменении значения атрибута фактически происходит создание новой записи в этой таблице. При этом поле value получает новое актуальное значение. Поле start_time по умлчанию устанавливается в current_timestamp, поле stop_time - в NULL, поле meas_time - в current_timestamp, поле insert_time - в current_timestamp, is_actual - в true.
Указанные поля пользователь может задать самостоятельно, однако start_time все равно будет выставлено в current_timestamp, а поле stop_time - в NULL.
Старая (исходная) запись при этом остается. Однако ее поле is_actual устанавливается в false, stop_time - в current_timestamp.

При удалении строки из данной таблицы реального удаления не происходит. Исходная запись остается. Однако ее поле is_actual устанавливается в false, stop_time - в current_timestamp.

ВАЖНО!!!
В НАСТОЯЩЕЕ ВРЕМЯ POSTGRESQL  НЕ ПОДДЕРЖИВАЕТ ВНЕШНИЕ КЛЮЧИ НА ИЕРАРХИЮ НАСЛЕДОВАНИЯ ТАБЛИЦ. ПО ЭТОЙ ПРИЧИНЕ ДЕЛАТЬ ВНЕШНИЙ КЛЮЧ ИЗ REC_ATTRS_VALUES НА Q_BASE_TABLE НЕЛЬЗЯ. В ИНТЕРНЕТЕ СУЩЕСТВУЮТ РАЗЛИЧНЫЕ РЕШЕНИЯ ДАННОЙ ПРОБЛЕМЫ, ОДНАКО ИХ РАБОТОСПОСОБНОСТЬ, А ОСОБЕННО БЫСТРОДЕЙСТВИЕ ВЫЗЫВАЮТ СОМНЕНИЕ. ПО ЭТОЙ ПРИЧИНЕ РАССМАТРИВАЕМЫЙ ВНЕШНИЙ КЛЮЧ НЕ ГЕНЕРИРУЕТСЯ СКРИПТОМ СОЗДАНИЯ БД.
ДЛЯ ПОДДЕРЖАНИЯ ССЫЛОЧНОЙ ЦЕЛОСТНОСТИ НАПИСАН ТРИГГЕР, КОТОРЫЙ ПРОВЕРЯЕТ ФАКТ СУЩЕСТВОВАНИЯ ЗАПИСИ, НА КОТОРУЮ ССЫЛАЕТСЯ СОЗДАВАЕМАЯ (ИЗМЕНЯЕМАЯ) ЗАПИСЬ В REC_ATTRS_VALUES. 
';

comment on column rec_attrs_values.value is
'Значение атрибута в текстовом представлении';

comment on column rec_attrs_values.start_time is
'Момент начала актуальности значения атрибута';

comment on column rec_attrs_values.stop_time is
'Момент утери актуальности значения атрибута';

comment on column rec_attrs_values.insert_time is
'Дата и время доведения значения атрибута в систему';

comment on column rec_attrs_values.id_io_object_src is
'Идентификатор ИО, который явился источником информации о значении атрибута.';

comment on column rec_attrs_values.id_io_object_src1 is
'Идентификатор объекта, за кого передали информацию. Если источник передаёт информацию сам за себя, то значение в этом поле совпадает со значением в поле <Источник>';

comment on column rec_attrs_values.is_actual is
'Флаг, определяющий актуально ли данное значение атрибута, било оно архивное.
Данный флаг задается автоматически при создании и изменении атрибута, зависит от значения поля "момент утери актуальности". Если текущая дата-время попадает в диапазон (момент начала актуальности) - (момент утери актуальности), то флаг устанавливается в true, в противном случае он равен false.';

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
'Порядок оведения сигналов адресатам.
Сначала 1, если 1 нет, тогда пробуем 2-му, если и его нет - 3-му и т.д.
 адресат 1
 адресат 2
 адресат 3
 адресат 4';

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
'Адресаты для доведения сигналов и приказов БУ ';

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
'рубрикатор для записей справочников

ВАЖНО!!!
В НАСТОЯЩЕЕ ВРЕМЯ POSTGRESQL  НЕ ПОДДЕРЖИВАЕТ ВНЕШНИЕ КЛЮЧИ НА ИЕРАРХИЮ НАСЛЕДОВАНИЯ ТАБЛИЦ. ПО ЭТОЙ ПРИЧИНЕ ДЕЛАТЬ ВНЕШНИЙ КЛЮЧ ИЗ record_rubricator НА Q_BASE_TABLE НЕЛЬЗЯ. В ИНТЕРНЕТЕ СУЩЕСТВУЮТ РАЗЛИЧНЫЕ РЕШЕНИЯ ДАННОЙ ПРОБЛЕМЫ, ОДНАКО ИХ РАБОТОСПОСОБНОСТЬ, А ОСОБЕННО БЫСТРОДЕЙСТВИЕ ВЫЗЫВАЮТ СОМНЕНИЕ. ПО ЭТОЙ ПРИЧИНЕ РАССМАТРИВАЕМЫЙ ВНЕШНИЙ КЛЮЧ НЕ ГЕНЕРИРУЕТСЯ СКРИПТОМ СОЗДАНИЯ БД.
ДЛЯ ПОДДЕРЖАНИЯ ССЫЛОЧНОЙ ЦЕЛОСТНОСТИ НАПИСАН ТРИГГЕР, КОТОРЫЙ ПРОВЕРЯЕТ ФАКТ СУЩЕСТВОВАНИЯ ЗАПИСИ, НА КОТОРУЮ ССЫЛАЕТСЯ СОЗДАВАЕМАЯ (ИЗМЕНЯЕМАЯ) ЗАПИСЬ В record_rubricator. 
';

comment on column record_rubricator.id_parent is
'Родительская рубрика';

comment on column record_rubricator.id_record is
'Ссылка за запись, к которой относится рубркатор';

comment on column record_rubricator.id_io_object is
'Ссылка на справочник. Если указан, то в рубрику могут быть помещены записи только из данного справочника';

comment on column record_rubricator.id_io_category is
'Ссылка на категорию. Если указана, то  в рубрику могут быть помещены записи только из справочников заданной категории';

comment on column record_rubricator.name is
'Название рубрики';

comment on column record_rubricator.description is
'Описание рубрики';

comment on column record_rubricator.r_icon is
'Иконка, используемая при отображении элемента в дереве рубрик. Если не задана, то используется значение по умолчанию (определяется клиентским приложением)';

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
'системный справочник "Справочник шаблонов отчетов".
Используется генератором отчетов openRPT.

генератор отчетов OpenRPT, который работает с данной таблицей, требует, чтобы в данном справочнике было поле report_id и считает его первичным ключем. Поэтому кроме всего прочего на данную таблицу назначен триггер который дублирует значение поля id в это поле.

данный справочник является глобальным объектового ведения, интегрируемым- т.е. на каждом объекте ведется своя таблица, но она может дополняться данными аналогичной таблицы другого объекта.';

comment on column report.report_id is
'генератор отчетов OpenRPT, который работает с данной таблицей, требует, чтобы это поле было и считает его первичным ключем. Поэтому кроме всего прочего на данную таблицу назначен триггер который дублирует значение поля id в это поле';

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

--необходимо, чтобы данный триггер отрабатывал раньше, чем триггер trgsetuid, 
--который задает значение колонке rr_name, используя значение колонки name
--поэтому называться он должен так, чтобы по алфавиту раньше шел.
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
'Перечень организаций, с которыми осуществляется синхноризация справочника шаблонов отчетов';

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
'Записи справочников, которые находятся в соответствующих рубриках

ВАЖНО!!!
В НАСТОЯЩЕЕ ВРЕМЯ POSTGRESQL  НЕ ПОДДЕРЖИВАЕТ ВНЕШНИЕ КЛЮЧИ НА ИЕРАРХИЮ НАСЛЕДОВАНИЯ ТАБЛИЦ. ПО ЭТОЙ ПРИЧИНЕ ДЕЛАТЬ ВНЕШНИЙ КЛЮЧ ИЗ rubric_records НА Q_BASE_TABLE НЕЛЬЗЯ. В ИНТЕРНЕТЕ СУЩЕСТВУЮТ РАЗЛИЧНЫЕ РЕШЕНИЯ ДАННОЙ ПРОБЛЕМЫ, ОДНАКО ИХ РАБОТОСПОСОБНОСТЬ, А ОСОБЕННО БЫСТРОДЕЙСТВИЕ ВЫЗЫВАЮТ СОМНЕНИЕ. ПО ЭТОЙ ПРИЧИНЕ РАССМАТРИВАЕМЫЙ ВНЕШНИЙ КЛЮЧ НЕ ГЕНЕРИРУЕТСЯ СКРИПТОМ СОЗДАНИЯ БД.
ДЛЯ ПОДДЕРЖАНИЯ ССЫЛОЧНОЙ ЦЕЛОСТНОСТИ НАПИСАН ТРИГГЕР, КОТОРЫЙ ПРОВЕРЯЕТ ФАКТ СУЩЕСТВОВАНИЯ ЗАПИСИ, НА КОТОРУЮ ССЫЛАЕТСЯ СОЗДАВАЕМАЯ (ИЗМЕНЯЕМАЯ) ЗАПИСЬ В rubric_records. ';

comment on column rubric_records.r_icon is
'Иконка, используемая при отображении элемента в дереве рубрик. Если не задана, то используется значение по умолчанию (определяется клиентским приложением)';

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
'Единый рубрикатор информационных объектов
Содержит набор рубрик, которые могут иметь родителя-рубрику или родителя-информационный объект.
Одновременное задание родителя-рубрики и родителя-ИО не допускается. 
Не допускается также и создание двух рубрик с одним объектом-родителем. Это связано с тем, что такая рубрика является для объекта корневой. И сигнализирует лишь о том, что у объекта имеются вложения

Таким образом в одной таблице хранится общий рубрикатор для нормативно-справочных документов, а также частные рубрикаторы для формуляров, которые описывают вложения формуляров, а также пользовательские рубрикаторы';

comment on column rubricator.id_search_template is
'поисковый запрос (фильтр) рубрики)';

comment on column rubricator.id_io_category is
'Допустимая категория вкладываемых в рубрику информационных объектов
Если задана, то это означает, что только ИО заданной категории могут включаться в данную рубрику.';

comment on column rubricator.r_icon is
'Иконка при отображении элемента в дереве рубрик. Если не задана, то используется иконка по умолчанию (определяется клиентским приложением)';

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
'Типы поисковых запросов
Справочник введен для возможности типизации и рубрикации списка поисковых запросов с целью удобства их отображения для пользователя';

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
'Шаблоны поиска, применяемые при осуществлении поиска информационных объектов и в справочниках. ';

comment on column search_templates.id_io_category is
'Определяет категорию, к экземплярам которой применим данный поисковый запрос.
Большинство поисковых запросов создается для информационных объектов, т.е. для экземпляров категории "справочник информационных объектов". Тем не менее будут поисковые запросы, относящиеся к отдельным справочникам. 

В дальнейшем необходимо учесть ситуацию, что поисковый запрос может быть применим для нескольких категорий (если они содержат среди атрибутов те, которые указаны в поисковом запросе)';

comment on column search_templates.creation_datetime is
'Дата и время создания поискового запроса';

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
'Типы отрезков для сегментов (ИК ТСО)';

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
'параметры отдельного отрезка графического элемента (ИК ТСО)';

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
'Типы фигур графических элементов (ИК ТСО):
- прямоугольник;
- эллипс;
- набор сегментов.';

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
'Перечень сервисов, запускаемых периодически';

comment on column sheduled_handlers.name is
'Название запускаемого сервиса (если не задано - при создании по умолчанию возьмется из справочника сервисов)';

comment on column sheduled_handlers.exec_period is
'периодичность выполнения данного сервиса
';

comment on column sheduled_handlers.last_exec is
'Дата и время последнего вызова';

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
'таблица содержит возможные переходы состояний ИО в данном жизненном цикле
Состояния должны входить в жизненный цикл категории! Это отслеживает триггер.
ИО могут переходить из любого состояния в любое. Наличие соответствующей записи в данной таблице означает лишь то, что после обработки в исходном состоянии ИО должен перейти в заданное состояние (и соответственно поставлен в очередь обработки, если таковая назначена для целевого состояния)';

comment on column state_crosses.id_life_cycle is
'в каком жизненном цикле переход допустим';

comment on column state_crosses.id_state_src is
'Исходное состояние';

comment on column state_crosses.id_state_dest is
'Состояние, в которое можно перейти из исходного';

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
'Справочник асинхронных квитанций, генерируемых сервером при создании, узменении и удалении тех или инх записей в справочниках (notify).
Название асинхронной квитанции должно быть уникальным, при этом допустимо генерировать квитанцию с одним именем для разных справочников.
Перечень справочников, для которых генерируется квитанция, задается в таблице table_notifies_io_objects (многие-ко-многим).
Асинхронная квитанция генерируется следующим образом
pg_notify(notify_name, payload)
payload формируется следующим образом:
tableName + ''~_~_~'' + idRecord + ''~_~_~'' + recordUniqueId + ''~_~_~'' +whatHappens
whatHappens формируется следующим образом:
1 - выполнена операция создания новой записи
2 - выполнена операция изменения записи
3 - выполнена операция удаления записи';

comment on column table_notifies.name is
'Название квитанции. Должно быть уникальным';

comment on column table_notifies.notify_where is
'Когда отправляется квитанция (старые чекбоксы)';

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
'Единицы измерения времени';

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
'Транспорты межобъектового обмена';

comment on column transport.local_address is
'Локальный адрес данного транспорта';

comment on column transport.local_port is
'Порт транспортной задачи, которая обрабатывает БД локальной  организации.
Порт используется только в IP-сетях, если используется транспорт http_connector';

comment on column transport.use_gateway is
'Должен ли транспорт быть подключен к шлюзу (ТПС) для взаимодействия с доугими объектами ';

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
'табель срочных донесений
При создании новой записи в табеле срочных донесений должна автоматически создаваться запись в журнале контроля исполнений';

comment on column tsd.id is
'порядковый номер записи табеля срочных донесений';

comment on column tsd.id_io_category is
'ИКФС идентификационный код формы сообщения (код шаблона сообщения) в котором необходимо представлять донесения по данному пункту ( заполняется из словаря имеющихся шаблонов);';

comment on column tsd.insert_time is
'дата и время внесения записи в табель срочных донесений';

comment on column tsd.exec_datetime is
'дата к которой необходимо исполнить  пункт табеля (первая дата исполнения пункта табеля срочных донесений-(дата в формате:дд.мм.гг)
время к которому необходимо представить донесение об исполнении пункта табеля срочных донесений (время в формате-чч.мм);';

comment on column tsd.exec_period is
'период исполнения данного пункта начиная с указанной даты и времени (может указываться в часах, сутках, месяцах, кварталах, годах (чч.ссс.мм.к.г; например-00.001.00.0.0-представляется ежесуточно);';

comment on column tsd.osnovanie is
'основание для внесения записи- входящий номер из журнала входящих распоряжений или входящий номер документа при его неавтоматизированном получении';

comment on column tsd.is_archived is
'Флаг, определяющий нахождение записи табеля в архиве

Перевод в архивное состояние означает, что записи в состоянии архив не участвуют в визуализации записей журнала и формировании записей журнала контроля исполнения входящих и исходящих распоряжений и табеля срочных донесений ККС СИТО

Перевод в активное состояние означает, что записи в состоянии активное участвуют в визуализации записей журнала и формировании записей журнала контроля исполнения входящих и исходящих распоряжений и табеля срочных донесений ККС СИТО';

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
'журнал учета и контроля исполнения  исходящих, входящих  распоряжений и табеля срочных донесений';

comment on column tsd_control_journal.id is
'порядковый номер записи журнала ';

comment on column tsd_control_journal.id_journal is
'пункт журнала учета распоряжений, если учитывается запись журнала учета распоряжений';

comment on column tsd_control_journal.id_io_object is
'ссылка на отчетный документ';

comment on column tsd_control_journal.insert_time is
'дата создания записи - дата внесения записи в журнал (дата в формате: дд.мм.гг);';

comment on column tsd_control_journal.planned_exec is
'планируемые дата и время исполнения - дата и время исполнения, указанные в контролируемом документе;';

comment on column tsd_control_journal.real_exec is
'реальные дата и время исполнения - реальные дата и  время исполнения;';

comment on column tsd_control_journal.is_archived is
'Флаг, определяющий нахождение записей журнала контроля в архиве';

comment on column tsd_control_journal.is_outed is
'Если данное поле задано как TRUE, то документ, созданный как ответ на распоряжение, отправлен адресату.
Для локальных распоряжений данное поле всегда выставляется как TRUE';

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
'Настройки системы ТСО';

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
'Справочник должностей (ИК ТСО)';

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
'Справочник отделов (ИК ТСО)';

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
'Справочник подразделений';

comment on column units.id is
'НЕ НАДО ЭТО ПОЛЕ ДЕЛАТЬ SERIAL! Оно наследуется от kks_roles. После создания данной таблицы отрабатывает скрипт, который задает ему значение по умолчанию в nextval(''kks_roles_id_seq'');';

comment on column units.id_organization is
'Организация, к которой относится данное подразделение.
Данное поле не является обязательным, поскольку в системе существуют системные пользователи (должностные дица) такие как admin, jupiter, которые не принадлежат ни одной организации';

comment on column units.id_curr_mode is
'Текущий режим подразделения, наследуется от вышестоящего подразделения (при условии его наличия, в противном случае - от организации, в которую подразделение входит)';

comment on column units.tree_symbol is
'значок подразделения в дереве ОШС (XPM)';

comment on column units.time_elapsed is
'Временной период, на который создается ФГ, если данное подразделение является функциональной группой';

comment on column units.is_archived is
'Флаг нахождения подразделения в архиве.
Нахождение в архиве фактически означает, что подразделение удалено.
Если подразделение находится в архиве, то все должности его также находятся в архиве и создавать новые в нем нельзя';

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
'Режимы, при которых подразделение существует';

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
'уровни срочности сообщений (распоряжений)';

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
'Системный справочник известных расширений прикрепляемых файлов';

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
'Взаимосвязи записей справочников с прикрепленными к ним файлами.
Сущемствует возможность прикреплять к записям справочников DynamicDocs различные файлы';

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
'Пользовательский шаблон дискреционных прав доступа пользователей (должностных лиц) к информационным объектам';

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
'системный справочник пользовательских очередей
данная таблица будет глобальная объектового ведения, интегрируемая т.е. на каждом объекте ведется своя таблица, но она может дополняться данными аналогичной таблицы другого объекта
';

comment on column user_chains.id_search_template is
'ссылка на поисковый запрос, ассоциированный с пользовательской очередью';

comment on column user_chains.id_io_category is
'Категория, на основе которой созданы справочники, подлежащие обработке пользовательским обработчиком.
Указывается дочерняя категория';

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
'Перечень организаций, с которыми осуществляется синхноризация справочника пользовательских очередей';

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
'Системная таблица, в которую помещается информация об измененных, добавленных, удаленных записях справочников, которые обрабатываются пользовательскими обработчиками';

comment on column user_handlers_data.rec_type is
'тип события
1 - запись была создана
2 - запись была изменена';

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
'Пользовательские рубрикаторы
Т.е. группа "мои документы" для каждого пользователя.
Пользователь может иметь только одну группу "мои документы". А она уже в свою очередь содержит дерево рубрик и подрубрик';

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
'Состояние пользователя (отпуск, командировка и т.п.)';

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
'Таблица содержит информацию о привязке шаблона подчиненной категории информационного объекта к пользователю.
Фактически речь идет о настройке отображения подчиненной таблицы, а также атрибутов (систеиных в том числе) информационного объекта

ответственность за соответствие шаблона соответствующим категориям несет система (написан триггер). ';

comment on column user_templates.type is
'Тип применения.
0 - шаблон применяется к подчиненной таблице
1 - шаблон применяется к пользовательским атрибутам';

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
'Таблица (справочник) пользователей, зарегистрированных в системе';

comment on column users.id is
'НЕ НАДО ЭТО ПОЛЕ ДЕЛАТЬ SERIAL! Оно наследуется от kks_roles. После создания данной таблицы отрабатывает скрипт, который задает ему значение по умолчанию в nextval(''kks_roles_id_seq'');';

comment on column users.lastname is
'фамилия';

comment on column users.firstname is
'Имя';

comment on column users.sirname is
'Отчество';

comment on column users.fio is
'Вычисляемое поле. Для удобства содержит фамилию и ИО
Иванов И.И.
Вычисляется из lastname, firstname, sirname';

comment on column users.insert_time is
'Время создания записи';

comment on column users.acc_right_num is
'Номер допуска';

comment on column users.acc_right_date is
'Дата выдачи допуска';

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
'Режим функционирования организации (подразделения)';

comment on column work_mode.symbol is
'Условный знак (в формате XPM), которым отображается текущий режим функционирования';

comment on column work_mode.audio_file is
'аудиофайл оповещения о вводе данного режима функционирования';

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
'тип режима функционирования организации (учебный или боевой).
считается, что любой режим функционирования можно отнести к одному из двух (в текущей версии) типов:
- учебному 
- боевому';

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
'Справочник XML-тэгов в XML-описании ряда специальных ЭИО
(для системы ИК ТСО)';

comment on column xml_paths.data_type is
'Тип данных';

comment on column xml_paths.xml_level is
'уровень вложенности тэга в xml-формате';

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
'Все возможные запросы к ИК ТСО';

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
'Вид XML-запроса к ИК ТСО: 
- регламентированный;
- нерегламентированный.';

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
'Справочник ответов на запросы к ИК ТСО';

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
'Типы запросов на XML (перечень регламентированных запросов в ИК ТСО)';

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

