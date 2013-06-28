select f_safe_drop_type('h_attr_attr_value');
create type h_attr_attr_value as (id_attr_attr int4, a_val varchar);

create or replace function insertRecAttrsAttrsValues(int8, varchar) returns int4 as
$BODY$
declare
    idRecAttrValue alias for $1;
    iValue alias for $2;

    idRecAttrAttrValue int8;
    r h_attr_attr_value%rowtype;
    ok boolean;
    str varchar;
begin

    for r in select id_attr_attr, a_val from parseAttrAttrValue(iValue)
    loop
        ok = verifyRecAttrAttr(idRecAttrValue, r.id_attr_attr);--verify that idRecAttrValue can contain value of current idAttrAttr
        if(ok isnull or ok <> true) then
            raise exception 'Incorrect data in attr_attr value for the given idRecAttrValue';
            return -1;
        end if;

        if(r.a_val <> 'NULL' and r.a_val <> '') then
            select getNextSeq('rec_attrs_attrs_values', 'id') into idRecAttrAttrValue;

            str = 'insert into rec_attrs_attrs_values (id, id_rec_attr_value, id_attr_attr, value) values (' || idRecAttrAttrValue || ', ' || idRecAttrValue || ', ' || r.id_attr_attr || ', ' || r.a_val || ')';
            execute str;
        end if;
    end loop;
    
    return 1;

end
$BODY$
language 'plpgsql';


--format: id~~~value^~^~^id~~~value^~^~^id~~~value ...
create or replace function parseAttrAttrValue(varchar) returns setof h_attr_attr_value as
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

create or replace function verifyRecAttrAttr(int8, int4) returns boolean as
$BODY$
declare
    idRecAttrValue alias for $1;
    idAttrAttr alias for $2;

    idAttrCat int4;
begin

    select rav.id_attr_category into idAttrCat 
    from 
        rec_attrs_values rav,
        attrs_categories ac,
        attributes a,
        attrs_attrs aa
    where
        rav.id = idRecAttrValue
        and rav.id_attr_category = ac.id
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

