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

--select createTriggerUIDEx('criteria');

\i ./functions/contribs/readd_contribs.sql

--In readd_functions this calls does not invoked
--So, we should to invoke that here
\i ./functions/general/generateuid.sql
\i ./functions/io_objects/triggers/readd_triggers.sql
\i ./functions/categories/triggers/readd_triggers.sql
\i ./functions/inf_exchange/createtriggers.sql

\i ./functions/io_objects/checkioforowner.sql

create table processing_variant (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_PROCESSING_VARIANT primary key (id)
)
inherits (root_table);

comment on table processing_variant is
'âàğèàíòû îáğàáîòêè ÈÎ â î÷åğåäÿõ';

select setMacToNULL('processing_variant');
select createTriggerUID('processing_variant');

create table processing_scenario (
   id                   SERIAL               not null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_PROCESSING_SCENARIO primary key (id)
)
inherits (root_table);

comment on table processing_scenario is
'Ñöåíàğèè îáğàáîòêè ÈÎ
(ïÿäü)';

select setMacToNULL('processing_scenario');
select createTriggerUID('processing_scenario');

insert into processing_scenario (id, name) values (1, 'ïÓÎÏ×ÎÏÊ ÓÃÅÎÁÒÉÊ');
insert into processing_variant (id, name) values (1, 'ïÓÎÏ×ÎÏÊ ×ÁÒÉÁÎÔ');
SELECT pg_catalog.setval('processing_scenario_id_seq', 1, true);
SELECT pg_catalog.setval('processing_variant_id_seq', 1, true);


alter table chains_data add column id_parent int4;
alter table chains_data add column id_processing_scenario int4;
alter table chains_data add column id_processing_variant int4;
update chains_data set id_processing_scenario = 1, id_processing_variant = 1;
alter table chains_data alter column id_processing_scenario set not null;
alter table chains_data alter column id_processing_variant set not null;

alter table chains_data
   add constraint FK_CHAINS_D_REFERENCE_CHAINS_D foreign key (id_parent)
      references chains_data (id)
      on delete restrict on update restrict;

alter table chains_data
   add constraint FK_CHAINS_D_REF_PROCESSING_VARIANT foreign key (id_processing_variant)
      references processing_variant (id)
      on delete restrict on update restrict;

alter table chains_data
   add constraint FK_CHAINS_D_REFERENCE_PROCESSI foreign key (id_processing_scenario)
      references processing_scenario (id)
      on delete restrict on update restrict;


select setAsLogging(1);
select setAsLogging(2);

\i ./functions/readd_functions.sql

\i ./functions/security/readd_security.sql

\i ./functions/tasks/readd_tasks_func.sql

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
insert into url_types (id, name, win_app, lin_app, extentions) values (11, 'Ôàéëû äëÿ âñòğîåííîé ïîääåğæêè ÃÈÑ (â ñïèñîê ôàéëîâ ÈÎ íå ïîïàäàşò!)', '', '', '{}');
SELECT pg_catalog.setval('url_types_id_seq', 11, true);

--ïî÷åìó-òî èíîãäà ğÿä àòğèáóòîâ íå îáíîâëÿåòñÿ.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);


commit;
