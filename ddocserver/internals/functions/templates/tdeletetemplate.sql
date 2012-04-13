create or replace function tDeleteTemplate(int4) returns int4 as
$BODY$
declare
    idTemplate alias for $1;
begin

    select tDeleteGroups(idTemplate);
    delete from io_views where id_io_template = idTemplate;
    delete from user_templates where id_template = idTemplate;
    delete from io_templates where id = idTemplate;
    
    return 1;

end
$BODY$
language 'plpgsql';
