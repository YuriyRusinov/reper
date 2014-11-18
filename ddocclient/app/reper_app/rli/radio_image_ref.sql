\encoding utf8

begin;

create or replace function rli_image_io () returns int4 as
$BODY$
declare
    idCategory int4;
    idTableCategory int4;
    query varchar;
    catNames varchar[];
    idCatTypes int4[];
    id_attr_image_width int4;
    id_attr_image_height int4;
    id_attr_type_ship int4;
    id_attr_desc int4;
    image_attr_names varchar[];
    id_attr_types int4[];
    attr_codes varchar[];
    attr_names varchar[];
    attr_titles varchar[];
    attr_table varchar;
    attr_column varchar;
    attr_defwidth int4[];
    attr_defVals varchar[];
    attr_mandatory boolean[];
    tableuid varchar;
    i int4;
    idObject int4;
begin
    idCategory := NULL::int4;
    idTableCategory := NULL::int4;
    idCatTypes := ARRAY [10, 1];
    catNames := ARRAY ['Радиолокационные изображения', 'Справочник радиолокационных изображений'];
    for i in 1..2
    loop
        if (idTableCategory is null) then
            query := E' select cInsert (' || quote_literal (catNames[i]) || ', NULL, NULL, '|| idCatTypes[i] || ' , NULL, false, NULL, false, 1);';
            raise notice '%', query;
            execute query into idTableCategory;
            perform cSetCompleted (idTableCategory, 1);
        else
            query := E'select cInsert (' || quote_literal (catNames[i]) || ', NULL, NULL, '|| idCatTypes[i] || ', ' || idTableCategory || ', true, NULL, false, 1);';
            raise notice '%', query;
            execute query into idCategory;
            perform cSetCompleted (idCategory, 1);
            perform cSetGlobal (idCategory);
        end if;
    end loop;
    id_attr_types := ARRAY[30, 30, 30, 2, 9];
    attr_codes := ARRAY ['id', 'image_width', 'image_height', 'type_ship', 'description'];
    attr_names := ARRAY ['Идентификатор', 'Ширина изображения', 'Высота изображения', 'Тип корабля', 'Описание'];
    attr_titles := ARRAY ['ИД', 'Ширина', 'Высота', 'Тип корабля', 'Описание'];
    attr_table := 'type_ship';
    attr_column := 'name';
    attr_defwidth := ARRAY [50, 50, 50, 50, 100];
    attr_defVals := ARRAY ['NULL', '0', '0', 'NULL', 'NULL'];
    attr_mandatory := ARRAY [false, true, true, true, false];
    select into tableuid unique_id from io_objects io where io.table_name = 'type_ship';
    raise warning '%', tableuid;
    for i in 1..5
    loop
        if (i <> 4) then
            query := E'select acInsert (' || idTableCategory || ', ' || id_attr_types[i] || ', ' || quote_literal (attr_codes[i]) ||'::varchar, ' || quote_literal (attr_names[i]) || '::varchar, ' || quote_literal (attr_titles[i]) || '::varchar, NULL::varchar, NULL::varchar,' || attr_defwidth[i] || ', ' || attr_defVals[i] || '::varchar, ' || attr_mandatory[i] || ', false, NULL::varchar);';
        else
            query := E'select acInsert (' || idTableCategory || ', ' || id_attr_types[i] || ', ' || quote_literal (attr_codes[i]) ||'::varchar, ' || quote_literal (attr_names[i]) || '::varchar, ' || quote_literal (attr_titles[i]) || '::varchar, '|| quote_literal (tableuid) || '::varchar, '|| quote_literal (attr_column) || '::varchar,' || attr_defwidth[i] || ', ' || attr_defVals[i] || '::varchar, ' || attr_mandatory[i] || ', false, NULL::varchar);';
        end if;
        raise warning '%', query;
        execute query;
    end loop;

    return 1;
end
$BODY$
language 'plpgsql';

select rli_image_io();
commit;

