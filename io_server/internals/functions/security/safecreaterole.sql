create or replace function safeCreateRole(name, name, boolean, boolean, boolean) returns int4 as
$BODY$
declare
    roleName alias for $1;
    sPass alias for $2;
    needInherit alias for $3;
    isUser alias for $4;
    asRoot alias for $5;

    query varchar;
    rolesCount int4;
begin
    select count(rolname) into rolesCount from pg_user where usename = roleName;
    if(rolesCount > 0) then
	return 0;
    end if;
    
    query := 'create user ' || roleName;

--    if(needInherit = false) then
--	query := query || ' noinherit ';
--    else
--	query := query || ' inherit ';
--    end if;
--    if(isUser = true) then
--	query := query || ' login';
--    else
--	query := query || ' nologin';
--    end if;
    
    execute query;

    if(asRoot = true) then
        query := 'alter user ' || roleName || ' with createdb createuser ';
    else
	query := 'alter role ' || roleName || ' WITH NOCREATEDB NOCREATEUSER ';
    end if;

    if(sPass is not null) then
        query := query || ' PASSWORD  ' || quote_literal(sPass);
    end if;

    execute query;
        
    return 1;
end
$BODY$
language 'plpgsql' security definer;
