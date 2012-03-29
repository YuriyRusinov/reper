create or replace function cSetGlobal(int4) returns int4 as
$BODY$
declare
    idCat alias for $1;
begin

    update io_categories set is_global = TRUE where id = idCat;
    if(FOUND = FALSE) then
        return 0;
    end if;

    return 1;

end
$BODY$ 
language 'plpgsql';
