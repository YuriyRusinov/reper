
--select * from attributes where id_a_type = 17;

--select * from cGetSourceTableNameForAttr(490, 1290);
create or replace function cGetSourceTableNameForAttr(int4, int4) returns varchar as
$BODY$
declare
    idObject alias for $1;
    idAttr alias for $2;

    tableName varchar;
    idCategory int4;
    idChildCategory int4;
    idAttribute int4;
    r record;
begin

    for r in 
        select c.id, c.id_child  
        from io_objects io, io_categories c
        where
            io.id = idObject
            and io.id_io_category = c.id
    loop
        idCategory = r.id;
        idChildCategory = r.id_child;
    end loop;

    if(idCategory isnull) then
        return NULL;
    end if;

    if(idChildCategory isnull) then
        select ac.id_io_attribute into idAttribute
        from attrs_categories ac
        where
             ac.id_io_category = idCategory
             and ac.id_io_attribute = idAttr;

    else
   
        select ac.id_io_attribute into idAttribute
        from attrs_categories ac
        where
            ac.id_io_category = idChildCategory
            and ac.id_io_attribute = idAttr;

        if(idAttribute isnull) then
            select ac.id_io_attribute into idAttribute
            from attrs_categories ac
            where
                ac.id_io_category = idCategory
                and ac.id_io_attribute = idAttr;
        end if;

    end if;

    if(idAttribute isnull) then
        return NULL;
    end if;

    select table_name into tableName from attributes where id = idAttribute;

    return tableName;

end
$BODY$
language 'plpgsql';

