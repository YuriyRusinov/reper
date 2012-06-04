create or replace function putRecIntoRubric (int4, int4) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    idRubric alias for $2;

    query varchar;
    cnt int4;
    r record;
    rubr_query varchar;
    rr record;
begin

    select count(*) into cnt from rubric_records where id_record=idRecord and id_rubric=idRubric;
    if (cnt is not null and cnt > 0) then
        return 1;
    end if;
    insert into rubric_records (id_rubric, id_record) values (idRubric, idRecord);

    return 1;
end
$BODY$
language 'plpgsql';
