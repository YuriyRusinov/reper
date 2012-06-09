create or replace function recUpdateRubric(int4, varchar, varchar, int4, int4, int4, int4) returns int4 as
$BODY$
declare
    id alias for $1;
    rName alias for $2;
    rDesc alias for $3;
    idParent alias for $4;
    idRecord alias for $5;

    idRubric int4;
begin

    select id into idRubric from record_rubricator where id1 = id;
    if(idRubric is null) then
        idRubric := recInsertRubric(idParent, idRecord, rName, rDesc);
        if(idRubric <= 0) then
           return -1;
        end if;
        return idRubric;
    end if;


    idRubric := recUpdateRubricLocal(idRubric, rName, rDesc);

    return idRubric;
end
$BODY$
language 'plpgsql';

create or replace function recUpdateRubricLocal(int4, varchar, varchar) returns int4 as
$BODY$
declare
    idRubric alias for $1;
    rName alias for $2;
    rDesc alias for $3;

begin

    if(idRubric is null or idRubric <= 0) then
        return -1;
    end if;

    update record_rubricator set
        name = rName,      
        description = rDesc
    where id = idRubric;

    if (not FOUND) then
        return -1;
    end if;

    return idRubric;
end
$BODY$
language 'plpgsql';