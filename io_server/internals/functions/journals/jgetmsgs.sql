select f_safe_drop_type('h_j_get_msgs');
create type h_j_get_msgs as(
                              id int4,
                              id_dl_sender int4,
                              dl_sender_name varchar,
                              id_dl_receiver int4,
                              dl_receiver_name varchar,
                              id_io_object int4,
                              object_name varchar,
                              id_io_category int4,
                              category_name varchar,
                              insert_time timestamp,
                              sent_datetime timestamp,
                              read_datetime timestamp,
                              message_body varchar,
                              msg_type int4,
                              id_urgency_level int4,
                              receive_datetime timestamp,
                              input_number varchar,
                              output_number varchar
                              );

/*
create or replace function jGetInMsgs(int4, date) returns setof h_j_get_msgs as
$BODY$
declare
    r h_j_get_msgs%rowtype;
    query varchar;
    insertTime timestamp;
    
    idDlReceiver alias for $1;
    insertDate alias for $2;
begin

    if(idDlReceiver isnull) then
        return;
    end if;

    query = ' select * from jGetInMsgs1(' || idDlReceiver || ', ' || asString(insertDate, true) || ')';
    query = query || ' UNION ';
    query = query || ' select * from jGetInMsgs2(' || idDlReceiver || ', ' || asString(insertDate, true) || ')';
    
    --raise notice 'THE QUERY = %', query;

    for r in execute query
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function jGetInMsgs1(int4, date) returns setof h_j_get_msgs as
$BODY$
declare
    r h_j_get_msgs%rowtype;
    query varchar;
    insertTime timestamp;
    
    idDlReceiver alias for $1;
    insertDate alias for $2;
begin

    if(idDlReceiver isnull) then
        return;
    end if;

    query = '
         select 
            msg.id, 
            msg.id_dl_sender, 
            d1.name as dl_sender_name, 
            msg.id_dl_receiver, 
            d2.name as dl_receiver_name, 
            msg.id_io_object,
            io.name as object_name,
            io.id_io_category,
            c.name as category_name, 
            msg.insert_time, 
            msg.sent_datetime, 
            msg.read_datetime, 
            msg.message_body, 
            1 as msg_type,
            msg.id_urgency_level,
            msg.receive_datetime
        from 
            message_journal msg, 
            "position" d1, 
            "position" d2, 
            io_objects io,
            io_categories c
        where 
            msg.id_io_object is not null
            and msg.id_io_object = io.id
            and io.id_io_category = c.id 
            and msg.id_dl_sender = d1.id 
            and msg.id_dl_receiver = d2.id 
            and msg.id_dl_receiver = ' || idDlReceiver;
    
    if(insertDate is not null) then
        query := query || ' and msg.insert_time >= ' || quote_literal(insertDate + time '00:00:00');
        query := query || ' and msg.insert_time <= ' || quote_literal(insertDate + time '23:59:59');
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

create or replace function jGetMsgsEx(date, date) returns setof h_j_get_msgs as
$BODY$
declare
    r h_j_get_msgs%rowtype;
    query varchar;

    idCurrentDl int4;
    
    insertDateStart alias for $1;
    insertDateStop alias for $2;
begin

    idCurrentDl = getCurrentDl();
    if(idCurrentDl isnull) then
        return;
    end if;

    query = '
         select 
            msg.id, 
            msg.id_dl_sender, 
            d1.name as dl_sender_name, 
            msg.id_dl_receiver, 
            d2.name as dl_receiver_name, 
            msg.id_io_object,
            io.name as object_name,
            io.id_io_category,
            c.name as category_name, 
            msg.insert_time, 
            msg.sent_datetime, 
            msg.read_datetime, 
            msg.message_body, 
            (case when id_dl_receiver = ' || idCurrentDl || ' then 1 -- на входящее
                  when id_dl_sender = ' || idCurrentDl || ' then 2  --исходящие
                  else 1 end) as msg_type,  --входящие
            msg.id_urgency_level,
            msg.receive_datetime,
            msg.input_number,
            msg.output_number
        from 
            message_journal msg left join tbl_io_objects io on io.id = msg.id_io_object left join io_categories c on c.id = io.id_io_category,
            "position" d1, 
            "position" d2
        where 
            msg.id_dl_sender = d1.id 
            and msg.id_dl_receiver = d2.id 
            --and (case when msg.id_io_object is not null then true = getPrivilege(getCurrentUser(), msg.id_io_object, 1, true) else 1 = 1 end)
            and (msg.id_dl_receiver = ' || idCurrentDl || ' or msg.id_dl_sender = ' || idCurrentDl || ' )';
    
    if(insertDateStart is not null) then
        query := query || ' and msg.insert_time >= ' || quote_literal(insertDateStart + time '00:00:00');
    end if;

    if(insertDateStop is not null) then
        query := query || ' and msg.insert_time <= ' || quote_literal(insertDateStop + time '23:59:59');
    end if;

    query = query || ' order by msg_type, read_datetime desc';
    
    for r in execute query
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql' security definer;

create or replace function jGetMsg(int4) returns setof h_j_get_msgs as
$BODY$
declare
    r h_j_get_msgs%rowtype;
    query varchar;

    idCurrentDl int4;
    
    idMsg alias for $1;
begin

    idCurrentDl = getCurrentDl();
    if(idCurrentDl isnull) then
        return;
    end if;

    query = '
         select 
            msg.id, 
            msg.id_dl_sender, 
            d1.name as dl_sender_name, 
            msg.id_dl_receiver, 
            d2.name as dl_receiver_name, 
            msg.id_io_object,
            io.name as object_name,
            io.id_io_category,
            c.name as category_name, 
            msg.insert_time, 
            msg.sent_datetime, 
            msg.read_datetime, 
            msg.message_body, 
            (case when id_dl_receiver = ' || idCurrentDl || ' then 1 -- на входящее
                  when id_dl_sender = ' || idCurrentDl || ' then 2  --исходящие
                  else 1 end) as msg_type,  --входящие
            msg.id_urgency_level,
            msg.receive_datetime,
            msg.input_number,
            msg.output_number
        from 
            message_journal msg left join tbl_io_objects io on io.id = msg.id_io_object left join io_categories c on c.id = io.id_io_category,
            "position" d1, 
            "position" d2
        where 
            msg.id_dl_sender = d1.id 
            and msg.id_dl_receiver = d2.id 
            --and (case when msg.id_io_object is not null then true = getPrivilege(getCurrentUser(), msg.id_io_object, 1, true) else 1 = 1 end )
            and msg.id = ' || idMsg || '
            and (msg.id_dl_receiver = ' || idCurrentDl || ' or msg.id_dl_sender = ' || idCurrentDl || ' )';
    
    for r in execute query
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql' security definer;

/*
create or replace function jGetInMsgs2(int4, date) returns setof h_j_get_msgs as
$BODY$
declare
    r h_j_get_msgs%rowtype;
    query varchar;
    insertTime timestamp;
    
    idDlReceiver alias for $1;
    insertDate alias for $2;
begin

    if(idDlReceiver isnull) then
        return;
    end if;

    query = '
        select 
            msg.id, 
            msg.id_dl_sender, 
            d1.name as dl_sender_name, 
            msg.id_dl_receiver, 
            d2.name as dl_receiver_name, 
            msg.id_io_object,
            NULL as object_name,
            NULL as id_io_category, 
            NULL as category_name, 
            msg.insert_time, 
            msg.sent_datetime, 
            msg.read_datetime, 
            msg.message_body, 
            1 as msg_type,
            msg.id_urgency_level,
            msg.receive_datetime 
        from 
            message_journal msg, 
            "position" d1, 
            "position" d2
        where 
            msg.id_io_object isnull
            and msg.id_dl_sender = d1.id 
            and msg.id_dl_receiver = d2.id 
            and msg.id_dl_receiver = ' || idDlReceiver;
    
    if(insertDate is not null) then
        query := query || ' and msg.insert_time >= ' || quote_literal(insertDate + time '00:00:00');
        query := query || ' and msg.insert_time <= ' || quote_literal(insertDate + time '23:59:59');
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
/*
create or replace function jGetOutMsgs(int4, date) returns setof h_j_get_msgs as
$BODY$
declare
    r h_j_get_msgs%rowtype;
    query varchar;
    insertTime timestamp;
    
    idDlSender alias for $1;
    insertDate alias for $2;
begin

    if(idDlSender isnull) then
        return;
    end if;

    query = ' select * from jGetOutMsgs1(' || idDlSender || ', ' || asString(insertDate, true) || ')';
    query = query || ' UNION ';
    query = query || ' select * from jGetOutMsgs2(' || idDlSender || ', ' || asString(insertDate, true) || ')';
    
    --raise notice 'THE QUERY = %', query;

    for r in execute query
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function jGetOutMsgs1(int4, date) returns setof h_j_get_msgs as
$BODY$
declare
    r h_j_get_msgs%rowtype;
    query varchar;
    insertTime timestamp;
    
    idDlSender alias for $1;
    insertDate alias for $2;
begin

    if(idDlSender isnull) then
        return;
    end if;

    query = '
        select 
            msg.id, 
            msg.id_dl_sender, 
            d1.name as dl_sender_name, 
            msg.id_dl_receiver, 
            d2.name as dl_receiver_name, 
            msg.id_io_object,
            io.name as object_name,
            io.id_io_category, 
            c.name as category_name, 
            msg.insert_time, 
            msg.sent_datetime, 
            msg.read_datetime, 
            msg.message_body, 
            2 as msg_type,
            msg.id_urgency_level,
            msg.receive_datetime
        from 
            message_journal msg, 
            "position" d1, 
            "position" d2, 
            io_objects io,
            io_categories c
        where 
            msg.id_io_object is not null 
            and msg.id_io_object = io.id
            and io.id_io_category = c.id
            and msg.id_dl_sender = d1.id 
            and msg.id_dl_receiver = d2.id 
            and msg.id_dl_sender = ' || idDlSender;
    
    if(insertDate is not null) then
        query := query || ' and msg.insert_time >= ' || quote_literal(insertDate + time '00:00:00');
        query := query || ' and msg.insert_time <= ' || quote_literal(insertDate + time '23:59:59');
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

create or replace function jGetOutMsgs2(int4, date) returns setof h_j_get_msgs as
$BODY$
declare
    r h_j_get_msgs%rowtype;
    query varchar;
    insertTime timestamp;
    
    idDlSender alias for $1;
    insertDate alias for $2;
begin

    if(idDlSender isnull) then
        return;
    end if;

    query = '
        select 
            msg.id, 
            msg.id_dl_sender, 
            d1.name as dl_sender_name, 
            msg.id_dl_receiver, 
            d2.name as dl_receiver_name, 
            msg.id_io_object,
            NULL as object_name,
            NULL as id_io_category, 
            NULL as category_name, 
            msg.insert_time, 
            msg.sent_datetime, 
            msg.read_datetime, 
            msg.message_body, 
            2 as msg_type,
            msg.id_urgency_level,
            msg.receive_datetime 
        from 
            message_journal msg, 
            "position" d1, 
            "position" d2
        where 
            msg.id_io_object isnull
            and msg.id_dl_sender = d1.id 
            and msg.id_dl_receiver = d2.id 
            and msg.id_dl_sender = ' || idDlSender;
    
    if(insertDate is not null) then
        query := query || ' and msg.insert_time >= ' || quote_literal(insertDate + time '00:00:00');
        query := query || ' and msg.insert_time <= ' || quote_literal(insertDate + time '23:59:59');
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
