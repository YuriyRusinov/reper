/*

begin;

select * from ioupdateattr_new(740,1071,'1~~~''1''',NULL,NULL,NULL,NULL,NULL) as result


select * from io_objects where id = 391
select * from io_categories where id = 367
select * from attributes where id in (select id_io_attribute from attrs_categories where id_io_category = 366)
select dropAttributeFromExistingQualifier(id, 391, false) from attributes where id in (1151, 1152, 1155, 1157)
select addAttributeToExistingQualifier(id, 391, false, false, NULL, false) from attributes where id in (1360, 1093, 1092, 1402);


select * from attributes where name in ('Адрес целиком', 'Номер дома', 'Корпус', 'Строение');
select * from io_objects where id_io_category = 367
select * from attributes where id = 1148

select cUpdateAttr(366, 1148, NULL, true, false)
alter table tbl_eio_table_391 add column item_is_leaf int4

select * from attrs_categories
select * from createTempTables();
select setCurrentDl(4);
select * from ioaddtorubric(613,46,'NULL');
select f_enable_triggers('attributes', true, NULL);
select * from ioinsert('Новый документ',305,1,1,NULL,NULL,'Администратор системы',1,NULL,1,1,'1',NULL,NULL,NULL,NULL,1);
rollback;

update attributes set code = 'm_has' where id = 1018
select * from attrs_categories where id_io_category = 304
select * from attributes where id in (select id_io_attribute from attrs_categories where id_io_category = 304)

select * from eio_table_391
update eio_table_391 set item_is_leaf = 1
select * from io_objects where id = 438

--select * from io_categories where id = 427
select * from attributes where code = 'dt'
--1195

insert into attrs_categories (id_io_category, id_io_attribute, name, is_mandatory, is_read_only, def_value) values (426, 1195, 'Дата и время расчета', true, false, 'current_timestamp');
alter table tbl_eio_table_438 add column dt timestamp without time zone;
update tbl_eio_table_438 set dt = current_timestamp;
alter table tbl_eio_table_438 alter column dt set not null;
alter table tbl_eio_table_438 alter column dt set default current_timestamp;


drop view eio_table_391 cascade;
alter table tbl_eio_table_391 rename to eio_table_391 ;
select acl_secureTable('eio_table_391');

select * from eio_table_438

select getRecordTableByUuid( system_type ) from getIncidentMessages(1971)

select * from fiks_incidents


alter table tbl_fiks_incidents_1 add column fias_uuid varchar;
alter table tbl_fiks_incidents_1 add CONSTRAINT "FK_fiks_incidents_1_REF_type_chs3" FOREIGN KEY (type_chs3)
      REFERENCES tbl_reestr_chs (id) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT;

alter table tbl_fiks_incidents_1 alter column incident set default 2829
alter table tbl_fiks_incidents_1 alter column "position" set not null;
alter  table tbl_fiks_incidents_1 alter column "position" set default 75;
--update tbl_fiks_incidents_1  set "position" = 75
select * from attributes where code = 'type_chs3'

drop view fiks_incidents_1 cascade;
alter table tbl_fiks_incidents_1 rename to fiks_incidents_1;
select acl_secureTable('fiks_incidents_1');

select * from tbl_fiks_incidents_1;
select * from io_objects where id = 524
select * from io_categories where id = 525
select * from attrs_categories where id_io_category = 524
update attrs_categories set is_mandatory = true, def_value = '2829' where id = 3575
select * from "position"
insert into attrs_categories (id_io_category, id_io_attribute, name, is_mandatory, is_read_only, def_value) values (524, 1331, 'Уникальный идентификатор ФИАС', false, false, NULL);


select * from attributes order by id desc
--select * from attrs_categories where id_io_attribute = 1342
select * from attributes where code = 'fias_uuid'

select addAttributeToExistingQualifier(1362, 520, false, false, NULL, false);
select * from reestr_alarm

select dropAttributeFromExistingQualifier(id, 520) from attributes where id in (1188, 1189, 1320, 1244, 1246, 1247, 1317)
select dropAttributeFromExistingQualifier(1321, 520)

select * from devize_ecor
select * from eio_table_438



alter table tbl_eio_table_346 drop column region;
alter table tbl_eio_table_346 drop column city;
alter table tbl_eio_table_346 drop column street;
alter table tbl_eio_table_346 drop column corps;
alter table tbl_eio_table_346 drop column house_number;
alter table tbl_eio_table_346 drop column floor;
alter table tbl_eio_table_346 drop column apartment;
select f_enable_triggers('attrs_categories', false, NULL::varchar[]);
update attrs_categories set name = 'Ссылка на адрес в системе ФИАС' where id_io_attribute = 1170
delete from attrs_categories where id_io_category = 342
select f_enable_triggers('attrs_categories', true, NULL::varchar[]);
select * from attributes a where a.id_attr_group = 21 
update attributes set code = code || '_str' where id_attr_group = 21 

select 
    addAttributeToExistingQualifier(id, 346, false, false, NULL)
    --dropAttributeFromExistingQualifier(id, 346)
from 
    attributes 
where id_attr_group = 21 

      select * from eio_table_346

select createTempTables();
select setCurrentDl(4);
select dropAttributeFromExistingQualifier(id, 422) from attributes where id in (1169, 1165, 1164, 1163, 1162, 1161, 1160, 1159);

      select * from io_categories where id_child = 342
      select * from io_objects where id_io_category = 343

--create or replace function 

select * from attributes where table_name = 'reestr_chs';

select addAttributeToExistingQualifier(id, 422, false, false, NULL) from attributes where id in (1352, 1353, 1354, 1355, 1356, 1357, 1358, 1359, 1360, 1092, 1093, 1094, 1095, 1096);
select addAttributeToExistingQualifier(id, 422, false, false, NULL) from attributes where id in (1197, 1294);

select * from attributes where id = 1170
update attributes set name = 'Ссылка на адрес в системе ФИАС', title = 'Ссылка на адрес в системе ФИАС' where id = 1170

select * from eio_table_422

select * from io_objects where id = 423; --403
select * from io_categories where id = 409
select * from attrs_categories where id_io_category = 402
select * from attrs_categories where id_io_category = 408


select * from io_objects where id = 539

select cCreateCopy (550)
select createTempTables();
select setCurrentDl(4);
select ioChangeCategory(539, 559);

select * from io_objects where id in (547, 560, 562)
select * from io_categories order by idwhere id = 566
select * from attrs_categories where id_io_category = 565
select * from tbl_eio_table_547
select * from eio_table_547
select * from tbl_eio_table_560
select * from eio_table_562
select * from attributes where code = 'date_date_date'
--1054 - дробное
--1053 - дата
-- 1052 - булево
select dropAttributeFromExistingQualifier(1054, 560, false);
select addAttributeToExistingQualifier(1054, 560, false, false, NULL, true)

select cDeleteCategory(587)

select * from attrs_categories where id_io_attribute = 1387

    select count(ac_to.*) 
    select ac_to.*
    from attrs_categories ac_to
    where 
        ac_to.id_io_category = 590 and ac_to.id_io_attribute not in (select ac_from.id_io_attribute from attrs_categories ac_from where ac_from.id_io_category = 566);


select renameAttributeInExistingQualifier(1387, 562, 'date_date_date1', true);

*/



