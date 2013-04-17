--create rubric both for global rubricator or io rubricator
create or replace function ioInsertInclude(int4, int4, varchar, varchar, varchar, int4, int4, varchar, varchar) returns int4 as
$BODY$
declare
    idParent alias for $1;
    idObject alias for $2;
    rName alias for $3;
    rCode alias for $4;
    rDesc alias for $5;
    idSearchTemplate alias for $6;
    idIOCategory alias for $7;
    uid alias for $8;
    rIcon alias for $9;

    idRubric int4;
    cnt int4;
begin

    select getNextSeq('rubricator', 'id') into idRubric;

    if(idParent is not null and idObject is not null) then

        return -1;
    end if;

    --if both parameters is null then we must verify that MyDocs rubricator is not created for the current user
    --only MyDoc rubricator can have both this parameters as NULL or PublicRubricator

    if(idParent isnull and idObject isnull) then
        select count(*) into cnt from user_rubricator where id_user = getCurrentUser();
        --raise notice '%', cnt;
        if(cnt > 0 and rName = 'Мои документы') then
            return -1;
        end if;
        
    end if;

    insert into rubricator (id, id_parent, id_io_object, name, code, description, id_search_template, id_io_category, unique_id, r_icon) 
    values (idRubric, idParent, idObject, rName, rCode, rDesc, idSearchTemplate, idIOCategory, uid, rIcon);
    if (not FOUND) then
        return -1;
    end if;

    return idRubric;
end
$BODY$
language 'plpgsql';
