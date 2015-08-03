create or replace function ioRemoveFromRubric(int4, int4) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idRubric alias for $2;
begin

    delete from io_rubricator where id_io_object = idObject and id_rubric = idRubric;

    return 1;

end
$BODY$
language 'plpgsql';
