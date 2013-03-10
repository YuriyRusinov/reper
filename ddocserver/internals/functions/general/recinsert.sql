create or replace function recInsert (varchar, varchar[], varchar[]) returns int8 as
$BODY$
declare
    ref_uid alias for $1;
    attrs_uids alias for $2;
    attrs_values alias for $3;

    idRec int8;
    ref_table_name varchar;
    query varchar;
    values_query varchar;
    r record;
    n int4;
    i int4;
    ii int4;
    rr record;
    at_type varchar;
    id_a_type int4;
    rTable varchar;
    refTable varchar;
    mainAttr varchar;
    childAttr varchar;
    ids varchar;
    unique_ids varchar[];
    exQuery varchar;
    aexQuery varchar;
    attr_code varchar;
    attrs_key varchar;
    attrs_vals_key varchar;
    rattr record;
    idAttrRec int8;
    rattrrec record;

    uniqueId varchar;

begin
    ref_table_name := iogettablenamebyuid (ref_uid);
    if (ref_table_name is null) then
        raise warning 'incorrect reference';
        return -1;
    end if;

    if (array_upper (attrs_uids, 1) <> array_upper (attrs_values, 1)) then
        raise warning 'incorrect dimensions';
        return -1;
    end if;

    if (lower (ref_table_name) = 'organization') then
        attrs_key := 'email_prefix';

        --attrs_vals_key := ARRAY[''];
        n := -1;
        i := 1;

        while (i <= array_upper (attrs_uids, 1) and n < 0) loop
            if (lower (attrs_uids[i]) = 'localorg-attributes-87') then --email_prefix
                n := i;
            else
                i := i+1;
            end if;
        end loop;

        if (n < 0) then
            raise warning 'cannot find key attribute';
            return -1;
        end if;

        attrs_vals_key := attrs_values[n];
    else
        attrs_key = 'unique_id';
        attrs_vals_key := attrs_values[1];
    end if;

    select rFindEqual (ref_table_name, attrs_key, attrs_vals_key) into idRec;

    --raise warning 'rFindEqual function completed, table is %', ref_table_name;

    if (idRec is not null and idRec > 0) then
        return idRec;
    end if;

    select getNextSeq(ref_table_name, E'id') into idRec;
    query := 'insert into ' || ref_table_name || ' (';
