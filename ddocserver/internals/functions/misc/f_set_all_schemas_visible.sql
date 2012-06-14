create or replace function f_set_all_schemas_visible() returns int4 as
$BODY$
declare
    r record;
    q varchar;
begin

    q = 'set search_path to public';
    for r in 
        select nspname from pg_namespace where nspname not like ('pg_%') and nspname <> 'information_schema' and nspname <> 'public'
    loop
        q = q || ', ' || r.nspname;
    end loop;

    execute q;
    raise warning '%', q;

    return 1;
end
$BODY$
language 'plpgsql';
