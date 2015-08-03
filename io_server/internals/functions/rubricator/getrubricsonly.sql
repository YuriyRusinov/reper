create or replace function getRubricatorsOnly(int4, boolean) returns setof h_io_get_rubrics as
$BODY$
declare 
    idUser alias for $1;
    bOnlyMyDocs alias for $2;

    rec h_io_get_rubrics%rowtype;
    rr h_io_get_rubrics%rowtype;
begin
    if(bOnlyMyDocs <> TRUE) then 
        for rec in
            select r.id, r.id_parent, r.id_io_object, r.id_search_template, r.id_io_category, r.name, r.code, r.description, 0, r.unique_id, NULL, r.r_icon
            from rubricator r
            where 
                r.id_io_object is null 
                and r.id_parent is null
                and r.id not in (select id_rubricator from user_rubricator where id_user <> idUser)
            order by r.id
        loop
            return next rec;
            for rr in 
                select * from ioGetSubRubricsOnly(rec.id)
            loop
                return next rr;
            end loop;

        end loop;
    else
        for rec in
            select r.id, r.id_parent, r.id_io_object, r.id_search_template, r.id_io_category, r.name, r.code, r.description, 0, r.unique_id, NULL, r.r_icon
            from rubricator r
            where 
                r.id_io_object is null 
                and r.id_parent is null
                and r.id in (select id_rubricator from user_rubricator where id_user = idUser)
            order by r.id
        loop
            return next rec;
            for rr in 
                select * from ioGetSubRubricsOnly(rec.id)
            loop
                return next rr;
            end loop;

        end loop;
    end if;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function ioGetSubRubricsOnly(int4) returns setof h_io_get_rubrics as
$BODY$
declare
    idRubric alias for $1;
    rec h_io_get_rubrics%rowtype;
    rr h_io_get_rubrics%rowtype;
begin

    for rec in 
        select r.id, r.id_parent, NULL, r.id_search_template, r.id_io_category, r.name, r.code, r.description, 1, r.unique_id, NULL, r.r_icon
        from rubricator r
        where r.id_parent = idRubric
        order by r.id
    loop

        return next rec;

        for rr in 
            select * from ioGetSubRubricsOnly(rec.id)
        loop
            return next rr;
        end loop;

    end loop;

    return;    
end
$BODY$
language 'plpgsql';


