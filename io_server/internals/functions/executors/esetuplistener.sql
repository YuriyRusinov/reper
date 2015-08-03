create or replace function eSetupListener(int4) returns int4 as
$BODY$
declare
    idCategory alias for $1;

    cnt int4;
    r RECORD;
    query varchar;
begin

    select count(*) into cnt from io_categories where id = idCategory;
    if(cnt = 0) then 
        return -1;
    end if;

    select count(*) into cnt from attrs_categories where id_io_category = idCategory;
    if(cnt = 0) then
        return -1;
    end if;

    --create table based on attributes params

    query := 'create table user_table_' || idCategory || ' ( user_table_' || idCategory || '_id serial not null, ';

    for r in 
        select
            a.id,
            a.code as a_code,
            at.code as at_code,
            ac.def_value,
            ac.is_mandatory
        from 
            attributes a,
            a_types at,
            attrs_categories ac
        where 
            ac.id_io_category = idCategory
            and ac.id_io_attribute = a.id
            and a.id_a_type = at.id
    loop

        query := query || r.a_code || ' ' || r.at_code;
        if(r.is_mandatory = TRUE) then
            query := query || ' NOT ';
        end if;

        query := query || ' NULL ';

        if(r.def_value is not null) then
            query := query || ' default ' || r.def_value;
        end if;

        query := query || ', ';
    end loop;

    if(FOUND = FALSE) then
        return -1;
    end if;

    query := query || ' constraint pk_user_table_' || idCategory || '_id primary key (user_table_' || idCategory || '_id)';
    query := query || ')';

    execute query;
    raise notice '%', query;

    query := 'GRANT ALL ON TABLE user_table_' || idCategory || ' TO public;';
    execute query;
    
    select f_create_trigger('trgusertable_' || idCategory || '_insert', 'after', 'update', 'io_objects', 'usertableinsertcheck(' || idCategory || ')');

/*    query := 'create trigger trgUserTable_' || idCategory || '_Insert 
               after update
               on io_objects
               for each row 
               execute procedure userTableInsertCheck(' || idCategory || ')';
    

    execute query;
*/
    
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function userTableInsertCheck() returns trigger as
$BODY$
declare
    idCategory int4;
    idObject int4;

    cnt int4;
    tName varchar;
    pkName varchar;

    query varchar;
    r RECORD;
    theFields varchar;
    theValues varchar;
    id int4;
begin

    if(old.is_completed <> 0 or old.is_completed = new.is_completed or new.is_completed = 0) then
        return new;
    end if;
    
    idCategory := TG_ARGV[TG_NARGS-1];
    idObject = new.id;

    if(new.id_io_category <> idCategory) then
        raise notice 'required category ID is not equal to NEW.ID!';
        return NULL;
    end if;

    tName := 'user_table_' || idCategory;
    pkName := tName || '_id';
    
    cnt := f_is_table_exist(tName, NULL);
    if(cnt <> 1) then
        raise notice 'required user table does not exist!';
        return NULL;
    end if;

    query := 'insert into ' || tName;
    theFields := pkName;

    id := getNextSeq(tName, pkName);
    theValues := '' || id;
    
    for r in
        select 
            av.value,
            a.code as a_code,
            at.code as at_code
        from 
            f_sel_attrs_values(idObject) av,
            attrs_categories ac,
            attributes a,
            a_types at
        where
            av.id_io_object = idObject
            and ac.id_io_category = idCategory
            and ac.id_io_attribute = a.id
            and av.id_attr_category = ac.id
            and a.id_a_type = at.id
    loop
        theFields := theFields || ', ' || r.a_code;
        theValues := theValues || ', ' || quote_literal(r.value) || '::' || r.at_code;
    end loop;

    query := query || ' (' || theFields || ')' || 'values (' || theValues || ')';
    execute query;
    if(FOUND = FALSE) then
        raise notice 'Cannot insert into user_table_% !', idCategory;
        raise notice 'the query is: %', query;
        return NULL;
    end if;
    
    return new;
end
$BODY$
language 'plpgsql';

create or replace function eStopListener(int4) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    trgName varchar;
    tName varchar;
begin

    trgName := 'trgusertable_' || idCategory || '_insert';
    tName := 'io_objects';
    
    perform f_safe_drop_trigger(trgName, tName);

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function eDropListener(int4) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    query varchar;
begin

    perform eStopListener(idCategory);

    query := 'drop table user_table_' || idCategory;

    execute query;

    return 1;
end
$BODY$
language 'plpgsql';
