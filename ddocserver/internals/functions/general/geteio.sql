create or replace function getEIO (int8, int4) returns varchar as
$BODY$
declare
    id_copy alias for $1;
    id_io_object alias for $2;

    tableName varchar;
    rTable varchar;
    refTable varchar;
    idIoCategory int4;
                        
    attrs_code varchar;
    fields varchar[];
    mainAttr varchar;
    childAttr varchar;
    r RECORD;
    rr RECORD;
    res_string varchar;
    result_values varchar[];
    r_query varchar;
    fff varchar;
    i int4;
begin

    select into tableName io.table_name from f_sel_io_objects(id_io_object) io where io.id = id_io_object;

    --raise notice '%', tableName;
    if (tableName is null) then
        raise notice 'Document % is not a reference', id_io_object;
        return NULL;
    end if;

    select into idIoCategory c.id_child from io_categories c inner join f_sel_io_objects(id_io_object) o on (o.id_io_category=c.id and o.id=id_io_object);
    if (idIoCategory is null) then
        raise notice 'Invalid category';
        return NULL;
    end if;

    --fields := ARRAY[''];
    for r in
        select a.id, a.code, a.id_a_type, a.table_name from attributes a inner join attrs_categories ac on (a.id=ac.id_io_attribute and ac.id_io_category=idIoCategory) order by a.id
    loop
        if (r.id_a_type = 17) then
            rTable := tableName || '_' || r.table_name;
            if (rTable = 'organization_work_mode' or 
                rTable = 'table_notifies_io_objects' or
                rTable = 'position_work_mode' or 
                rTable = 'units_work_mode' or 
                rTable = 'io_objects_organization') 
            then
                refTable := rTable;
            else
                refTable := rTable || '_ref_' || r.id;
            end if;
            mainAttr := 'id_' || tableName;
            childAttr := 'id_' || r.table_name;

            attrs_code := '( select getExValuesId (' || id_copy || ',' || quote_literal (refTable) || ',' || quote_literal (mainAttr) || ',' || quote_literal (childAttr) || '))';
            raise notice '%', attrs_code;
        else
            if(r.code <> 'uuid_t') then
                attrs_code = r.code;
            else
                continue;
            end if;
        end if;

        --for all system qualifiers exclude fields from q_base_table!
        -- system qualifiers have ID < 300
/*        if (fields is null and (tableName = 'organization' or 
                                tableName = 'organization_type' or 
                                tableName = 'transport' or 
                                tableName = 'work_mode' or 
                                tableName = 'position' or 
                                tableName = 'units' or 
                                tableName = 'organization_transport' or 
                                tableName = 'a_groups' or 
                                tableName = 'io_views' or 
                                tableName = 'a_groups' or 
                                table_name = 'ranks'))    */
        if(fields is null and id_io_object < 300)
        then
            fields := ARRAY ['unique_id'];
            fields := array_append(fields, 'NULL');
            fields := array_append(fields, '1');
            fields := array_append(fields, 'NULL');
            fields := array_append(fields, 'NULL');
            fields := array_append(fields, 'NULL');
            fields := array_append(fields, 'NULL');
            fields := array_append (fields, attrs_code);
        elsif (fields is null) then
            fields := ARRAY ['unique_id'];
            fields := array_append(fields, 'uuid_t');
            fields := array_append(fields, 'id_io_state');
            fields := array_append(fields, 'r_icon');
            fields := array_append(fields, 'rr_name');
            fields := array_append(fields, 'record_fill_color');
            fields := array_append(fields, 'record_text_color');
            fields := array_append (fields, attrs_code);
        else
            fields := array_append (fields, attrs_code);
        end if;
    end loop;


    r_query := 'select  ';

    for i in 1..array_upper (fields, 1)
    loop

        if (fields[i] like '(%') then
            r_query := r_query || fields[i] || ' as f' || i;
        else
            if(i >= 8) then
                r_query := r_query || '"' || fields[i] || '"' || ' as f' || i;
            else
                r_query := r_query || fields[i] || ' as f' || i;
            end if;
        end if;

        if (i < array_upper (fields, 1)) then
            r_query := r_query || ', ';
        end if;

    end loop;

    r_query := r_query || ' from ' || tableName || ' where id = ' || id_copy;

    return r_query;

end
$BODY$
language 'plpgsql';
