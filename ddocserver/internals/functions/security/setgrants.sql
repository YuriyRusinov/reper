create or replace function setGrantsEx(varchar) returns int4 as
$BODY$
declare
    roleName alias for $1;
    
    r record;
    q varchar;
    rName varchar;

    readTables varchar[];
    nReadTables int4;
    
    revTables varchar[];
    nRevTables int4;

    insTables varchar[];
    insTablesSeq varchar[];
    nInsTablesSeq int4;
    nInsTables int4;

    i int4;
begin

    if(roleName isnull) then
        rName = 'public';
    else
        rName = roleName;
    end if;

-- (1) read only tables;

    --tables[1] = 'root_table'; --?????
    -- (A) чисто read-only таблицы
    readTables[1] = 'a_views';
    readTables[2] = 'a_types';
    readTables[3] = 'url_extentions';
    readTables[4] = 'maclabels';
    readTables[5] = 'url_types';
    readTables[6] = 'io_sync_types';
    readTables[7] = 'document_types';
    readTables[8] = 'document_subtypes';
    readTables[9] = 'document_kinds';
    readTables[10] = 'io_states';
    readTables[11] = 'io_category_types';
    readTables[12] = 'urgency_levels';
    readTables[13] = 'jr_states';

    --OSHS
    readTables[14] = 'organization_type';
    readTables[15] = 'receive_order';
    readTables[16] = 'receivers';
    readTables[17] = 'organization';
    readTables[18] = 'organization_work_mode';
    readTables[19] = 'coworkers';
    readTables[20] = 'work_mode';
    readTables[21] = 'work_mode_type';
    readTables[22] = 'position_work_mode';
    readTables[23] = 'units_work_mode';
    readTables[24] = 'units';
    readTables[25] = 'user_state';
    readTables[26] = 'users';
    readTables[27] = 'ranks';
    readTables[28] = 'position';
    readTables[29] = 'kks_roles';

    -- (A)

    --(B) таблицы, право записи и изменения в которые есть не у всех
    
    readTables[30] = 'io_categories';
    readTables[31] = 'attributes';--если есть право создавать, изменять категории, то есть право insert, update, delete в данную таблицу
    readTables[32] = 'attrs_categories'; --если есть право создавать, изменять, удалять категории, то есть право insert, update, delete в данную таблицу
    readTables[33] = 'categories_rubrics'; --если есть право создавать, изменять, удалять категории, то есть право insert, update, delete в данную таблицу


    readTables[34] = 'io_urls';
--    readTables[35] = 'urls_objects';
    readTables[35] = 'tbl_io_objects';--нельзя делать прямые запросы к данной таблице, только через функции
    readTables[36] = 'tbl_attrs_values'; --нельзя делать прямые запросы к данной таблице, только через функции
    readTables[37] = 'io_life_cycle';
    readTables[38] = 'object_ref_tables';
    readTables[39] = 'position_types';
    readTables[40] = 'mimetypes';
    readTables[41] = 'attrs_groups';
    readTables[42] = 'attrs_attrs';
--(1)

--(2)
-- доступ к этим таблицам есть только у администратора
    --syncronization
    --revoked
    revTables[1] = 'log';
    revTables[2] = 'io_sync_archive';
    revTables[3] = 'io_last_sync';
    revTables[4] = 'queue_results';
    revTables[5] = 'out_sync_queue';
    revTables[6] = 'cmd_confirmations';
    --access
    revTables[7] = 'access_categories_table';
    revTables[8] = 'access_table';
    revTables[9] = 'access_rubric_table';
    revTables[10] = 'acl_template_exceptions';
    revTables[11] = 'roles_actions';
    revTables[12] = 'actions';
    revTables[13] = 'user_acl_templates';
    revTables[14] = 'acl_template_groups';
    revTables[15] = 'acl_template_group_types';
--    revTables[16] = 'acl_system_template_groups';
--    revTables[17] = 'acl_system_templates';


