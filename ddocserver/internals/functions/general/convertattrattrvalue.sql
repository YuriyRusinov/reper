--from unique_id to id
create or replace function convertAttrAttrValue(varchar) returns varchar as
$BODY$
declare
    srcValue alias for $1;

    dstValue varchar;
    idAttrAttr int4;
    idAttrType int4;
    r record;
    rr record;
    rrr record;
    aValue varchar;
    q varchar;
begin

    dstValue = '';

    for r in select attr_attr_unique_id, a_val from parseAttrAttrValueEx(srcValue)
    loop
        select id into idAttrAttr from attrs_attrs where unique_id = r.attr_attr_unique_id;
        if(idAttrAttr isnull) then
            continue;
        end if;
        
        aValue = r.a_val;
        
        if(char_length(dstValue) > 1) then
            dstValue = dstValue || '^~^~^';
        end if;

        for rr in select a.id_a_type, a.table_name from attributes a, attrs_attrs aa where a.id = aa.id_attr_child and aa.id = idAttrAttr
        loop
            idAttrType = rr.id_a_type;

            --verify for ref attributes. if is ref then replace unique_id in value to ID
            --note that only atList and atCheckList (old) can place in complex attributes

            if(idAttrType = 2 and rr.table_name is not null) then
                if(rr.table_name = 'organization') then
                    q = 'select id from "' || rr.table_name || '" where email_prefix = ' || quote_literal(aValue);
                else
                    q = 'select id from "' || rr.table_name || '" where unique_id = ' || quote_literal(aValue);
                end if;

                for rrr in execute q
                loop
                    aValue = rrr.id;
                end loop;
            end if;

            if(idAttrType = 12 and rr.table_name is not null) then
                if(rr.table_name = 'organization') then
                    q = 'select id from "' || rr.table_name || '" where email_prefix in (select unnest(' || quote_literal(aValue) || '::varchar[]))';
                else
                    q = 'select id from "' || rr.table_name || '" where unique_id in (select unnest(' || quote_literal(aValue) || '::varchar[]))';
                end if;

                aValue = '';
                for rrr in execute q
                loop
                    if(char_length(aValue) > 1) then
                        aValue = aValue || ',';
                    else
                        aValue = '{';
                    end if;

                    aValue = aValue || rrr.id;
                end loop;

                aValue = aValue || '}';
            end if;
        end loop;

        dstValue = dstValue || idAttrAttr || '~~~' || aValue;
    end loop;

    return dstValue;
end
$BODY$
language 'plpgsql';

--from id to unique_id
create or replace function convertAttrAttrValue1(varchar) returns varchar as
$BODY$
declare
    srcValue alias for $1;

    dstValue varchar;
    uidAttrAttr varchar;
    idAttrType int4;
    r record;
    rr record;
    rrr record;
    aValue varchar;
    q varchar;
begin

    dstValue = '';

    for r in select id_attr_attr, a_val from parseAttrAttrValue(srcValue)
    loop
        select unique_id into uidAttrAttr from attrs_attrs where id = r.id_attr_attr;
        if(uidAttrAttr isnull) then
            continue;
        end if;
        
        aValue = r.a_val;
        
        if(char_length(dstValue) > 1) then
            dstValue = dstValue || '^~^~^';
        end if;

        for rr in 
            select a.id_a_type, a.table_name 
            from attributes a, attrs_attrs aa 
            where a.id = aa.id_attr_child and aa.id = r.id_attr_attr
        loop
            idAttrType = rr.id_a_type;

            --verify for ref attributes. if is ref then replace unique_id in value to ID
            --note that only atList and atCheckList (old) can place in complex attributes

            if(idAttrType = 2 and rr.table_name is not null) then
                if(aValue isnull or trim(aValue) = '' or aValue = '''''') then
                    continue;
                end if;

                if(rr.table_name = 'organization') then
                    q = 'select email_prefix as u_id from "' || rr.table_name || '" where id = ' || aValue;
                else
                    q = 'select unique_id as u_id from "' || rr.table_name || '" where id = ' || aValue;
                end if;

                for rrr in execute q
                loop
                    aValue = rrr.u_id;
                end loop;
            end if;

            if(idAttrType = 12 and rr.table_name is not null) then
                if(rr.table_name = 'organization') then
                    q = 'select email_prefix as u_id from "' || rr.table_name || '" where id in (select unnest(' || quote_literal(aValue) || '::int8[]))';
                else
                    q = 'select unique_id as u_id from "' || rr.table_name || '" where id in (select unnest(' || quote_literal(aValue) || '::int8[]))';
                end if;

                aValue = '';
                for rrr in execute q
                loop
                    if(char_length(aValue) > 1) then
                        aValue = aValue || ',';
                    else
                        aValue = '{';
                    end if;

                    aValue = aValue || rrr.u_id;
                end loop;

                aValue = aValue || '}';
            end if;
        end loop;

        dstValue = dstValue || uidAttrAttr || '~~~' || aValue;
    end loop;

    return dstValue;
