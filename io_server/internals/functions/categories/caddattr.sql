drop function if exists caddattr(int4, int4, varchar, boolean, boolean);
create or replace function cAddAttr(int4, int4, varchar, boolean, boolean, varchar, int4) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    idAttr alias for $2;
    defValue alias for $3;
    isMandatory alias for $4;
    isReadOnly alias for $5;
    acDirectives alias for $6;
    acOrder alias for $7;

    cnt int4;
    r record;
    idCategoryAttr int4;
begin

    idCategoryAttr := NULL;
    for r in 
        select 
            id, def_value, is_mandatory, is_read_only, directives, "order" 
        from attrs_categories 
       where 
           id_io_category = idCategory and id_io_attribute = idAttr
    loop
        idCategoryAttr := r.id;
        
        if(idCategoryAttr is not null) then
            if(r.def_value <> defValue or r.is_mandatory <> isMandatory or r.is_read_only <> isReadOnly or r.directives <> acDirectives or r."order" <> acOrder) then
                update attrs_categories set 
                    def_value = defValue, 
                    is_mandatory = isMandatory, 
                    is_read_only = isReadOnly, 
                    directives = acDirectives, 
                    "order" = acOrder 
                where id_io_category = idCategory and id_io_attribute = idAttr;
            end if;

            return idCategoryAttr;
        end if;
    end loop;


    idCategoryAttr := getNextSeq('attrs_categories', 'id');

    insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only, directives, "order")
    values(idCategoryAttr, idCategory, idAttr, defValue, isMandatory, isReadOnly, acDirectives, acOrder);

    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return idCategoryAttr;

end
$BODY$
language 'plpgsql';

