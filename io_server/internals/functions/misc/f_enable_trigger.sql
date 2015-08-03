create or replace function f_enable_trigger(varchar, varchar, bool) returns int4 as
$BODY$
declare
    tableName alias for $1;
    triggerName alias for $2;
    bEnable alias for $3;
    q varchar;
begin

    if(bEnable = false) then
        q = 'alter table ' || tableName || ' disable trigger ' || triggerName;
    else
        q = 'alter table ' || tableName || ' enable trigger ' || triggerName;
    end if;

    execute q;

    return 1;
end
$BODY$
language 'plpgsql';


create or replace function f_enable_triggers(varchar, bool, varchar[]) returns int4 as
$BODY$
declare
    tableName alias for $1;
    bEnable alias for $2;
    tExclude alias for $3; --исключить из списка

    r record;
begin

    for r in 
        select 
            t.tgname 
        from 
            pg_trigger t, 
	    pg_class c 
        where 
            t.tgrelid = c.oid
            and c.relname = tableName
            and t.tgisinternal = false
            and case when tExclude isnull then 1=1 else t.tgname <> ANY (tExclude) end
    loop
        perform f_enable_trigger(tableName, r.tgname::varchar, bEnable);
    end loop;

    return 1;
end
$BODY$
language 'plpgsql';


--select f_enable_triggers('attrs_categories', false, ARRAY['trgsetuid']);
