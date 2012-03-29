select f_safe_drop_type('h_io_get_rubrics');
create type h_io_get_rubrics as(id int4, 
                                id_parent int4, 
                                id_io_object int4, 
                                id_search_template int4,
                                id_io_category int4,
                                name varchar, 
                                code varchar, 
                                description varchar, 
                                type int4,
                                unique_id varchar,
                                is_automated bool); 

create or replace function ioGetRubrics(int4) returns setof h_io_get_rubrics as
$BODY$
declare 
    idObject alias for $1;
    rec h_io_get_rubrics%rowtype;
    rr h_io_get_rubrics%rowtype;
begin

    for rec in
        select 
            r.id, 
            r.id_parent, 
            r.id_io_object, 
            r.id_search_template, 
            r.id_io_category, 
            r.name, 
            r.code, 
            r.description, 
            0,
            r.unique_id,
            NULL
        from rubricator r
        where r.id_io_object = idObject
        order by r.id
    loop
        return next rec;
        for rr in 
            select * from ioGetSubRubrics(rec.id)
        loop
            return next rr;
        end loop;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function ioGetSubRubrics(int4) returns setof h_io_get_rubrics as
$BODY$
declare
    idRubric alias for $1;
    rec h_io_get_rubrics%rowtype;
    rr h_io_get_rubrics%rowtype;
begin

    for rec in 
        select r.id, r.id_parent, NULL, r.id_search_template, r.id_io_category, r.name, r.code, r.description, 1, r.unique_id
        from rubricator r
        where r.id_parent = idRubric
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

    return;    
end
$BODY$
language 'plpgsql';

create or replace function ioGetRubricItems(int4) returns setof h_io_get_rubrics as
$BODY$
declare
    idRubric alias for $1;
    r h_io_get_rubrics%rowtype;
begin

    for r in 
        select 
            ior.id_io_object, 
            ior.id_rubric, 
            NULL, 
            rubr.id_search_template, 
            rubr.id_io_category, 
            o.name, 
            NULL, --o.code, 
            o.description, 
            2, 
            rubr.unique_id,
            ior.is_automated
        from io_rubricator ior 
             inner join tbl_io_objects o on (ior.id_rubric = idRubric and ior.id_io_object = o.id)
             inner join rubricator rubr on (ior.id_rubric = rubr.id)
        where 
             true = getPrivilege(getCurrentUser(), ior.id_io_object, 1, true) 
        order by 1
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql' security definer;
