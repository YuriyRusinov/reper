create or replace function ioSearchQuery (varchar, int4, varchar) returns varchar as
$BODY$
declare
    ioValue alias for $1;
    ioAttrId alias for $2;
    oper alias for $3;

    isLike boolean;
    isNumeric boolean;
    query varchar;
    r_query varchar;
    type_query varchar;
    type_code varchar;
    p_str varchar;
    val_arr int4[];
    i int4;
    r RECORD;
begin

    type_query := 'select at.code from a_types at inner join attributes a on (at.id=a.id_a_type and a.id=' || ioAttrId || ' );';

    for r in execute type_query
    loop
         type_code := r.code;
    end loop;
    if (not FOUND) then
        return null;
    end if;

    isNumeric := type_code = 'INT4' or type_code = 'FLOAT8';

    if (not isNumeric and is_float8 (ioValue) or isNumeric and not is_float8 (ioValue)) then
        return null;
    end if;

    isLike := (lower (oper) = 'like');

    query := 'select av.id_io_object from attributes a inner join attrs_categories ac on (ac.id_io_attribute = a.id and a.id = ';
    query := query || ioAttrId || ' )';
    query := query || ' inner join attrs_values av on (';

    if (is_int4 (ioValue)) then
        query := query || ' is_int4 (av.value) and int4 (av.value) ' || oper || ' ';
        query := query || int4 (ioValue);
    elsif (is_float8 (ioValue)) then
        query := query || ' is_float8 (av.value) and float8 (av.value) ' || oper || ' ' ;
        query := query || float8 (ioValue);
    elsif (is_int4 (ioValue)) then
        query := query || ' is_int4 (av.value) and int4 (av.value) ' || oper || ' ';
        query := query || int4 (ioValue);
    elsif (is_array_int4 (ioValue)) then
        query := query || ' is_array_int4 (av.value) and ';
        if (oper = '=' or oper = '!=' or oper = '<>') then
            query := query || 'string_to_array (substring (av.value, 2, char_length (av.value)-2),' || quote_literal(',') || ')::int4[] ' || oper || ' ';
            p_str := substring (ioValue, 2, char_length (ioValue)-2);
            --RAISE NOTICE '%', p_str;
            select into val_arr string_to_array (p_str, '')::int4[];

            RAISE WARNING '% %', val_arr, query;
            query := query || ' ARRAY[' || array_to_string (val_arr, '') || ' ]';
        else
            p_str := substring (ioValue, 2, char_length (ioValue)-2);
            select into val_arr string_to_array (p_str, '')::int4[];
            for i in 1..array_upper (val_arr, 1)
            loop
                query := query || ' ' || val_arr[i];
                if (oper = 'in') then
                    query := query || ' = any (' || 'string_to_array (substring (av.value, 2, char_length (av.value)-2),' || quote_literal(',') || ')::int4[]) ';
                    if (i <= array_upper (val_arr, 1)-1) then
                        query := query || ' and';
                    end if;
                elsif (oper = 'not in') then
                    query := query || ' <> all (' || 'string_to_array (substring (av.value, 2, char_length (av.value)-2),' || quote_literal(',') || ')::int4[]) ';
                    if (i <= array_upper (val_arr, 1)-1) then
                        query := query || ' and';
                    end if;
                end if;
            end loop;
        end if;
    elsif(is_boolean(ioValue)) then
        query := query || 'upper(av.value) ' || oper || ' ';
        query := query || 'upper(' || quote_literal(ioValue) || ')';
    else
        query := query || ' av.value ' || oper || ' ';
        query := query || quote_literal (ioValue);
    end if;

    query := query || ' and av.id_attr_category = ac.id);';
    r_query := query;
    RAISE WARNING '%', r_query;

    return r_query;

end
$BODY$
language 'plpgsql';


create or replace function ioSearch(varchar, int4, varchar) returns setof int4 as
$BODY$
declare
    ioValue alias for $1;
    ioAttrId alias for $2;
    oper alias for $3;

    isLike boolean;
    isNumeric boolean;
    query varchar;
    type_query varchar;
    type_code varchar;
    p_str varchar;
    val_arr int4[];
    i int4;
    r RECORD;
begin

    type_query := 'select at.code from a_types at inner join attributes a on (at.id=a.id_a_type and a.id=' || ioAttrId || ' );';

    for r in execute type_query
    loop
         type_code := r.code;
    end loop;
    if (not FOUND) then
        return;
    end if;

    isNumeric := type_code = 'INT4' or type_code = 'FLOAT8';

