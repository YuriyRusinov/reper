
create or replace function tableToIO(varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;

    idObject int4;
    idCategory int4;
    idChildCategory int4;

    idAttribute int4;
    idAttrType int4;
    refTable varchar;
    refColumn varchar;
    defValue varchar;
    
    r record;
begin

    select cInsert('Child category for ' || tableName, 
                   NULL, --code
                   NULL, --desc
                   10,  --io_type
                   NULL, --child
                   false, --isMain
                   NULL, --unique_id
                   false, --is_global
                   1 --id_state
                  ) into idChildCategory;

    if(idChildCategory < 0) then
        raise warning 'ERROR: cannot create child (table) category for table %', tableName;
        return -1;
    end if;

----
----insert of attributes for child category
---- необходимо определить, есть ли в таблице поле id типа serial.
-- если нет, то его необходимо создать в ней
-- таблицу для справочника мы должны подцепить старую! Не создавать на базе воссозданной категории новую таблицу, 
-- а именно так в настоящее время делается
    for r in 
        select 
            a.attname,
            a.attnum,
            a.attnotnull,
            a.atthasdef,
            t.typname 
        from
            pg_class c,
            pg_attribute a,
            pg_type t
        where
            a.attrelid = c.oid
            and c.relname = tableName
            and a.atttypid = t.oid
            and a.attnum > 0
            and a.attislocal = true
    loop
        idAttrType = --....
        select acInsert(idChildCategory,
                        idAttrType,
                        r.attname, --code
                        r.attname, --name
                        r.attname, --title
                        refTable, --table
                        refColumn, --column
                        100, --def_width
                        defValue, --def_value
                        r.attnotnull, --is_mandatory
                        false, --is_read_only
                        NULL --unique_id
                       ) into idAttribute;

        if(idAttribute < 0) then
            raise warning 'ERROR: cannot create attribute from column of table %', tableName;
            return -4;
        end if;
    end loop;

    select cInsert('Main category for ' || tableName, 
                   NULL, --code
                   NULL, --desc
                   10,  --io_type
                   idChildCategory, --child
                   true, --isMain
                   NULL, --unique_id
                   false, --is_global
                   1 --id_state
                  ) into idCategory;

    if(idCategory < 0) then
        raise warning 'ERROR: cannot create main (qualifier) category for table %', tableName;
        return -2;
    end if;
    
    select ioInsert('Qualifier for table ' || tableName,
                    idCategory,
                    getCurrentUser(), --author
                    1, --id_state
                    NULL, --table_name (will autocreated new table!) 
                    NULL, --desc
                    NULL, --ioInfo
                    getCurrentMaclabelId(), --id_maclabel
                    NULL, --unique_id
                    1, --id_sync_type (does not sync)
                    getLocalOrgId(), --localorg
                    false --is_global,
                    NULL,
                    NULL
                   ) into idObject;

    if(idObject < 0) then
        raise warning 'ERROR: cannot create information object for table %', tableName;
        return -3;
    end if;
    
    return idObject;
end
$BODY$
language 'plpgsql';




--select * from pg_constraint where conrelid = (select oid from pg_class where relname = 'io_objects')


/*
select 
    --c.relname, 
    --a.*,
    a.attname,
    a.attnum,
    a.attnotnull,
    a.atthasdef,
    t.typname 
from 
    pg_class c,
    pg_attribute a,
    pg_type t

where
    a.attrelid = c.oid
    and c.relname = 'io_objects'
    and a.atttypid = t.oid
    and a.attnum > 0
    and a.attislocal = true;
    */
    