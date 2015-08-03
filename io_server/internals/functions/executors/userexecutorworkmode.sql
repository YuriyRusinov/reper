create or replace function userExecutorWorkMode(int4, int4, int4) returns int4 as
$BODY$
declare
    idOrganization alias for $1;
    idNewMode alias for $2;
    idOldMode alias for $3;
begin

    return 1;

end
$BODY$
language 'plpgsql';
