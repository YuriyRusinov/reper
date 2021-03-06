create or replace function uRemoveFromGroup(varchar, varchar) returns int4 as
$BODY$
declare
    uname alias for $1;
    gname alias for $2;

    query varchar;
begin

    query := 'alter group ' || gname || ' drop user ' || uname;

    EXECUTE query;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function uRemoveFromGroup(int4, varchar) returns int4 as
$BODY$
declare
    userId alias for $1;
    gname alias for $2;

    uname varchar;
    res int4;
begin
    
    select role_name into uname from ONLY kks_roles where id = userId;

    res = uRemoveFromGroup(uname, gname);

    return res;
end
$BODY$
language 'plpgsql';

create or replace function uRemoveFromGroup(varchar, int4) returns int4 as
$BODY$
declare
    uname alias for $1;
    gId alias for $2;

    gname varchar;
    res int4;
begin
    
    select role_name into gname from ONLY kks_roles where id = gId;

    res = uRemoveFromGroup(uname, gname);

    return res;
end
$BODY$
language 'plpgsql';

create or replace function uRemoveFromGroup(int4, int4) returns int4 as
$BODY$
declare
    userId alias for $1;
    gId alias for $2;

    gname varchar;
    uname varchar;
    res int4;
begin
    
    select role_name into gname from ONLY kks_roles where id = gId;
    select role_name into uname from ONLY kks_roles where id = userId;


    res = uRemoveFromGroup(uname, gname);

    return res;
end
$BODY$
language 'plpgsql';

