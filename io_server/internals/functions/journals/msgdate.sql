create or replace function getMessagesNumberByDate (int4, timestamp) returns int4 as
$BODY$
declare
    cur_dls alias for $1;
    date0 alias for $2;

    query varchar;
    r record;
    num int4;
begin

    query := '
            select COUNT(*) AS cnt
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
                and msg.id_dl_receiver = ' || cur_dls;

    if (date0 is not null) then
        query := query || ' and date(msg.sent_datetime) = ' || quote_literal (date0);
    end if;

    raise notice '%', query;

    for r in execute query
    loop
        num := r.cnt;
    end loop;

    return num;

end
$BODY$
language 'plpgsql';

create or replace function getMsgByDate (int4, timestamp) returns setof h_j_get_msgs as
$BODY$
declare
    cur_dls alias for $1;
    date0 alias for $2;

    query varchar;
    r h_j_get_msgs%rowtype;
begin

    query := '
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
            msg.receive_datetime, 
            msg.message_body, 
            1 as msg_type,
            msg.id_urgency_level 
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
            and msg.id_dl_receiver = ' || cur_dls;

    if (date0 is not null) then
        query := query || ' and date(msg.sent_datetime) = ' || quote_literal (date0);
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
