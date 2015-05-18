create or replace function cCreateCopy(int4) returns int4 as
$BODY$
declare
    idFrom alias for $1;
    idTo int4;
    idChild int4;
    idChild2 int4;
begin

    select id_child into idChild from io_categories where id = idFrom;
    if(idChild is not null) then
        idChild = cCreateCopy(idChild);
        if(idChild <= 0) then
            return idChild;
        end if;
    end if;

    select id_child2 into idChild2 from io_categories where id = idFrom;
    if(idChild2 is not null) then
        idChild2 = cCreateCopy(idChild2);
        if(idChild2 <= 0) then
            return idChild2;
        end if;
    end if;

    select getNextSeq('io_categories', 'id') into idTo;

    insert into io_categories (id,
                               id_io_category_type,
                               id_child,
                               id_child2,
                               is_main,
                               name,
                               --code,
                               description,
                               is_system,
                               is_global,
                               id_io_state)

                        select idTo, 
                               c.id_io_category_type,
                               idChild,
                               idChild2,
                               c.is_main,
                               c.name || '_copy_' || idTo,
                               --c.code || '_copy',
                               c.description,
                               c.is_system,
                               false,
                               --uniqueID,
                               --c.is_global,
                               c.id_io_state 
                         from  io_categories c where id = idFrom;

    insert into access_categories_table (id_io_category, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use) 
    select idTo, a.id_role, a.allow_read, a.allow_readlist, a.allow_delete, a.allow_update, a.allow_use from access_categories_table a where id_io_category = idFrom and id_role not in (-10, 1);

    insert into attrs_categories (id_io_category, id_io_attribute, name, def_value, is_mandatory, is_read_only, "order", directives) 
    select idTo, ac.id_io_attribute, ac.name, ac.def_value, ac.is_mandatory, ac.is_read_only, ac."order", ac.directives from attrs_categories ac where id_io_category = idFrom;

    --select 5555, ac.id_io_attribute, ac.name, ac.def_value, ac.is_mandatory, ac.is_read_only, ac."order" from attrs_categories ac where id_io_category = 550;
    perform cSetCompleted(idTo, 1);

    update io_categories set is_global = (select c.is_global from io_categories c where id = idFrom) where id = idTo;
    
    return idTo;


end
$BODY$
language 'plpgsql';
