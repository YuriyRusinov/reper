create or replace function cAddAttr(int4, int4, varchar, boolean, boolean) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    idAttr alias for $2;
    defValue alias for $3;
    isMandatory alias for $4;
    isReadOnly alias for $5;

    cnt int4;
begin

    select count(*) into cnt from attrs_categories where id_io_category = idCategory and id_io_attribute = idAttr;
    if(cnt > 0) then
        return 1;
    end if;

    insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only)
    values(idCategory, idAttr, defValue, isMandatory, isReadOnly);

    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return 1;

end
$BODY$
language 'plpgsql';
