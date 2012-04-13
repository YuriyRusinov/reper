create or replace function tDeleteGroups(int4) returns int4 as
$BODY$
declare
    idTemplate alias for $1;
begin
    
    delete from a_groups where id in (select id from tGetAttrGroups(idTemplate) where entity_type = 0);

    return 1;
end
$BODY$
language 'plpgsql';
