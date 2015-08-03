create or replace function ivInsert(int4, int4, int4, boolean, int4, varchar) returns int4 as
$BODY$
declare
    idCategoryAttr alias for $1;
    idTemplate alias for $2;
    idGroup alias for $3;
    isReadOnly alias for $4;
    gOrder alias for $5;
    defValue alias for $6;

    cnt int4;
    idViewAttr int4;
begin

    select id into idViewAttr from io_views where id_attr_category = idCategoryAttr and id_io_template = idTemplate;
    if(idViewAttr is not null) then
        update io_views set def_value = defValue, is_read_only = isReadOnly, "order" = gOrder, id_a_group = idGroup where id_attr_category = idCategoryAttr and id_io_template = idTemplate;
        return idViewAttr;
    end if;

    idViewAttr := getNextSeq('io_views', 'id');

    insert into io_views(id, id_attr_category, id_io_template, def_value, is_read_only, "order", id_a_group)
    values(idViewAttr, idCategoryAttr, idTemplate, defValue, isReadOnly, gOrder, idGroup);

    if(FOUND = FALSE) then
        return -1;
    end if;
    
    return idViewAttr;

end
$BODY$
language 'plpgsql';

