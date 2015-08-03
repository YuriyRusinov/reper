select f_safe_drop_type('h_get_search_group');
create type h_get_search_group as (id int4,
                                   id_parent int4,
                                   operation int4,
                                   is_not boolean,
                                   type int4);

create or replace function getGroups(int4, bool) returns setof h_get_search_group as
$BODY$
declare
    idGroup alias for $1;
    allTree alias for $2;

    r h_get_search_group%rowtype;
    rr h_get_search_group%rowtype;
    bAllTree bool;
begin

    bAllTree = FALSE;
    if(allTree is not null) then
        bAllTree = allTree;
    end if;

    for r in
        select id, id_parent, operation, is_not, 0
        from
            groups g
        where
            g.id = idGroup
    loop
        return next r;
        for rr in
            select * from getChildGroups (r.id, bAllTree)
        loop
            return next rr;
        end loop;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function getChildGroups (int4, bool) returns setof h_get_search_group as
$BODY$
declare
    idParentGroup alias for $1;
    allTree alias for $2;

    r h_get_search_group%rowtype;
    rr h_get_search_group%rowtype;
    bAllTree bool;
begin

    bAllTree = FALSE;
    if(allTree is not null) then
        bAllTree = allTree;
    end if;

    for r in
        select id, id_parent, operation, is_not, 1
        from
            groups g
        where
            g.id_parent = idParentGroup
        order by id
    loop
        return next r;
        if(bAllTree = TRUE) then
            for rr in
                select * from getChildGroups (r.id, bAllTree)
            loop
                return next rr;
            end loop;
        end if;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