--доступ к этим таблицам только через функции
    revTables[16] = 'system_table';
    revTables[17] = 'error';
    revTables[18] = 'group_error';

    --KCT
    revTables[19] = 'message_journal';
    revTables[20] = 'command_journal';
    revTables[21] = 'tsd';
    revTables[22] = 'tsd_control_journal';

    revTables[23] = 'transport';
    revTables[24] = 'organization_transport';
--(2)

--  select, insert, delete, update
--delete and update controlled from triggers

    insTables[1] = 'io_templates';
    insTablesSeq[1] = 'io_templates_id_seq';
    insTables[2] = 'io_views';
    insTables[3] = 'a_groups';
    insTablesSeq[2] = 'a_groups_id_seq';
    insTables[4] = 'user_templates';

    --select only!
    --insert, delete, update controlled from trigger
    insTables[5] = 'io_objects_organization';
    insTables[6] = 'report_organization';
    insTables[7] = 'user_chains_organization';

    insTables[8] = 'mail_lists';
    insTablesSeq[3] = 'mail_lists_id_seq';
    insTables[9] = 'mail_lists_position';

    insTables[10] = 'report';
    insTablesSeq[4] = 'report_id_seq';
    
    insTables[11] = 'user_chains';
    insTablesSeq[5] = 'user_chains_id_seq';
    insTables[12] = 'user_handlers_data';

    insTables[13] = 'user_templates';--insert controlled from trigger

    insTables[14] = 'io_rubricator';
    insTables[15] = 'rubricator';
    insTablesSeq[6] = 'rubricator_id_seq';
    insTables[16] = 'user_rubricator';--insert controlled from trigger

    
    insTables[17] = 'search_templates';
    insTablesSeq[7] = 'search_templates_id_seq';
    insTables[18] = 'groups';
    insTablesSeq[8] = 'groups_id_seq';
    insTables[19] = 'criteria_types';
    insTablesSeq[9] = 'criteria_types_id_seq';
    insTables[20] = 'criteria';
    insTablesSeq[10] = 'criteria_id_seq';
    insTables[21] = 'operations';
    insTablesSeq[11] = 'operations_id_seq';
    insTables[22] = 'groups_criteria';

--just full access
    --xml
    ---???
    insTables[23] = 'xml_query_types';
    insTablesSeq[12] = 'xml_query_types_id_seq';
    insTables[24] = 'xml_formats';
    insTablesSeq[13] = 'xml_formats_id_seq';
    insTables[25] = 'xml_paths';
    insTablesSeq[14] = 'xml_paths_id_seq';
    insTables[26] = 'xml_query_kinds';
    insTablesSeq[15] = 'xml_query_kinds_id_seq';
    insTables[27] = 'xml_queries';
    insTablesSeq[16] = 'xml_queries_id_seq';
    insTables[28] = 'xml_query_responses';
    insTablesSeq[17] = 'xml_query_responses_id_seq';

    insTables[29] = 'urls_objects';
    insTables[30] = 'attrs_groups';
    insTables[31] = 'indicator_type';
    insTablesSeq[18] = 'indicator_type_id_seq';
    insTables[32] = 'indicator';
    insTablesSeq[19] = 'indicator_id_seq';
    insTables[33] = 'indicators_values';
    insTablesSeq[20] = 'indicators_values_id_seq';
    insTables[34] = 'io_types';
    insTablesSeq[21] = 'io_types_id_seq';
    insTables[35] = 'search_template_types';
    insTablesSeq[22] = 'search_template_types_id_seq';
    insTables[36] = 'attrs_attrs_values';
    insTablesSeq[23] = 'attrs_attrs_values_id_seq';
    insTables[37] = 'rec_attrs_values';
    insTablesSeq[24] = 'rec_attrs_values_id_seq';
    insTables[38] = 'urls_records';
    insTablesSeq[25] = 'urls_records_id_seq';
    insTables[39] = 'record_rubricator';
    insTablesSeq[26] = 'record_rubricator_id_seq';
    insTables[40] = 'rubric_records';
    insTablesSeq[27] = 'rubric_records_id_seq';

    

    nReadTables := array_upper (readTables, 1);
    for i in 1..nReadTables
    loop
        
        q = 'revoke all on "' || readTables[i] || '" from ' || rName || '; grant select, references on "' || readTables[i] || '" to ' || rName;
        execute q;
    end loop;


    nRevTables := array_upper (revTables, 1);
    for i in 1..nRevTables
    loop
        q = 'revoke all on "' || revTables[i] || '" from ' || rName;
        execute q;
    end loop;
    
    nInsTables := array_upper (insTables, 1);
    for i in 1..nInsTables
    loop
        q = 'revoke all on "' || insTables[i] || '" from ' || rName || '; grant select, references, insert, update, delete on "' || insTables[i] || '" to ' || rName;
        execute q;
    end loop;

    nInsTablesSeq := array_upper (insTablesSeq, 1);
    for i in 1..nInsTablesSeq
    loop
        q = 'grant all on "' || insTablesSeq[i] || '" to ' || rName;
        execute q;
    end loop;

    q = 'grant all on q_base_table_id_seq to ' || rName;
    execute q;

    return 1;

