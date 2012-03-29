create or replace function generateSearchQueryForRubr (int4) returns varchar as
$BODY$
declare
    id_search_template alias for $1;

    id_io_category int4;

    res_query varchar;
begin

    select id_child into id_io_category from io_categories c inner join io_objects io on (io.id_io_category = c.id and io.table_name='io_objects');

    res_query := generateSearchQuery (id_io_category, 'io_objects', id_search_template);

    res_query := res_query || ';';

    return res_query;
end
$BODY$
language 'plpgsql';

create or replace function generateSearchQuery (int4, varchar, int4) returns varchar as
$BODY$
declare
    id_category alias for $1;
    table_name alias for $2;
    id_search_template alias for $3;

    res_query varchar;
    attrs_query varchar;
    ar record;
    rgroups record;
    nattrs int4;
    i int4;
    id_main_group int4;
    id_groups int4[];
    attrs_from_cat int4[];
    ngr int4;
    fromWord varchar;
    usedTableNames varchar[];
    tName varchar;
    cName varchar;
    cnt int4;
    whereWord varchar;
    joinWord varchar;
    attrs varchar;
    filterQuery varchar;
begin

    fromWord := table_name;
    whereWord = ' 1=1 ';
    joinWord = ''; 
    usedTableNames = ARRAY[table_name];
    attrs := '';

    if (id_search_template is not null) then
        attrs_query := 'select count(*) as cnt from attributes a inner join attrs_categories ac on (a.id=ac.id_io_attribute and ac.id_io_category=' || id_category || ');';
        for ar in
            execute attrs_query
        loop
            nattrs := ar.cnt;
        end loop;

        attrs_query := 'select a.id from attributes a inner join attrs_categories ac on (a.id=ac.id_io_attribute and ac.id_io_category=' || id_category || ');';
        i := 0;
        for ar in
            execute attrs_query
        loop
            if (i=0) then
                attrs_from_cat := ARRAY[ar.id];
            else
                attrs_from_cat := attrs_from_cat || ar.id;
            end if;
            i := i+1;
        end loop;

        select id_group into id_main_group from iogetsearchtemplate (id_search_template);
        --raise warning 'id of main group is %', id_main_group;

        id_groups := ARRAY[id_main_group];
        i := 0;
        for rgroups in
            select id from getgroups (id_main_group, true)
        loop
            if (i > 0) then
                id_groups := id_groups || rgroups.id;
            end if;
            i := i+1;
        end loop;
        ngr := array_upper (id_groups, 1);
        --raise warning 'groups are %', id_groups;

        for i in 1..array_upper (id_groups, 1)
        loop
            --raise warning '%', id_groups[i];
            attrs_query := 'select c.attr_id from criteria c inner join groups_criteria gc on (c.id=gc.id_criteria and gc.id_group=' || id_groups[i] || ')';
            for ar in
                execute attrs_query
            loop
                if (not array[ar.attr_id] <@ attrs_from_cat) then
                    raise warning 'incorrect attribute in search criterion! id_group = %, id_category = %', id_groups[i], id_category;
                    return null;
                end if;
            end loop;
        end loop;
    end if;

    attrs_query := 'select a.code, a.id_a_type, a.table_name, a.column_name, ac.is_mandatory from attributes a inner join attrs_categories ac on (a.id=ac.id_io_attribute and ac.id_io_category=' || id_category || ');';
    for ar in
        execute attrs_query
    loop
        if (ar.code = 'id' or ar.id_a_type = 17) then --atCheckBoxEx
            continue;
        end if;

        if (ar.id_a_type = 2 or ar.id_a_type = 3) then  --atList, atParent
            tName := ar.table_name;
            cName := ar.column_name;

            --raise warning '% %', tName, cName;

            if (tName is null or length (tName) = 0 or
                cName is null or length (cName) = 0) then
                raise warning 'Corrupt attribute in IO! It MUST be reference to table and column but they are NULL!';
                return null;
            end if;

            select count(*) into cnt from unnest (usedTableNames) as f where f=tName;
            --raise warning 'tables number %', cnt;
            usedTableNames := usedTableNames || tName;

            if (cnt > 0) then
                fromWord := fromWord || ', ' || tName || ' ' || tName || cnt;
                if (not ar.is_mandatory) then
                    whereWord := whereWord || ' and (' || table_name || '.' || ar.code || ' is null or ' || table_name || '.' || ar.code || ' = ' || tName || cnt || '.id )';
                    joinWord := joinWord || ' left join ' || tName || ' ' || tName || cnt || ' on ' || table_name || '.' || ar.code || ' = ' || tName || cnt || '.id ';
                    attrs := attrs || ', case when ' || table_name || '.' || ar.code || ' is null then NULL else ' || tName || cnt || '.' || cName || ' end as ' || ar.code;
                else
                    whereWord := whereWord || ' and ' || table_name || '.' || ar.code || ' = ' || tName || cnt || '.id';
                    joinWord := joinWord || ' left join ' || tName || ' ' || tName || cnt || ' on ' || table_name || '.' || ar.code || ' = ' || tName || cnt || '.id ';
                    attrs := attrs || ', ' || tName || cnt || '.' || cName || ' as ' || ar.code;
                end if;
            else
                fromWord := fromWord || ', ' || tName || ' ';
                if (not ar.is_mandatory) then
                    whereWord := whereWord || ' and (' || table_name || '.' || ar.code || ' is null or ' || table_name || '.' || ar.code || ' = ' || tName || '.id )';
                    joinWord := joinWord || ' left join ' || tName || ' on ' || table_name || '.' || ar.code || ' = ' || tName || '.id ';
                    attrs := attrs || ', case when ' || table_name || '.' || ar.code || ' is null then NULL else ' || tName || '.' || cName || ' end as ' || ar.code;
                else
                    whereWord := whereWord || ' and ' || table_name || '.' || ar.code || ' = ' || tName || '.id';
                    joinWord := joinWord || ' left join ' || tName || ' on ' || table_name || '.' || ar.code || ' = ' || tName || '.id ';
                    attrs := attrs || ', ' || tName || '.' || cName || ' as ' || ar.code;
                end if;
            end if;
        else
            attrs := attrs || ', ' || table_name || '.' || ar.code;
        end if;
    end loop;

    res_query := 'select distinct ' || table_name || '.id' || attrs || ' from ' || table_name || ' ' || joinWord || ' where 1 = 1 ';

    if (id_search_template is not null) then
        filterQuery := generateFilterQuery (table_name, id_search_template);
        if (filterQuery is not null) then
            res_query := res_query || filterQuery;
        end if;
    end if;

    res_query := res_query || ' order by ' || table_name || '.id';

    raise warning '!!!RES_QUERY = %', res_query;

    return res_query;
