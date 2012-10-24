create or replace function is_float8 (varchar) returns boolean as
$BODY$
declare
    value alias for $1;

    query varchar;
    doubleRegExp varchar;
    isValDouble boolean;
    r RECORD;
begin
    doubleRegExp := E'\^(-?(\[0-9\]+(\\.\[0-9\]\*(e-?\[0-9\]+)?)?\|\\.\[0-9\]+(e-?\[0-9\]+)?)?)\$';
    query := 'select substring (' || quote_literal(value) || ',' || quote_literal (doubleRegExp) || ') is not null as isdoubleval;';
    isValDouble := false;
    for r in execute query
    loop
        isValDouble := r.isdoubleval;
    end loop;

    return isValDouble;

end
$BODY$
language 'plpgsql';

create or replace function is_int4 (varchar) returns boolean as
$BODY$
declare
    value alias for $1;

    query varchar;
    intRegExp varchar;
    isValInt boolean;
    r RECORD;
begin
    intRegExp := E'\^(-?(\[0-9\]+))\$';
    query := 'select substring (' || quote_literal(value) || ',' || quote_literal (intRegExp) || ') is not null as isintval;';
    isValInt := false;
    for r in execute query
    loop
        isValInt := r.isintval;
    end loop;

    return isValInt;

end
$BODY$
language 'plpgsql';

create or replace function is_int8 (varchar) returns boolean as
$BODY$
declare
    value alias for $1;

    query varchar;
    intRegExp varchar;
    isValInt boolean;
    r RECORD;
begin
    intRegExp := E'\^(-?(\[0-9\]+))\$';
    query := 'select substring (' || quote_literal(value) || ',' || quote_literal (intRegExp) || ') is not null as isintval;';
    isValInt := false;
    for r in execute query
    loop
        isValInt := r.isintval;
    end loop;

    return isValInt;

end
$BODY$
language 'plpgsql';

create or replace function is_array_int4 (varchar) returns boolean as
$BODY$
declare
    value alias for $1;

    query varchar;
    b_str varchar;
    e_str varchar;
    isArr boolean;
    reg_exp varchar;
    p_arr varchar[];
    p_str varchar;
    w varchar;
    i int4;
    r RECORD;
begin
    reg_exp := E'\^(\{+)';
    query := 'select substring (' || quote_literal (value) || ',' || quote_literal (reg_exp) || ') as bstr;';
    for r in execute query
    loop
        b_str := r.bstr;
    end loop;
    RAISE NOTICE '%', b_str;
    isArr := b_str is not null and char_length (b_str) = 1;
    if (not isArr) then
        return isArr;
    end if;

    reg_exp := E'(\}+\$)';
    query:= 'select substring (' || quote_literal (value) || ',' || quote_literal (reg_exp) || ') as estr;';
    for r in execute query
    loop
        e_str := r.estr;
    end loop;
    RAISE NOTICE '%', e_str;
    isArr := isArr and e_str is not null and char_length (e_str) = 1;
    if (not isArr) then
        return isArr;
    end if;
    query := 'select substring (' || quote_literal (value) || ' from position (' || quote_literal (b_str) || 'in' || quote_literal (value) || ')+1 for position (' || quote_literal (e_str) || ' in ' ||  quote_literal (value) || ')-2 ) as str';
    RAISE NOTICE '%', query;
    for r in execute query
    loop
        p_str := r.str;
    end loop;
    isArr := isArr and p_str is not null;
    if (not isArr) then
        return isArr;
    end if;
    query := 'select string_to_array (' || quote_literal (p_str) || ', '''') as arr';
    RAISE NOTICE '%', query;
    for r in execute query
    loop
        p_arr := r.arr;
    end loop;
    RAISE NOTICE '%', p_arr;
    isArr := isArr and p_arr is not null and array_upper (p_arr, 1) > 0;
    i:= 1;
    while (isArr and i <= array_upper (p_arr, 1)) loop
        w := p_arr[i];
        RAISE NOTICE '%', w;
        isArr := isArr and is_int4 (p_arr[i]);
        i := i+1;
    end loop;

    return isArr;
end
$BODY$
language 'plpgsql';

create or replace function is_array_int8 (varchar) returns boolean as
$BODY$
declare
    value alias for $1;

    query varchar;
    b_str varchar;
    e_str varchar;
    isArr boolean;
    reg_exp varchar;
    p_arr varchar[];
    p_str varchar;
    w varchar;
    i int4;
    r RECORD;
begin
    reg_exp := E'\^(\{+)';
    query := 'select substring (' || quote_literal (value) || ',' || quote_literal (reg_exp) || ') as bstr;';
    for r in execute query
    loop
        b_str := r.bstr;
    end loop;
    RAISE NOTICE '%', b_str;
    isArr := b_str is not null and char_length (b_str) = 1;
    if (not isArr) then
        return isArr;
    end if;

    reg_exp := E'(\}+\$)';
    query:= 'select substring (' || quote_literal (value) || ',' || quote_literal (reg_exp) || ') as estr;';
    for r in execute query
    loop
        e_str := r.estr;
    end loop;
    RAISE NOTICE '%', e_str;
    isArr := isArr and e_str is not null and char_length (e_str) = 1;
    if (not isArr) then
        return isArr;
    end if;
    query := 'select substring (' || quote_literal (value) || ' from position (' || quote_literal (b_str) || 'in' || quote_literal (value) || ')+1 for position (' || quote_literal (e_str) || ' in ' ||  quote_literal (value) || ')-2 ) as str';
    RAISE NOTICE '%', query;
    for r in execute query
    loop
        p_str := r.str;
    end loop;
    isArr := isArr and p_str is not null;
    if (not isArr) then
        return isArr;
    end if;
    query := 'select string_to_array (' || quote_literal (p_str) || ', '''') as arr';
    RAISE NOTICE '%', query;
    for r in execute query
    loop
        p_arr := r.arr;
    end loop;

    RAISE NOTICE '%', p_arr;

    isArr := isArr and p_arr is not null and array_upper (p_arr, 1) > 0;
    i:= 1;
    while (isArr and i <= array_upper (p_arr, 1)) loop
        w := p_arr[i];
        isArr := isArr and is_int8 (p_arr[i]);
        i := i+1;
    end loop;

    return isArr;
