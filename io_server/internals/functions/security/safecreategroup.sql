create or replace function safeCreateGroup(name) returns int4 as
$BODY$
declare
    roleName alias for $1;

    query varchar;
    rolesCount int4;
begin
    select count(groname) into rolesCount from pg_group where groname = roleName;
    if(rolesCount > 0) then
	return 0;
    end if;
    
    query := 'create group ' || roleName;

    execute query;

    return 1;
end
$BODY$
language 'plpgsql' security definer;
