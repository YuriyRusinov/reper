select f_safe_drop_type('h_get_partlow_info_stream');
create type h_get_partlow_info_stream as(id int4, name varchar);

create or replace function getPartLowInfoForStreams() returns setof h_get_partlow_info_stream as
$BODY$
declare
    --idStreamParams alias for $1;
    r h_get_partlow_info_stream % rowtype;
begin

    for r in 
        select distinct pl.id, pl.name
        from 
            partition_lows pl 
            inner join message_streams mstr on (pl.id = mstr.id_partition_low)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