create or replace function addAttributeToExistingQualifier(int4, int4, bool, bool, varchar, bool) returns int4 as
$BODY$
declare
    idAttribute alias for $1;
    idObject alias for $2;
    isMandatory alias for $3;
    isReadOnly alias for $4;
    defValue alias for $5;
    bCreateCopy alias for $6; --если справочников по категории заданного ИО сделано несколько, 
                              --то установка данного флага в true заставит сделать копию данной категории и заданному ИО назначить именно ее. 
                              --Соответственно атрибут будет добавлен именно в копию этой категории

    idChildCategory int4;
    idCategory int4;

    tableName varchar; --value in table_name field of io_objects
    r record;
    q varchar;
    cnt int4;

    attrName varchar;
    idAttrType int4;
    attrType varchar;
    attrCode varchar;
    attrRefTable varchar; --value in table_name field of attributes
    
begin

    for r in 
        select c.id_child, io.table_name, c.id
        from
            io_categories c,
            io_objects io
        where
            io.id = idObject
            and io.id_io_category = c.id
    loop
        idChildCategory = r.id_child;
        tableName = r.table_name;
        idCategory = r.id;
    end loop;

    if(idChildCategory isnull or tableName isnull) then
        return -1;
    end if;

    --если атрибут уже есть в категории, то ничего не делаем
    select count(id_io_attribute) into cnt from attrs_categories where id_io_category = idChildCategory and id_io_attribute = idAttribute;
    if(cnt <> 0) then
        raise warning 'Attribute % already added into category %', idAttribute, idChildCategory;
        return -2;
    end if;

    --получаем информацию о добавляемом атрибуте
    for r in 
        select 
            a.name,
            a.code as attr_code,
            a.id_a_type,
            a.table_name,
            at.code as attr_type
        from attributes a, a_types at
        where 
            a.id = idAttribute
            and a.id_a_type = at.id
    loop
        attrName = r.name;
        attrCode = r.attr_code;
        idAttrType = r.id_a_type;
        attrRefTable = r.table_name;
        attrType = r.attr_type;
    end loop;

    --если количество ИО по заданной категории больше 1 и при этом параметр bCreateCopy = true, 
    --делаем копию этой категории, меняем данному ИО категорию на новую
    --это надо для того, чтобы не менять структуру таблиц во всех остальных ИО, созданных по исходной категории
    select count(*) into cnt from io_objects where id_io_category = idCategory;
    if(cnt > 1) then
        if(bCreateCopy = true) then
            idCategory = cCreateCopy(idCategory);
            if(idCategory <= 0) then
                return -5;
            end if;
            
            select ioChangeCategory(idObject, idCategory) into cnt;
            if(cnt <= 0) then
                rollback;
                return -4;
            end if;

            --заново перечитаем дочернюю категорию (т.к. ее значение изменилось)
            for r in 
                select c.id_child
                from io_categories c
                where c.id = idCategory
            loop
                idChildCategory = r.id_child;
            end loop;
                
        end if;
    end if;

    --поскольу мы выключаем временно триггеры, то заблокируем таблицу от доступа других пользователей. По завершению транзакции доступ будет автоматически разблокирован
    lock table attrs_categories in access exclusive mode;

    --внутри ф-и addAttrToTable вызывается ф-я acl_secureTable(). Соответственно, нельзя данный блок поставить после вызова addAttrToTable()
    perform f_enable_triggers('attrs_categories', false, ARRAY['trgsetuid']);
    insert into attrs_categories (id_io_category, id_io_attribute, name, is_mandatory, is_read_only, def_value) values (idChildCategory, idAttribute, attrName, isMandatory, isReadOnly, defValue);
    perform f_enable_triggers('attrs_categories', true, ARRAY['trgsetuid']);

    for r in 
        --если делалась копия категории, то данный цикл однозначно отработает только один раз. В противном случае - будут изменены таблицы всех ИО, созданных по данной категории.
        select table_name from io_objects where id_io_category = idCategory    
    loop
        select addAttrToTable(idAttribute, r.table_name, attrCode, attrRefTable, attrType, idAttrType, isMandatory, defValue) into cnt;
        if(cnt <= 0) then
            --если не удалось изменить структуру таблицы, удалим атрибут из категории.
            perform f_enable_triggers('attrs_categories', false, ARRAY['trgsetuid']);
            delete from attrs_categories where id_io_category = idChildCategory and id_io_attribute = idAttribute;
            perform f_enable_triggers('attrs_categories', true, ARRAY['trgsetuid']);
  
            --поскольку в случае, если не удалось изменить структуру одной из изменяемых таблиц, необходимо выполнить откат изменений во всех других таблицах
            --мы вынуждены вызывать исключительную ситуацию
            raise exception 'Cannot change table structure! Table = %, result = %', r.table_name, cnt;

            return cnt;
        end if;
    end loop;

    return 1;

