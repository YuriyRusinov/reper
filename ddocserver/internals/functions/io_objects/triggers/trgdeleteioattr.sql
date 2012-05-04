create or replace function ioDeleteAttrCheck() returns trigger as
$BODY$
declare
    r record;
    tableName varchar;
    query varchar;
begin
    
    if(old.is_actual = false) then
        raise exception 'You cannot delete attribute values placed in archive!';
        return NULL;
    end if;

    for r in 
        select a.id, a.id_a_type, a.table_name 
        from attributes a, attrs_categories ac
        where a.id = ac.id_io_attribute and ac.id = old.id_attr_category
    loop
        if(r.id_a_type <> 17) then --CheckBoxListEx
            return old;
        end if;

        tableName := 'io_object_' || r.table_name || '_ref_' || r.id;
            
        query := 'select f_safe_drop_table(' || quote_literal(tableName) || ')';
        execute query;
    end loop;

    return old;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgiodeleteattr', 'attrs_values');

select f_create_trigger('trgiodeleteattr', 'before', 'delete', 'attrs_values', 'iodeleteattrcheck()');

/*
create trigger trgIODeleteAttr 
before delete
on tbl_attrs_values 
for each row 
execute procedure ioDeleteAttrCheck();
*/
