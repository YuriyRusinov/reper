create or replace function acGetAttrCategoryId(int4, int4) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    idAttr alias for $2;

    idCatAttr int4;
begin

    select id into idCatAttr from attrs_categories where id_io_attribute = idAttr and id_io_category = idCategory;

    return idCatAttr;

end
$BODY$
language 'plpgsql';


create or replace function acGetAttrCategoryIdByIO(int4, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idAttr alias for $2;

    idCatAttr int4;
    idCategory int4;
begin

    select id_io_category into idCategory from tbl_io_objects where id = idObject;

    select id into idCatAttr from attrs_categories where id_io_attribute = idAttr and id_io_category = idCategory;

    return idCatAttr;

end
$BODY$
language 'plpgsql' security definer;
