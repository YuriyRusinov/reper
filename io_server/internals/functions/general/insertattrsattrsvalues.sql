--select f_safe_drop_type('h_attr_attr_value');
--create type h_attr_attr_value as (id_attr_attr int4, a_val varchar);

create or replace function insertAttrsAttrsValues(int4, varchar) returns int4 as
$BODY$
declare
    idAttrValue alias for $1;
    iValue alias for $2;

    idAttrAttrValue int8;
    r h_attr_attr_value%rowtype;
    ok boolean;
    str varchar;
    realVal varchar;
begin

    for r in select id_attr_attr, a_val from parseAttrAttrValue(iValue)
    loop
        ok = verifyAttrAttr(idAttrValue, r.id_attr_attr);--verify that idAttrValue can contain value of current idAttrAttr
        if(ok isnull or ok <> true) then
            raise exception 'Incorrect data in attr_attr value for the given idAttrValue';
            return -1;
        end if;

        execute 'select $1' into realVal using r.a_val;
        if(realVal isnull) then
            
            continue;
        end if;

        if(lower(r.a_val) <> 'null' and lower(r.a_val) <> 'null::bigint' and r.a_val <> '') then
            select getNextSeq('attrs_attrs_values', 'id') into idAttrAttrValue;
        
            str = 'insert into attrs_attrs_values (id, id_attr_value, id_attr_attr, value) values (' || idAttrAttrValue || ', ' || idAttrValue || ', ' || r.id_attr_attr || ', ' || r.a_val || ')';
            execute str;
        end if;
    end loop;
    
    return 1;

end
$BODY$
language 'plpgsql';


create or replace function verifyAttrAttr(int4, int4) returns boolean as
$BODY$
declare
    idAttrValue alias for $1;
    idAttrAttr alias for $2;

    idAttrCat int4;
begin

    select av.id_attr_category into idAttrCat 
    from 
        attrs_values av,
        attrs_categories ac,
        attributes a,
        attrs_attrs aa
    where
        av.id = idAttrValue
        and av.id_attr_category = ac.id
        and ac.id_io_attribute = a.id
        and a.id = aa.id_attr_parent
        and aa.id = idAttrAttr;

    if(idAttrCat is null) then
        return false;
    end if;

    return true;
end
$BODY$
language 'plpgsql';


-- in insertrecattrsattrsvalues.sql

--format: id~~~value^~^~^id~~~value^~^~^id~~~value ...
/*create or replace function parseAttrAttrValue(varchar) returns setof h_attr_attr_value as
$BODY$
declare
    iVal alias for $1;
    r h_attr_attr_value%rowtype;
begin

    for r in 
        select b.id_val[1], b.id_val[2] 
        from 
        (
            select string_to_array(a.attr, '~~~') as id_val
            from 
            (
                    select unnest(string_to_array(iVal, '^~^~^')) as attr 
            ) as a 
        ) as b
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
*/

