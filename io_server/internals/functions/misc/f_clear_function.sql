create or replace function f_clear_function() returns int4 as
$BODY$
begin
    return 1;
end
$BODY$
language 'plpgsql';
