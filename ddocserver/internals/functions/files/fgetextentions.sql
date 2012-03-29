select f_safe_drop_type('h_f_get_extentions');
create type h_f_get_extentions as(id int4, 
                                  name varchar);

create or replace function fGetExtentions(int4) returns setof h_f_get_extentions as
$BODY$
declare
    r h_f_get_extentions%rowtype;
    idFileType alias for $1;
    theExtentions int4[];
begin

    select extentions into theExtentions from url_types where id = idFileType;

    for r in 
        select id, name from url_extentions where id = ANY(theExtentions)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
