select f_safe_drop_type('h_j_get_cmds');
create type h_j_get_cmds as(
                              id int4,
                              id_dl_from int4,
                              dl_from_name varchar,
                              id_dl_executor int4,
                              dl_executor_name varchar,
                              executor_name varchar,
                              id_dl_to int4,
                              dl_to_name varchar,
                              id_io_category int4,
                              category_name varchar,
                              id_jr_state int4,
                              jr_state_name varchar,
                              insert_time timestamp,
                              is_automated boolean,
                              exec_datetime timestamp,
                              exec_period int4[2],
                              message_body varchar,
                              is_archived boolean,
                              cmd_type int4,
                              id_urgency_level int4,
                              id_io_object_att int4,
                              receive_datetime timestamp,
                              accepted_datetime timestamp,
                              real_exec timestamp,
                              read_datetime_e timestamp,
                              read_datetime_c timestamp,
                              input_number varchar,
                              output_number varchar
                              );

/*
create or replace function jGetInCmds(int4, date, boolean, boolean, boolean) returns setof h_j_get_cmds as
$BODY$
declare
    r h_j_get_cmds%rowtype;
    query varchar;
    insertTime timestamp;
    
    idDlExecutor alias for $1;
    insertDate alias for $2;
    bUseExecuted alias for $3;
    bUseArchived alias for $4;
    bUseSubordinated alias for $5;
begin

    if(idDlExecutor isnull) then
        return;
    end if;

    query = '
        select 
            cmd.id, 
            cmd.id_dl_from, 
            d1.name as dl_from_name, 
            cmd.id_dl_executor, 
            d2.name as dl_executor_name, 
            case when u.id_organization isnull then NULL else e.name end as executor_name, 
            cmd.id_dl_to, 
            d3.name as dl_to_name, 
            cmd.id_io_category, 
            c.name as category_name, 
            cmd.id_jr_state, 
            s.name as jr_state_name, 
            cmd.insert_time, 
            cmd.is_automated, 
            cmd.exec_datetime, 
            cmd.exec_period, 
            cmd.message_body, 
            cmd.is_archived, 
            1 as cmd_type,
            cmd.id_urgency_level,
            cmd.id_io_object_att,
            cmd.receive_datetime,
            cmd.accepted_datetime,
            cj.real_exec,
            cmd.read_datetime_e,
            cmd.read_datetime_c
        from 
            command_journal cmd left join tsd_control_journal cj on cj.id_journal = cmd.id , 
            "position" d1, 
            "position" d2, 
            "position" d3, 
            io_categories c, 
            jr_states s, 
            organization e, 
            units u
            
        where 
            cmd.id_io_category = c.id 
            and cmd.id_jr_state = s.id 
            and cmd.id_dl_from = d1.id 
            and cmd.id_dl_to = d3.id 
            and cmd.id_dl_executor = d2.id 
            and d2.id_unit = u.id 
            and cmd.id_jr_state <> 7 --виртуальная команда.
            and (u.id_organization isnull or u.id_organization = e.id)';
    
    if(bUseSubordinated = false) then
        query := query || ' and cmd.id_dl_executor = ' || idDlExecutor;
    end if;

    if(bUseSubordinated = true) then
        query := query || ' and (cmd.id_dl_executor = ' || idDlExecutor || ' or cmd.id_dl_executor in (select id from getSubordinates(' || idDlExecutor || ')))';
    end if;

    if(insertDate is not null) then
        query := query || ' and cmd.insert_time >= ' || quote_literal(insertDate + time '00:00:00');
        query := query || ' and cmd.insert_time <= ' || quote_literal(insertDate + time '23:59:59');
    end if;
    
    if(bUseExecuted = FALSE) then
        query := query || ' and cmd.id_jr_state <> 4';
    end if;

    if(bUseArchived = FALSE) then
        query := query || ' and cmd.is_archived = false';
    end if;
    

    raise notice 'THE QUERY = %', query;

    for r in execute query
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function jGetOutCmds(int4, date, boolean, boolean, boolean) returns setof h_j_get_cmds as
$BODY$
declare
    r h_j_get_cmds%rowtype;
    query varchar;
    insertTime timestamp;
    
    idDlFrom alias for $1;
    insertDate alias for $2;
    bUseExecuted alias for $3;
    bUseArchived alias for $4;
    bUseSubordinated alias for $5;
begin

    if(idDlFrom isnull) then
        return;
    end if;

    query = '
        select 
            cmd.id, 
            cmd.id_dl_from, 
            d1.name as dl_from_name, 
            cmd.id_dl_executor, 
            d2.name as dl_executor_name, 
            case when u.id_organization isnull then NULL else e.name end as executor_name,
            cmd.id_dl_to, 
            d3.name as dl_to_name, 
            cmd.id_io_category, 
            c.name as category_name, 
            cmd.id_jr_state, 
            s.name as jr_state_name, 
            cmd.insert_time, 
            cmd.is_automated, 
            cmd.exec_datetime, 
            cmd.exec_period, 
            cmd.message_body, 
            cmd.is_archived, 
            2 as cmd_type,
            cmd.id_urgency_level,
            cmd.id_io_object_att,
            cmd.receive_datetime,
            cmd.accepted_datetime,
            cj.real_exec,
            cmd.read_datetime_e,
            cmd.read_datetime_c 
        from 
            command_journal cmd left join tsd_control_journal cj on cj.id_journal = cmd.id , 
            "position" d1, 
            "position" d2, 
            "position" d3, 
            io_categories c, 
            jr_states s, 
            organization e, 
            units u 
        where 
            cmd.id_io_category = c.id 
            and cmd.id_jr_state = s.id 
            and cmd.id_dl_from = d1.id 
            and cmd.id_dl_to = d3.id 
            and cmd.id_dl_executor = d2.id 
            and d2.id_unit = u.id 
            and cmd.id_jr_state <> 7 --виртуальная команда.
            and (u.id_organization isnull or u.id_organization = e.id)  ';
    
    if(bUseSubordinated = false) then
        query := query || ' and cmd.id_dl_from = ' || idDlFrom;
    end if;

    if(bUseSubordinated = true) then
        query := query || ' and (cmd.id_dl_from = ' || idDlFrom || ' or cmd.id_dl_from in (select id from getSubordinates(' || idDlFrom || ')))';
    end if;

    if(insertDate is not null) then
        query := query || ' and cmd.insert_time >= ' || quote_literal(insertDate + time '00:00:00');
        query := query || ' and cmd.insert_time <= ' || quote_literal(insertDate + time '23:59:59');
    end if;
    
    if(bUseExecuted = FALSE) then
        query := query || ' and cmd.id_jr_state <> 4';
    end if;

    if(bUseArchived = FALSE) then
        query := query || ' and cmd.is_archived = false';
    end if;
    

    raise notice 'THE QUERY = %', query;

    for r in execute query
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';
*/

