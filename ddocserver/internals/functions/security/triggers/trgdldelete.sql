CREATE OR REPLACE FUNCTION dlDeleteCheck() RETURNS trigger AS
$BODY$
DECLARE

    res int4;
    query varchar;
    idOrg int4;
    r record;
    io_uid varchar;

BEGIN

    --public positions removing send
    if(old.is_public = TRUE and getCurrentUser() <> 2) then --not jupiter
        for r in select id from organization where id not in (select getLocalOrgId()) and is_created = 1
        loop
            select unique_id into io_uid from io_objects where id = 26; --position qualifier
            perform addSyncRecord(r.id, old.id, old.unique_id, io_uid, 'position', 3, 11);
        end loop;
    end if;

    RETURN old;
END;
$BODY$
LANGUAGE 'plpgsql';

select f_safe_drop_trigger('trgdldelete', 'position');

create trigger trgDlDelete
after delete
on "position"
for each row 
execute procedure dlDeleteCheck();
