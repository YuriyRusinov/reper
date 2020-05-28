create or replace function safeCreateUser(name, name, boolean) returns int4 as
$BODY$
declare
    roleName alias for $1;
    sPass alias for $2;
    asRoot alias for $3;

    query varchar;
    rolesCount int4;
begin
    select count(usename) into rolesCount from pg_user where usename = roleName;
    if(rolesCount > 0) then
	return 0;
    end if;
    
    query := 'create user ' || roleName;

    execute query;

    if(asRoot = true) then
        query := 'alter user ' || roleName || ' with createdb createrole ';
    else
	query := 'alter user ' || roleName || ' WITH NOCREATEDB NOCREATEROLE ';
    end if;

    if(sPass is not null) then
        query := query || ' PASSWORD  ' || quote_literal(sPass);
    end if;

    execute query;
        
    return 1;
end
$BODY$
language 'plpgsql' security definer;
