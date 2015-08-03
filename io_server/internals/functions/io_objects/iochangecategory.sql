create or replace function ioChangeCategory(int4, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idCategoryTo alias for $2;

    idCategoryFrom int4;

    idChildCategoryTo int4;
    idChildCategoryFrom int4;
    
    r record;
    cnt int4;
begin


    for r in 
        select c.id_child, c.id
        from
            io_categories c,
            io_objects io
        where
            io.id = idObject
            and io.id_io_category = c.id
    loop
        idChildCategoryFrom = r.id_child;
        idCategoryFrom = r.id;
    end loop;

    for r in 
        select c.id_child
        from io_categories c
        where
            c.id = idCategoryTo
    loop
        idChildCategoryTo = r.id_child;
    end loop;


    --не разрешаем менять категорию ИО на другую категори, если они имеют разный тип (справочник - не справочник).
    if((idChildCategoryFrom is not null and idChildCategoryTo isnull) or (idChildCategoryFrom isnull and idChildCategoryTo is not null)) then
        return -2;
    end if;
    
    --проверяем совпадение атрибутов в главных категориях
    select count(ac_from.*) into cnt 
    from attrs_categories ac_from 
    where 
        ac_from.id_io_category = idCategoryFrom and ac_from.id_io_attribute <> 402 and ac_from.id_io_attribute not in (select ac_to.id_io_attribute from attrs_categories ac_to where ac_to.id_io_category = idCategoryTo and ac_to.id_io_attribute <> 402 );

    if(cnt <> 0) then
        return -3;
    end if;

    select count(ac_to.*) into cnt 
    from attrs_categories ac_to
    where 
        ac_to.id_io_category = idCategoryTo and ac_to.id_io_attribute <> 402 and ac_to.id_io_attribute not in (select ac_from.id_io_attribute from attrs_categories ac_from where ac_from.id_io_category = idCategoryFrom and ac_from.id_io_attribute <> 402);

    if(cnt <> 0) then
        return -4;
    end if;

  
    --проверяем совпадение атрибутов в дочерних категориях
    if(idChildCategoryFrom is not null and idChildCategoryTo is not null) then
        select count(ac_from.*) into cnt 
        from attrs_categories ac_from 
        where 
            ac_from.id_io_category = idChildCategoryFrom and ac_from.id_io_attribute not in (select ac_to.id_io_attribute from attrs_categories ac_to where ac_to.id_io_category = idChildCategoryTo);

        if(cnt <> 0) then
            return -5;
        end if;

        select count(ac_to.*) into cnt 
        from attrs_categories ac_to
        where 
            ac_to.id_io_category = idChildCategoryTo and ac_to.id_io_attribute not in (select ac_from.id_io_attribute from attrs_categories ac_from where ac_from.id_io_category = idChildCategoryFrom);

        if(cnt <> 0) then
            return -6;
        end if;
    end if;

    --если категории одинаковые, то разрешаем изменение (перед этим заблокируем таблицу от доступа других пользователей. По завершению транзакции доступ будет автоматически разблокирован)
    
    lock table tbl_io_objects in access exclusive mode;

    --меняем категорию
    perform f_enable_triggers('tbl_io_objects', false, NULL);
    update tbl_io_objects set id_io_category = idCategoryTo where id = idObject;
    perform f_enable_triggers('tbl_io_objects', true, NULL);

    lock table tbl_attrs_values in access exclusive mode;

    --меняем ссылки на атрибуты в таблице значений атрибутов для ИО (для табличных категорий никаких действий производить не надо)
    perform f_enable_triggers('attrs_values', false, NULL);
    for r in 
        select 
            ac_from.id as id_from, ac_to.id as id_to
        from
            attrs_categories ac_from,
            attrs_categories ac_to
        where
            ac_from.id_io_category = idCategoryFrom
            and ac_to.id_io_category = idCategoryTo
            and ac_from.id_io_attribute = ac_to.id_io_attribute
    loop
        update attrs_values set id_attr_category = r.id_to where id_io_object = idObject and id_attr_category = r.id_from;
    end loop;

    perform f_enable_triggers('attrs_values', true, NULL);

    return 1;

end
$BODY$
language 'plpgsql' security definer;
