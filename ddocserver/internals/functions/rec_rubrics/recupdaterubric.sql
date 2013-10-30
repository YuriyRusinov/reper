create or replace function recUpdateRubric(int8, varchar, varchar, int4, int4, int4, int8, varchar, varchar) returns int8 as
$BODY$
declare
    rId alias for $1;
    rName alias for $2;
    rDesc alias for $3;
    idParent alias for $4;
    idRecord alias for $5;
    rIcon alias for $6;
    rUid alias for $7;

    idRubric int8;
begin

    select id into idRubric from record_rubricator where id = rId;
    if(idRubric is null) then
        idRubric := recInsertRubric(idParent, idRecord, rName, rDesc, rIcon, rUid);
        if(idRubric <= 0) then
           return -1;
        end if;
        return idRubric;
    end if;


    idRubric := recUpdateRubricLocal(idRubric, rName, rDesc, rIcon, rUid);

    return idRubric;
end
$BODY$
language 'plpgsql';

create or replace function recUpdateRubricEx(varchar, varchar, varchar, int8, int8, varchar) returns int8 as
$BODY$
declare
    rUid alias for $1;
    rName alias for $2;
    rDesc alias for $3;
    idParent alias for $4;
    idRecord alias for $5;
    rIcon alias for $6;

    idRubric int4;
begin

    select id into idRubric from record_rubricator where unique_id = rUid;
    if(idRubric is null) then
        idRubric = recInsertRubric(idParent, idRecord, rName, rDesc, rIcon, rUid);
        if(idRubric <= 0) then
           return -1;
        end if;
        return idRubric;
    end if;

    idRubric := recUpdateRubricLocal(idRubric, rName, rDesc, rIcon, rUid);

    return idRubric;
end
$BODY$
language 'plpgsql';

create or replace function recUpdateRubricLocal(int8, varchar, varchar, varchar, varchar) returns int8 as
$BODY$
declare
    idRubric alias for $1;
    rName alias for $2;
    rDesc alias for $3;
    rIcon alias for $4;
    rUid alias for $5;

begin

    if(idRubric is null or idRubric <= 0) then
        return -1;
    end if;

    update record_rubricator set
        name = rName,      
        description = rDesc,
        r_icon = rIcon
    where id = idRubric;

    if (not FOUND) then
        return -1;
    end if;

    return idRubric;
end
$BODY$
language 'plpgsql';
