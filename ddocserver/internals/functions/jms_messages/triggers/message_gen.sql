create or replace function messageGen () returns trigger as
$BODY$
declare
    id_mess_stream int4;

    cnt_mess int4;
    id_record int4;
    cnt_rec int4;

    tname varchar;
    id_object int4;
    query varchar;
    id_recs bigint[];
    id_addrlist int4[];
    xml_doc xml;
    r record;
    id_maclabel int4;
    result int4;
begin
    id_mess_stream := new.id_message_stream;

    select count(*) into cnt_mess from message_series mser where mser.id_message_stream = id_mess_stream;
    select io.table_name into tname from message_streams mstreams inner join tbl_io_objects io on (mstreams.id_io_object = io.id and mstreams.id=id_mess_stream);
    if (tname is null) then
        return null;
    end if;
    select io.id into id_object from message_streams mstreams inner join tbl_io_objects io on (mstreams.id_io_object = io.id and mstreams.id=id_mess_stream);
    query := E'select count(*) from ' || tname;
    execute query into cnt_rec;
    query := E'select id from ' || tname || E' order by id;';
    id_recs := ARRAY[]::bigint[];
    for r in
        execute query
    loop
        id_recs := id_recs || r.id::bigint;
    end loop;

    id_record := id_recs[(cnt_mess+2) % cnt_rec];
    id_maclabel := getcurrentmaclabelid ();
    if (id_maclabel is null) then
        id_maclabel := 1;
    end if;

    id_addrlist := ARRAY[]::int4[];
    for r in
        select mstr.id_dl_receiver from message_streams mstr where mstr.id=id_mess_stream
    loop
        id_addrlist := id_addrlist || r.id_dl_receiver::int4;
    end loop;
    select into xml_doc recToXML (NULL::integer, id_object, id_record, id_addrlist);
    if (xml_doc is null) then
        result := jms_schema.add_out_mes (new.unique_id, id_maclabel-1, 0, false, text(''));
    else
        result := jms_schema.add_out_mes (new.unique_id, id_maclabel-1, 0, false, text(xml_doc));
    end if;
    if (result is not null and result > 0) then
        return new;
    end if;

    return null;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger ('trgmessagegen', 'message_series');

create trigger trgmessagegen
after insert
on message_series
for each row
execute procedure messageGen ();
