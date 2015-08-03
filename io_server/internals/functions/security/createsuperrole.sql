create or replace function createSuperRole(varchar, varchar) returns void as
$BODY$
declare
    sUser alias for $1;
    sPass alias for $2;
    query varchar;
begin

    perform safeCreateUser(sUser, NULL, true);

    query := 'alter USER ' || sUser || ' with createdb createuser password ' || quote_literal(sPass);
    execute query;

    perform addGroup('admins', 'this group contains superusers. Do not use this group for any nonadministrative operations', true);
    perform addUser(sUser, 'admins', 'This user can only be used for administrative operations. Do not use that for any another.', NULL, true);

    return;

end
$BODY$
language 'plpgsql' security definer;
