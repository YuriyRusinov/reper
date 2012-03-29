create or replace function getRubricators(int4, boolean) returns setof h_io_get_rubrics as
$BODY$
declare 
    idUser alias for $1;
    bOnlyMyDocs alias for $2;

    rec h_io_get_rubrics%rowtype;
    rr h_io_get_rubrics%rowtype;
begin

    if(bOnlyMyDocs <> TRUE) then 
        for rec in
            select r.id, r.id_parent, r.id_io_object, r.id_search_template, r.id_io_category, r.name, r.code, r.description, 0, r.unique_id
            from rubricator r
            where 
                r.id_io_object is null 
                and r.id_parent is null
                and r.id not in (select id_rubricator from user_rubricator where id_user <> idUser)
            order by r.id
        loop
            return next rec;
            for rr in 
                select * from ioGetSubRubrics(rec.id)
            loop
                return next rr;
            end loop;

            for rr in 
                select * from ioGetRubricItems(rec.id)
            loop
                return next rr;
            end loop;
        end loop;
    else
        for rec in
            select r.id, r.id_parent, r.id_io_object, r.id_search_template, r.id_io_category, r.name, r.code, r.description, 0, r.unique_id
            from rubricator r
            where 
                r.id_io_object is null 
                and r.id_parent is null
                and r.id in (select id_rubricator from user_rubricator where id_user = idUser)
            order by r.id
        loop
            return next rec;
            for rr in 
                select * from ioGetSubRubrics(rec.id)
            loop
                return next rr;
            end loop;

            for rr in 
                select * from ioGetRubricItems(rec.id)
            loop
                return next rr;
            end loop;
        end loop;
    end if;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function getRubric(int4) returns setof h_io_get_rubrics as
$BODY$
declare 
    idRubr alias for $1;

    rec h_io_get_rubrics%rowtype;
    rr h_io_get_rubrics%rowtype;
begin

    for rec in
        select r.id, r.id_parent, r.id_io_object, r.id_search_template, r.id_io_category, r.name, r.code, r.description, 0, r.unique_id
        from rubricator r
        where 
            r.id = idRubr
        order by r.id
    loop
        return next rec;
        for rr in 
            select * from ioGetSubRubrics(rec.id)
        loop
            return next rr;
        end loop;

        for rr in 
            select * from ioGetRubricItems(rec.id)
        loop
            return next rr;
        end loop;
    end loop;
end
$BODY$
language 'plpgsql';
