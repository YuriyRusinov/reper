\encoding utf8

begin;

--insert into io_categories (id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values (10, NULL, false, 'Типы кораблей'
create or replace function a_type_ships_cat () returns int4 as
$BODY$
declare
    idCategory int4;
    unique_cat_id varchar;
    query varchar;
    i int4;
    idChild int4;
    idCatTypes int4[];
    catNames varchar[];
    idTableCategory int4;
begin
    --perform createTempTables();
    --perform setCurrentDl(4);

    idCategory := NULL::int4;
    idTableCategory := NULL::int4;
    idCatTypes := ARRAY [10, 1];
    catNames := ARRAY ['Типы кораблей', 'Справочник типов кораблей'];

    for i in 1..2
    loop
        if (idCategory is not null) then
            idTableCategory := idCategory;
        end if;
        select getNextSeq ('io_categories', 'id') into idCategory;
        unique_cat_id := 'localorg-categories-' || idCategory;

        if (idTableCategory is null) then
            query := E'insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values (''' || unique_cat_id || ''',' || idCategory || ', ' || idCatTypes[i] || ', NULL, false, ''' || catNames[i] || ''', ''SYSCATEGORY_' || idCategory || ''', NULL::varchar, false, false, 1)';
        else
            query := E'insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values (''' || unique_cat_id || ''',' || idCategory || ', ' || idCatTypes[i] || ', '|| idTableCategory || ' , false, ''' || catNames[i] || ''', ''SYSCATEGORY_' || idCategory || ''', NULL::varchar, false, false, 1)';
        end if;
        raise notice '%', query;
        execute query;
        perform cSetCompleted (idCategory, 1);
        perform cSetGlobal (idCategory);
    end loop;
/*
    select getNextSeq ('io_categories', 'id') into idCategory;
    unique_cat_id := 'localorg-categories-' || idCategory;
    query := E'insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values (''' || unique_cat_id || ''',' || idCategory || ', 1, ' || idCategory-1 || ', true, ''Справочник типов кораблей'', ''SYSCATEGORY_' || idCategory || ''', NULL::varchar, false, true, 1)';
    raise notice '%', query;
    perform query;
    perform cSetCompleted (idCategory, 1);
    perform cSetGlobal (idCategory);
*/
    return 1;
end
$BODY$
language 'plpgsql';

select a_type_ships_cat ();
drop function a_type_ships_cat ();

commit;

begin;
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-1', 1, 'Авианосец', 'CV');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-2', 2, 'Крейсер УРО', 'CG');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-3', 3, 'Эскадренный миноносец УРО', 'DDG');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-4', 4, 'Эскадренный миноносец', 'DD');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-5', 5, 'Фрегат УРО', 'FFG');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-6', 6, 'Фрегат', 'FFL');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-7', 7, 'Корвет', 'FS');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-8', 8, 'Патрульный корабль', 'PF');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-9', 9, 'Ракетный катер', 'PHM');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-10', 10, 'Торпедный катер', 'PT');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-11', 11, 'Патрульный катер', 'PTF');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-12', 12, 'Десантный корабль штабной', 'LCC');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-13', 13, 'Десантный корабль универсальный', 'LPA');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-14', 14, 'Десантный корабль вертолетоносец', 'LPH');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-15', 15, 'Десантный корабль танкодесантный', 'LST');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-16', 16, 'Десантный катер', 'LCA');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-17', 17, 'Транспортное судно универсальное', 'AKS');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-18', 18, 'Транспортное судно снабжения', 'AFS');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-19', 19, 'Транспортное судно боеприпасы', 'AE');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-20', 20, 'Танкер-заправщик', 'AO');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-21', 21, 'Минно-тральный корабль', 'AM');
select setval('type_ship_id_seq', 22, false);

commit;