end
$BODY$
language 'plpgsql' security definer;


-- далее работа с конкретной таблицей (справочником)

create or replace function addAttrToTable(int4, varchar, varchar, varchar, varchar, int4, bool, varchar) returns int4 as
$BODY$
declare
    idAttribute alias for $1;
    tableName alias for $2; --value in table_name field of io_objects
    attrCode alias for $3;
    attrRefTable alias for $4;--value in table_name field of attributes
    attrType alias for $5;
    idAttrType alias for $6;
    isMandatory alias for $7;
    defValue alias for $8;

    rTableName varchar; --real table name (with tbl_)
    rAttrRefTable varchar; --real table name (ith tbl_)

    cnt int4;
    q varchar;
    r record;
    isExist int4;
    
begin

    --Hack для выбора еального названия таблицы (tbl_ или не tbl_)
    select f_is_table_exist(tableName, NULL) into isExist; 
    if(isExist = 1) then
        rTableName := tableName;
    else
        select f_is_table_exist('tbl_' || tableName, NULL) into isExist; 
        if(isExist = 1) then
            rTableName := 'tbl_' || tableName;
        else
            raise exception 'Table % does not exist!', tableName;
            return 0;
        end if;
    end if;


    --hack для выбора правильного названия таблицы, на которую ссылается добавляемый атрибут (для атрибутов-ссылок)
    if(attrRefTable is not null) then
        select f_is_table_exist(attrRefTable, NULL) into isExist; 
        if(isExist = 1) then
            rAttrRefTable := attrRefTable;
        else
            select f_is_table_exist('tbl_' || attrRefTable, NULL) into isExist; 
            if(isExist = 1) then
                rAttrRefTable := 'tbl_' || attrRefTable;
            else
                raise exception 'Table % does not exist!', attrRefTable;
                return 0;
            end if;
        end if;
    end if;

    --обязательные атрибуты должны иметь значение
    --соответственно, если в изменяемой таблице имеются записи, а значение по умолчанию для атрибута не задано - завершаем работу функции
    if(idAttrType <> 17 and isMandatory = true and defValue isnull) then
        cnt = 0;
        q = 'select count(*) as cnt from ' || tableName;
        for r in execute q
        loop
            cnt = r.cnt;
        end loop;

        if(cnt > 0) then
            raise warning 'New attribute is marked as mandatory, and default value for that is NULL. But qualifier table contains some records. Cannot set NULL values to new field!';
            return -3;
        end if;
    end if;
    

        --поскольку меняем структуру таблицы - заблокируем доступ к ней.
    q = 'lock table ' || rTableName || ' in access exclusive mode';
    execute q;

    if(idAttrType <> 17) then --набор элементов справочника
        q = 'alter table ' || rTableName || ' add column "' || attrCode || '" ' || attrType;
        execute q;
    end if;

    if(idAttrType = 2) then --элемент справочника
        q = 'alter table ' || rTableName || ' add CONSTRAINT "fk_' || tableName || '_ref_' || attrCode || '" FOREIGN KEY ("' || attrCode || '")
                                                  REFERENCES ' || rAttrRefTable || ' (id) MATCH SIMPLE
                                                  ON UPDATE RESTRICT ON DELETE RESTRICT';
        execute q;
    end if;

    if(idAttrType = 3) then --parent
        q = 'alter table ' || rTableName || ' add CONSTRAINT "fk_' || tableName || '_ref_' || attrCode || '" FOREIGN KEY ("' || attrCode || '")
                                                  REFERENCES ' || rTableName || ' (id) MATCH SIMPLE
                                                  ON UPDATE RESTRICT ON DELETE RESTRICT';
        execute q;
    end if;

    if(idAttrType = 17) then --набор элементов справочника

        q = 'CREATE TABLE ' || tableName || '_' || attrRefTable || '_ref_' || idAttribute || '
             (
                 "id_' || tableName || '" bigint,
                 "id_' || attrRefTable || '" bigint,

                 CONSTRAINT "fk_id_' || tableName || '_' || attrRefTable || '_ref_' || idAttribute || '_1" FOREIGN KEY ("id_' || tableName || '")
                 REFERENCES ' || rTableName || ' (id) MATCH SIMPLE
                 ON UPDATE CASCADE ON DELETE CASCADE,

                 CONSTRAINT "fk_id_' || tableName || '_' || attrRefTable || '_ref_' || idAttribute || '_2" FOREIGN KEY ("id_' || attrRefTable || '")
                 REFERENCES ' || rAttrRefTable || ' (id) MATCH SIMPLE
                 ON UPDATE CASCADE ON DELETE CASCADE
             )';

        execute q;
    end if;

    if(idAttrType = 19) then --ссылка на цвет записи (фон)
        q = 'alter table ' || rTableName || ' add CONSTRAINT "fk_' || tableName || '_ref_' || attrCode || '" FOREIGN KEY ("' || attrCode || '")
                                                  REFERENCES ' || rAttrRefTable || ' (id) MATCH SIMPLE
                                                  ON UPDATE RESTRICT ON DELETE RESTRICT';
        execute q;
    
    end if;

    if(idAttrType = 26) then --ссылка на цвет записи (текст)
        q = 'alter table ' || rTableName || ' add CONSTRAINT "fk_' || tableName || '_ref_' || attrCode || '" FOREIGN KEY ("' || attrCode || '")
                                                  REFERENCES ' || rAttrRefTable || ' (id) MATCH SIMPLE
                                                  ON UPDATE RESTRICT ON DELETE RESTRICT';
        execute q;
    end if;

    if(idAttrType <> 17) then --набор элементов справочника
        if(defValue is not null) then
            q = 'alter table ' || rTableName || ' alter column "' || attrCode || '" set default ' || asString(defValue, true);
            execute q;
            q = 'update ' || rTableName || ' set "' || attrCode || '" = ' || asString(defValue, true);
            execute q;
        end if;
    
        if(isMandatory = true) then
            q = 'alter table ' || rTableName || ' alter column "' || attrCode || '" set not null';
            execute q;
        end if;
    end if;

    perform f_safe_drop_view(tableName);
    q = 'alter table ' || rTableName || ' rename to ' || tableName;
    execute q;
    perform acl_secureTable(tableName);

    return 1;

