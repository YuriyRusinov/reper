create or replace function recRemoveFromRubric(int8, int4) returns int4 as
$BODY$
declare
    idRec alias for $1;
    idRubric alias for $2;
begin

    delete from rubric_records where id_record = idRec and id_rubric = idRubric;

    return 1;

end
$BODY$
language 'plpgsql';
