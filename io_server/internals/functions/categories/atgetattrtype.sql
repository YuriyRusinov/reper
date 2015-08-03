select f_safe_drop_type('h_get_attr_type');
create type h_get_attr_type as(id int4,
                               name varchar,
                               code varchar);

create or replace function atGetAttrType(int4) returns setof h_get_attr_type as
$BODY$
declare
    idAttrType alias for $1;

    r h_get_attr_type%rowtype;
begin

    for r in 
        select 
            att.id, 
            att.name, 
            att.code 
        from 
            a_types att
        where 
            att.id = idAttrType
        loop
            return next r;
        end loop;  

    return;

end
$BODY$
language 'plpgsql';

/*
В каждой таблице колонки имеют уникальное название (код атрибута)
поэтому пара таблица-код атрибута однозначно идентифицируют атрибут
(не путать с парой характеристик код атрибута-название ссылочной таблицы в справочнике атрибутов!!!!)
*/
create or replace function atGetAttrType(varchar, varchar) returns setof h_get_attr_type as
$BODY$
declare
    attrCode alias for $1;
    tableName alias for $2;

    r h_get_attr_type%rowtype;
begin

    for r in 
        select 
            att.id, 
            att.name, 
            att.code 
        from 
            a_types att,
            attributes a,
            io_objects io,
            attrs_categories ac,
            io_categories c
        where 
            att.id = a.id_a_type
            and a.code = attrCode
            and io.table_name = tableName
            and io.id_io_category = c.id
            and c.id_child = ac.id_io_category
            and ac.id_io_attribute = a.id
            
        loop
            return next r;
        end loop;  

    return;

end
$BODY$
language 'plpgsql';

