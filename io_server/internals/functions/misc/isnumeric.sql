create or replace function isNumeric(text) returns bool as
$BODY$
declare
	iString alias for $1;
	ok bool;
begin

	select textregexeq(iString,E'^[[:digit:]]+(\.[[:digit:]]+)?$') into ok;
	return ok;

end
$BODY$
language 'plpgsql';

create or replace function isInteger(text) returns bool as
$BODY$
declare
	iString alias for $1;
	ok bool;
begin

	select textregexeq(iString,E'^[[:digit:]]+(\.[[:digit:]]+)?$') into ok;
	return ok;

end
$BODY$
language 'plpgsql';

create or replace function to_int4(varchar) returns int4 as 
$BODY$
declare
       i int4;
begin
       i := $1::int4;
       return i;

       EXCEPTION WHEN 
                    invalid_text_representation or 
                    numeric_value_out_of_range 
                 then
                    return NULL;
                 when OTHERS
                 then 
                    return NULL;
end;
$BODY$
language 'plpgsql' immutable strict;

create or replace function to_int8(varchar) returns int8 as 
$BODY$
declare
       i int8;
begin
       i := $1::int8;
       return i;

       EXCEPTION WHEN 
                    invalid_text_representation or 
                    numeric_value_out_of_range 
                 then
                    return NULL;
                 when OTHERS
                 then 
                    return NULL;
end;
$BODY$
language 'plpgsql' immutable strict;

create or replace function to_float8(varchar) returns float8 as 
$BODY$
declare
       i float8;
begin
       i := $1::float8;
       return i;

       EXCEPTION WHEN 
                    invalid_text_representation or 
                    numeric_value_out_of_range 
                 then
                    return NULL;
                 when OTHERS
                 then 
                    return NULL;
end;
$BODY$
language 'plpgsql' immutable strict;

create or replace function to_timestamp(varchar) returns timestamp as
$BODY$
declare
       i timestamp;
begin
       i := $1::timestamp;
       return i;

       EXCEPTION WHEN 
                    invalid_text_representation or 
--                    datetime_value_out_of_range or
                    invalid_datetime_format
                 then
                    return NULL;
                 when OTHERS
                 then 
                    return NULL;
end;
$BODY$
language 'plpgsql' immutable strict;

create or replace function to_time(varchar) returns time as
$BODY$
declare
       i time;
begin
       i := $1::time;
       return i;

       EXCEPTION WHEN 
                    invalid_text_representation or 
--                    datetime_value_out_of_range or
                    invalid_datetime_format
                 then
                    return NULL;
                 when OTHERS
                 then 
                    return NULL;
end;
$BODY$
language 'plpgsql' immutable strict;

create or replace function to_date(varchar) returns date as
$BODY$
declare
       i date;
begin
       i := $1::date;
       return i;

       EXCEPTION WHEN 
                    invalid_text_representation or 
--                    datetime_value_out_of_range or
                    invalid_datetime_format
                 then
                    return NULL;
                 when OTHERS
                 then 
                    return NULL;
end;
$BODY$
language 'plpgsql' immutable strict;
