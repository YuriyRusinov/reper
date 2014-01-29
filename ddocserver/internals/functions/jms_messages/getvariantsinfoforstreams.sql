select f_safe_drop_type('h_get_v_info_stream');
create type h_get_v_info_stream as(id int4, name varchar);

create or replace function getVariantsInfoForStreams() returns setof h_get_v_info_stream as
$BODY$
declare
    --idStreamParams alias for $1;
    r h_get_v_info_stream % rowtype;
begin

    for r in 
        select distinct v.id, v.name
        from 
            processing_variant v 
            inner join message_streams mstr on (v.id = mstr.id_processing_variant)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
