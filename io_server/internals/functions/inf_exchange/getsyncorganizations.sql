select f_safe_drop_type('h_sync_orgs');
create type h_sync_orgs as(id int4);

create or replace function getSyncOrganizations(int4, int4) returns setof h_sync_orgs as
$BODY$
declare
    idObject alias for $1;
    eType alias for $2;
    
    r h_sync_orgs%rowtype;
    query varchar;
    tableName varchar;
    theField varchar;
begin

    if(eType = 1) then --objects
        tableName = 'io_objects_organization';
        theField = 'id_io_objects';
    elsif(eType = 2) then --categories
        return;
    else
        tableName = 'io_objects_organization';
        theField = 'id_io_objects';
    end if;

    query = 'select id_organization from ' || tableName || ' where ' || theField || ' = ' || idObject;

    for r in execute query
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
