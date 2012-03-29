create or replace function rFindEqual (varchar, varchar, varchar) returns int4 as
$BODY$
declare
    table_name alias for $1;
    attr_code alias for $2;
    attr_value alias for $3;

    query varchar;
    r record;
    rr record;
    at_type varchar;
    i int4;
    n int4;
begin

    query := 'select id from ' || table_name || ' where ';
    if (lower (table_name) = 'organization') then
        query := query || 'email_prefix = ' || quote_literal (attr_value);
    else
        query := query || 'unique_id = ' || quote_literal (attr_value);
    end if;
/*    n := array_upper (attrs_code, 1);
    for i in 1..n loop
        for rr in
            select at.code from attributes a inner join a_types at on (a.id_a_type=at.id and a.code=attrs_code[i])
        loop
            at_type := rr.code;
        end loop;
        if (attrs_code[i]='id') then
            continue;
        end if;
        query := query || attrs_code[i] || '=';
        if (at_type = 'VARCHAR') then
            query := query || quote_literal (attrs_values[i]);
        elsif (at_type = 'TIMESTAMP' or at_type = 'DATE') then
            query := query || quote_literal (attrs_values[i]) || '::timestamp';
        else
            query := query || attrs_values[i];
        end if;
        if (i < n) then
            query := query || ' and ';
        end if;
    end loop;
*/
    for r in
        execute query
    loop
        return r.id;
    end loop;

    return -1;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';
