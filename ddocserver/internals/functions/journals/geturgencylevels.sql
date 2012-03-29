select f_safe_drop_type('h_j_get_urgency_level');
create type h_j_get_urgency_level as (id int4, 
                                      name varchar,
                                      code varchar);

create or replace function geturgencylevels () returns setof h_j_get_urgency_level as
$BODY$
declare
    r h_j_get_urgency_level%rowtype;
begin

    for r in
        select id,name,code from urgency_levels
    loop
        return next r;
    end loop;
    return;
end
$BODY$
language 'plpgsql';