end
$BODY$
language 'plpgsql';

create or replace function generateFilterQuery (varchar, int4) returns varchar as
$BODY$
declare
    table_name alias for $1;
    id_search_template alias for $2;

    res_query varchar;
    id_main_group int4;
begin

    if (id_search_template is null or table_name is null) then
        raise exception 'Cannot generate search query without template or table';
        return null;
    end if;
    
    select id_group into id_main_group from iogetsearchtemplate (id_search_template);
    raise notice 'id of filters main group is %', id_main_group;

    if (id_main_group is null) then
        return null;
    end if;

    raise warning '1 RES_QUERY = %', res_query;
    res_query := parseGroup (id_main_group, table_name);
    if (res_query is null or length (res_query) = 0) then
        return null;
    end if;

    raise warning '2 RES_QUERY = %', res_query;
    res_query := ' and ' || res_query;
    raise warning '3 RES_QUERY = %', res_query;

    return res_query;
end
$BODY$
language 'plpgsql';

create or replace function parseGroup (int4, varchar) returns varchar as
$BODY$
declare
    idGroup alias for $1;
    table_name alias for $2;

    res_query varchar;
    tmp varchar;
    groupOper varchar;
    oper int4;
    grCount int4;
    fCount int4;
    i int4;

    r record;
    id_group int4;
    id_filter int4;
    v_str varchar;
