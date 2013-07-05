create or replace function aaInsertCheck() returns trigger as 
$BODY$
declare
    refTableName varchar;
    r record;
    aCode varchar; 
    aCount int4;
    aName varchar;
    idType int4;
begin

    if(TG_OP = 'UPDATE') then
        if(old.id_attr_parent <> new.id_attr_parent or old.id_attr_child <> new.id_attr_child) then
            raise exception 'You cannot update IDs of attr_child or attr_parent in table attrs_attrs!';
            return NULL;
        end if;
    end if;

    if(TG_OP = 'INSERT') then
        if(new.id_attr_parent = new.id_attr_child) then
            raise exception 'Complex attribute cannot have itself as a child!';
            return NULL;
        end if;

        --complex attributes are allowed ONLY for attr_types = 32 (complex attrs)
        select id_a_type into idType from attributes where id = new.id_attr_parent;
        if(idType isnull or idType <> 32) then
            raise exception 'Complex attributes are allowed only for attributes with type = 32 (complex attrs)!';
            return NULL;
        end if;

        --in complex attributes we can add ONLY NOT complex childs! (id_a_type <> 32)
        select id_a_type into idType from attributes where id = new.id_attr_child;
        if(idType isnull or idType = 32) then
            raise exception 'Complex attributes can only contain not complex childs! (type <> 32)!';
            return NULL;
        end if;

        if(idType = 26 or
           idType = 25 or
           idType = 18 or
           idType = 19 or 
           idType = 17 or 
           idType = 3
          ) then
            raise exception 'Complex attributes cannot contain attributes of given type!';
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
