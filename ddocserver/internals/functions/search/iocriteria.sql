select f_safe_drop_type ('h_get_search_criteria');
create type h_get_search_criteria as (id int4,
                                      attr_id int4,
                                      value varchar,
                                      id_type int4,
                                      id_operation int4,
                                      case_sensitive boolean,
                                      is_not boolean);

create or replace function ioGetCriterion (int4) returns setof h_get_search_criteria as
$BODY$
declare
    idCriterion alias for $1;

    r h_get_search_criteria%rowtype;
begin

    for r in
        select c.id, c.attr_id, c.value, c.id_type, c.id_operation, c.case_sensitive, c.is_not
        from
            criteria c
        where
            c.id = idCriterion
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function ioGetCriterionFromGroup (int4) returns setof h_get_search_criteria as
$BODY$
declare
    idGroup alias for $1;

    r h_get_search_criteria%rowtype;
begin

    for r in
        select 
            c.id, 
            c.attr_id, 
            c.value, 
            c.id_type, 
            c.id_operation, 
            c.case_sensitive, 
            c.is_not 
        from  
            criteria c 
            inner join groups_criteria gc on (c.id=gc.id_criteria and gc.id_group=idGroup)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
