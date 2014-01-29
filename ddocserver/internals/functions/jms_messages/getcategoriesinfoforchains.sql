select f_safe_drop_type('h_get_cat_info_chains');
create type h_get_cat_info_chains as(id int4, name varchar);

create or replace function getCategoriesInfoForChains() returns setof h_get_cat_info_chains as
$BODY$
declare
    --idStreamParams alias for $1;
    r h_get_cat_info_chains % rowtype;
begin

    for r in 
        select distinct c.id, c.name
        from 
            io_categories c 
            inner join io_objects io on (io.id_io_category=c.id) 
            inner join chains_data d on (io.id=d.id_io_object)
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
