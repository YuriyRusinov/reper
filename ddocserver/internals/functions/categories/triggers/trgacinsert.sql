/*
Мы разрешаем повторение кодов атрибутов, но мы запрещаем наличие атрибутов с одинаковыми кодами в одной категории
*/
create or replace function acInsertCheck() returns trigger as 
$BODY$
declare
    refTableName varchar;
    r record;
    aCode varchar; 
    aCount int4;
    aName varchar;
begin
    select code into aCode from attributes where id = new.id_io_attribute;
    if(aCode isnull) then
        return NULL;
    end if;

    if(TG_OP = 'INSERT') then
        select count(a.id) into aCount
        from 
            attributes a, 
            attrs_categories ac
        where 
            a.id = ac.id_io_attribute
            and ac.id_io_category = new.id_io_category
            and a.code = aCode;

        if(aCount <> 0) then
            return NULL;
        end if;
    end if;

    if(TG_OP = 'UPDATE') then
        if(old.id_io_attribute <> new.id_io_attribute or old.id_io_category <> new.id_io_category) then
            raise exception 'You cannot update IDs id category or attribute in table attrs_categories!';
            return NULL;
        end if;
    end if;

    select name into aName from attributes where id = new.id_io_attribute;
    
    new.name := aName;

    return new;
end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trgacinsert', 'attrs_categories');

select f_create_trigger('trgacinsert', 'before', 'insert or update', 'attrs_categories', 'acinsertcheck()');
