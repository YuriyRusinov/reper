create or replace function recInsertRubric(int4, int4, varchar, varchar) returns int4 as
$BODY$
declare
    idParent alias for $1;
    idRecord alias for $2;
    rName alias for $3;
    rDesc alias for $4;

    idRubric int4;
    cnt int4;
begin

    select getNextSeq('record_rubricator', 'id') into idRubric;

    if(idParent is not null and idRecord is not null) then

        return -1;
    end if;

    --if both parameters is null then we must verify that MyDocs rubricator is not created for the current user
    --only MyDoc rubricator can have both this parameters as NULL or PublicRubricator

    if(idParent isnull and idRecord isnull) then
        return -1;
    end if;

    insert into record_rubricator (id1, id_parent, id_record, name, description) 
    values (idRubric, idParent, idRecord, rName, rDesc);
    if (not FOUND) then
        return -1;
    end if;

    return idRubric;
end
$BODY$
language 'plpgsql';
