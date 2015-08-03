select f_safe_drop_type('h_get_io_info_chains');
create type h_get_io_info_chains as(id int4, name varchar);

create or replace function getIOInfoForChains() returns setof h_get_io_info_chains as
$BODY$
declare
    --idStreamParams alias for $1;
    r h_get_io_info_chains % rowtype;
begin

    for r in 
        select distinct io.id, io.name
        from 
            io_objects io 
            inner join chains_data d on (io.id=d.id_io_object)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
