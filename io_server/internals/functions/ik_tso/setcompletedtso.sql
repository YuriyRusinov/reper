
create or replace function addTSOUpdating(varchar, int4, bool) returns int4 as
$BODY$
declare
    tableName alias for $1;
    theId alias for $2;
    isInsert alias for $3;

    cnt int4;
begin

    select count(*) into cnt from t_updated_records where table_name = tableName and id = theId;
    if(cnt > 0) then
        return 1;
    end if;

    insert into t_updated_records (table_name, id, is_insert) values (tableName, theId, isInsert);

    return 1;

end
$BODY$
language 'plpgsql';


create or replace function startTSOUpdating() returns int4 as
$BODY$
declare
begin
    create temp table t_updated_records(table_name varchar, id int4, is_insert bool);
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function stopTSOUpdating() returns int4 as
$BODY$
declare
    r record;
    q varchar;
begin

    for r in
        select 
            table_name,
            id
        from 
            t_updated_records    
    loop

        q = 'update ' || r.table_name || ' set is_completed = false where id = ' || r.id;
        execute q;

        q = 'update ' || r.table_name || ' set is_completed = true where id = ' || r.id;
        execute q;
 
    end loop;

    drop table t_updated_records;
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function addTSOGuardObj(int4, int4) returns int4 as
$BODY$
declare
    idSafe alias for $1;
    idDevice alias for $2;

    cnt int4;
begin
    select count(*) into cnt from t_guard_obj where id_safe = idSafe and id_device = idDevice;
    if(cnt > 0) then
        return 1;
    end if;

    insert into t_guard_obj (id_safe, id_device) values (idSafe, idDevice);

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function startTSOGuardObj() returns int4 as
$BODY$
declare
begin
    create temp table t_guard_obj(id_safe int4, id_device int4);
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function stopTSOGuardObj() returns int4 as
$BODY$
declare
    r record;
begin
    for r in
        select 
            t.id_safe, 
            t.id_device as newDev, 
            g.id_guard_objects, 
            g.id_device as oldDev
        from 
            t_guard_obj t
            full outer join guard_objects_devices g on t.id_safe = g.id_guard_objects and t.id_device = g.id_device
    loop

        if(r.id_safe isnull and r.id_guard_objects is not null) then
            delete from guard_objects_devices where id_guard_objects = r.id_guard_objects and id_device = r.oldDev;
        end if;

        if(r.id_safe is not null and r.id_guard_objects isnull) then
            insert into guard_objects_devices (id_guard_objects, id_device) values(r.id_safe, r.newDev);
        end if;
    end loop;


    drop table t_guard_obj;
    return 1;
end
$BODY$
language 'plpgsql';



create or replace function startTSOCreatePerson() returns int4 as
$BODY$
declare
begin
    create temp table t_person_info(id int4, 
                                    extra_id int4, 
                                    lastname varchar, 
                                    firstname varchar, 
                                    sirname varchar, 
                                    id_position int4, 
                                    position_name varchar,
                                    id_unit int4,
                                    unit_name varchar);
    return 1;
end
$BODY$
language 'plpgsql';

create or replace function addTSOPersonInfo(int4, varchar, varchar) returns int4 as
$BODY$
declare
    idPerson alias for $1;
    f_name alias for $2;
    f_value alias for $3;

    theId int4;
    columnName varchar;
    theValue varchar;
    q varchar;
begin

    if(f_name = 'extra_id') then
        columnName = 'extra_id';
        theValue = asString(f_value, false);
    elsif(f_name = 'Фамилия') then
        columnName = 'lastname';
        theValue = asString(f_value, true);
    elsif(f_name = 'Имя') then
        columnName = 'firstname';
        theValue = asString(f_value, true);
    elsif(f_name = 'Отчество') then
        columnName = 'sirname';
        theValue = asString(f_value, true);
    elsif(f_name = 'Отдел.id') then
        columnName = 'id_unit';
        theValue = asString(f_value, false);
    elsif(f_name = 'Отдел') then
        columnName = 'unit_name';
        theValue = asString(f_value, true);
    elsif(f_name = 'Должность.id') then
        columnName = 'id_position';
        theValue = asString(f_value, false);
    elsif(f_name = 'Должность') then
        columnName = 'position_name';
        theValue = asString(f_value, true);
    else
        return -1;
    end if;
    
    select id into theId from t_person_info where id = idPerson;
    if(theId isnull) then
        q = 'insert into t_person_info (id, ' || columnName || ') values(' || idPerson || ', ' || theValue || ')';
    else
        q = 'update t_person_info set ' || columnName || ' = ' || theValue || ' where id = ' || idPerson;
    end if;

    execute q;

    --raise warning '---- % ----', q;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function stopTSOCreatePerson() returns int4 as
$BODY$
declare
    r record;
    q varchar;
    cnt int4;

    idUnit int4;
    idPosition int4;
    idPerson int4;
begin

    for r in
        select 
            id, 
            extra_id, 
            lastname, 
            firstname, 
            sirname, 
            id_position, 
            position_name,
            id_unit,
            unit_name
        from 
            t_person_info
    loop
        --подразделение
        if(r.id_unit is not null) then
            select id into idUnit from tso_units where extra_id = r.id_unit;
            if(idUnit isnull) then
                select getNextSeq('tso_units', 'id') into idUnit;
                INSERT INTO tso_units (id, extra_id, name) VALUES (idUnit, r.id_unit, r.unit_name);
            end if;
        else 
            idUnit = NULL;
        end if;
        --должность
        if(r.id_position is not null) then
            select id into idPosition from tso_positions where extra_id = r.id_position;
            if(idPosition isnull) then
                select getNextSeq('tso_positions', 'id') into idPosition;
                INSERT INTO tso_positions (id, extra_id, name) VALUES (idPosition, r.id_position, r.position_name);
            end if;
        else 
            idPosition = NULL;
        end if;

        select id into idPerson from persons where id = r.id;
        --idPerson = r.id;
        if(idPerson isnull) then
            --select getNextSeq('persons', 'id') into idPerson;
            idPerson = r.id;
            q = 'insert into persons (id, id_tso_position, id_tso_unit, lastname, firstname, sirname, extra_id) ' || 
                'values (' || idPerson || ', ' || 
                              asString(idPosition, false) || ', ' || 
                              asString(idUnit, false) || ', ' ||
                              asString(r.lastname, true) || ', ' ||
                              asString(r.firstname, true) || ', ' ||
                              asString(r.sirname, true) || ', ' ||
                              asString(r.extra_id, false) || ')';
        else
            q = 'update persons set id_tso_position = ' || asString(idPosition, false) || ', ' ||
                                   'id_tso_unit = ' || asString(idUnit, false) || ', ' ||
                                   'lastname = ' || asString(r.lastname, true) || ', ' ||
                                   'firstname = ' || asString(r.firstname, true) || ', ' ||
                                   'sirname = ' || asString(r.sirname, true) || 
                ' where id = ' || idPerson;
                        

        end if;

        --raise warning '%', q;
        execute q;

        perform addTSOUpdating('persons', idPerson, true);

    end loop;

    drop table t_person_info;
    return 1;
end
$BODY$
language 'plpgsql';
