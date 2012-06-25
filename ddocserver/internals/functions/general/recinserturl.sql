create or replace function recInsertUrl(int8, int4, varchar) returns int4 as
$BODY$
declare
    idRecord alias for $1;
    idUrl alias for $2;
    urlName alias for $3;
begin

    insert into urls_records (id_record, id_url, name) values(idRecord, idUrl, urlName);
    if (FOUND = FALSE) then
        return -1;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';
