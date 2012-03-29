create or replace function ioDeleteIncludes(int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
begin
    
    delete from access_rubric_table where id_rubric in (select id from ioGetRubrics(idObject) where type in (0, 1));

    delete from io_rubricator where id_rubric in (select id from ioGetRubrics(idObject) where type in (0, 1));
    
    delete from rubricator where id in (select id from ioGetRubrics(idObject) where type in (0, 1));

    return 1;

    exception when OTHERS
    then
        return -1;
end
$BODY$
language 'plpgsql' security definer;

create or replace function ioDeleteIncludes(int4, int4[]) returns int4 as
$BODY$
declare
    idObject alias for $1;
    excludeIds alias for $2;
begin
    
    delete 
    from 
        access_rubric_table 
    where 
        id_rubric in (select id from ioGetRubrics(idObject) where type in (0, 1)) 
        and id_rubric not in (select id from rubricator where id = ANY (excludeIds));

    delete 
    from 
        io_rubricator 
    where 
        id_rubric in (select id from ioGetRubrics(idObject) where type in (0, 1)) 
        and id_rubric not in (select id from rubricator where id = ANY (excludeIds));
    
    delete 
    from 
        rubricator 
    where 
        id in (select id from ioGetRubrics(idObject) where type in (0, 1)) 
        and id not in (select id from rubricator where id = ANY (excludeIds));

    return 1;

    exception when OTHERS
    then
        return -1;
end
$BODY$
language 'plpgsql' security definer;
