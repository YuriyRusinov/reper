create or replace function acl_secureTable(varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    r record;

    sParams varchar;
    sAliases varchar;
    sInsertAttrs varchar;
    sInsertValues varchar;
    aliasIndex int4;
    q varchar;

    qRule varchar;
    sRuleAttrs varchar;

    qDelete varchar;
    qRuleDelete varchar;

    qUpdate varchar;
    qRuleUpdate varchar;
    sUpdateValues varchar;
    updateIndex int4;
    isExist int4;

    dropFuncsQuery varchar;
    
begin

    select f_is_view_exist(tableName) into isExist; 

    q = 'alter table "' || tableName || '" rename to "tbl_' || tableName || '"';
    execute q;

    if(isExist = 0) then
        q = 'alter sequence "' || tableName || '_id_seq" rename to "tbl_' || tableName || '_id_seq"';
        --execute q;
    end if;

    q = 'create or replace function "f_sel_' || tableName || '"() returns setof "tbl_' || tableName || E'" as \n\$BODY\$\n';
    q = q || E'declare \n r "tbl_' || tableName || E'"%rowtype; \n begin \n';
    q = q || 'if(getPrivilege(getCurrentUser(), ioGetObjectIDByTableName(' || quote_literal(tableName) || '), 2, true) = false) then raise exception ' || quote_literal('You have insufficient permissions to do the operation!') || E'; return; end if;\n';
    q = q || 'for r in select * from "tbl_' || tableName || '" loop return next r; end loop;';
    q = q || E'return; \nend \n\$BODY\$ \nlanguage ' || quote_literal('plpgsql');
    q = q || ' security definer;';
    --raise warning '%', q;
    --return 1;
    execute q;    

    --q = 'create or replace view ' || tableName || ' as select * from tbl_' || tableName || ' where true = getPrivilege(getCurrentUser(), ioGetObjectIDByTableName(' || quote_literal(tableName) || '), 2, true);';
    q = 'create or replace view "' || tableName || '" as select * from "f_sel_' || tableName || '"()';
    execute q;

    q = 'revoke all on "tbl_' || tableName || '" from public;';
    execute q;

    q = 'grant all on "' || tableName || '" to public;';
    execute q;

    if(isExist = 0) then
        q = 'grant all on "tbl_' || tableName || '_id_seq" to public';
        --execute q;
    end if;


    aliasIndex = 1;
    updateIndex = 1;
    sAliases = '';
    sUpdateValues = '';
    
    for r in
        select 
            a.attname,
            t.typname
        from 
            pg_attribute a, 
            pg_class c,
            pg_type t
        where
            a.attrelid = c.oid
            and a.atttypid = t.oid
            and c.relname = tableName
            and a.attname not in ('tableoid', 'cmax', 'xmax', 'cmin', 'xmin', 'ctid')
    loop
        if(aliasIndex = 1) then
            sParams = r.typname;
            sInsertAttrs = '"' || r.attname || '"';
            sInsertValues = '"ii_' || r.attname || '"';
            sRuleAttrs = 'new."' || r.attname || '"';
        else
            sParams = sParams || ', ' || r.typname; 
            sInsertAttrs = sInsertAttrs || ', "' || r.attname || '"';
            sInsertValues = sInsertValues || ', "ii_' || r.attname || '"';
            sRuleAttrs = sRuleAttrs || ', new."' || r.attname || '"';
        end if; 

        sAliases = sAliases || '"ii_' || r.attname || E'" alias for \$' || aliasIndex || E';\n';
        if(r.attname <> 'id') then
            if(updateIndex = 1) then
                sUpdateValues = sUpdateValues || ' "' || r.attname || '" = "ii_' || r.attname || '"';
            else
                sUpdateValues = sUpdateValues || E',\n "' || r.attname || '" = "ii_' || r.attname || '"';
            end if;
            
            updateIndex = updateIndex + 1;
        end if;

        aliasIndex = aliasIndex + 1;
    end loop;
    
    --raise warning E'sParams = % \nsAliases = % \nsInsertValues = %\n', sParams, sAliases, sInsertValues;

    q = 'create or replace function "f_ins_' || tableName || '"(';
    q = q || sParams || E') returns int4 as \n\$BODY\$\n declare \n' || sAliases || E' \nbegin \n'; 
    q = q || 'if(getPrivilege(getCurrentUser(), ioGetObjectIDByTableName(' || quote_literal(tableName) || '), 4, true) = false) then raise exception ' || quote_literal('You have insufficient permissions to do the operation!') || E'; return 0; end if;\n';
    q = q || 'insert into "tbl_' || tableName || '" (' || sInsertAttrs || ') values (' || sInsertValues || E');\n';
    q = q || E'return 1; \nend \n\$BODY\$ \nlanguage ' || quote_literal('plpgsql');
    q = q || ' security definer;';
    
    --raise warning E'%\n\n', q;
    execute q;

    qRule = 'create or replace rule "r_ins_' || tableName || '" as on insert to "' || tableName || '" do instead select "f_ins_' || tableName || '" (' || sRuleAttrs || ');';
    --raise warning E'%\n\n', qRule;
    execute qRule;

    qDelete = 'create or replace function "f_del_' || tableName || E'"(int8) returns int4 as \n\$BODY\$ \ndeclare \n ii_id alias for $1; \nbegin \n';
    qDelete = qDelete || 'if(getPrivilege(getCurrentUser(), ioGetObjectIDByTableName(' || quote_literal(tableName) || '), 4, true) = false) then raise exception ' || quote_literal('You have insufficient permissions to do the operation!') || E'; return 0; end if;\n';
    qDelete = qDelete || 'delete from "tbl_' ||tableName || E'" where id = ii_id;\n';
    qDelete = qDelete || E'return 1::int4; \nend \n\$BODY\$ \nlanguage ' || quote_literal('plpgsql');
    qDelete = qDelete || ' security definer;';

    --raise warning E'%\n\n', qDelete;
    execute qDelete;

    qRuleDelete = 'create or replace rule "r_del_' || tableName || '" as on delete to "' || tableName || '" do instead select "f_del_' || tableName || '"(old.id);';
    --raise warning E'%\n\n', qRuleDelete;
    execute qRuleDelete;

    qUpdate = 'create or replace function "f_upd_' || tableName || '"(' || sParams || E') returns int4 as \n\$BODY\$ \ndeclare \n' || sAliases || E' \nbegin \n';
    qUpdate = qUpdate || 'if(getPrivilege(getCurrentUser(), ioGetObjectIDByTableName(' || quote_literal(tableName) || '), 4, true) = false) then raise exception ' || quote_literal('You have insufficient permissions to do the operation!') || E'; return 0; end if;\n';
    qUpdate = qUpdate || 'update "tbl_' || tableName || E'" set \n' || sUpdateValues || E'\n where id = ii_id;\n';
    qUpdate = qUpdate || E'return 1; \nend \n\$BODY\$ \nlanguage ' || quote_literal('plpgsql');
    qUpdate = qUpdate || ' security definer;';

    --raise warning E'%\n\n', qUpdate;
    execute qUpdate;

    qRuleUpdate = 'create or replace rule "r_upd_' || tableName || '" as on update to "' || tableName || '" do instead select "f_upd_' || tableName || '" (' || sRuleAttrs || ');';
    --raise warning E'%\n\n', qRuleUpdate;
    execute qRuleUpdate;

    dropFuncsQuery := 'create or replace function "f_drop_funcs_' || tableName || E'"() returns int4 as \n\$BODY\$\n';
    dropFuncsQuery := dropFuncsQuery || E'begin \n';
    dropFuncsQuery := dropFuncsQuery || ' drop function f_sel_' || tableName || E'();\n';
    dropFuncsQuery := dropFuncsQuery || ' drop function f_del_' || tableName || E'(int8);\n';
    dropFuncsQuery := dropFuncsQuery || ' drop function f_ins_' || tableName || E'(' || sParams || E');\n';
    dropFuncsQuery := dropFuncsQuery || ' drop function f_upd_' || tableName || E'(' || sParams || E');\n';
    dropFuncsQuery := dropFuncsQuery || E' return 1;\n end \n\$BODY\$ \nlanguage ' || quote_literal('plpgsql');

    execute dropFuncsQuery;


    return 1; 
end
$BODY$
language 'plpgsql';
