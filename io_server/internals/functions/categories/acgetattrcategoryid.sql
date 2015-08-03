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

create or replace function acGetAttrCategoryIdByRec(int8, int4) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    idAttr alias for $2;

    tableName varchar;
    tName varchar;
    idCategory int4;
    idCatAttr int4;
begin

    SELECT p.relname into tableName
    FROM q_base_table qq, pg_class p
    WHERE qq.id = idRecord and qq.tableoid = p.oid;

    if(substr(tableName, 1, 4) = 'tbl_') then
        tName = substr(tableName, 5);
    else
        tName = tableName;
    end if;

    select id_io_category into idCategory from tbl_io_objects where table_name = tName;
    if(idCategory isnull) then
        select id_io_category into idCategory from tbl_io_objects where table_name = tableName;
        if(idCategory isnull) then
            return -1;
        end if;
    end if;

    select id_child into idCategory from io_categories where id = idCategory;
    if(idCategory isnull) then
        return -1;
    end if;

    select id into idCatAttr from attrs_categories where id_io_attribute = idAttr and id_io_category = idCategory;
    if(idCatAttr isnull) then
        return -1;
    end if;

    return idCatAttr;

end
$BODY$
language 'plpgsql' security definer;

create or replace function acGetAttrCategoryIdByRec2(int8, int4) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    idAttr alias for $2;

    tableName varchar;
    tName varchar;
    idCategory int4;
    idCatAttr int4;
begin

    SELECT p.relname into tableName
    FROM q_base_table qq, pg_class p
    WHERE qq.id = idRecord and qq.tableoid = p.oid;

    if(substr(tableName, 1, 4) = 'tbl_') then
        tName = substr(tableName, 5);
    else
        tName = tableName;
    end if;

    select id_io_category into idCategory from tbl_io_objects where table_name = tName;
    if(idCategory isnull) then
        select id_io_category into idCategory from tbl_io_objects where table_name = tableName;
        if(idCategory isnull) then
            return -1;
        end if;
    end if;

    select id_child2 into idCategory from io_categories where id = idCategory;
    if(idCategory isnull) then
        return -1;
    end if;

    select id into idCatAttr from attrs_categories where id_io_attribute = idAttr and id_io_category = idCategory;
    if(idCatAttr isnull) then
        return -1;
    end if;

    return idCatAttr;

end
$BODY$
language 'plpgsql' security definer;
