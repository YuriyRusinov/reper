select f_safe_drop_type('h_get_search_template');
create type h_get_search_template as (id int4,
                                      id_group int4,
                                      name varchar,
                                      author int4,
                                      id_search_template_type int4);

create or replace function ioGetSearchTemplate () returns setof h_get_search_template as
$BODY$
declare
    r h_get_search_template%rowtype;
begin

    for r in
        select
            st.id,
            st.id_group,
            st.name,
            st.author,
            st.id_search_template_type
        from
            search_templates st order by st.id
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function ioGetSearchTemplate (int4) returns setof h_get_search_template as
$BODY$
declare
    idSearchTemplate alias for $1;
    r h_get_search_template%rowtype;
begin

    for r in
        select
            st.id,
            st.id_group,
            st.name,
            st.author,
            st.id_search_template_type
        from
            search_templates st
        where
            st.id = idSearchTemplate
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
