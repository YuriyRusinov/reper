create or replace function processSync() returns bool as
$BODY$
declare
    bYes int4;
begin

    bYes = getSystemParamInt(2);
    if(bYes = 1) then
       return TRUE;
    end if;
    
    return FALSE;
end
$BODY$
language 'plpgsql';

create or replace function startSync() returns int4 as
$BODY$
declare
    cnt int4;
begin
    select count(*) into cnt from system_table where id_system = 2;
    if(cnt = 0) then
        insert into system_table (id_system, int_parameter, description) 
        values (2, 1, 'Inf exchange. If 1 then process is running');
        if(FOUND = FALSE) then
            raise notice 'Cannot insert inf exchange modificator into system_table!';
            return 0;
        end if;
    else
        update system_table set int_parameter = 1, description = 'Inf exchange. If 1 then process is running' where id_system = 2;
        if(FOUND = FALSE) then
            raise notice 'Cannot update inf exchange modificator in system_table!';
            return 0;
        end if;
    end if;    

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function stopSync() returns int4 as
$BODY$
declare
    cnt int4;
begin
    select count(*) into cnt from system_table where id_system = 2;
    if(cnt = 0) then
        insert into system_table (id_system, int_parameter, description) 
        values (2, 0, 'Inf exchange. If 1 then process is running');
        if(FOUND = FALSE) then
            raise notice 'Cannot insert inf exchange modificator into system_table!';
            return 0;
        end if;
    else
        update system_table set int_parameter = 0, description = 'Inf exchange. If 1 then process is running' where id_system = 2;
        if(FOUND = FALSE) then
            raise notice 'Cannot update inf exchange modificator in system_table!';
            return 0;
        end if;
    end if;    

    return 1;
end
$BODY$
language 'plpgsql';
