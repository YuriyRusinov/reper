create or replace function getCurrentProcessingVariant() returns int4 as
$BODY$
declare
    vValueStr varchar;
begin
    select value into vValueStr from tbl_attrs_values where id_io_object = 107 and id_attr_category = 635;
    if(vValueStr isnull) then
        return 1;
    end if;

    return vValueStr::int4;
end
$BODY$
language 'plpgsql' security definer;
