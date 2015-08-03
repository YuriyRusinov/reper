create or replace function uhInsert(varchar, int4, int4, varchar, int4, varchar) returns int4 as
$BODY$
declare
    uhName alias for $1;
    idSearchTemplate alias for $2;
    idCategory alias for $3;
    procName alias for $4;
    trgLevel alias for $5;
    uhDesc alias for $6;

    idHandler int4;
begin

    if(procName isnull or trgLevel isnull or uhName isnull or trgLevel <= 0) then
        return -1;
    end if;
 
    select getNextSeq('user_chains', 'id') into idHandler;

    insert into user_chains(id, name, id_search_template, id_io_category, proc_name, trig_level, description)
    values (idHandler, uhName, idSearchTemplate, idCategory, procName, trgLevel, uhDesc);

    return idHandler;

end
$BODY$
language 'plpgsql';

create or replace function uhCheck() returns trigger as
$BODY$
declare
    
begin

    if(TG_OP = 'INSERT') then
        perform uhInsertCheck(new.id_io_category);
        return new;
    end if;

    if(TG_OP = 'UPDATE') then
        perform uhDeleteCheck(old.id_io_category);
        perform uhInsertCheck(new.id_io_category);
        return new;
    end if;

    if(TG_OP = 'DELETE') then
        perform uhDeleteCheck(old.id_io_category);
        return old;
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trguhcheck', 'user_chains');

create trigger trgUHCheck
before insert or update or delete
on user_chains
for each row 
execute procedure uhCheck();

create or replace function uhInsertCheck(int4) returns int4 as
$BODY$
declare
    idChildCategory alias for $1;
begin
    
     perform createTriggerUserHandler(io.table_name) 
     from io_objects io, io_categories c
     where io.id_io_category = c.id
           and c.id_child = idChildCategory;

    perform createTriggerUserHandler(table_name) from object_ref_tables where id_io_category = idChildCategory;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function uhDeleteCheck(int4) returns int4 as
$BODY$
declare
    idCategory alias for $1;
begin

    select f_safe_drop_trigger('trguhdatainsert', table_name) from io_objects where id_io_category = idCategory;

     perform f_safe_drop_trigger('trguhdatainsert', io.table_name)
     from io_objects io, io_categories c
     where io.id_io_category = c.id
           and c.id_child = idChildCategory;

    perform f_safe_drop_trigger('trguhdatainsert', table_name) from object_ref_tables where id_io_category = idChildCategory;

    return 1;

end
$BODY$
language 'plpgsql';
