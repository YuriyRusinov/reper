create or replace function getCurrentProcessingScenario() returns int4 as
$BODY$
declare
    scValueStr varchar;
begin
    select value into scValueStr from tbl_attrs_values where id_io_object = 107 and id_attr_category = 634;
    if(scValueStr isnull) then
        return 1;
    end if;

    return scValueStr::int4;
end
$BODY$
language 'plpgsql' security definer;