end
$BODY$
language 'plpgsql' security definer;

create or replace function dropAttributeFromExistingQualifier(int4, int4, bool) returns int4 as
$BODY$
declare
    idAttribute alias for $1;
    idObject alias for $2;
    bCreateCopy alias for $3; --если справочников по категории заданного ИО сделано несколько, 
                              --то установка данного флага в true заставит сделать копию данной категории и заданному ИО назначить именно ее. 
                              --Соответственно атрибут будет добавлен именно в копию этой категории

    idChildCategory int4;
    idCategory int4;

    tableName varchar; --value in table_name field of io_objects

    r record;
    q varchar;
    cnt int4;

    idAttrType int4;
    attrCode varchar;
    attrRefTable varchar; --value in table_name field of attributes
    
begin

    for r in 
        select c.id_child, io.table_name, c.id
        from
            io_categories c,
            io_objects io
        where
            io.id = idObject
            and io.id_io_category = c.id
    loop
        idChildCategory = r.id_child;
        tableName = r.table_name;
        idCategory = r.id;
    end loop;

    if(idChildCategory isnull or tableName isnull) then
        return -1;
    end if;

    select count(id_io_attribute) into cnt from attrs_categories where id_io_category = idChildCategory and id_io_attribute = idAttribute;
    if(cnt = 0) then
        raise warning 'Attribute % does not contains in category %', idAttribute, idChildCategory;
        return -2;
    end if;

    --получаем параметры удаляемого атрибута
    for r in 
        select 
            a.name,
            a.code as attr_code,
            a.id_a_type,
            a.table_name,
            at.code as attr_type
        from attributes a, a_types at
        where 
            a.id = idAttribute
            and a.id_a_type = at.id
    loop
        --attrName = r.name;
        attrCode = r.attr_code;
        idAttrType = r.id_a_type;
        attrRefTable = r.table_name;
        --attrType = r.attr_type;
    end loop;

    --если количество ИО по заданной категории больше 1 и при этом параметр bCreateCopy = true, 
    --делаем копию этой категории, меняем данному ИО категорию на новую
    --это надо для того, чтобы не менять структуру таблиц во всех остальных ИО, созданных по исходной категории
    select count(*) into cnt from io_objects where id_io_category = idCategory;
    if(cnt > 1) then
        if(bCreateCopy = true) then
            idCategory = cCreateCopy(idCategory);
            if(idCategory <= 0) then
                return -5;
            end if;
            
            select ioChangeCategory(idObject, idCategory) into cnt;
            if(cnt <= 0) then
                return cnt;
            end if;

            --заново перечитаем дочернюю категорию (т.к. ее значение изменилось)
            for r in 
                select c.id_child
                from io_categories c
                where c.id = idCategory
            loop
                idChildCategory = r.id_child;
            end loop;
                
        end if;
    end if;


    --поскольу мы выключаем временно триггеры, то заблокируем таблицу от доступа других пользователей. По завершению транзакции доступ будет автоматически разблокирован
    lock table attrs_categories in access exclusive mode;
    lock table io_views in access exclusive mode;

    perform f_enable_triggers('attrs_categories', false, NULL::varchar[]);
    delete from io_views where id_attr_category = (select id from attrs_categories where id_io_category = idChildCategory and id_io_attribute = idAttribute);
    delete from attrs_categories where id_io_category = idChildCategory and id_io_attribute = idAttribute;
    perform f_enable_triggers('attrs_categories', true, NULL::varchar[]);

    for r in 
        --если делалась копия категории, то данный цикл однозначно отработает только один раз. В противном случае - будут изменены таблицы всех ИО, созданных по данной категории.
        select table_name from io_objects where id_io_category = idCategory    
    loop

        select dropAttrFromTable(idAttribute, r.table_name, attrCode, attrRefTable, idAttrType) into cnt;
        if(cnt <= 0) then
            --поскольку в случае, если не удалось изменить структуру одной из изменяемых таблиц, необходимо выполнить откат изменений во всех других таблицах
            --мы вынуждены вызывать исключительную ситуацию
            raise exception 'Cannot change table structure! Table = %, result = %', r.table_name, cnt;

            return cnt;
        end if;
    end loop;

    return 1;

