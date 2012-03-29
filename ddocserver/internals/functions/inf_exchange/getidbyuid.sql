create or replace function getIDByUID(varchar, varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    uid alias for $2;

    id int4;
    q varchar;
    r record;
    uidColumn varchar;
begin
    if(tableName isnull) then
        return NULL;
    end if;

    if(tableName = 'organization') then
        uidColumn = 'email_prefix';
    else 
        uidColumn = 'unique_id';
    end if;
    
    q = 'select id from ' || tableName || ' where ' || uidColumn || ' = ' || quote_literal(uid);
    for r in execute q
    loop
        return r.id;
    end loop;

    return NULL;

end
$BODY$
language 'plpgsql';
