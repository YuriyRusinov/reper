create or replace function cGetForbiddenTypes () returns int4[] as
$BODY$
declare
    fTypes int4[];
begin
    fTypes := ARRAY[8, 9, 10];
    return fTypes;
end
$BODY$
language 'plpgsql';
