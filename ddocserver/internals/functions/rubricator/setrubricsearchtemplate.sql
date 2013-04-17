create or replace function setRubricSearchTemplate(int4, int4) returns int4 as
$BODY$
declare
    idRubric alias for $1;
    idSearchTemplate alias for $2;

begin

    if(idRubric is null or idRubric <= 0) then
        return -1;
    end if;

    update rubricator set
        id_search_template = idSearchTemplate
    where id = idRubric;

    if (not FOUND) then
        return -1;
    end if;

    return idRubric;
end
$BODY$
language 'plpgsql';
