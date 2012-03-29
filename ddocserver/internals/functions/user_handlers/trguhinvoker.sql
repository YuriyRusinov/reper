create or replace function trgFuncUHInvoker() returns trigger as
$BODY$
declare
    r record;
    query varchar;
    cnt int4;
begin

    for r in
        select id_io_category, proc_name, trig_level 
        from user_chains
        where id_io_category = new.id_io_category
    loop
        if(r.trig_level = 1) then
            query = 'select ' || r.proc_name || '(' || quote_literal(new.table_name) || ')';
            execute query;
            return new;
        end if;

        select count(*) into cnt from user_handlers_data where table_name = new.table_name;
        if(cnt = r.trig_level) then
            query = 'select ' || r.proc_name || '(' || quote_literal(new.table_name) || ')';
            execute query;
            return new;
        end if;
    end loop;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trguhinvoker', 'user_handlers_data');

create trigger trgUHInvoker
after insert
on user_handlers_data
for each row 
execute procedure trgFuncUHInvoker();


create or replace function trgFuncUHDataInsert() returns trigger as
$BODY$
declare
    ok bool;
    idChildCategory int4;
    idObject int4;
    recType int4;
    r record;
    theTable varchar;
begin

    theTable = TG_RELNAME;
    if(substr(theTable, 1, 4) = 'tbl_') then
        theTable = substr(theTable, 5);
    end if;

    for r in 
        select io.id, c.id_child from io_objects io, io_categories c where io.table_name = theTable and io.id_io_category = c.id
    loop
        idChildCategory = r.id_child;
        idObject = r.id;
    end loop;

    if(idChildCategory isnull) then  --maybe additional table
        for r in 
            select id_io_object, id_io_category from object_ref_tables where table_name = theTable
        loop
           idChildCategory = r.id_io_category;
           idObject = r.id_io_object;
        end loop;
        
        if(idChildCategory isnull) then
            return new; --smth wrong... just exit
        end if;
    end if;

    ok = uhCheckCondition(idObject);

    if(ok = false) then --do nothing
        return new;
    end if;
    
    if(TG_OP = 'INSERT') then
        recType = 1;
    else
        recType = 2;
    end if;

    insert into user_handlers_data (id, id_io_category, table_name, rec_type)  --child_category!
    values(new.id, idChildCategory, theTable, recType);

    return new;
end
$BODY$
language 'plpgsql';


create or replace function createTriggerUserHandler(varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    query varchar;
begin

    perform f_safe_drop_trigger('trguhdatainsert', tableName);

    query = 'create trigger trgUHDataInsert
             after insert or update
             on ' || tableName || '
             for each row 
             execute procedure trgFuncUHDataInsert();';

    execute query;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function uhCheckCondition(int4) returns bool as
$BODY$
declare
    idObject alias for $1;
begin
    return true;
end
$BODY$
language 'plpgsql';

select f_safe_drop_type('h_user_handlers_data');
create type h_user_handlers_data as(id int4, id_io_category int4, table_name varchar, rec_type int4);

create or replace function uhGetMyRecords(varchar) returns setof h_user_handlers_data as
$BODY$
declare
    tableName alias for $1;

    r h_user_handlers_data%rowtype;
begin

    for r in 
        select id, id_io_category, table_name, rec_type
        from user_handlers_data
        where table_name = tableName
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function uhDeleteMyRecords(varchar) returns int4 as 
$BODY$
declare
    tableName alias for $1;
begin

    delete from user_handlers_data where table_name = tableName;

    return 1;

end
$BODY$
language 'plpgsql';

