\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql
\i ./functions/general/createtriggeruidex.sql


drop function recInsertRubric(int8, int8, varchar, varchar, varchar, varchar);
drop function recUpdateRubric(int8, varchar, varchar, int4, int4, int4, int8, varchar, varchar);
drop function recUpdateRubricEx(varchar, varchar, varchar, int8, int8, varchar);
drop function recUpdateRubricLocal(int8, varchar, varchar, varchar, varchar);

alter table record_rubricator add column id_io_object int4;
alter table record_rubricator add column id_io_category int4;

alter table record_rubricator
   add constraint FK_RECORD_R_REFERENCE_IO_OBJEC foreign key (id_io_object)
      references tbl_io_objects (id)
      on delete restrict on update restrict;

alter table record_rubricator
   add constraint FK_RECORD_R_REFERENCE_IO_CATEG foreign key (id_io_category)
      references io_categories (id)
      on delete restrict on update restrict;

alter table q_base_table add column rr_name varchar;

create or replace function aaa() returns int4 as
$BODY$
declare
    r record;
    tName varchar;
    q varchar;
    i int4;
    bHasName bool;
begin

    for r in select table_name from io_objects where id > 300 and table_name is not null
    loop

        tName = r.table_name;

        
        select f_is_view_exist(tName) into i;
        if(i = 0) then
            continue;
        end if;
        
        q = 'drop view ' || tName || ' cascade';
        execute q;
        
        q = 'select f_drop_funcs_' || tName || '();';
        execute q;
        
        q = 'alter table tbl_' || tName || ' rename to ' || tName;
        execute q;

        perform acl_secureTable(tName);
      
        select f_is_column_exist('tbl_' || tName, 'name') into bHasName;
        if(bHasName = true) then
            q = 'update tbl_' || tName || ' set rr_name = name;';
        else
            q = 'update tbl_' || tName || ' set rr_name = ' || quote_literal('Record ') || ' || id';
        end if;

        execute q;
        
    end loop;

    return 1;
end
$BODY$
language 'plpgsql' security definer;

select aaa();
drop function aaa();


\i ./functions/contribs/readd_contribs.sql

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/categories/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql


select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/readd_tasks_func.sql

--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÀ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÊÀÒÅÃÎĞÈÉ (ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_CATEGORIES_TABLE ÍÅ ÍÀÄÎ)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');
select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');

insert into attributes (unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-378', 378, 9, 'rr_name', 'Íàçâàíèå', 'Íàçâàíèå', NULL, NULL, 300, TRUE);

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÎÂ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÈÎ (ÍÀÄÎ ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--ÑŞÄÀ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÒÀÁËÈÖ ÑÏĞÀÂÎÍÈÊÎÂ (ÅÑËÈ ÑÎÇÄÀÂÀËÈÑÜ ÈÎ)


----------
----------


--ÂÑÅ ÎÑÒÀËÜÍÎÅ ÇÄÅÑÜ

update attributes set id_a_type = 30 where id = 1;

--ïî÷åìó-òî èíîãäà ğÿä àòğèáóòîâ íå îáíîâëÿåòñÿ.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
