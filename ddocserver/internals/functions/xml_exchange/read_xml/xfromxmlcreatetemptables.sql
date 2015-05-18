create or replace function xFromXMLCreateTempTables() returns int4 as
$BODY$
begin
    create temp table x_parsed_categories
    (
        id               serial not null,
        unique_id        varchar not null,
        real_id          int4 not null,
        name             varchar not null,
        description      varchar,
        id_child         int4,
        id_child2        int4,
        c_type           int4,
        b_exist bool --true if category already exist in database
    );

    create temp table x_parsed_attributes
    (
        id               serial not null,
        cat_real_id      int4 not null,
        real_id          int4 not null,
        unique_id        varchar not null,
        tag_name         varchar not null,
        code             varchar not null,
        title            varchar not null,
        name             varchar not null,
        table_name       varchar,
        column_name      varchar,
        ref_column_name  varchar,
        mandatory        bool not null,
        read_only        bool not null,
        def_width        int4,
        def_value        varchar,
        id_attr_type     int4 not null,
        b_exist          bool --true if attribute already exist in database
    );

    create temp table x_parsed_inf_resources
    (
        id                      serial not null,
        uuid_t                  uuid not null,
        unique_id               varchar not null,
        real_id                 int8 not null,
        id_category             int4,
        id_table_category       int4,
        id_indicators_category  int4,
        name                    varchar not null,
        description             varchar,
        table_name              varchar, --название таблицы БД, в которой будет находиться запись
        ir_type                 int4, -- 1 - document, 2 - qualifier, 3 - record (multiRecord отсутствует, т.к. она все равно парсится как несколько отдельных записей)
        b_exist                 bool not null --true if inf_ressource already exist in database
    );

    create temp table x_parsed_attr_values
    (
        id               serial not null,
        id_inf_resource  int8 not null,
        tag_name         varchar not null,
        the_value        varchar,
        a_type           int4 --1 - обычный, 2 - ссылка, 3 - мультиссылка. Для удобства запомним тип атрибута. Важно для ссылок и мультиссылок
    );

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xFromXMLDropTempTables() returns int4 as
$BODY$
begin

    drop table if exists x_parsed_attributes;
    drop table if exists x_parsed_categories;
    drop table if exists x_parsed_inf_resources;
    drop table if exists x_parsed_attr_values;
    
    return 1;
end
$BODY$
language 'plpgsql';

