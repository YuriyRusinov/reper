create or replace function f_is_table_has_parent(varchar, varchar) returns bool as
$BODY$
declare
    tChild alias for $1;
    tParent alias for $2;

    r record;
    ok bool;
begin

    for r in 
        select c2.relname
        from 
            pg_inherits h,
            pg_class c1,
            pg_class c2
        where 
            c1.relname = tChild
            and c1.oid = h.inhrelid
            and h.inhparent = c2.oid
    loop
        if(r.relname = tParent) then
            return true;
        end if;

        ok := f_is_table_has_parent(r.relname::varchar, tParent);
        if(ok = true) then
            return true;
        end if;
    end loop;

    return false;

end
$BODY$
language 'plpgsql';
