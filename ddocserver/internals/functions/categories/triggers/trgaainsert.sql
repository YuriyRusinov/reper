create or replace function aaInsertCheck() returns trigger as 
$BODY$
declare
    refTableName varchar;
    r record;
    aCode varchar; 
    aCount int4;
    aName varchar;
begin

    if(TG_OP = 'UPDATE') then
        if(old.id_attr_parent <> new.id_attr_parent or old.id_attr_child <> new.id_attr_child) then
            raise exception 'You cannot update IDs of attr_child or attr_parent in table attrs_attrs!';
            return NULL;
        end if;
    end if;

    select name into aName from attributes where id = new.id_attr_child;
    
    new.name := aName;

    return new;
end
$BODY$ 
language 'plpgsql';

select f_safe_drop_trigger('trgaainsert', 'attrs_attrs');

select f_create_trigger('trgaainsert', 'before', 'insert or update', 'attrs_attrs', 'aainsertcheck()');