end
$BODY$
language 'plpgsql' security definer;



create or replace function setGrants(bool) returns int4 as
$BODY$
declare
    ibGrant alias for $1;
    
    bGrant bool;
    r record;
    query varchar;
begin

    bGrant = ibGrant;
    if(bGrant isnull) then
        bGrant = true;
    end if;

    for r in 
        select tablename from pg_catalog.pg_tables where schemaname = 'public'
    loop
        if(bGrant = true) then
            query := ' grant all on ' || r.tablename || ' to public';
        else
            query := ' revoke all on ' || r.tablename || ' from public';
        end if;
        execute query;
    end loop;

    for r in 
        select relname from pg_catalog.pg_statio_all_sequences where schemaname = 'public'
    loop
        if(bGrant = true) then
            query := ' grant all on ' || r.relname || ' to public';
        else
            query := ' revoke all on ' || r.relname || ' from public';
        end if;
        execute query;
    end loop;

    revoke execute on function setgrants() from public;

    return 1;

end
$BODY$
language 'plpgsql' security definer;

create or replace function setGrants(varchar) returns int4 as
$BODY$
declare
    theTable alias for $1;
    r record;
    query varchar;
    cnt int4;
begin
    
    select count(*) into cnt from pg_catalog.pg_tables where tablename = theTable and schemaname = 'public';
    if(cnt = 0) then
        return 0;
    end if;

    query := ' grant all on ' || theTable || ' to public';
    execute query;

    query := ' grant all on ' || theTable || '_id_seq to public';
    execute query;

    query = 'grant all on q_base_table_id_seq to public';
    execute query;


    return 1;

end
$BODY$
language 'plpgsql' security definer;

--used for tables that is realized attr type atCheckListEx (17)
--that table does not have any sequences (SERIAL attributes)
create or replace function setGrants1(varchar) returns int4 as
$BODY$
declare
    theTable alias for $1;
    r record;
    query varchar;
    cnt int4;
begin
    
    select count(*) into cnt from pg_catalog.pg_tables where tablename = theTable and schemaname = 'public';
    if(cnt = 0) then
        return 0;
    end if;

    query := ' grant all on ' || theTable || ' to public';
    execute query;

    return 1;

end
$BODY$
language 'plpgsql' security definer;

create or replace function setGrants(varchar, varchar) returns int4 as
$BODY$
declare
    theTable alias for $1;
    theUser alias for $2;

    r record;
    query varchar;
    cnt int4;
    gr varchar;
begin
    
    select count(*) into cnt from pg_catalog.pg_tables where tablename = theTable and schemaname = 'public';
    if(cnt = 0) then
        return 0;
    end if;

    gr = ' ';
    select count(*) into cnt from pg_group where groname = theUser;
    if(cnt > 0) then
        gr = ' GROUP ';
    end if;

    query := ' grant all on ' || theTable || ' to ' || gr || theUser;
    execute query;

    query := ' grant all on ' || theTable || '_id_seq to ' || gr || theUser;
    execute query;

    query = 'grant all on q_base_table_id_seq to ' || gr || theUser;
    execute query;

    return 1;

end
$BODY$
language 'plpgsql' security definer;