raise warning 'asasas';
    --if (not isNumeric and is_float8 (ioValue) or isNumeric and not is_float8 (ioValue)) then
    if (isNumeric and not is_float8 (ioValue)) then
        return;
    end if;

    isLike := (lower (oper) = 'like');

    query := 'select av.id_io_object from attributes a inner join attrs_categories ac on (ac.id_io_attribute = a.id and a.id = ';
    query := query || ioAttrId || ' )';
    query := query || ' inner join attrs_values av on (';

    if (is_int4 (ioValue)) then
        query := query || ' is_int4 (av.value) and int4 (av.value) ' || oper || ' ';
        query := query || int4 (ioValue);
    elsif (is_float8 (ioValue)) then
        query := query || ' is_float8 (av.value) and float8 (av.value) ' || oper || ' ' ;
        query := query || float8 (ioValue);
    elsif (is_int4 (ioValue)) then
        query := query || ' is_int4 (av.value) and int4 (av.value) ' || oper || ' ';
        query := query || int4 (ioValue);
    elsif (is_array_int4 (ioValue)) then
        query := query || ' is_array_int4 (av.value) and ';
        if (oper = '=' or oper = '!=' or oper = '<>') then
            query := query || 'string_to_array (substring (av.value, 2, char_length (av.value)-2),' || quote_literal(',') || ')::int4[] ' || oper || ' ';
            p_str := substring (ioValue, 2, char_length (ioValue)-2);
            RAISE WARNING '%', p_str;
            select into val_arr string_to_array (p_str, '')::int4[];

            RAISE WARNING '% %', val_arr, query;
            query := query || ' ARRAY[' || array_to_string (val_arr, '') || ' ]';
        else
            p_str := substring (ioValue, 2, char_length (ioValue)-2);
            select into val_arr string_to_array (p_str, '')::int4[];
            for i in 1..array_upper (val_arr, 1)
            loop
                query := query || ' ' || val_arr[i];
                if (oper = 'in') then
                    query := query || ' = any (' || 'string_to_array (substring (av.value, 2, char_length (av.value)-2),' || quote_literal(',') || ')::int4[]) ';
                    if (i <= array_upper (val_arr, 1)-1) then
                        query := query || ' and';
                    end if;
                elsif (oper = 'not in') then
                    query := query || ' <> all (' || 'string_to_array (substring (av.value, 2, char_length (av.value)-2),' || quote_literal(',') || ')::int4[]) ';
                    if (i <= array_upper (val_arr, 1)-1) then
                        query := query || ' and';
                    end if;
                end if;
            end loop;
        end if;
    elsif(is_boolean(ioValue)) then
        query := query || 'upper(av.value) ' || oper || ' ';
        query := query || 'upper(' || quote_literal(ioValue) || ')';
    else
        query := query || ' av.value ' || oper || ' ';
        query := query || quote_literal (ioValue);
    end if;

    query := query || ' and av.id_attr_category = ac.id);';
    RAISE WARNING '%', query;

    for r in 
        execute query
    loop
        return next r.id_io_object;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
/*
create or replace function ioSearch(varchar, varchar, varchar) returns setof int4 as
$BODY$
declare
    ioValue alias for $1;
    ioAttrCode alias for $2;
    oper alias for $3;

    ioAttrId int4;
    attr_query varchar;
    r RECORD;
    rres RECORD;
begin

    attr_query := 'select id from attributes a where a.code=' || quote_literal (ioAttrCode);
    for r in execute attr_query
    loop
        ioAttrId := r.id;
    end loop;
    RAISE NOTICE '%', ioAttrId;
    if (not FOUND) then
        return;
    end if;

    for rres in 
        select * from ioSearch (ioValue, ioAttrId, oper) as id
    loop
        return next rres.id;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
*/

create or replace function ioSearch(varchar, varchar) returns setof int4 as
$BODY$
declare
    ioValue alias for $1;
    oper alias for $2;

    isLike boolean;
    isDouble boolean;

    query varchar;
    r RECORD;
begin

    isLike := (oper = 'like');
    isDouble := is_float8 (ioValue);

    RAISE NOTICE '%', isDouble;

    query := 'select distinct io.id from io_objects io inner join (attrs_values av inner join attributes a on (av.id_io_attribute = a.id )) on (';
    if (isDouble and not isLike) then
        query := query || 'io.id ' || oper || ioValue || ' or ';
    end if;
    query := query || '(io.id=av.id_io_object and (a.table_name isnull and a.column_name isnull and ';
    --av.id_io_object from attributes a inner join attrs_values av on (av.id_io_attribute = a.id and (a.table_name isnull and a.column_name isnull and ';

    if (isDouble) then
        query := query || 'is_float8 (av.value) and float8 (av.value) ';
    else
        query := query || 'av.value ';
    end if;

    query := query || oper || ' ' ;

    if (isDouble) then
        query := query || ioValue || '::float8 )';
    else
        query := query || quote_literal (ioValue) || ' )';
    end if;

    query := query || ' or a.table_name is not null and a.column_name is not null and av.value in (select av1.value from attrs_values av1 inner join attributes a1 on (a1.id=av1.id_io_attribute and a1.table_name is not null and a1.column_name is not null and matchAttrValue (a1.id, av1.value, ' || asString (ioValue, true) || ' ,' || asString (oper, true) || ' )))))';

    RAISE NOTICE '%', query;

    for r in 
        execute query
    loop
        return next r.id;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