end
$BODY$
language 'plpgsql' security definer;


-- далее работа с конкретной таблицей (справочником)

create or replace function dropAttrFromTable(int4, varchar, varchar, varchar, int4) returns int4 as
$BODY$
declare
    idAttribute alias for $1;
    tableName alias for $2; --value in table_name field of io_objects

    attrCode alias for $3;
    attrRefTable alias for $4;--value in table_name field of attributes
    idAttrType alias for $5; --тип удаляемого атрибута

    rTableName varchar; --real table name (with tbl_)
    rAttrRefTable varchar; --real table name (ith tbl_)

    cnt int4;
    q varchar;
    r record;
    isExist int4;
    
begin

    --Hack для выбора реального названия таблицы (tbl_ или не tbl_)
    select f_is_table_exist(tableName, NULL) into isExist; 
    if(isExist = 1) then
        rTableName := tableName;
    else
        select f_is_table_exist('tbl_' || tableName, NULL) into isExist; 
        if(isExist = 1) then
            rTableName := 'tbl_' || tableName;
        else
            raise exception 'Table % does not exist!', tableName;
            return 0;
        end if;
    end if;


    --hack для выбора правильного названия таблицы, на которую ссылается удаляемый атрибут (для атрибутов-ссылок типа многие-ко-многим)
    if(attrRefTable is not null) then
        select f_is_table_exist(attrRefTable, NULL) into isExist; 
        if(isExist = 1) then
            rAttrRefTable := attrRefTable;
        else
            select f_is_table_exist('tbl_' || attrRefTable, NULL) into isExist; 
            if(isExist = 1) then
                rAttrRefTable := 'tbl_' || attrRefTable;
            else
                raise exception 'Table % does not exist!', attrRefTable;
                return 0;
            end if;
        end if;
    end if;

    if(idAttrType <> 17) then --набор элементов справочника
        --поскольку меняем структуру таблицы - заблокируем доступ к ней.
        q = 'lock table ' || rTableName || ' in access exclusive mode';
        execute q;

        q = 'alter table ' || rTableName || ' drop column "' || attrCode || '"';
        execute q;

        perform f_safe_drop_view(tableName);
        q = 'alter table ' || rTableName || ' rename to ' || tableName;
        execute q;
        perform acl_secureTable(tableName);

        return 1;
        
    end if;

    --id_a_type = 17 (набор элементов справочника)
    if(attrRefTable isnull) then
        raise warning 'attribute has type = 17 (many-to-many) but ref_table is NULL!';
        return -1;
    end if;

    q = 'drop table ' || tableName || '_' || attrRefTable || '_ref_' || idAttribute;
    execute q;

    perform f_safe_drop_view(tableName);
    q = 'alter table ' || rTableName || ' rename to ' || tableName;
    execute q;
    perform acl_secureTable(tableName);

    
    return 1;