end
$BODY$
language 'plpgsql';

--from id to real values for refs
create or replace function convertAttrAttrValue2(varchar) returns varchar as
$BODY$
declare
    srcValue alias for $1;

    dstValue varchar;
    idAttrAttr varchar;
    idAttrType int4;
    tableName varchar;
    r record;
    rrr record;
    aValue varchar;
    q varchar;
    isExist int4;
begin

    dstValue = '';

    for r in 
        select p.id_attr_attr, p.a_val, a.id_a_type, a.table_name, a.column_name 
        from 
            parseAttrAttrValue(srcValue) p,
            attrs_attrs aa,
            attributes a
        where
            p.id_attr_attr = aa.id
            and aa.id_attr_child = a.id
    loop
        --select unique_id into uidAttrAttr from attrs_attrs where id = r.id_attr_attr;
        --if(uidAttrAttr isnull) then
        --    continue;
        --end if;
        
        aValue = r.a_val;
        
        if(char_length(dstValue) > 1) then
            dstValue = dstValue || '^~^~^';
        end if;

        --for rr in 
        --    select a.id_a_type, a.table_name 
        --    from attributes a, attrs_attrs aa 
        --    where a.id = aa.id_attr_child and aa.id = r.id_attr_attr
        --loop
        idAttrType = r.id_a_type;
        idAttrAttr = r.id_attr_attr;

        tableName = r.table_name;
        --Hack для выбора реального названия таблицы (tbl_ или не tbl_)
        select f_is_table_exist(tableName, NULL) into isExist; 
        if(isExist <> 1) then
            select f_is_table_exist('tbl_' || tableName, NULL) into isExist; 
            if(isExist = 1) then
                tableName := 'tbl_' || tableName;
            else
                raise exception 'Table % does not exist!', tableName;
                return NULL;
            end if;
        end if;
        
       

        --verify for ref attributes. if is ref then replace unique_id in value to ID
        --note that only atList and atCheckList (old) can place in complex attributes
        if(idAttrType = 2 and r.table_name is not null) then
            if(aValue is not null and  trim(aValue) <> '' and aValue <> '''''') then
                q = 'select "' || r.column_name || '" as u_id from "' || tableName || '" where id = ' || aValue;

                --aValue = '';
                for rrr in execute q
                loop
                    aValue = rrr.u_id;
                end loop;
            --else
            --    aValue = '';
            end if;
        end if;

        if(idAttrType = 12 and r.table_name is not null) then
            q = 'select array_agg("' || r.column_name || '")::varchar as result from ' || tableName || ' where id = ANY(' || asString(aValue, true) || ')';
    
            --aValue = '';
            for rrr in execute q
            loop
                aValue = r.result;
            end loop;
        end if;

        dstValue = dstValue || idAttrAttr || '~~~' || aValue;
    end loop;

    return dstValue;
end
$BODY$
language 'plpgsql' security definer;
