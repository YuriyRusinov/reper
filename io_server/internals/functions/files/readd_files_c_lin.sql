
create or replace function exec_spi(varchar) returns varchar 
    as '$libdir/libfloader.so', 'exec_spi'  language 'c' strict;

create or replace function rGetFile(int4, int4, int8) returns bytea 
    as '$libdir/libfloader.so', 'rgetfile'  language 'c' strict security definer;

create or replace function rGetFileByUrl(varchar, int4, int8) returns bytea 
    as '$libdir/libfloader.so', 'rgetfilebyurl'  language 'c' strict security definer;

create or replace function rGetFileSizeByUrl(varchar) returns int8
    as '$libdir/libfloader.so', 'rgetfilesizebyurl'  language 'c' strict security definer;

create or replace function rWriteFile(int4, bytea, int4) returns int4 
    as '$libdir/libfloader.so', 'rwritefile'  language 'c' strict security definer;

create or replace function rGetAbsUrlEx(int4) returns text 
    as '$libdir/libfloader.so', 'rgetabsurlex'  language 'c' security definer;

create or replace function rDeleteFile(int4) returns int4
    as '$libdir/libfloader.so', 'rdeletefile'  language 'c' strict security definer;

create or replace function rXPM2JPEG(bytea) returns bytea 
    as '$libdir/libfloader.so', 'rxpm2jpeg'  language 'c' strict security definer;

create or replace function initrand (bigint) returns int4
    as '$libdir/libfloader.so', 'initrand' language 'c' strict security definer;

create or replace function droprand () returns int4
    as '$libdir/libfloader.so', 'droprand' language 'c' strict security definer;

create or replace function unirand () returns float8
    as '$libdir/libfloader.so', 'unirand' language 'c' strict security definer;

create or replace function gaussrand (float8) returns float8
    as '$libdir/libfloader.so', 'gaussrand' language 'c' strict security definer;

create or replace function exprand (float8) returns float8
    as '$libdir/libfloader.so', 'exprand' language 'c' strict security definer;

create or replace function saverand() returns int4
    as '$libdir/libfloader.so', 'saverand' language 'c' strict security definer;

create or replace function loadrand() returns int4
    as '$libdir/libfloader.so', 'loadrand' language 'c' strict security definer;

--create or replace function square_image(bytea) returns float8
--    as '$libdir/libfloader.so', 'square_image' language 'c' strict security definer;

select f_safe_drop_type('hist_point');
create type hist_point as (inum bigint, bin float8);

create or replace function histogram (varchar, float8, float8, int4) returns setof hist_point
    as '$libdir/libfloader.so', 'histogram' language 'c' strict security definer;
/*
create or replace function testargs(varchar) returns int4
    as '$libdir/libfloader.so', 'testargs' language 'c' strict security definer;

create or replace function testspi(text,boolean,int) returns int4
    as '$libdir/libfloader.so','testspi' language 'c' strict;
*/

create or replace function rGetHashFile(varchar) returns varchar
    as '$libdir/libfloader.so', 'rgethashfile' language 'c' strict security definer;