begin

    if (idGroup is null) then
        raise notice 'invalid group';
        return null;
    end if;

    raise warning '0 idGroup = %,  RES_QUERY = %', idGroup, res_query;

    tmp := ' ( ';
    select operation into oper from groups g where g.id=idGroup;
    if (oper > 0) then
        groupOper := ' and ';
    else
        groupOper := ' or ';
    end if;

    grCount := 0;

    for r in
        select * from getChildGroups (idGroup, false)
    loop
        id_group := r.id;

        v_str := parseGroup (id_group, table_name);
        tmp := tmp || v_str;

        if (tmp is null or length(tmp) = 0) then
            continue;
        end if;

        if (grCount = 0) then
            res_query := tmp;
        else
            res_query := res_query || groupOper || tmp;
        end if;

        grCount := grCount + 1;
        tmp = '';
        raise warning '4 idGroup = %,  RES_QUERY = %', idGroup, res_query;
    end loop;

    tmp := null;
    if (grCount = 0) then
        tmp := ' ( ';
    else
        tmp := ' ' || groupOper || ' ( ';
    end if;

    select count(*) into fCount from iogetcriterionfromgroup (idGroup);
    if (fCount <=0) then
        res_query = res_query || ' ) ';
        return res_query;
    end if;

    fCount := 0;
    for r in
        select * from iogetcriterionfromgroup (idGroup)
    loop
        id_filter := r.id;

        v_str := parseFilter (id_filter, table_name);

        if (tmp is null) then
            tmp := '';
        end if;

        --raise notice 'parse filter tmp is %, v_str is %', tmp, v_str;

        if (fCount = 0) then
            tmp := tmp || v_str;
        else
            tmp := tmp || groupOper || v_str;
        end if;

        fCount := fCount + 1;
    end loop;

    if (res_query is null) then
        res_query := '';
    end if;

    if (fCount > 0) then
        res_query := res_query || tmp || ' ) ';
    else
        res_query := res_query ||  ' ) ';
    end if;
    raise warning '6 RES_QUERY = %', res_query;
    if (grCount > 0) then
        res_query := res_query || ' )';
    end if;

    --raise notice 'parse group result query is %', res_query;
    raise warning '7 RES_QUERY = %', res_query;
    return res_query;
end
$BODY$
language 'plpgsql';

create or replace function parseFilter (int4, varchar) returns varchar as
$BODY$
declare
    idFilter alias for $1;
    table_name alias for $2;

    r record;
    res_query varchar;
    code varchar;
    values varchar[];
    case_sensitive boolean;
    upper_str varchar;
    endupper_str varchar;
    search_pos int4;
    search_query varchar;
    rs record;
begin

    if (idFilter is null) then
        raise notice 'Incorrect filters conditions';
        return null;
    end if;

    for r in
        select 
            f.id, 
            f.attr_id, 
            f.value, 
            f.id_type, 
            f.id_operation, 
            f.is_not, 
            f.case_sensitive, 
            o.the_value as operation_name, 
            at.code as atype, 
            a.code as attr_code
        from 
            (select * from iogetcriterion (idFilter)) as f 
            inner join operations o on (o.id=f.id_operation) 
            inner join attributes a on (f.attr_id = a.id) 
            inner join a_types at on (a.id_a_type=at.id) 
    loop
        if (upper (r.atype) != 'VARCHAR') then
            case_sensitive := true;
        else
            case_sensitive := r.case_sensitive;
        end if;

        if (case_sensitive) then
            upper_str := '';
            endupper_str := '';
        else
            upper_str := ' upper(';
            endupper_str := ' )';
        end if;
        values := string_to_array (r.value, ',');
        --raise notice 'parse filter parameters are % operation name % values %', r.id, quote_literal (r.operation_name), values;
        if (table_name is null or length (table_name)=0) then
            res_query := '"' || r.attr_code || '"';
        else
            res_query := table_name || '."' || r.attr_code || '"';
        end if;

        if (r.id_operation < 13) then
            res_query := res_query || ' ' || r.operation_name;
        elsif (r.id_operation = 13) then --in (select ...)
            res_query := res_query || ' in';
        end if;

        if (r.id_operation = 6 or --in
            r.id_operation = 7) then --not in
            res_query := res_query || ' (' || r.value || ')';

        elsif ( (r.id_operation >= 1 and r.id_operation <= 5) or r.id_operation = 10) then -- =  >  <  >=  <=  <> 
            res_query := res_query || upper_str || ' ' || quote_literal(r.value) || endupper_str;
            
        elsif (r.id_operation = 9) then --like
            res_query := res_query || upper_str || ' ' || quote_literal (r.value) || endupper_str;
            
        elsif (r.id_operation = 8) then --between
            if (array_upper (values, 1) < 2) then
                raise notice 'Incorrect values size ';
                return null;
            end if;

            res_query := res_query || values[1] || ' and ' || values[2];
            
        elsif (r.id_operation = 13) then --in (select ...)
            res_query := res_query || ' (';
            select into search_pos position (lower('search') in lower (r.value));
            --raise notice 'position is %', search_pos;

            search_query := substring (r.value from 1 for search_pos+5 );
            search_query := search_query || 'Query' ||  substring (r.value from search_pos+6 ) || 'as sq';
            --raise notice 'search query is %', search_query;
            for rs in
                execute search_query
            loop
                res_query := res_query || substring (rs.sq from 1 for char_length(rs.sq)-1);
            end loop;
            res_query := res_query || ' )';
        end if;
    end loop;

    --raise notice 'parse filter result query is %', res_query;
    return res_query;
end
$BODY$
language 'plpgsql';
