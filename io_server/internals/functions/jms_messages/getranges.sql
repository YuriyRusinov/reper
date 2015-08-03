select f_safe_drop_type('h_get_ranges');
create type h_get_ranges as(r_index int4, step float8);

create or replace function getRanges(float8, float8, int4) returns setof h_get_ranges as
$BODY$
declare
    v_min alias for $1;
    v_max alias for $2;
    v_count alias for $3;

    r h_get_ranges%rowtype;

    i int4;
    step float8;
begin

    if(v_count <= 0 or v_min = v_max) then
        raise exception 'Incorrect input data in getRanges() function!';
        return;
    end if;
    
    for i in 1..v_count
    loop
        step = v_min + ((v_max-v_min)/v_count)*(i-1);
        r.r_index = i-1; --counting from 0
        r.step = step; 
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
