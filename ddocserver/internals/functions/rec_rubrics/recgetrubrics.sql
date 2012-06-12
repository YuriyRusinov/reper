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
            0
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
        order by r.id1
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

    for r in
        select
            rr.id_record,
            NULL,
            rr.id_rubric,
            NULL,
            NULL,
            2
        from
            rubric_records rr inner join
            record_rubricator rec
            on (rr.id_rubric = rec.id1 and rec.id1=idRubric)
        order by 1
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql' security definer;
