drop function if exists function aInsertAttrAttr(int4, int4, varchar, boolean, boolean, varchar);
create or replace function aInsertAttrAttr(int4, int4, varchar, boolean, boolean, varchar, varchar, int4) returns int4 as
$BODY$
declare
    idParent alias for $1;
    idAttr alias for $2;
    defValue alias for $3;
    isMandatory alias for $4;
    isReadOnly alias for $5;
    attrAttrUniqueId alias for $6;
    aaDirectives alias for $7;
    aaOrder alias for $8;

    cnt int4;
    idAttrAttr int4;
    idType int4;
begin

    select id into idAttrAttr from attrs_attrs where id_attr_parent = idParent and id_attr_child = idAttr;
    if(idAttrAttr is not null) then
        update attrs_attrs set 
            def_value = defValue, 
            is_mandatory = isMandatory, 
            is_read_only = isReadOnly, 
            "order" = aaOrder, 
            directives = aaDirectives 
        where id_attr_parent = idParent and id_attr_child = idAttr;
        return idAttrAttr;
    end if;

    idAttrAttr := getNextSeq('attrs_attrs', 'id');

    insert into attrs_attrs(id, id_attr_parent, id_attr_child, def_value, is_mandatory, is_read_only, unique_id, "order", directives)
    values(idAttrAttr, idParent, idAttr, defValue, isMandatory, isReadOnly, attrAttrUniqueId, aaOrder, aaDirectives);

    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return idAttrAttr;

end
$BODY$
language 'plpgsql';
