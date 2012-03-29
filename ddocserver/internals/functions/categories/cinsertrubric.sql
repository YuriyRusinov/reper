create or replace function cInsertRubric(int4, int4, varchar) returns int4 as
$BODY$
declare
    idParent alias for $1;
    idCategory alias for $2;
    rName alias for $3;
    idRubric int4;
begin

    select getNextSeq('categories_rubrics', 'id') into idRubric;
    if(idParent is not null and idCategory is not null) then
        return -1;
    end if;

    if(idParent isnull and idCategory isnull) then
        return -1;
    end if;

    insert into categories_rubrics (id, id_parent, id_category, name) values(idRubric, idParent, idCategory, rName);

   return idRubric;
end
$BODY$
language 'plpgsql';
