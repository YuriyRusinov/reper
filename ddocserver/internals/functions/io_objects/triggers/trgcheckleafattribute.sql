CREATE OR REPLACE FUNCTION checkLeafAttribute() RETURNS trigger AS
$BODY$
declare

    parentCode varchar;
    parentValue int8;
    sql varchar;
    childCount int4;
begin
    
    if(TG_NARGS < 1) then
        raise exception 'incorrect trigger function calling';
        return NULL;
    end if;

    parentCode = TG_ARGV[0];    

    if(TG_OP = 'INSERT') then
        if(new.item_is_leaf is null or new.item_is_leaf <> 2) then
            new.item_is_leaf = 1;
        end if;

        EXECUTE 'SELECT ($1)."' || parentCode || '"::int8' INTO parentValue USING NEW;

        if(parentValue is null) then
            return new;
        end if;
        
        sql = 'update ' || TG_RELNAME || ' set item_is_leaf = 3 where id = ' || parentValue;
        execute sql;

        return new;
    end if;

    if(TG_OP = 'UPDATE') then
        --raise exception 'aaa % -- % - %', TG_RELNAME;--, parentCode, parentValue;
        if(old.item_is_leaf = new.item_is_leaf) then
            return new;
        end if;

        if(new.item_is_leaf = 1) then

            EXECUTE 'select count(id) from ' || TG_RELNAME || ' where "' || parentCode || '" = ' || new.id into childCount;
            
            if(childCount > 0) then
                new.item_is_leaf = 3;
            end if;
        end if;
    end if;

    if(TG_OP = 'DELETE') then
        EXECUTE 'SELECT ($1)."' || parentCode || '"::int8' INTO parentValue USING OLD;
        
        if(parentValue is null) then
            return old;
        end if;
        EXECUTE 'select count(id) from ' || TG_RELNAME || ' where "' || parentCode || '" = ' || parentValue into childCount;
        --raise exception '--- %', childCount;
        if(childCount = 1) then -- т.к. запись еще реально не удалена, то запрос вернет эту последнюю запись
            sql = 'update ' || TG_RELNAME || ' set item_is_leaf = 2 where id = ' || parentValue;
            --raise exception '%', sql;
            execute sql;
        end if;

        return old;
    end if;

    

    return new;
end
$BODY$ 
language 'plpgsql' SECURITY DEFINER;

