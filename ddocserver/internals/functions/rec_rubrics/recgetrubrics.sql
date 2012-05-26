select f_safe_drop_type('h_get_rec_rubrics');
create type h_get_rec_rubrics as (id int4, 
                                  id_parent int4, 
                                  id_record int4, 
                                  name varchar, 
                                  description varchar, 
                                  type int4);

create or replace function recGetRubrics(int4) returns setof h_get_rec_rubrics as
$BODY$
declare 
    idRecord alias for $1;
    rec h_get_rec_rubrics%rowtype;
    rr h_get_rec_rubrics%rowtype;
begin

    for rec in
        select 
            r.id1, 
            r.id_parent, 
            r.id_record, 
            r.name, 
            r.description, 
            0,
        from record_rubricator r
        where r.id_record = idRecord
        order by r.id1
    loop
        return next rec;
        for rr in 
            select * from recGetSubRubrics(rec.id)
        loop
            return next rr;
        end loop;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function recGetSubRubrics(int4) returns setof h_get_rec_rubrics as
$BODY$
declare
    idRubric alias for $1;
    rec h_get_rec_rubrics%rowtype;
    rr h_get_rec_rubrics%rowtype;
begin

    for rec in 
        select r.id1, r.id_parent, NULL, r.name, r.description, 1
        from record_rubricator r
        where r.id_parent = idRubric
        order by r.id
    loop

        return next rec;

        for rr in 
            select * from recGetSubRubrics(rec.id)
        loop
            return next rr;
        end loop;

        for rr in 
            select * from recGetRubricItems(rec.id)
        loop
            return next rr;
        end loop;
        
    end loop;

    return;    
end
$BODY$
language 'plpgsql';

create or replace function recGetRubricItems(int4) returns setof h_get_rec_rubrics as
$BODY$
declare
    idRubric alias for $1;
    r h_get_rec_rubrics%rowtype;
begin
/*
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
        from rubric_records ior 
             inner join tbl_io_objects o on (ior.id_rubric = idRubric and ior.id_io_object = o.id)
             inner join rubricator rubr on (ior.id_rubric = rubr.id)
        where 
             true = getPrivilege(getCurrentUser(), ior.id_io_object, 1, true) 
        order by 1
    loop
        return next r;
    end loop;
*/    
    return;
end
$BODY$
language 'plpgsql' security definer;
