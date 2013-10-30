create or replace function recDelRubric(int4) returns int4 as 
$BODY$
declare
    idRubric alias for $1;
begin

--    if(rGetPrivilege(getCurrentUser(), idRubric, 3/*delete*/, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;


--    delete from access_rubric_table where id_rubric in (select id from ioGetSubRubrics(idRubric) where type in(0,1));

    delete from rubric_records where id_rubric in (select id from recGetSubRubrics(idRubric) where type in (0, 1));

--    delete from user_rubricator where id_rubricator in (select id from ioGetSubRubrics(idRubric) where type in (0, 1));

    delete from record_rubricator where id in (select id from recGetSubRubrics(idRubric) where type in (0, 1));

----

--    delete from access_rubric_table where id_rubric = idRubric;

--    delete from io_rubricator where id_rubric = idRubric;

--    delete from user_rubricator where id_rubricator = idRubric;

--    delete from rubricator where id = idRubric;

    return 1;
end
$BODY$
language 'plpgsql' security definer;

create or replace function recDeleteRubrics(int8) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    cnt int4;
    r record;
    i int4;
    res int4;
begin

    select count(*) into cnt from rubric_records rr where rr.id_record=idRecord;

    if(cnt isnull or cnt = 0) then
        return 1;
    end if;

    
    for r in
        select rec.id from record_rubricator rec inner join rubric_records rr on (rec.id = rr.id_rubric and rr.id_record=idRecord)
    loop
        i := r.id;
        select into res recDelRubric (i);
        if(res <> 1) then
            return res;
        end if;
    end loop;

    return res;

end
$BODY$
language 'plpgsql' security definer;

create or replace function recDeleteIncludes(int8, int8[]) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    excludeIds alias for $2;
begin
    
    delete 
    from 
        rubric_records 
    where 
        id_rubric in (select id from recGetRubrics(idRecord) where type in (0, 1)) 
        and id_rubric not in (select id from record_rubricator where id = ANY (excludeIds));
    
    delete 
    from 
        record_rubricator 
    where 
        id in (select id from recGetRubrics(idRecord) where type in (0, 1)) 
        and id not in (select id from record_rubricator where id = ANY (excludeIds));

    return 1;

    --exception when OTHERS
    --then
    --    return -1;
end
$BODY$
language 'plpgsql' security definer;
