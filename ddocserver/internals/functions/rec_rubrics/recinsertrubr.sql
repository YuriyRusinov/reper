create or replace function recInsertRubric(int8, int8, int4, int4, varchar, varchar, varchar, varchar) returns int8 as
$BODY$
declare
    idParent alias for $1;
    idRecord alias for $2;
    idObject alias for $3;
    idCategory alias for $4;
    rName alias for $5;
    rDesc alias for $6;
    rIcon alias for $7;
    rUid alias for $8;

    idRubric int8;
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

    query := E'insert into record_rubricator (id, id_parent, id_record, id_io_object, id_io_category, name, description, r_icon, unique_id) values (';
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

    if (idObject is null) then
        query := query || E'NULL,';
    else
        query := query || idObject || E',';
    end if;

    if (idCategory is null) then
        query := query || E'NULL,';
    else
        query := query || idCategory || E',';
    end if;

    query := query || quote_literal (rName) || E',';

    if (rDesc is null) then
        query := query || E'NULL,';
    else
        query := query || quote_literal (rDesc) || E',';
    end if;

    if (rIcon is null) then
        query := query || E'NULL,';
    else
        query := query || quote_literal (rIcon) || E',';
    end if;

    if (rUid is null) then
        query := query || E'NULL)';
    else
        query := query || quote_literal (rUid) || E')';
    end if;

    execute query;
    if (not FOUND) then
        return -1;
    end if;

    return idRubric;
end
$BODY$
language 'plpgsql';
