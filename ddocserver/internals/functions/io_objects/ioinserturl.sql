create or replace function ioInsertUrl(int4, int4, varchar) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idUrl alias for $2;
    urlName alias for $3;
begin

    insert into urls_objects (id_io_object, id_url, name) values(idObject, idUrl, urlName);
    if (FOUND = FALSE) then
        return -1;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';
