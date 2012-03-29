create or replace function checkForExist(varchar) returns int4 as
$BODY$
declare
    uid alias for $1;

    idObject int4;
begin

    select id into idObject from f_sel_io_objects(uid) where unique_id = uid;
    if(idObject isnull) then
        return -1;
    end if;

    return idObject;

end
$BODY$
language 'plpgsql';
