create or replace function deleteRubric(int4) returns int4 as 
$BODY$
declare
    idRubric alias for $1;
begin

    if(rGetPrivilege(getCurrentUser(), idRubric, 3/*delete*/, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;


    delete from access_rubric_table where id_rubric in (select id from ioGetSubRubrics(idRubric) where type in(0,1));

    delete from io_rubricator where id_rubric in (select id from ioGetSubRubrics(idRubric) where type in (0, 1));

    delete from user_rubricator where id_rubricator in (select id from ioGetSubRubrics(idRubric) where type in (0, 1));

    delete from rubricator where id in (select id from ioGetSubRubrics(idRubric) where type in (0, 1));

----

    delete from access_rubric_table where id_rubric = idRubric;

    delete from io_rubricator where id_rubric = idRubric;

    delete from user_rubricator where id_rubricator = idRubric;

    delete from rubricator where id = idRubric;

    return 1;
end
$BODY$
language 'plpgsql' security definer;

create or replace function deleteRubrics(int4[]) returns int4 as
$BODY$
declare
    ids alias for $1;
    cnt int4;
    i int4;
    res int4;
begin

    cnt := array_upper(ids, 1);

    if(cnt isnull or cnt = 0) then
        return 1;
    end if;

    
    for i in 1..cnt
    loop
        select deleteRubric(ids[i]) into res;
        if(res <> 1) then
            return res;
        end if;
    end loop;

    return res;

end
$BODY$
language 'plpgsql' security definer;
