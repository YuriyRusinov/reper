create or replace function isIOSynced (bigint) returns boolean as
$BODY$
declare
    id_cat alias for $1;

    cnt bigint;
    res boolean;
begin

    select count(*) into cnt from io_objects io inner join io_objects_organization ioo on (io.id=ioo.id_io_objects and io.id_owner_org!=ioo.id_organization and io.id_io_category=id_cat);

    res := cnt > 0;
    return res;
end
$BODY$
language 'plpgsql'; 
