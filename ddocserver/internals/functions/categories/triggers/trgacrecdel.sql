create or replace function aRecDelCheck() returns trigger as
$BODY$
declare
    idCategory bigint;

    idMainCategory bigint;
    idType bigint;
    isCatKind int4;
    cnt bigint;
begin
    return old;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');

select f_create_trigger('trgacrecinsert', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');
