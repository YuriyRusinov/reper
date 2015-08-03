select f_safe_drop_type('h_get_io_info_stream');
create type h_get_io_info_stream as(id int4, name varchar);

create or replace function getIOInfoForStreams() returns setof h_get_io_info_stream as
$BODY$
declare
    --idStreamParams alias for $1;
    r h_get_io_info_stream % rowtype;
begin

    for r in 
        select distinct io.id, io.name
        from 
            io_objects io 
            inner join message_streams mstr on (io.id = mstr.id_io_object)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
