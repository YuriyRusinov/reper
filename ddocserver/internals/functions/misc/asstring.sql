create or replace function asString(int4, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    string varchar;

begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    string := '' || num;
    
    return string;
end
$BODY$
language 'plpgsql';

create or replace function asString(int8, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    needQuote alias for $2;
    string varchar;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    string := '' || num;

    if(needQuote = TRUE) then
        return quote_literal(string);
    end if;
    
    return string;
end
$BODY$
language 'plpgsql';

create or replace function asString(int4[], boolean) returns varchar as
$BODY$
declare
    nums alias for $1;
    need_quote alias for $2;
begin
    if(nums isnull) then
	return 'NULL';
    end if;
    
    if(need_quote = true) then
        return quote_literal('{' || array_to_string(nums, ',') || '}');
    end if;

    return '{' || array_to_string(nums, ',') || '}';
    
end
$BODY$
language 'plpgsql';

create or replace function asString(float8[], boolean) returns varchar as
$BODY$
declare
    nums alias for $1;
    need_quote alias for $2;
begin
    if(nums isnull) then
	return 'NULL';
    end if;
    
    if(need_quote = true) then
        return quote_literal('{' || array_to_string(nums, ',') || '}');
    end if;

    return '{' || array_to_string(nums, ',') || '}';
end
$BODY$
language 'plpgsql';

create or replace function asString(varchar[], boolean) returns varchar as
$BODY$
declare
    nums alias for $1;
    need_quote alias for $2;
begin
    if(nums isnull) then
	return 'NULL';
    end if;
    
    if(need_quote = true) then
        return quote_literal('{' || array_to_string(nums, '~^~') || '}');
    end if;

    return '{' || array_to_string(nums, '~^~') || '}';
end
$BODY$
language 'plpgsql';

create or replace function asString1(varchar[], boolean) returns varchar as
$BODY$
declare
    nums alias for $1;
    need_quote alias for $2;
begin
    if(nums isnull) then
	return '';
    end if;
    
    if(need_quote = true) then
        return quote_literal(array_to_string(nums, ''));
    end if;

    return array_to_string(nums, '');
end
$BODY$
language 'plpgsql';

create or replace function asString(float8, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    string varchar;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    string := ' ' || num;
    return ltrim(string);
    
    return string;
end
$BODY$
language 'plpgsql';

create or replace function asString(timestamp, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    need_quote alias for $2;
    string varchar;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    string := '' || num;
    --return ltrim(string);
    if(need_quote = true) then
        return quote_literal(string);
    end if;
    
    return string;
end
$BODY$
language 'plpgsql';

create or replace function asString(date, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    need_quote alias for $2;
    string varchar;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    string := '' || num;
    --return ltrim(string);
    if(need_quote = true) then
        return quote_literal(string);
    end if;
    
    return string;
end
$BODY$
language 'plpgsql';

create or replace function asString(time, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    need_quote alias for $2;
    string varchar;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    string := '' || num;
    --return ltrim(string);
    if(need_quote = true) then
        return quote_literal(string);
    end if;
    
    return string;
end
$BODY$
language 'plpgsql';

create or replace function asString(time with time zone, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    need_quote alias for $2;
    string varchar;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    string := '' || num;
    --return ltrim(string);
    if(need_quote = true) then
        return quote_literal(string);
    end if;
    
    return string;
end
$BODY$
language 'plpgsql';

create or replace function asString(interval, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    need_quote alias for $2;
    string varchar;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    string := '' || num;
    --return ltrim(string);
    if(need_quote = true) then
        return quote_literal(string);
    end if;
    
    return string;
end
$BODY$
language 'plpgsql';


create or replace function asString(boolean, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    string varchar;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    if(num = TRUE) then
	string := 'TRUE';
    else 
	string := 'FALSE';
    end if;

    return string;
end
$BODY$
language 'plpgsql';

create or replace function asString(varchar, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    need_quote alias for $2;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    if(need_quote = true) then
        return quote_literal(num);
    end if;

    return num;
end
$BODY$
language 'plpgsql';

create or replace function asString(text, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    need_quote alias for $2;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    if(need_quote = true) then
        return quote_literal(num::varchar);
    end if;
    return num::varchar;
end
$BODY$
language 'plpgsql';
/*
create or replace function asString(geometry, boolean) returns varchar as'
declare
    num alias for $1;
    need_quote alias for $2;
    sGeom varchar;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    sGeom := asEwkt(num);

    if(need_quote = true) then
        return quote_literal(sGeom);
    end if;

    return sGeom;
end
'language 'plpgsql';
*/

create or replace function asString(BIT, boolean) returns varchar as
$BODY$
declare
    num alias for $1;
    need_quote alias for $2;
    isrid int4;
begin
    if(num isnull) then
	return 'NULL';
    end if;
    
    if(need_quote = true) then
        return quote_literal(bit_to_text(num)::varchar);
    end if;

    return bit_to_text(num)::varchar;
end
$BODY$
language 'plpgsql';
