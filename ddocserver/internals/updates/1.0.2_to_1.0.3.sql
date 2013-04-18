\encoding win1251

begin;

select createTempTables();
select setCurrentDl(4);

select setAsNotLogging(1);
select setAsNotLogging(2);


\i ./functions/misc/f_safe_drop_trigger.sql
\i ./functions/misc/f_create_trigger.sql
\i ./functions/misc/f_is_table_exist.sql

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

\i ./functions/tasks/zarya22/readd_zarya_func.sql


drop view shu_chksum;
select f_drop_funcs_shu_chksum();
alter sequence shu_chksum_id_seq rename to tbl_shu_chksum_id_seq;
alter table tbl_shu_chksum rename to shu_chksum;
alter table shu_chksum add column unique_id varchar;
alter table shu_chksum add column last_update timestamp;
update shu_chksum set unique_id = generateUID(id, 'shu_chksum'), last_update = current_timestamp;
alter table shu_chksum alter column unique_id set not null;
alter table shu_chksum alter column last_update set not null;
alter table shu_chksum alter column last_update set default current_timestamp;
alter table shu_chksum inherit root_table;
select setMacToNULL('shu_chksum');
select createTriggerUID('shu_chksum');
select acl_secureTable('shu_chksum');

drop view shu_domains;
select f_drop_funcs_shu_domains();
alter sequence shu_domains_id_seq rename to tbl_shu_domains_id_seq;
alter table tbl_shu_domains rename to shu_domains;
alter table shu_domains add column unique_id varchar;
alter table shu_domains add column last_update timestamp;
update shu_domains set unique_id = generateUID(id, 'shu_domains'), last_update = current_timestamp;
alter table shu_domains alter column unique_id set not null;
alter table shu_domains alter column last_update set not null;
alter table shu_domains alter column last_update set default current_timestamp;
alter table shu_domains inherit root_table;
select setMacToNULL('shu_domains');
select createTriggerUID('shu_domains');
select acl_secureTable('shu_domains');


drop view shu_positions;
select f_drop_funcs_shu_positions();
alter sequence shu_positions_id_seq rename to tbl_shu_positions_id_seq;
alter table tbl_shu_positions rename to shu_positions;
alter table shu_positions add column unique_id varchar;
alter table shu_positions add column last_update timestamp;
update shu_positions set unique_id = generateUID(id, 'shu_positions'), last_update = current_timestamp;
alter table shu_positions alter column unique_id set not null;
alter table shu_positions alter column last_update set not null;
alter table shu_positions alter column last_update set default current_timestamp;
alter table shu_positions inherit root_table;
select setMacToNULL('shu_positions');
select createTriggerUID('shu_positions');
select acl_secureTable('shu_positions');


drop view shu_acs;
select f_drop_funcs_shu_acs();
alter sequence shu_acs_id_seq rename to tbl_shu_acs_id_seq;
alter table tbl_shu_acs rename to shu_acs;
alter table shu_acs add column unique_id varchar;
alter table shu_acs add column last_update timestamp;
update shu_acs set unique_id = generateUID(id, 'shu_acs'), last_update = current_timestamp;
alter table shu_acs alter column unique_id set not null;
alter table shu_acs alter column last_update set not null;
alter table shu_acs alter column last_update set default current_timestamp;
alter table shu_acs inherit root_table;
select setMacToNULL('shu_acs');
select createTriggerUID('shu_acs');
select acl_secureTable('shu_acs');




drop view shu_dls;
select f_drop_funcs_shu_dls();
alter sequence shu_dls_id_seq rename to tbl_shu_dls_id_seq;
alter table tbl_shu_dls rename to shu_dls;
alter table shu_dls add column unique_id varchar;
alter table shu_dls add column last_update timestamp;
update shu_dls set unique_id = generateUID(id, 'shu_dls'), last_update = current_timestamp;
alter table shu_dls alter column unique_id set not null;
alter table shu_dls alter column last_update set not null;
alter table shu_dls alter column last_update set default current_timestamp;
alter table shu_dls inherit root_table;
select setMacToNULL('shu_dls');
select createTriggerUID('shu_dls');
select acl_secureTable('shu_dls');

drop view shu_orgs;
select f_drop_funcs_shu_orgs();
alter sequence shu_orgs_id_seq rename to tbl_shu_orgs_id_seq;
alter table tbl_shu_orgs rename to shu_orgs;
alter table shu_orgs add column unique_id varchar;
alter table shu_orgs add column last_update timestamp;
update shu_orgs set unique_id = generateUID(id, 'shu_orgs'), last_update = current_timestamp;
alter table shu_orgs alter column unique_id set not null;
alter table shu_orgs alter column last_update set not null;
alter table shu_orgs alter column last_update set default current_timestamp;
alter table shu_orgs inherit root_table;
select setMacToNULL('shu_orgs');
select createTriggerUID('shu_orgs');
select acl_secureTable('shu_orgs');

insert into shu_acs(code, name, uri) values ('zarya22', 'zarya22', 'http://zaria22.acs.mil');
insert into shu_acs(code, name, uri) values ('vershina2', 'vershina2', 'http://vershina2.acs.mil');

insert into shu_domains(code, name) values('intelligence.Ground', 'intelligence.Ground');
insert into shu_domains(code, name) values('intelligence.Air', 'intelligence.Air');
insert into shu_domains(code, name) values('intelligence.Sea', 'intelligence.Sea');

insert into shu_positions(code, name) values('nachGRNapr', 'nachGRNapr');
insert into shu_positions(code, name) values('operDezh', 'operDezh');

insert into shu_orgs(code, name, uri) values('oskUVO', 'oskUVO', 'http://zaria22.acs.mil/ou/oskUVO');
insert into shu_orgs(code, name, uri) values('orgUnit', 'orgUnit', 'http://vershina2.acs.mil/ou/orgUnit');

insert into shu_dls(name, uri, id_acs, id_org, id_pos) values('nachGRNapr', 'http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr', 1, 1, 1);
insert into shu_dls(name, uri, id_acs, id_org, id_pos) values('operDezh', 'http://vershina2.acs.mil/ou/orgUnit/pu/operDezh', 2, 2, 2);



--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÀ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÊÀÒÅÃÎĞÈÉ (ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_CATEGORIES_TABLE ÍÅ ÍÀÄÎ)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');


select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÎÂ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÈÎ (ÍÀÄÎ ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');


select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--ÑŞÄÀ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÒÀÁËÈÖ ÑÏĞÀÂÎÍÈÊÎÂ (ÅÑËÈ ÑÎÇÄÀÂÀËÈÑÜ ÈÎ)


----------
----------


--ÂÑÅ ÎÑÒÀËÜÍÎÅ ÇÄÅÑÜ


--ïî÷åìó-òî èíîãäà ğÿä àòğèáóòîâ íå îáíîâëÿåòñÿ.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
