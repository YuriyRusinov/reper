

select f_safe_drop_type('h_get_ref_attr_values');
create type h_get_ref_attr_values as (id int8, displayed_value varchar, id_ref_value int8); --id_ref_value - используется только если получилась ситуация, 
                                                                                            --когда отображаемая колонка, на которую ссылается атрибут, сама является ссылкой на другую таблицу.
                                                                                            --в этом случае отображаем реально отображаемое значение, а промежуточную ссылку запоминаем в этом поле

create or replace function getRefAttrValues(int4, bool, bool, varchar) returns setof h_get_ref_attr_values as
$BODY$
declare
    idAttribute alias for $1;
    isXML alias for $2;
    orderBy alias for $3;
    parentTableName alias for $4; --only for atParent attributes

    columnName varchar;
    tableName varchar;
    idAttrType int4;
    idSearchTemplate int4;

    refColumnName varchar;
    refTableName varchar;
    idRefAttrType int4;
    distinctWord varchar;
    orderByWord varchar;
    filterSQL varchar;

    r record;
    sql varchar;

    res h_get_ref_attr_values%rowtype;
    
begin

    idAttrType = -1;
    for r in 
        select 
            id, 
            id_a_type, 
            table_name, 
            column_name, 
            ref_column_name, 
            id_ref_attr_type, 
            id_search_template 
        from attributes where id = idAttribute
    loop
        columnName = r.column_name;
        refColumnName = r.ref_column_name;
        tableName = r.table_name;
        idAttrType = r.id_a_type;
        idRefAttrType = r.id_ref_attr_type;
        idSearchTemplate = r.id_search_template;
    end loop;

    if(idAttrType = -1) then
        return;
    end if;

    if(idAttrType <> 2 and idAttrType <> 3 and idAttrType <> 12 and idAttrType <> 17 and idAttrType <> 19 and idAttrType <> 26) then
        return;
    end if;

    if(refColumnName isnull or refColumnName = '') then
        refColumnName = 'id';
    end if;

    if(isXML is not null and isXML = true) then
        distinctWord = 'distinct';
    else
        distinctWord = '';
    end if;

    if(idAttrType = 3) then --atParent
        tableName = parentTableName;
    end if;


    if((tableName isnull or tableName = '') or (columnName isnull or columnName = '')) then
        return;
    end if;

    if(idRefAttrType = 2 or idRefAttrType = 3 or idRefAttrType = 12 or idRefAttrType = 17 or idRefAttrType = 19 or idRefAttrType = 26) then
        refColumnName = '';
        for r in select table_name, column_name from aGetAttribute(columnName, tableName)
        loop
            refTableName = r.table_name;
            refColumnName = r.column_name;
        end loop;

        if(refColumnName isnull or refColumnName = '') then
            return;
        end if;

        sql = 'select ' || distinctWord || ' ' || tableName || '.id, ' || refTableName || '."' || refColumnName || '", ' || tableName || '."' || columnName || '" from ' || refTableName || ' inner join ' || tableName || ' on (' || refTableName || '."' || refColumnName || '" = ' || tableName || '."' || columnName || '")';
        --raise exception '%', sql;

    else

        if(orderBy is not null and orderBy = true) then
            orderByWord = 'order by "' || columnName || '"';
        else
            orderByWord = '';
        end if;

        if(idSearchTemplate is not null) then
            filterSQL = generateFilterQuery(tableName, idSearchTemplate);
        else
            filterSQL = '';
        end if;
        
        sql = 'select id, "' || columnName || '", "' || refColumnName || '" from ' || tableName || ' where 1=1 ' || filterSQL || ' ' || orderByWord;
        --raise exception '%', sql;
        
    end if;

    for res in execute sql
    loop
        return next res;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

--getRefAttrValues(int4, bool, bool, varchar)
select * from getRefAttrValues(1237, false, true, NULL::varchar);