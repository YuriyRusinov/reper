create or replace function recUpdate (varchar, varchar[], varchar[], varchar, varchar) returns int8 as
$BODY$
declare
    table_uid alias for $1;
    attrs_uids alias for $2;
    attrs_values alias for $3;
    key_attr alias for $4;
    key_val alias for $5;

    idRec int8;

    ref_table_name varchar;
    r_query varchar;
    n int4;
    i int4;
    a_type_code varchar;
    attr_code varchar;
    id_attr_type int4;
    r record;
    rr record;
    rTable varchar;
    refTable varchar;
    mainAttr varchar;
    childAttr varchar;
    ids varchar;
    unique_ids varchar[];
    exQuery varchar;
    aexQuery varchar;
    rattr record;
    rattrrec record;
    idAttrRec int8;
    ii int4;
begin
    ref_table_name := iogettablenamebyuid (table_uid);
    idRec := getidbyuid (ref_table_name, key_val);

    if (array_upper (attrs_uids, 1) <> array_upper (attrs_values, 1)) then
        raise warning 'incorrect dimensions';
        return -1;
    end if;

    r_query := 'UPDATE ' || ref_table_name || ' SET ';
    n := array_upper (attrs_uids, 1);
    for i in 1..n
    loop
        for r in
            select at.id as id_a_type, at.code from attributes a inner join a_types at on (a.id_a_type=at.id and a.unique_id=attrs_uids[i])
        loop
            a_type_code := r.code;
            id_attr_type := r.id_a_type;
        end loop;

        if (id_attr_type = 7 or attrs_uids[i] = 'localorg-attributes-1') then
            continue;
        end if;

        select into attr_code a.code from attributes a where a.unique_id=attrs_uids[i];
        if (attr_code is null) then
            raise warning 'incorrect attribute unique_id';
            return -1;
        end if;
        if (id_attr_type = 17) then
            for rr in
                select a.id,a.table_name from attributes a where a.unique_id = attrs_uids[i] --agetattribute (attr_code)
            loop
                rTable := ref_table_name || '_' || rr.table_name;
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
                    rTable = 'life_cycle_io_states' or
                    rTable = 'io_processing_order_chains'
                  ) then
                    refTable := rTable;
                else
                    raise warning 'another table %', rr.id;
                    refTable := rTable || '_ref_' || rr.id;
                end if;
                mainAttr := 'id_' || ref_table_name;
                childAttr := 'id_' || rr.table_name;
                raise warning 'recUpdate, % % %', refTable, mainAttr, childAttr;

                ids := attrs_values[i];
                select into unique_ids regexp_split_to_array (attrs_values[i], E'\\{|\\,|\\}');
                if (length(trim(ids)) = 0 or attrs_values[i] is null) then
                    ids := '{}';
                else
                    ids := '{';
                    for ii in 1..array_upper (unique_ids, 1)
                    loop
                        if (ii > 1 and length (trim(unique_ids[ii-1])) >0) then
                            ids := ids || ',';
                        end if;
                        if (length (trim(unique_ids[ii])) != 0) then
                            ids := ids || getidbyuid (rr.table_name, unique_ids[ii]);
                        end if;
                    end loop;
                    ids := ids || '}';
                end if;
                if (exQuery is null) then
                    exQuery := 'select aUpdateExValues(' || quote_literal (refTable) || ',' || idRec || ',' || quote_literal (ids) || ',' || quote_literal (mainAttr) || ',' || quote_literal (childAttr) ||')';
                else
                    exQuery := exQuery || 'select aUpdateExValues(' || quote_literal (refTable) || ',' || idRec || ',' || quote_literal (ids) || ',' || quote_literal (mainAttr) || ',' || quote_literal (childAttr) ||')';
                end if;
                continue;
            end loop;
        elsif (id_attr_type = 12) then
            r_query := r_query || '"' || attr_code || '"';
            if (attrs_values[i] is null or length (trim (attrs_values[i])) =0
                or position ('null' in lower (attrs_values[i])) != 0) then
                r_query := r_query || '=' || 'NULL::int8[]';
            else
                r_query := r_query || '=' || quote_literal('{' || attrs_values[i] || '}');
            end if;
            if (i < n) then
                r_query := r_query || ',';
            end if;
         elsif (id_attr_type = 2 or id_attr_type = 3 or id_attr_type = 19 or id_attr_type = 26) then
            r_query := r_query || '"' || attr_code || '"';
            for rattr in
                select * from agetattributeByUID (attr_uids[i])
            loop
                if (id_attr_type = 3) then
                    rTable := ref_table_name;
                else
                    rTable := rattr.table_name;
                end if;
            end loop;
            if (position ('null' in lower (attrs_values[i])) != 0) then
                r_query := r_query || '= NULL';
            else
                if (rTable = 'work_mode' or
                       rTable = 'organization_type' or
                       --rTable = 'organization_transport' or
                       --rTable = 'transport' or
                       rTable = 'position' or
                       rTable = 'unit'
                    ) then
                    idAttrRec := attrs_values[i];
                    raise warning 'attr code is % value % int8 %', attr_code, attrs_values[i], idAttrRec;
                elsif (ref_table_name = 'organization' or rTable = 'organization') then
                    idAttrRec := NULL::int8;
                    aexQuery := 'select id from organization where email_prefix= ' || quote_literal (attrs_values[i]);
                    for rattrrec in
                        execute aexQuery
                    loop
                        idAttrRec := rattrrec.id;
                    end loop;
                    raise warning 'id organization is %, value is %', idAttrRec, attrs_values[i];
                else
                    idAttrRec := NULL::int8;
                    aexQuery := 'select id::int8 from ' || rTable || ' where unique_id = ' || quote_literal (attrs_values[i]);
                    raise warning 'attr code is % table name is %', attr_code, rTable;
                    for rattrrec in
                        execute aexQuery
                    loop
                        idAttrRec := rattrrec.id;
                    end loop;
                end if;
                if (idAttrRec is not null) then
                    r_query := r_query || '=' || idAttrRec;
                else
                    r_query := r_query || '=' || 'NULL::int4';
                end if;
                if (i<n) then
                    r_query := r_query || ',';
                end if;
            end if;
        else
            r_query := r_query || '"' || attr_code || '"';
            if (a_type_code = 'VARCHAR' or a_type_code = 'TEXT' or a_type_code = 'BYTEA' or a_type_code = 'MACLABEL') then
                r_query := r_query || '=' || quote_literal (attrs_values[i]);
            elsif (a_type_code = 'TIMESTAMP' or a_type_code = 'DATE' or a_type_code = 'TIME') then
                if (length (trim (attrs_values[i])) = 0) then
                    r_query := r_query || '= NULL::timestamp';
                else
                    r_query := r_query || '=' || quote_literal (attrs_values[i]) || '::timestamp';
                end if;
            elsif (a_type_code = 'GEOMETRY') then
                if (length (trim (attrs_values[i])) = 0) then
                    r_query := r_query || '= NULL::geometry';
                else
                    r_query := r_query || '=' || quote_literal (attrs_values[i]) || '::geometry';
                end if;
            elsif (position ('null' in lower (attrs_values[i])) != 0) then
                r_query := r_query || '=' || 'NULL';
            elsif (length (trim (attrs_values[i])) = 0 and a_type_code <> 'VARCHAR' and a_type_code <> 'TEXT' and a_type_code <> 'BYTEA') then
                r_query := r_query || '=' || 'NULL';
            elsif (a_type_code = 'XML') then
                r_query := r_query || '=' || 'xmlparse (document(' || quote_literal (attrs_values[i]) || '))';
            elsif (a_type_code = 'BOOL') then
                if (lower (attrs_values[i]) = 't') then
                    r_query := r_query || '=' || 'TRUE';
                else
                    r_query := r_query || '=' || 'FALSE';
                end if;
            else
                r_query := r_query || '=' || attrs_values[i];
            end if;
            if (i<n) then
                r_query := r_query || ',';
            end if;
        end if;
    end loop;

    r_query := regexp_replace (r_query, E'\\,$', '');
    r_query := r_query || ' WHERE ' || key_attr || '=' || quote_literal (key_val);

--    raise warning 'update query is %', r_query;

    execute r_query;
--    if (not FOUND) then
--        return -1;
--    else
--        raise warning 'record is inserted';
        if (exQuery is not null) then
--            raise warning 'additional query is %', exQuery;
            execute exQuery;
--            if (not FOUND) then
--                return -1;
--            end if;
        end if;
        return idRec;
--    end if;

    return 1;

    exception when others then
        raise warning 'update exception';
        return -1;
end
$BODY$
language 'plpgsql';
