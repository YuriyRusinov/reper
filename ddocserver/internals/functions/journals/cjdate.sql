create or replace function getCommandsNumberByDate (timestamp) returns int4 as
$BODY$
declare
    date0 alias for $1;

    query varchar;
    num int4;
    r record;
begin

    query := '
        select count(*) as cnt
        from
            command_journal cmd,
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
            and u.id_organization = e.id';
    if (date0 is not null) then
        query := query || ' and date(cmd.exec_datetime) = ' || quote_literal (date0);
    end if;

    for r in execute query loop
        num = r.cnt;
    end loop;

    return num;
end
$BODY$
language 'plpgsql';

create or replace function getCmdByDate (timestamp) returns setof h_j_get_cmds as
$BODY$
declare
    date0 alias for $1;

    query varchar;
    r h_j_get_cmds%rowtype;
begin

    query := '
        select
            cmd.id,
            cmd.id_dl_from,
            d1.name as dl_from_name,
            cmd.id_dl_executor,
            d2.name as dl_executor_name,
            e.name as executor_name,
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
            cmd.id_io_object_att

       from command_journal cmd,
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
            and u.id_organization = e.id';
    if (date0 is not null) then
        query := query || ' and date(cmd.exec_datetime) = ' || quote_literal (date0);
    end if;
    raise notice '%', query;
    for r in execute query
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
