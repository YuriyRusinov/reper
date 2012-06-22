create or replace function cUpdateAttr(int4, int4, varchar, boolean, boolean) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    idAttr alias for $2;
    
    defValue alias for $3;
    isMandatory alias for $4;
    isReadOnly alias for $5;

    cnt int4;
    idCategoryAttr int4;
begin

    select id into idCategoryAttr from attrs_categories where id_io_category = idCategory and id_io_attribute = idAttr;
    if(idCategoryAttr is not null) then
        update attrs_categories set def_value = defValue, is_mandatory = isMandatory, is_read_only = isReadOnly where id_io_category = idCategory and id_io_attribute = idAttr;
        return idCategoryAttr;
    end if;

    idCategoryAttr := getNextSeq('attrs_categories', 'id');

    insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only)
    values(idCategoryAttr, idCategory, idAttr, defValue, isMandatory, isReadOnly);

    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return idCategoryAttr;

end
$BODY$
language 'plpgsql';

