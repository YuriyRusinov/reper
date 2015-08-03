create or replace function getPassportUrlType (varchar) returns int4 as
$BODY$
declare
    mimetype0 alias for $1;

    id_url_ext int4;
    id_url_type int4;
begin
    select into id_url_ext ue.id from url_extentions ue inner join mimetypes mt on (ue.id_mimetype=mt.id and lower (mt.name)=lower (mimetype0));
    if (id_url_ext is null) then
        return 1;
    end if;
    select into id_url_type ut.id from url_types ut where ut.extentions @> ARRAY[id_url_ext] limit 1;
    if (id_url_type is null) then
        return 1;
    else
        return id_url_type;
    end if;
end
$BODY$
language 'plpgsql';
