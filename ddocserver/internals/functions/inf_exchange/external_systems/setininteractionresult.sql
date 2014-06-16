create or replace function setInInteractionResult(int8, int4) returns int4 as
$BODY$
declare
    idQueue alias for $1;
    res alias for $2;
begin

    update in_external_queue set interaction_result = res where id = idQueue;

    return 1;

end
$BODY$
language 'plpgsql';