end
$BODY$
language 'plpgsql' security definer;



create or replace function renameAttributeInExistingQualifier(int4, int4, varchar, bool) returns int4 as
$BODY$
declare
    idAttribute alias for $1;
    idObject alias for $2;
    attrCodeTo alias for $3;
    bCreateCopy alias for $4; --если справочников по категории заданного ИО сделано несколько, 
                              --то установка данного флага в true заставит сделать копию данной категории и заданному ИО назначить именно ее. 
                              --Соответственно атрибут будет добавлен именно в копию этой категории

    idChildCategory int4;
    idCategory int4;

    idAttributeNew int4; -- копия исходного атрибута с переименованным кодом

    tableName varchar; --value in table_name field of io_objects

    r record;
    q varchar;
    cnt int4;

    idAttrType int4;
    attrCode varchar;
    attrRefTable varchar; --value in table_name field of attributes
    
begin

    if(attrCodeTo isnull or trim(attrCodeTo) = '') then --некорректное наименование кода атрибута
        return -1;
    end if;

    for r in 
        select c.id_child, io.table_name, c.id
        from
            io_categories c,
            io_objects io
        where
            io.id = idObject
            and io.id_io_category = c.id
    loop
        idChildCategory = r.id_child;
        tableName = r.table_name;
        idCategory = r.id;
    end loop;

    if(idChildCategory isnull or tableName isnull) then
        return -1;
    end if;

    select count(id_io_attribute) into cnt from attrs_categories where id_io_category = idChildCategory and id_io_attribute = idAttribute;
    if(cnt = 0) then
        raise warning 'Attribute % does not contains in category %', idAttribute, idChildCategory;
        return -2;
    end if;

    --получаем параметры удаляемого атрибута
    for r in 
        select 
            a.name,
            a.code as attr_code,
            a.id_a_type,
            a.table_name,
            at.code as attr_type
        from attributes a, a_types at
        where 
            a.id = idAttribute
            and a.id_a_type = at.id
    loop
        --attrName = r.name;
        attrCode = r.attr_code;
        idAttrType = r.id_a_type;
        attrRefTable = r.table_name;
        --attrType = r.attr_type;
    end loop;


    if(attrCode = attrCodeTo) then
        --нечего переименовывать
        return 1;
    end if;
    
    if(idAttrType = 17) then --для атрибутов многие-ко-многим разрешаем переименование кода атрибута безусловно, т.к. в таблицах таких колонок все равно нет
        lock table attributes in access exclusive mode;
        perform f_enable_triggers('attributes', false, NULL::varchar[]);
        update attributes set code = attrCodeTo where id = idAttribute;
        perform f_enable_triggers('attributes', true, NULL::varchar[]);
        
        return 1;
    end if;


    --поскольу мы выключаем временно триггеры, то заблокируем таблицу от доступа других пользователей. По завершению транзакции доступ будет автоматически разблокирован
    lock table attrs_categories in access exclusive mode;
    lock table io_views in access exclusive mode;

    idAttributeNew = idAttribute; --если будем делать копию атрибута и категории, то присвоим ему новое значение. А если не будем делать копию, то далее будем работать с idAttributeNew

    --если количество ИО по категории, которая содержит заданный атрибут, больше 1 и при этом параметр bCreateCopy = true, 
    --делаем копию этого атрибута и этой категории, меняем данному ИО категорию на новую
    --это надо для того, чтобы не менять структуру таблиц во всех остальных ИО, созданных по категории, содержащей исходный атрибут
    select count(io.table_name) into cnt
    from 
        attrs_categories ac, 
        io_categories c1, 
        io_categories c2,
        io_objects io
    where 
        ac.id_io_attribute = idAttribute
        and ac.id_io_category = c2.id
        and c2.id = c1.id_child
        and c1.id = io.id_io_category
        and io.table_name is not null;

    if(cnt > 1) then
        if(bCreateCopy = true) then
            idAttributeNew = aCreateCopy(idAttribute, attrCodeTo);
            if(idAttributeNew <= 0) then
                rollback;
                return idAttributeNew;
            end if;

            --если по категории, которая содержит целевой ИО, создан только он (только один ИО), то копию категории делать не надо
            --в противном случае сделаем также копию категории и зададим ее изменяемому ИО
            select count(*) into cnt from io_objects where id_io_category = idCategory;
            if(cnt > 1) then
                idCategory = cCreateCopy(idCategory);
                if(idCategory <= 0) then
                    rollback;
                    return -5;
                end if;
            
                select ioChangeCategory(idObject, idCategory) into cnt;
                if(cnt <= 0) then
                    rollback;
                    return cnt;
                end if;

                --заново перечитаем дочернюю категорию (т.к. ее значение изменилось)
                for r in 
                    select c.id_child
                    from io_categories c
                    where c.id = idCategory
                loop
                    idChildCategory = r.id_child;
                end loop;
 
            end if;


            perform f_enable_triggers('attrs_categories', false, ARRAY['trgsetuid']);
            insert into attrs_categories (id_io_category, id_io_attribute, name, is_mandatory, is_read_only, def_value) 
                select idChildCategory, idAttributeNew, ac.name, ac.is_mandatory, ac.is_read_only, ac.def_value 
                from attrs_categories ac where id_io_attribute = idAttribute and id_io_category = idChildCategory;
            perform f_enable_triggers('attrs_categories', true, ARRAY['trgsetuid']);


            perform f_enable_triggers('attrs_categories', false, NULL::varchar[]);
            delete from io_views where id_attr_category = (select id from attrs_categories where id_io_category = idChildCategory and id_io_attribute = idAttribute);
            delete from attrs_categories where id_io_category = idChildCategory and id_io_attribute = idAttribute;
            perform f_enable_triggers('attrs_categories', true, NULL::varchar[]);

        end if;
    end if;


    if(idAttribute = idAttributeNew) then --если атрибуту не делали копию, то необходимо переименовать ему код
        lock table attributes in access exclusive mode;
        perform f_enable_triggers('attributes', false, NULL::varchar[]);
        update attributes set code = attrCodeTo where id = idAttribute;
        perform f_enable_triggers('attributes', true, NULL::varchar[]);
    end if;


    for r in 
        select io.table_name
        from 
            attrs_categories ac, 
            io_categories c1, 
            io_categories c2,
            io_objects io
        where 
            ac.id_io_attribute = idAttributeNew
            and ac.id_io_category = c2.id
            and c2.id = c1.id_child
            and c1.id = io.id_io_category
            and io.table_name is not null
    loop

        select renameAttrInTable(r.table_name, attrCode, attrCodeTo) into cnt;
        if(cnt <= 0) then
            --поскольку в случае, если не удалось изменить структуру одной из изменяемых таблиц, необходимо выполнить откат изменений во всех других таблицах
            raise warning 'Cannot change table structure! Table = %, result = %', r.table_name, cnt;
            rollback;
            return cnt;
        end if;
    end loop;

    return 1;