--    raise warning 'query insert is %', query;
    n := array_upper (attrs_uids, 1);
    values_query := 'values (';

    --сначала зададим id (всегда новый)
    query := query || 'id, ';
    values_query := values_query || idRec || ', ';

    for i in 1..n loop
        select into attr_code a.code from attributes a where a.unique_id=attrs_uids[i];
        uniqueId = attrs_uids[i];

        if (attrs_uids[i] = 'localorg-attributes-1') then --id
            continue;--id задаем всегда новый перед циклом
        elsif (attr_code is null and attrs_uids[i] = 'unique_id') then
            query := query || 'unique_id';
            values_query := values_query || quote_literal (attrs_vals_key);
            if (i < n) then
                query := query || ',';
                values_query := values_query || ',';
            end if;
        else
            for rr in
                select a.id, at.id as id_attr_type, at.code, a.table_name as attr_table from attributes a inner join a_types at on (a.id_a_type=at.id and a.unique_id = attrs_uids[i])
            loop
                at_type := rr.code;
                id_a_type := rr.id_attr_type;

                if (id_a_type = 17) then
                    raise warning 'attr table is %', rr.attr_table;
                    rTable := ref_table_name || '_' || rr.attr_table;
                    raise warning 'attr table is %', rTable;
                    if (rTable = 'organization_work_mode' or 
                        rTable = 'position_work_mode' or 
                        rTable = 'units_work_mode' or 
                        rTable = 'io_objects_organization' or 
                        rTable = 'user_chains_organization' or
                        rTable = 'report_organization' or
                        rTable = 'guard_objects_devices' or
                        rTable = 'access_cards_access_plan' or
                        rTable = 'mail_lists_position' or
                        rTable = 'life_cycle_io_states'
                      ) then
                        refTable := rTable;
                    else
                        refTable := rTable || '_ref_' || rr.id;
                    end if;

                    mainAttr := 'id_' || ref_table_name;
                    childAttr := 'id_' || rr.attr_table;

                    ids := attrs_values[i];
                    select into unique_ids regexp_split_to_array (attrs_values[i], E'\\{|\\,|\\}');
                    if (length(trim(ids)) = 0 or attrs_values[i] is null) then
                        ids := '{}';
                    else
                        ids := '{';
                        for ii in 1..array_upper (unique_ids, 1)
                        loop
                            --raise warning 'row 146 % % %', attrs_values[i], unique_ids[ii], ids;
                            if (unique_ids[ii] is not null and length (trim(unique_ids[ii])) != 0) then
                                ids := ids || getidbyuid (rr.attr_table, unique_ids[ii]);
                                if (unique_ids[ii] is not null and ii < array_upper(unique_ids, 1)-1 and length (trim(unique_ids[ii+1])) >0) then
                                    ids := ids || ',';
                                end if;
                            end if;
                        end loop;
                        ids := ids || '}';
                    end if;
                    if (exQuery is null) then
                        exQuery := 'select aInsertExValues(' || quote_literal (refTable) || ',' || idRec || ',' || quote_literal (ids) || ',' || quote_literal (mainAttr) || ',' || quote_literal (childAttr) ||')';
                    else
                        exQuery := exQuery || 'select aInsertExValues(' || quote_literal (refTable) || ',' || idRec || ',' || quote_literal (ids) || ',' || quote_literal (mainAttr) || ',' || quote_literal (childAttr) ||')';
                    end if;
                    raise warning 'external query is %, record id is % ids is %', exQuery, idRec, ids;

                    continue;
                elsif (id_a_type = 12) then
                    query := query || '"' || attr_code || '"';
                    if (attrs_values[i] is null or length (trim (attrs_values[i])) =0
                        or position ('null' in lower (attrs_values[i])) != 0) then
                        values_query := values_query || 'NULL::int8[]';
                    else
                        values_query := values_query || quote_literal('{' || attrs_values[i] || '}');
                    end if;
                    if (i < n) then
                        query := query || ',';
                    else
                        query := query || ') ';
                    end if;
                    if (i < n) then
                        values_query := values_query || ',';
                    else
                        values_query := values_query || ')';
                    end if;
                elsif (id_a_type = 2 or id_a_type = 3 or id_a_type = 19 or id_a_type = 26) then
                    --raise warning 'reference attribute % % % %', ref_table_name, query, values_query, attrs_values[i];
                    for rattr in
                        select * from aGetAttributeByUID (uniqueId)
                    loop
                        if (id_a_type = 3) then
                            rTable := ref_table_name;
                        else
                            rTable := rattr.table_name;
                        end if;
                    end loop;
                    query := query || '"' || attr_code || '"';
                    if (i < n) then
                        query := query || ',';
                    else
                        query := query || ') ';
                    end if;
                    if (position ('null' in lower (attrs_values[i])) != 0) then
                        values_query := values_query || 'NULL';
                    else
                        if (rTable = 'work_mode' or
                               rTable = 'organization_type' or
                               --rTable = 'organization_transport' --or
                               --rTable = 'transport' or
                               rTable = 'position' or
                               rTable = 'units'
                            ) then
                            idAttrRec := attrs_values[i];
                            raise warning 'attr code is % value % int8 %', attr_code, attrs_values[i], idAttrRec;
                        elsif (ref_table_name = 'organization' or rTable = 'organization') then
                            idAttrRec := NULL::int4;
                            aexQuery := 'select id from organization where email_prefix= ' || quote_literal (attrs_values[i]);
                            for rattrrec in
                                execute aexQuery
                            loop
                                idAttrRec := rattrrec.id;
                            end loop;
                            raise warning 'id organization is %, value is %', idAttrRec, attrs_values[i];
                        else
                            aexQuery := 'select id from ' || rTable || ' where unique_id = ' || quote_literal (attrs_values[i]);
                            idAttrRec := NULL::int8;
                            
                            for rattrrec in
                                execute aexQuery
                            loop
                                idAttrRec := rattrrec.id;
                            end loop;

                            if (idAttrRec is not null) then
                                raise warning 'ref is not null value is %', idAttrRec;
                            end if;
                        end if;
                        if (idAttrRec is not null) then
                            values_query := values_query || idAttrRec;
                            raise warning 'ref is not null value is %', idAttrRec;
                        else
                            values_query := values_query || 'NULL';
                        end if;
                    end if;
                    if (i < n) then
                        values_query := values_query || ',';
                    else
                        values_query := values_query || ')';
                    end if;
                else
                    query := query || '"' || attr_code || '"';
                    if (i < n) then
                        query := query || ',';
                    else
                        query := query || ') ';
                    end if;

                    if (position ('null' in lower (attrs_values[i])) != 0) then--length (trim (attrs_values[i])) = 0) then
                        values_query := values_query || 'NULL';
                    elsif(length (trim (attrs_values[i])) = 0 and at_type <> 'VARCHAR' and at_type <> 'TEXT' and at_type <> 'BYTEA') then
                        values_query := values_query || 'NULL';
                    elsif (at_type = 'VARCHAR' or at_type = 'TEXT' or at_type = 'BYTEA') then
                        values_query := values_query || quote_literal (attrs_values[i]);
                    elsif (at_type = 'TIMESTAMP' or at_type = 'DATE' or at_type = 'TIME') then
                        values_query := values_query || quote_literal (attrs_values[i]) || '::timestamp';
                    elsif (at_type = 'BOOL') then
                        values_query := values_query || quote_literal (attrs_values[i]);
                    elsif (at_type = 'MACLABEL') then
                        values_query := values_query || quote_literal (attrs_values[i]);
                    elsif (at_type = 'INT8[]' or at_type = 'INT4[]') then
                        values_query := values_query || quote_literal('{' || attrs_values[i] || '}');
                    elsif (at_type = 'XML') then
                        values_query := values_query || 'xmlparse (document(' || quote_literal (attrs_values[i]) || '))';
                    else
                        values_query := values_query || attrs_values[i];
                    end if;


                    if (i < n) then
                        values_query := values_query || ',';
                    else
                        values_query := values_query || ')';
                    end if;
                end if;
                raise warning 'values are %', values_query;
                raise warning 'query is %', query;
            end loop;
        end if;
    end loop;

    raise warning 'merge queries';
    query := regexp_replace (query, E'\\,$', ')');
    values_query := regexp_replace (values_query, E'\\,$', ')');
    query := query || values_query;

    raise warning 'queries are merged, result is %', query;

    execute query;
--    raise warning 'query is executed';
--    if (not FOUND) then
--        return -1;
--    else
--        raise warning 'record is inserted';
        if (exQuery is not null) then
            raise warning 'additional query is %', exQuery;
            execute exQuery;
--            if (not FOUND) then
--                return -1;
--            end if;
        end if;
        return idRec;
--    end if;

    --exception when others then
    --    raise warning 'exception';
    --    return -1;
end
$BODY$
language 'plpgsql';
