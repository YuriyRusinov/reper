create or replace function ioUpdateInclude(varchar, varchar, varchar, varchar, int4, int4, int4, int4) returns int4 as
$BODY$
declare
    uid alias for $1;
    rName alias for $2;
    rCode alias for $3;
    rDesc alias for $4;
    idParent alias for $5;
    idObject alias for $6;
    idCategory alias for $7;
    idSearchTemplate alias for $8;

    idRubric int4;
begin

    select id into idRubric from rubricator where unique_id = uid;
    if(idRubric is null) then
        idRubric = ioInsertInclude(idParent, idObject, rName, rCode, rDesc, idSearchTemplate, idCategory, uid);
        if(idRubric <= 0) then
           return -1;
        end if;
        return idRubric;
    end if;


    idRubric = ioUpdateIncludeLocal(idRubric, rName, rCode, rDesc, idCategory, idSearchTemplate);

    return idRubric;
end
$BODY$
language 'plpgsql';

create or replace function ioUpdateIncludeLocal(int4, varchar, varchar, varchar, int4, int4) returns int4 as
$BODY$
declare
    idRubric alias for $1;
    rName alias for $2;
    rCode alias for $3;
    rDesc alias for $4;
    idCategory alias for $5;
    idSearchTemplate alias for $6;

begin

    if(idRubric is null or idRubric <= 0) then
        return -1;
    end if;

    update rubricator set
        name  = rName,      
        code = rCode,
        description = rDesc,
        id_io_category = idCategory,
        id_search_template = idSearchTemplate
    where id = idRubric;

    if (not FOUND) then
        return -1;
    end if;

    return idRubric;
end
$BODY$
language 'plpgsql';