create or replace function jGetCmdsEx(date, date, boolean) returns setof h_j_get_cmds as
$BODY$
declare
    r h_j_get_cmds%rowtype;
    query varchar;
    insertTime timestamp;
    
--    idDlFrom alias for $1;
    insertDateStart alias for $1;
    insertDateStop alias for $2;
    bUseExecuted alias for $3;
--    bUseArchived alias for $4;
--    bUseSubordinated alias for $5;

    idCurrentDl int4;
begin


    idCurrentDl = getCurrentDl();
    if(idCurrentDl isnull) then
        return;
    end if;

    query = '
        select 
            cmd.id, 
            cmd.id_dl_from, 
            d1.name as dl_from_name, 
            cmd.id_dl_executor, 
            d2.name as dl_executor_name, 
            case when u.id_organization isnull then NULL else e.name end as executor_name,
            cmd.id_dl_to, 
            d3.name as dl_to_name, 
            cmd.id_io_category, 
            c.name as category_name, 
            cmd.id_jr_state, 
            s.name as jr_state_name, 
            cmd.insert_time, 
            cmd.is_automated, 
            cmd.exec_datetime, 
            cmd.exec_period, 
            cmd.message_body, 
            cmd.is_archived, 
            (case when id_dl_to = ' || idCurrentDl || ' then 2 -- на контроле
                  when id_dl_from = ' || idCurrentDl || ' then 3  --исходящие
                  else 1 end) as cmd_type,  --входящие
            cmd.id_urgency_level,
            cmd.id_io_object_att,
            cmd.receive_datetime,
            cmd.accepted_datetime,
            cj.real_exec,
            cmd.read_datetime_e,
            cmd.read_datetime_c,
            cmd.input_number,
            cmd.output_number 
        from 
            command_journal cmd left join tsd_control_journal cj on cj.id_journal = cmd.id , 
            "position" d1, 
            "position" d2, 
            "position" d3, 
            io_categories c, 
            jr_states s, 
            organization e, 
            units u 
        where 
            cmd.id_io_category = c.id 
            and cmd.id_jr_state = s.id 
            and cmd.id_dl_from = d1.id 
            and cmd.id_dl_to = d3.id 
            and cmd.id_dl_executor = d2.id 
            and d2.id_unit = u.id 
            and cmd.id_jr_state <> 7 --виртуальная команда.
            and (u.id_organization isnull or u.id_organization = e.id)
            and (cmd.id_dl_from = ' || idCurrentDl || ' or cmd.id_dl_to = ' || idCurrentDl || ' or cmd.id_dl_executor = ' || idCurrentDl || ')
            and cmd.is_archived = false
        ';

