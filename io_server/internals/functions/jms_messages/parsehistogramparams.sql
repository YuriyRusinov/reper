create or replace function parseHistogramParams(int4, varchar) returns int4[] as
$BODY$
declare
    idAttrValue alias for $1;
    key alias for $2;

    mainStr varchar;

    vList int4[];
begin

    select value into mainStr from tbl_attrs_values where id = idAttrValue;

    select parseHistogramParams(mainStr, key) into vList;

    return vList;
 end
 $BODY$
 language 'plpgsql' security definer;

 
create or replace function parseHistogramParams(varchar, varchar) returns int4[] as
$BODY$
declare
    mainStr alias for $1;
    key alias for $2;

    iStart int4;
    iEnd int4;
    str varchar;

    vList int4[];
begin

    select position(key || ' begins' in mainStr) into iStart;
    select position(key || ' end' in mainStr) into iEnd;

    select substring ( mainStr from iStart for (iEnd - iStart) ) into str;

    select position('^~^~^' in str)+5 into iStart;
    select substring(str from iStart) into str;

    
    select array_agg(a.a) into vList
    from 
        (select unnest(string_to_array(str, '^~^~^')) a ) as a
    where a.a is not null and a.a <> '';

    return vList;
end
$BODY$
language 'plpgsql' security definer;

/*
select parseHistogramParams(6680, 'scenario');
select parseHistogramParams(6680, 'variant');
select parseHistogramParams(6680, 'categories');
select parseHistogramParams(6680, 'IO');
select parseHistogramParams(6680, 'services');
select parseHistogramParams(6680, 'part_lows');
*/

