select f_safe_drop_type('h_r_get_url_types');
create type h_r_get_url_types as(id int4,   
                                 name varchar,
                                 win_app varchar,
                                 lin_app varchar,
                                 extentions int4[]);

create or replace function rGetUrlTypes() returns setof h_r_get_url_types as
$BODY$
declare
    r h_r_get_url_types%rowtype;
    theExtentions int4[];
begin

    for r in 
        select 
            ut.id, ut.name, ut.win_app, ut.lin_app, ut.extentions
        from url_types  ut
        where ut.id <> 11 --system type for embedded GIS files
        order by 2
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
