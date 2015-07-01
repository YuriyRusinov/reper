--select * from getRootRecord(2810, true);

select f_safe_drop_type('h_get_root_record');
create type h_get_root_record as(id_record int8, uuid_t uuid, rr_name varchar, id_parent int8, ii_order int4);

create or replace function getRootRecord(int8, bool) returns setof h_get_root_record as
$BODY$
declare
    idRecord alias for $1;
    bOnlyTop alias for $2; --TRUE - выводить только одну запись
                           --FALSE - выводить всю иерархию, начиная с верхнего элемента
    
    r h_get_root_record%rowtype;
    
    tableName varchar;
    parentColumn varchar;
    q varchar;
    idChildCategory int4;
    
begin

    tableName = getRecordTable(idRecord);
    if(tableName isnull) then
        return;
    end if;

    select a.code into parentColumn
    from io_categories c, io_objects io, attrs_categories ac, attributes a
    where
        c.id = io.id_io_category
        and io.table_name = tableName
        and c.id_child = ac.id_io_category
        and ac.id_io_attribute = a.id
        and a.id_a_type = 3
    limit 1;

    if(parentColumn isnull) then
        q = 'select tbl.id, tbl.uuid_t::varchar, tbl.rr_name, NULL, 0 from ' || tableName || ' tbl where tbl.id = ' || idRecord;
        for r in execute q
        loop
            return next r;
        end loop;

        return;
    end if;
    
    q = 'with recursive table_rec (id, uuid_t, rr_name, "' || parentColumn || '", ii_rec_order) 
        as 
        (
            select distinct 
                tbl.id, 
                tbl.uuid_t::varchar,
                tbl.rr_name,
                tbl."' || parentColumn || '",
                0 
            from ' || tableName || ' tbl
            where 
                tbl.id = ' || idRecord || '

            UNION ALL 

            select 
                tbl.id, 
                tbl.uuid_t::varchar,
                tbl.rr_name,
                tbl."' || parentColumn || '",
                table_rec.ii_rec_order+1 
            from 
                table_rec, ' || tableName || ' tbl
            where 
                table_rec."' || parentColumn || '" = tbl."id"
        )

        select distinct 
                table_rec.id, 
                table_rec.uuid_t,
                table_rec.rr_name,
                table_rec."' || parentColumn || '",
                table_rec.ii_rec_order 
        from 
            table_rec

        order by 
            table_rec.ii_rec_order desc
    ';
    

    for r in execute q
    loop
        return next r;
        if(bOnlyTop = true) then
            return;
        end if;
    end loop;

    return;
    
end
$BODY$
language 'plpgsql';
