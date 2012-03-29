create or replace function checkLastErrorTable() returns void as
$BODY$
declare
    cnt int4;
begin

    select f_is_temp_table_exist('last_error') into cnt;
    if(cnt = 0) then
        perform createLastErrorTable();
    end if;
    
    return;
    
end
$BODY$
language 'plpgsql';


create or replace function setLastErrorData(varchar) returns void as
$BODY$
declare
    errData alias for $1;
begin
    
    perform checkLastErrorTable();

    update last_error
    set error_data = errData;
        
    return;
    
--    exception when 
--                    undefined_table
--              then
--                    perform createLastErrorTable();
--                    
--                    perform setLastErrorData(errData);
--                    return;
end
$BODY$
language 'plpgsql';

create or replace function updateLastError(varchar, varchar) returns void as
$BODY$
declare
    sql_state alias for $1;
    sql_errm  alias for $2;
    
    r RECORD;
    undef_error_name varchar;
    undef_error_code char(5);
begin
    
    undef_error_name := 'undefined error';
    undef_error_code := 'U0000';
    
    perform checkLastErrorTable();

    for r in
        select 
            err.sqlstate, 
            err.name, 
            err.id_group, 
            errg.name as group_name
        from 
            error err, 
            group_error errg 
        where 
            err.sqlstate = sql_state 
            and err.id_group = errg.id_group
    loop
        if(r.sqlstate isnull) then
            --insert into last_error values(undef_error_code, undef_error_name, sql_errm, NULL, NULL);
            update last_error 
            set 
                error_name = undef_error_name, 
                error_sqlstate = undef_error_code, 
                error_message = sql_errm, 
                error_group = NULL, 
                error_group_name = NULL, 
                error_data = NULL;
            return;
        end if;

        --insert into last_error values(r.sqlstate, r.name, sql_errm, r.id_group, r.group_name); 
        update last_error 
        set 
            error_name = r.name, 
            error_sqlstate = r.sqlstate, 
            error_message = sql_errm, 
            error_group = r.id_group, 
            error_group_name = r.group_name, 
            error_data = NULL;
        return;
    end loop;
    
    --insert into last_error values(undef_error_code, undef_error_name, sql_errm, NULL, NULL);
    update last_error 
    set 
        error_name = undef_error_name, 
        error_sqlstate = undef_error_code, 
        error_message = sql_errm, 
        error_group = NULL, 
        error_group_name = NULL, 
        error_data = NULL;
    
    return;
    
--    exception when 
--                    undefined_table
--              then
--                    perform createLastErrorTable();
--                    
--                    perform updateLastError(sql_state, sql_errm);
--                    return;
end
$BODY$
language 'plpgsql';

create or replace function clearLastError() returns void as
$BODY$
begin
    --truncate last_error;
    --insert into last_error values('00000', 'SUCCESSFUL COMPLETION', 'SUCCESSFUL COMPLETION', 1, 'SUCCESSFUL COMPLETION');

    perform checkLastErrorTable();
    
    update last_error set 
            error_name = 'SUCCESSFUL COMPLETION', 
            error_sqlstate = '00000', 
            error_message = 'SUCCESSFUL COMPLETION', 
            error_group = 1, 
            error_group_name = 'SUCCESSFUL COMPLETION',
            error_data = NULL;
    
    return;
    
--    exception when 
--                    undefined_table
--              then
--                    perform createLastErrorTable();
--                    
--                    perform clearLastError();
--                    return;
end
$BODY$
language 'plpgsql';

select f_safe_drop_type('h_get_last_error');
create type h_get_last_error as(ERROR_SQLSTATE       CHAR(5),
                                ERROR_NAME           VARCHAR,
                                ERROR_MESSAGE        VARCHAR,
                                ERROR_GROUP          INT4,
                                ERROR_GROUP_NAME     VARCHAR,
                                ERROR_DATA           VARCHAR);

create or replace function getLastError() returns setof h_get_last_error as
$BODY$
declare
    r h_get_last_error%rowtype;
begin

    perform checkLastErrorTable();
    
    for r in 
        select * from last_error limit 1
    loop
        return next r;
        return;
    end loop;
    
    return;

--    exception when 
--                    undefined_table
--              then
--                    perform createLastErrorTable();
--                    
--                    for r in 
--                        select * from getLastError()
--                    loop
--                        return next r;
--                    end loop;
--                    return;
end
$BODY$
language 'plpgsql';

create or replace function getLastErrorCode() returns char(5) as
$BODY$
declare
    err_code char(5);
begin
    
    perform checkLastErrorTable();
    
    select error_sqlstate into err_code from last_error limit 1;
    
    return err_code;
    
--    exception when 
--                    undefined_table
--              then
--                    perform createLastErrorTable();
--                    return getLastErrorCode();
end
$BODY$
language 'plpgsql';


create or replace function createLastErrorTable() returns void as 
$BODY$
begin

    create temp table last_error 
    (
        ERROR_SQLSTATE       CHAR(5)              not null,
        ERROR_NAME           VARCHAR              not null,
        ERROR_MESSAGE        VARCHAR              null,
        ERROR_GROUP          INT4                 null,
        ERROR_GROUP_NAME     VARCHAR              null,
        ERROR_DATA           VARCHAR              null
    );

    insert into last_error values('00000', 'SUCCESSFUL COMPLETION', 'SUCCESSFUL COMPLETION', 1, 'SUCCESSFUL COMPLETION');
    
    return;
    
--    exception when OTHERS
--        then 
--            raise notice '%', sqlstate;
end
$BODY$
language 'plpgsql';
