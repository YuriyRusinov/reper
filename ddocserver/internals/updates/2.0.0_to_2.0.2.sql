\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);

--ÑŞÄÀ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÒÀÁËÈÖ ÑÏĞÀÂÎ×ÍÈÊÎÂ (ÅÑËÈ ÑÎÇÄÀÂÀËÈÑÜ ÈÎ)
--ÈÕ ÎÁßÇÀÒÅËÜÍÎ ÏÅĞÅÄ ÑÎÇÄÀÍÈÅÌ ÔÓÍÊÖÈÉ!!!
alter table report drop column if exists report_id;
alter table report add column report_id int4;
update report set report_id = id;
alter table report alter column report_id set not null;

alter table report drop column if exists name;
alter table report add column name varchar;
update report set name = report_name;
alter table report alter column name set not null;

alter table report alter column id type int8;
alter table report_organization alter column id_report type int8;
alter table report add column uuid_t uuid, add column id_io_state int4, add column rr_name varchar, add column r_icon varchar, add column record_fill_color int8, add column record_text_color int8;
            
update report set id = pg_catalog.nextval('q_base_table_id_seq');
alter table report alter column id set DEFAULT nextval('q_base_table_id_seq'::regclass);
update report set uuid_t = generateUUID();

alter table report alter column id set not null;
alter table report alter column unique_id set not null;
alter table report alter column uuid_t set not null;
alter table report alter column last_update set not null;
alter table report alter column last_update set default current_timestamp;
update report set id_io_state = 1;
alter table report alter column id_io_state set not null;
alter table report alter column id_io_state set default 1;
update report set rr_name = name;
alter table report alter column rr_name set not null;


alter table report inherit q_base_table;

alter table report add CONSTRAINT fk_q_base_table_ref_io_state_report FOREIGN KEY (id_io_state)
      REFERENCES io_states (id) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT;


create unique index i_report_id on report using btree (id);
create unique index i_report_unique_id on report using btree (unique_id);


select createTriggerUUID('report');
select createTriggerQBaseTableCheck1('report');

create or replace function check_report_id() returns trigger as
$BODY$
declare
    
begin
    if(new.report_id isnull) then
        new.report_id = new.id;
    end if;

    if(new.report_name isnull and new.name is not null) then
        new.report_name = new.name;
    end if;

    if(new.report_name is not null and new.name isnull) then
        new.name = new.report_name;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

--íåîáõîäèìî, ÷òîáû äàííûé òğèããåğ îòğàáàòûâàë ğàíüøå, ÷åì òğèããåğ trgsetuid, 
--êîòîğûé çàäàåò çíà÷åíèå êîëîíêå rr_name, èñïîëüçóÿ çíà÷åíèå êîëîíêè name
--ïîıòîìó íàçûâàòüñÿ îí äîëæåí òàê, ÷òîáû ïî àëôàâèòó ğàíüøå øåë.
select f_safe_drop_trigger('trg_check_report_id', 'report');
create trigger trg_check_report_id
before insert or update
on report
for each row 
execute procedure check_report_id();

---------------------------
---------------------------

\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql
\i ./functions/general/createtriggeruidex.sql

\i ./functions/contribs/readd_contribs.sql

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/categories/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql


\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/readd_tasks_func.sql

--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÀ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÊÀÒÅÃÎĞÈÉ (ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_CATEGORIES_TABLE ÍÅ ÍÀÄÎ)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
select f_safe_drop_trigger('trgacrecinsert', 'attrs_categories');
select f_safe_drop_trigger('trgacrecdelete', 'attrs_categories');
select f_safe_drop_trigger('trgacinsert', 'attrs_categories');

update attributes set code = 'report_source', id_a_type = 20, table_name = NULL, column_name = NULL where id = 104; --(unique_id, id, id_a_type, code, name, title, table_name, column_name, def_width, is_system) values('localorg-attributes-104', 104, 2, 'report_source', 'Øàáëîí îò÷åòà', 'Øàáëîí îò÷åòà', 'report', 'report_name', 150, TRUE);
update attrs_categories set id_io_attribute = 2 where id = 229; --name
insert into attrs_categories (id, id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (699, 71, 104, NULL, true, false); --report_source

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');
select f_create_trigger('trgacinsert', 'before', 'insert or update', 'attrs_categories', 'acinsertcheck()');
select f_create_trigger('trgacrecinsert', 'before', 'insert or update', 'attrs_categories', 'arecinsertcheck()');
select f_create_trigger('trgacrecdelete', 'before', 'delete', 'attrs_categories', 'arecdelcheck()');


--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÎÂ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÈÎ (ÍÀÄÎ ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
select f_safe_drop_trigger('trgsyncrecords', 'io_objects');

delete from access_table where id_io_object = 35;
delete from log where id_io_object = 35;
update tbl_io_objects set id = 300 where id = 35 ;--ñïğàâî÷íèê îò÷åòîâ
insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update)
values (300, -10, TRUE, TRUE, FALSE, FALSE);

select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');
select f_create_trigger('trgsyncrecords', 'before', 'insert or update or delete', 'io_objects', 'syncrecords()');


select setAsLogging(1);
select setAsLogging(2);

----------
----------


--ÂÑÅ ÎÑÒÀËÜÍÎÅ ÇÄÅÑÜ

--ïî÷åìó-òî èíîãäà ğÿä àòğèáóòîâ íå îáíîâëÿåòñÿ.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
