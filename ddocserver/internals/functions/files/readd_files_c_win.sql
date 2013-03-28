
create or replace function exec_spi(varchar) returns varchar 
    as 'libfloader.dll', 'exec_spi'  language 'c' strict;

create or replace function rGetFile(int4, int4, int8) returns bytea 
    as 'libfloader.dll', 'rgetfile'  language 'c' strict security definer;

create or replace function rGetFileByUrl(varchar, int4, int8) returns bytea 
    as 'libfloader.dll', 'rgetfilebyurl'  language 'c' strict security definer;

create or replace function rGetFileSizeByUrl(varchar) returns int8
    as 'libfloader.dll', 'rgetfilesizebyurl'  language 'c' strict security definer;

create or replace function rWriteFile(int4, bytea, int4) returns int4 
    as 'libfloader.dll', 'rwritefile'  language 'c' strict security definer;

create or replace function rGetAbsUrlEx(int4) returns text 
    as 'libfloader.dll', 'rgetabsurlex'  language 'c' security definer;

create or replace function rDeleteFile(int4) returns int4
    as 'libfloader.dll', 'rdeletefile'  language 'c' strict security definer;

--create or replace function rXPM2JPEG(bytea) returns bytea 
--    as 'libfloader.dll', 'rxpm2jpeg'  language 'c' strict security definer;

create or replace function initrand () returns int4
    as '$libdir/libfloader.so', 'initrand' language 'c' strict security definer;

create or replace function droprand () returns int4
    as '$libdir/libfloader.so', 'initrand' language 'c' strict security definer;

create or replace function unirand () returns float8
    as '$libdir/libfloader.so', 'unirand' language 'c' strict security definer;

create or replace function gaussrand () returns float8
    as '$libdir/libfloader.so', 'gaussrand' language 'c' strict security definer;

create or replace function exprand () returns float8
    as '$libdir/libfloader.so', 'exprand' language 'c' strict security definer;

