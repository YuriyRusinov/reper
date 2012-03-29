create or replace function setAlarmEventsState(int4) returns int4 as
$BODY$
declare
    aState alias for $1;
    aColor varchar;
    aText varchar;
    cnt int4;
begin
/*
    if(aState = 1) then --Исправно
        aColor = '4278255487';  --зеленое
        aText = 'Нет необработанных тревожных событий';
    else
        aColor = '4294923775';  --розовое
        aText = 'Появились необработанные тревожные события';
    end if;


    perform ioSetUpdating(66);

    select av.id_attr_category into cnt from attrs_values av, attrs_categories ac
    where
        av.id_io_object = 66
        av.id_attr_category = ac.id
        and ac.id_io_attribute = 220
        and ac.id_io_category = 134;

    if(cnt isnull) then
        insert into attrs_values (id_io_object, id_attr_category, value) values (66, cnt, aColor);
    else 
        update attrs_values set
            value = aColor
        where
            id_io_object = 66
            and id_attr_category = cnt;
    end if;

    update io_objects set record_fill_color = aColor::int8 where id = 66;

    select av.id_attr_category into cnt from attrs_values av, attrs_categories ac
    where
        av.id_io_object = 66
        av.id_attr_category = ac.id
        and ac.id_io_attribute = 219
        and ac.id_io_category = 134;

    if(cnt isnull) then
        insert into attrs_values (id_io_object, id_attr_category, value) values (66, cnt, aText);
    else 
        update attrs_values set
            value = aText
        where
            id_io_object = 66
            and id_attr_category = cnt;
    end if;

    perform ioSetCompleted(66, 2);
 */
    return 1;

end
$BODY$
language 'plpgsql';

create or replace function setFaultDevicesState(int4) returns int4 as
$BODY$
declare
    aState alias for $1;
    aColor varchar;
    aText varchar;
    cnt int4;
begin
/*
    perform ioSetUpdating(68);

    if(aState = 1) then --Исправно
        aColor = '4278255487';--зеленое
        aText = 'Все устройства ТСО исправны';
    else
        aColor = '4294967040'; --желтое
        aText = 'Появились неисправные устройства';
    end if;

    select count(*) into cnt from attrs_values 
    where
        id_io_object = 68
        and id_io_attribute = 220;

    if(cnt = 0) then
        insert into attrs_values (id_io_object, id_io_category, id_io_attribute, value) values (68, 136, 220, aColor);
    else 
        update attrs_values set
            value = aColor
        where
            id_io_object = 68
            and id_io_attribute = 220;
    end if;

    update io_objects set record_fill_color = aColor::int8 where id = 68;

    select count(*) into cnt from attrs_values 
    where
        id_io_object = 68
        and id_io_attribute = 219;

    if(cnt = 0) then
        insert into attrs_values (id_io_object, id_io_category, id_io_attribute, value) values (68, 136, 219, aColor);
    else 
        update attrs_values set
            value = aText
        where
            id_io_object = 68
            and id_io_attribute = 219;
    end if;

    perform ioSetCompleted(68, 2);
----

    perform ioSetUpdating(67);
    
    select count(*) into cnt from attrs_values 
    where
        id_io_object = 67
        and id_io_attribute = 220;

    if(cnt = 0) then
        insert into attrs_values (id_io_object, id_io_category, id_io_attribute, value) values (67, 138, 220, aColor);
    else 
        update attrs_values set
            value = aColor
        where
            id_io_object = 67
            and id_io_attribute = 220;
    end if;

    update io_objects set record_fill_color = aColor::int8 where id = 67;

    select count(*) into cnt from attrs_values 
    where
        id_io_object = 67
        and id_io_attribute = 219;

    if(cnt = 0) then
        insert into attrs_values (id_io_object, id_io_category, id_io_attribute, value) values (67, 138, 219, aColor);
    else 
        update attrs_values set
            value = aText
        where
            id_io_object = 67
            and id_io_attribute = 219;
    end if;

    perform ioSetCompleted(67, 2);
 */
    return 1;

end
$BODY$
language 'plpgsql';