/*    if(bUseSubordinated = false) then
        query := query || ' and cmd.id_dl_from = ' || idDlFrom;
    end if;

    if(bUseSubordinated = true) then
        query := query || ' and (cmd.id_dl_from = ' || idDlFrom || ' or cmd.id_dl_from in (select id from getSubordinates(' || idDlFrom || ')))';
    end if;
*/

    if(insertDateStart is not null) then
        query := query || ' and cmd.insert_time >= ' || quote_literal(insertDateStart + time '00:00:00');
    end if;

    if(insertDateStop is not null) then
        query := query || ' and cmd.insert_time <= ' || quote_literal(insertDateStop + time '23:59:59');
    end if;
    
    if(bUseExecuted isnull or bUseExecuted = FALSE) then
        query := query || ' and cmd.id_jr_state <> 4';
    end if;

/*
    if(bUseArchived = FALSE) then
        query := query || ' and cmd.is_archived = false';
    end if;
*/    
    query = query || ' order by cmd_type, id_jr_state, insert_time';


    --raise warning 'THE QUERY = %', query;

    for r in execute query
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql' security definer;

create or replace function jGetCmd(int4) returns setof h_j_get_cmds as
$BODY$
declare
    idCmd alias for $1;

    r h_j_get_cmds%rowtype;
    query varchar;
    
    idCurrentDl int4;
begin


    idCurrentDl = getCurrentDl();
    if(idCurrentDl isnull) then
        return;
    end if;

    query = '
        select 
            cmd.id, 
            cmd.id_dl_from, 
            d1.name as dl_from_name, 
            cmd.id_dl_executor, 
            d2.name as dl_executor_name, 
            case when u.id_organization isnull then NULL else e.name end as executor_name,
            cmd.id_dl_to, 
            d3.name as dl_to_name, 
            cmd.id_io_category, 
            c.name as category_name, 
            cmd.id_jr_state, 
            s.name as jr_state_name, 
            cmd.insert_time, 
            cmd.is_automated, 
            cmd.exec_datetime, 
            cmd.exec_period, 
            cmd.message_body, 
            cmd.is_archived, 
            (case when id_dl_to = ' || idCurrentDl || ' then 2 -- на контроле
                  when id_dl_from = ' || idCurrentDl || ' then 3  --исходящие
                  else 1 end) as cmd_type,  --входящие
            cmd.id_urgency_level,
            cmd.id_io_object_att,
            cmd.receive_datetime,
            cmd.accepted_datetime,
            cj.real_exec,
            cmd.read_datetime_e,
            cmd.read_datetime_c,
            cmd.input_number,
            cmd.output_number  
        from 
            command_journal cmd left join tsd_control_journal cj on cj.id_journal = cmd.id , 
            "position" d1, 
            "position" d2, 
            "position" d3, 
            io_categories c, 
            jr_states s, 
            organization e, 
            units u 
        where 
            cmd.id_io_category = c.id 
            and cmd.id_jr_state = s.id 
            and cmd.id_dl_from = d1.id 
            and cmd.id_dl_to = d3.id 
            and cmd.id_dl_executor = d2.id 
            and d2.id_unit = u.id 
            and cmd.id_jr_state <> 7 --виртуальная команда.
            and (u.id_organization isnull or u.id_organization = e.id)
            and (cmd.id_dl_from = ' || idCurrentDl || ' or cmd.id_dl_to = ' || idCurrentDl || ' or cmd.id_dl_executor = ' || idCurrentDl || ')
            and cmd.is_archived = false
            and cmd.id = ' || idCmd;

    for r in execute query
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql' security definer;
