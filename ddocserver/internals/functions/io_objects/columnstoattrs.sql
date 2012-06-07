select f_safe_drop_type('h_columns_desc');

create type h_columns_desc as(nspname varchar, relname varchar, rel_desc varchar, attname varchar, attnum int4, att_type varchar, description varchar, relfname varchar, con_attfnum int4, attfname varchar, att_typlen int4, att_len int4);

create or replace function translateColumnsToAttrs(varchar[]) returns setof h_columns_desc as
$BODY$
declare
    schemaNames alias for $1;

    schemas varchar[];
    r h_columns_desc%rowtype;
begin

    if(schemaNames isnull) then
        schemas[1] = 'public';
    else
        schemas = schemaNames;
    end if;

    for r in 
        select
            attrs.nspname,
            attrs.relname,
            attrs.rel_desc, 
            attrs.attname,
            attrs.attnum,
            attrs.att_type,
            attrs.description,
            cons.relfname,
            cons.con_attfnum,
            a.attname,
            attrs.att_len,
            attrs.atttypmod
        from 
            ((select 
                c.relname, 
                a.attname,
                a.attnum,
                (case when tt.typname is null then t.typname else tt.typname end) as att_type,
                (case when t.typtype = 'd' then t.typtypmod else NULL end) as att_len,
                a.atttypmod,
                d.description,
                n.nspname,
                d1.description as rel_desc
            from 
                pg_namespace n,
                pg_class c 
                left join pg_description d1 on (d1.objoid = c.oid and d1.objsubid = 0)
                inner join pg_attribute a on (c.oid = a.attrelid) 
                    left join pg_description d on (a.attnum = d.objsubid and d.objoid = c.oid),
                pg_type t left join pg_type tt on t.typbasetype = tt.oid
    
            where 
                n.nspname = ANY (schemas) --('nub', 'dic')
                and c.relnamespace = n.oid
                --and c.oid = a.attrelid
                and c.relkind = 'r'
                and a.atttypid = t.oid
                and attname not in ('tableoid', 'cmax', 'cmin', 'xmax', 'xmin', 'ctid')
                --and a.attnum = d.objsubid
                --and d.objoid = c.oid
            ) as attrs 

            left join 

           (select 
               con.conname,
               con.contype,
               c.relname as relname,
               c1.relname as relfname,
               c1.oid as attfrelid,
               unnest(con.conkey) as con_attnum,
               unnest(con.confkey) as con_attfnum
           from 
               pg_constraint con,
               pg_class c,
               pg_class c1,
               pg_namespace n
           where

               n.nspname = ANY (schemas) --in ('nub', 'dic')
               and c.relnamespace = n.oid
               and c.oid = con.conrelid
               and c1.oid = con.confrelid
               and array_upper(con.conkey, 1) = 1) as cons     

           on (attrs.relname = cons.relname and attrs.attnum = cons.con_attnum ) ) left join
           pg_attribute a on (cons.attfrelid = a.attrelid and a.attnum = cons.con_attfnum)

        order by 1, 2, 4
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
