create or replace function cInsert(varchar, varchar, varchar, int4, int4, boolean, varchar, boolean, int4) returns int4 as
$BODY$
declare
    cName alias for $1;
    cCode alias for $2;
    cDesc alias for $3;
    idType alias for $4;
    idChild alias for $5;
    isMain alias for $6;
    uniqueID alias for $7;
    isGlobal alias for $8;
    idState alias for $9;
    
    idCategory int4;
begin

    select cFindEqual(cName, cCode, cDesc, idType, idChild, isMain) into idCategory;
    if(idCategory is not null and idCategory > 0) then
        return idCategory;
    end if;
    
    select getNextSeq('io_categories', 'id') into idCategory;

    insert into io_categories (id,
                               id_io_category_type,
                               id_child,
                               is_main,
                               name,
                               code,
                               description,
                               is_system,
                               unique_id,
                               is_global,
                               id_io_state)
                       values (idCategory,
                               idType,
                               idChild,
                               isMain,
                               cName,
                               cCode,
                               cDesc,
                               false,
                               uniqueID,
                               isGlobal,
                               idState);
    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return idCategory;
end
$BODY$
language 'plpgsql';
