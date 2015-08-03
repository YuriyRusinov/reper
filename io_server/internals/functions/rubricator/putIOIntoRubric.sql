create or replace function putIOIntoRubric () returns int4 as
$BODY$
declare
    idRubric int4;

    query varchar;
    r record;
    rubr_query varchar;
    rr record;
begin

    query := 'select r.id, r.id_search_template from rubricator r where r.id_search_template is not null';
    for r in
        execute query
    loop
        idRubric := r.id;
        delete from io_rubricator ior where ior.id_rubric = idRubric and is_automated = true;
        rubr_query := generateSearchQuery (13, 'io_objects', r.id_search_template);
        if (rubr_query is not null and length(rubr_query) > 0) then
            for rr in
                execute rubr_query
            loop
                --raise notice '% %', rr.id, idRubric;
                insert into io_rubricator (id_io_object, id_rubric, is_automated) values (rr.id, idRubric, true);
            end loop;
        end if;
    end loop;

    return 1;
end
$BODY$
language 'plpgsql';


create or replace function putIOIntoRubric (int4) returns int4 as
$BODY$
declare
    idRubric alias for $1;

    idSearchTemplate int4;
    rubr_query varchar;
    rr record;
begin

    select r.id_search_template into idSearchTemplate from rubricator r where r.id_search_template is not null and id = idRubric;
    if(idSearchTemplate isnull) then
        return 0;
    end if;

    delete from io_rubricator ior where ior.id_rubric = idRubric and is_automated = true;
    rubr_query := generateSearchQuery (13, 'io_objects', idSearchTemplate);
    if (rubr_query is not null and length(rubr_query) > 0) then
        for rr in
            execute rubr_query
        loop
            insert into io_rubricator (id_io_object, id_rubric, is_automated) values (rr.id, idRubric, true);
        end loop;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';
