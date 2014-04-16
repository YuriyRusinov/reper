create or replace function recUpdateRubric(int8, varchar, varchar, int4, int4, int4, int8, int4, int4, varchar, varchar) returns int8 as
$BODY$
declare
    rId alias for $1;
    rName alias for $2;
    rDesc alias for $3;
    idParent alias for $4;
    idRecord alias for $5;
    idObject alias for $6;
    idCategory alias for $7;
    rIcon alias for $8;
    rUid alias for $9;

    idRubric int8;
begin

    select id into idRubric from record_rubricator where id = rId;
    if(idRubric is null) then
        idRubric := recInsertRubric(idParent, idRecord, idObject, idCategory, rName, rDesc, rIcon, rUid);
        if(idRubric <= 0) then
           return -1;
        end if;
        return idRubric;
    end if;


    idRubric := recUpdateRubricLocal(idRubric, idObject, idCategory, rName, rDesc, rIcon, rUid);

    return idRubric;
end
$BODY$
language 'plpgsql';

create or replace function recUpdateRubricEx(varchar, varchar, varchar, int8, int8, int4, int4, varchar) returns int8 as
$BODY$
declare
    rUid alias for $1;
    rName alias for $2;
    rDesc alias for $3;
    idParent alias for $4;
    idRecord alias for $5;
    idObject alias for $6;
    idCategory alias for $7;
    rIcon alias for $8;

    idRubric int4;
begin

    select id into idRubric from record_rubricator where unique_id = rUid;
    if(idRubric is null) then
        idRubric = recInsertRubric(idParent, idRecord, idObject, idCategory, rName, rDesc, rIcon, rUid);
        if(idRubric <= 0) then
           return -1;
        end if;
        return idRubric;
    end if;

    idRubric := recUpdateRubricLocal(idRubric, idObject, idCategory, rName, rDesc, rIcon, rUid);

    return idRubric;
end
$BODY$
language 'plpgsql';

create or replace function recUpdateRubricLocal(int8, int4, int4, varchar, varchar, varchar, varchar) returns int8 as
$BODY$
declare
    idRubric alias for $1;
    idObject alias for $2;
    idCategory alias for $3;
    rName alias for $4;
    rDesc alias for $5;
    rIcon alias for $6;
    rUid alias for $7;

begin

    if(idRubric is null or idRubric <= 0) then
        return -1;
    end if;

    update record_rubricator set
        id_io_object = idObject,
        id_io_category = idCategory,
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
