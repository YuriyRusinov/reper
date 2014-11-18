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

        if (idTableCategory is null) then
            query := E' select cInsert (' || quote_literal (catNames[i]) || ', NULL, NULL, '|| idCatTypes[i] || ' , NULL, false, NULL, false, 1);';
            raise notice '%', query;
            execute query into idTableCategory;
            perform cSetCompleted (idTableCategory, 1);
/*            --;E'insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values (''' || unique_cat_id || ''',' || idCategory || ', ' || idCatTypes[i] || ', NULL, false, ''' || catNames[i] || ''', ''SYSCATEGORY_' || idCategory || ''', NULL::varchar, false, false, 1)';*/
        else
            query := E'select cInsert (' || quote_literal (catNames[i]) || ', NULL, NULL, '|| idCatTypes[i] || ', ' || idTableCategory || ', true, NULL, false, 1);';
            raise notice '%', query;
            execute query into idCategory;
            perform cSetCompleted (idCategory, 1);
            perform cSetGlobal (idCategory);
/*            query := E'insert into io_categories (unique_id, id, id_io_category_type, id_child, is_main, name, code, description, is_system, is_global, id_io_state) values (''' || unique_cat_id || ''',' || idCategory || ', ' || idCatTypes[i] || ', '|| idTableCategory || ' , true, ''' || catNames[i] || ''', ''SYSCATEGORY_' || idCategory || ''', NULL::varchar, false, false, 1)';*/
        end if;
    end loop;

    return idCategory;
end
$BODY$
language 'plpgsql';

create or replace function attr_type_ships_cat () returns int4 as
$BODY$
declare
    idCategory int4;
    idTableCategory int4;
    unique_attr_cid varchar;
    idCatAttr int4;
    idAttrs int4[];
    i int4;
    num int4;
    a_name varchar;
    mAttrs boolean[];
    id_attr_type int4;
    idObject int4;
    oldTable varchar;
    dropquery varchar;
begin
    select a_type_ships_cat () into idCategory;
    select into idTableCategory id_child from io_categories where id=idCategory;

--    select getNextSeq ('attrs_categories', 'id') into idCatAttr;
--    unique_attr_cid := 'localorg-attrs_categories-' || idCatAttr;
    idAttrs := ARRAY [1,2,3,6];
    mAttrs := ARRAY [false,true,false,true];
    num := array_upper (idAttrs, 1);
    for i in 1..num
    loop
        select a.id_a_type into id_attr_type from attributes a where a.id=idAttrs[i];
        select into a_name name from attributes where id=idAttrs[i];
        select cAddAttr (idTableCategory, idAttrs[i], NULL, mAttrs[i], false) into idCatAttr;
        raise warning '%', idCatAttr;
        --acInsert (idTableCategory, id_attr_type, a_name, a_code, a_title
/*        select getNextSeq ('attrs_categories', 'id') into idCatAttr;
        unique_attr_cid := 'localorg-attrs_categories-' || idCatAttr;
*/
/*        insert into attrs_categories (unique_id, id, id_io_category, id_io_attribute, name, is_mandatory, is_read_only) values (unique_attr_cid, idCatAttr, idTableCategory, idAttrs[i], a_name, mAttrs[i], false);
*/
    end loop;
    select ioInsert('Qualifier for table type_ship',
                    idCategory,
                    getCurrentUser(), --author
                    1, --id_state
                    NULL, --'type_ship', --table_name (will autocreated new table!) 
                    NULL, --desc
                    NULL, --ioInfo
                    getCurrentMaclabelId(), --id_maclabel
                    NULL, --unique_id
                    1, --id_sync_type (does not sync)
                    getLocalOrgId(), --localorg
                    false, --is_global,
                    NULL::int4, --id_search_template
                    NULL::varchar, --ref_table 
                    NULL::int8, --fill_color
                    NULL::int8, --test_color
                    NULL::int4 --id_type
                   ) into idObject;
    select o.table_name into oldTable from tbl_io_objects o where o.id=idObject;
    update tbl_io_objects set table_name='type_ship' where id=idObject;
    --raise warning '%', oldTable;
    dropquery := E'drop table ' || oldTable;
    perform dropquery;
    return idObject;
end
$BODY$
language 'plpgsql';

select createtemptables();
select setcurrentdl (4);
select attr_type_ships_cat ();
--drop function attr_type_ships_cat ();
--drop function a_type_ships_cat ();

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
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-17', 17, 'Транспортное судно универсальное',  'AKS');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-18', 18, 'Транспортное судно снабжения', 'AFS');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-19', 19, 'Транспортное судно боеприпасы', 'AE');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-20', 20, 'Танкер-заправщик', 'AO');
insert into type_ship (unique_id, id, name, code) values ('localorg_type_ship-21', 21, 'Минно-тральный корабль', 'AM');
select setval('type_ship_id_seq', 22, false);

commit;
