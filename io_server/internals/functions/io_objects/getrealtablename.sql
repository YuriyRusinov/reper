
--Функция используется при создании таблиц ИО
--когда надо решить, какое название таблице присваивать
--такая логика необходима, поскольку мы позволяем пользователям самостоятельно задавать названия таблиц справочников
--в этом случае, если таблица с заданным названием существует,
--то необходимо ей сгенерировать автоматическое уникальное
create or replace function getRealTableName(varchar, int4) returns varchar as
$BODY$
declare
    io_tableName alias for $1;
    idObject alias for $2;

    bExist bool;
    res int4;
    tName varchar;
begin

    bExist = false;

    select into res count (*) from pg_tables as t where t.tablename = io_tableName;
    if (res > 0) then
        raise warning 'entity % (table) already exists in database', io_tableName;
        bExist = true;
    end if;

    select into res count (*) from pg_views as v where v.viewname = io_tableName;
    if (res > 0) then
        raise warning 'entity % (view) already exists in database', io_tableName;
        bExist = true;
    end if;

    if(bExist = false) then
        return io_tableName;
    end if;


    tName = io_tableName || '_' || idObject;

    return getRealTableName(tName, idObject);

end
$BODY$
language 'plpgsql';
