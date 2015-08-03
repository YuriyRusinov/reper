create or replace function putRecIntoRubric (int8, int4, VARCHAR) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    idRubric alias for $2;
    rIcon alias for $3;

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
    insert into rubric_records (id_rubric, id_record, r_icon) values (idRubric, idRecord, rIcon);

    return 1;
end
$BODY$
language 'plpgsql';
