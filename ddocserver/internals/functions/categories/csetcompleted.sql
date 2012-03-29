create or replace function cSetCompleted(int4, int4) returns int4 as
$BODY$
declare
    idCat alias for $1;
    oper alias for $2;
begin

    update io_categories set is_completed = oper where id = idCat;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function cSetUpdating(int4) returns int4 as
$BODY$
declare
    idCat alias for $1;
begin

    update io_categories set is_completed = 0 where id = idCat;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';
