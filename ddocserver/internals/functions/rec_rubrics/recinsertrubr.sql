create or replace function recInsertRubric(int4, int8, varchar, varchar) returns int4 as
$BODY$
declare
    idParent alias for $1;
    idRecord alias for $2;
    rName alias for $3;
    rDesc alias for $4;

    idRubric int4;
    cnt int4;
    query varchar;
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

    select count(*) into cnt from q_base_table where id=idRecord;

    raise warning 'cnt is %', cnt;

    query := E'insert into record_rubricator (id, id_parent, id_record, name, description) values (';
    query := query || idRubric || E',';
    if (idParent is null) then
        query := query || E'NULL,';
    else
        query := query || idParent || E',';
    end if;
    if (idRecord is null) then
        query := query || E'NULL,';
    else
        query := query || idRecord || E',';
    end if;
    query := query || quote_literal (rName) || E',';
    if (rDesc is null) then
        query := query || E'NULL)';
    else
        query := query || quote_literal (rDesc) || E')';
    end if;
    raise warning '%', query;
    execute query;
    if (not FOUND) then
        return -1;
    end if;

    return idRubric;
end
$BODY$
language 'plpgsql';
