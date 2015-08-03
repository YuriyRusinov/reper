create or replace function getCurrentDl() returns int4 as
$BODY$
declare
   idDl int4;
begin
    select id_dl into idDl from curr_dl;

    return idDl;
end
$BODY$
language 'plpgsql';
