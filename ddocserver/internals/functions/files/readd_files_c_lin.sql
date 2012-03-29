
create or replace function exec_spi(varchar) returns varchar 
    as '$libdir/libfloader.so', 'exec_spi'  language 'c' strict;

create or replace function rGetFile(int4, int4, int8) returns bytea 
    as '$libdir/libfloader.so', 'rgetfile'  language 'c' strict security definer;

create or replace function rGetFileByUrl(varchar, int4, int8) returns bytea 
    as '$libdir/libfloader.so', 'rgetfilebyurl'  language 'c' strict security definer;

create or replace function rGetFileSizeByUrl(varchar) returns int4
    as '$libdir/libfloader.so', 'rgetfilesizebyurl'  language 'c' strict security definer;

create or replace function rWriteFile(int4, bytea, int4) returns int4 
    as '$libdir/libfloader.so', 'rwritefile'  language 'c' strict security definer;

create or replace function rGetAbsUrlEx(int4) returns text 
    as '$libdir/libfloader.so', 'rgetabsurlex'  language 'c' security definer;

create or replace function rDeleteFile(int4) returns int4
    as '$libdir/libfloader.so', 'rdeletefile'  language 'c' strict security definer;

create or replace function rXPM2JPEG(bytea) returns bytea 
    as '$libdir/libfloader.so', 'rxpm2jpeg'  language 'c' strict security definer;