end
$BODY$
language 'plpgsql' security definer;


-- далее работа с конкретной таблицей (справочником)

create or replace function renameAttrInTable(varchar, varchar, varchar) returns int4 as
$BODY$
declare
    tableName alias for $1; --value in table_name field of io_objects

    attrCodeFrom alias for $2;
    attrCodeTo alias for $3;
    
    rTableName varchar; --real table name (with tbl_)

    q varchar;
    isExist int4;
    
begin

    --Hack для выбора реального названия таблицы (tbl_ или не tbl_)
    select f_is_table_exist(tableName, NULL) into isExist; 
    if(isExist = 1) then
        rTableName := tableName;
    else
        select f_is_table_exist('tbl_' || tableName, NULL) into isExist; 
        if(isExist = 1) then
            rTableName := 'tbl_' || tableName;
        else
            raise exception 'Table % does not exist!', tableName;
            return 0;
        end if;
    end if;


    --поскольку меняем структуру таблицы - заблокируем доступ к ней.
    q = 'lock table ' || rTableName || ' in access exclusive mode';
    execute q;

    q = 'alter table ' || rTableName || ' rename column "' || attrCodeFrom || '" to "' || attrCodeTo || '"';
    execute q;

    perform f_safe_drop_view(tableName);
    q = 'alter table ' || rTableName || ' rename to ' || tableName;
    execute q;
    perform acl_secureTable(tableName);

    return 1;

end
$BODY$
language 'plpgsql' security definer;



