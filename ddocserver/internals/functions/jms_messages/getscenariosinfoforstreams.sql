select f_safe_drop_type('h_get_sc_info_stream');
create type h_get_sc_info_stream as(id int4, name varchar);

create or replace function getScenariosInfoForStreams() returns setof h_get_sc_info_stream as
$BODY$
declare
    --idStreamParams alias for $1;
    r h_get_sc_info_stream % rowtype;
begin

    for r in 
        select distinct sc.id, sc.name
        from 
            processing_scenario sc 
            inner join message_streams mstr on (sc.id = mstr.id_processing_scenario)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