end
$BODY$
language 'plpgsql';


create or replace function is_boolean (varchar) returns boolean as
$BODY$
declare
    value alias for $1;

    bool_reg varchar;
    retVal boolean;
begin

    bool_reg := E'(^t(r|ru|rue)?$)|(^f(a|al|als|alse)?$)';
    select into retVal value ~* bool_reg;--'(^[t](r?u?e?)*$|^[f](a?l?s?e?)*$)';
    RAISE NOTICE '%', retVal;

    return retVal;
end
$BODY$
language 'plpgsql';

create or replace function is_iso_datetime (varchar) returns boolean as
$BODY$
declare
    value alias for $1;

    datetime_reg varchar;
    retVal boolean;
begin

    datetime_reg := E'^(\[\\\\+-\]?\\\\d\{4\}(?!\\\\d\{2\}\\\\b))((-?)((0\[1-9\]|1\[0-2\])(\\\\3(\[12\]\\\\d|0\[1-9\]|3\[01\]))?|W(\[0-4\]\\\\d|5\[0-2\])-?\[1-7\]|(00\[1-9\]|0\[1-9\]\\\\d|\[12\]\\\\d\{2\}|3(\[0-5\]\\\\d|6\[1-6\])))(\[T\\\\s]((\[01\]\\\\d|2\[0-3\])((:?)\[0-5\]\\\\d)?|24\\\\:?00)?(\\\\15([0-5]\\\\d))?([zZ]|([\\\\+-])([01]\\\\d|2[0-3]):?([0-5]\\\\d)?)?)?)?$';
    RAISE NOTICE '%', datetime_reg;
--    select into retVal value ~* '^(\[\\\\+-\]?\\\\d\{4\}(?!\\\\d\{2\}\\\\b))((-?)((0\[1-9\]|1\[0-2\])(\\\\3(\[12\]\\\\d|0\[1-9\]|3\[01\]))?|W(\[0-4\]\\\\d|5\[0-2\])-?\[1-7\]|(00\[1-9\]|0\[1-9\]\\\\d|\[12\]\\\\d\{2\}|3(\[0-5\]\\\\d|6\[1-6\])))(\[T\\\\s]((\[01\]\\\\d|2\[0-3\])((:?)\[0-5\]\\\\d)?|24\\\\:?00)?(\\\\15([0-5]\\\\d))?([zZ]|([\\\\+-])([01]\\\\d|2[0-3]):?([0-5]\\\\d)?)?)?)?$';
    select into retVal value ~* datetime_reg;
    RAISE NOTICE '%', retVal;

    return retVal;
end
$BODY$
language 'plpgsql';

create or replace function is_date (varchar) returns boolean as
$BODY$
declare
    val alias for $1;
    ret date;
begin
    ret = to_date(val);
    if(ret isnull) then
        return false;
    end if;

    return true;
end
$BODY$
language 'plpgsql';

create or replace function is_time (varchar) returns boolean as
$BODY$
declare
    val alias for $1;
    ret time;
begin
    ret = to_time(val);
    if(ret isnull) then
        return false;
    end if;

    return true;
end
$BODY$
language 'plpgsql';

create or replace function is_timestamp (varchar) returns boolean as
$BODY$
declare
    val alias for $1;
    ret timestamp;
begin
    ret = to_timestamp(val);
    if(ret isnull) then
        return false;
    end if;

    return true;
end
$BODY$
language 'plpgsql';

