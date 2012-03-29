select f_safe_drop_type('h_c_get_rubrics');
create type h_c_get_rubrics as(id int4, 
                               id_parent int4, 
                               id_category int4, 
                               name varchar, 
                               type int4); 

create or replace function cGetRubrics(int4) returns setof h_c_get_rubrics as
$BODY$
declare 
    idCategory alias for $1;
    rec h_c_get_rubrics%rowtype;
    rr h_c_get_rubrics%rowtype;
begin

    for rec in
        select r.id, r.id_parent, r.id_category, r.name, 0
        from categories_rubrics r
        where r.id_category = idCategory
    loop
        return next rec;
        for rr in 
            select * from cGetSubRubrics(rec.id)
        loop
            return next rr;
        end loop;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function cGetSubRubrics(int4) returns setof h_c_get_rubrics as
$BODY$
declare
    idRubric alias for $1;
    rec h_c_get_rubrics%rowtype;
    rr h_c_get_rubrics%rowtype;
begin

    for rec in 
        select r.id, r.id_parent, NULL, r.name, 1
        from categories_rubrics r
        where r.id_parent = idRubric
        order by 1
    loop

        return next rec;

        for rr in 
            select * from cGetSubRubrics(rec.id)
        loop
            return next rr;
        end loop;

    end loop;

    return;    
end
$BODY$
language 'plpgsql';
