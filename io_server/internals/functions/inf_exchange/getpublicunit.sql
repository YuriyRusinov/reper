create or replace function getPublicUnit(int4) returns int4 as
$BODY$
declare
    idOrg alias for $1;
    idUnit int4;
begin

    select id into idUnit from units where id_organization = idOrg and name = 'Публичные должностные лица';

    return idUnit;

end
$BODY$
language 'plpgsql';
