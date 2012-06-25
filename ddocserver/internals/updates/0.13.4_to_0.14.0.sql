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

create table record_rubricator (
   id                   BIGSERIAL            not null,
   id_parent            INT8                 null,
   id_record            INT8                 null,
   name                 VARCHAR              not null,
   description          VARCHAR              null,
   constraint PK_RECORD_RUBRICATOR primary key (id)
);

comment on table record_rubricator is
'ğóáğèêàòîğ äëÿ çàïèñåé ñïğàâî÷íèêîâ

ÂÀÆÍÎ!!!
Â ÍÀÑÒÎßÙÅÅ ÂĞÅÌß POSTGRESQL  ÍÅ ÏÎÄÄÅĞÆÈÂÀÅÒ ÂÍÅØÍÈÅ ÊËŞ×È ÍÀ ÈÅĞÀĞÕÈŞ ÍÀÑËÅÄÎÂÀÍÈß ÒÀÁËÈÖ. ÏÎ İÒÎÉ ÏĞÈ×ÈÍÅ ÄÅËÀÒÜ ÂÍÅØÍÈÉ ÊËŞ× ÈÇ record_rubricator ÍÀ Q_BASE_TABLE ÍÅËÜÇß. Â ÈÍÒÅĞÍÅÒÅ ÑÓÙÅÑÒÂÓŞÒ ĞÀÇËÈ×ÍÛÅ ĞÅØÅÍÈß ÄÀÍÍÎÉ ÏĞÎÁËÅÌÛ, ÎÄÍÀÊÎ ÈÕ ĞÀÁÎÒÎÑÏÎÑÎÁÍÎÑÒÜ, À ÎÑÎÁÅÍÍÎ ÁÛÑÒĞÎÄÅÉÑÒÂÈÅ ÂÛÇÛÂÀŞÒ ÑÎÌÍÅÍÈÅ. ÏÎ İÒÎÉ ÏĞÈ×ÈÍÅ ĞÀÑÑÌÀÒĞÈÂÀÅÌÛÉ ÂÍÅØÍÈÉ ÊËŞ× ÍÅ ÃÅÍÅĞÈĞÓÅÒÑß ÑÊĞÈÏÒÎÌ ÑÎÇÄÀÍÈß ÁÄ.
ÄËß ÏÎÄÄÅĞÆÀÍÈß ÑÑÛËÎ×ÍÎÉ ÖÅËÎÑÒÍÎÑÒÈ ÍÀÏÈÑÀÍ ÒĞÈÃÃÅĞ, ÊÎÒÎĞÛÉ ÏĞÎÂÅĞßÅÒ ÔÀÊÒ ÑÓÙÅÑÒÂÎÂÀÍÈß ÇÀÏÈÑÈ, ÍÀ ÊÎÒÎĞÓŞ ÑÑÛËÀÅÒÑß ÑÎÇÄÀÂÀÅÌÀß (ÈÇÌÅÍßÅÌÀß) ÇÀÏÈÑÜ Â record_rubricator. 
';

alter table record_rubricator
   add constraint FK_RECORD_R_REFERENCE_RECORD_R foreign key (id_parent)
      references record_rubricator (id)
      on delete restrict on update restrict;


create table rubric_records (
   id_rubric            INT8                 not null,
   id_record            INT8                 not null,
   constraint PK_RUBRIC_RECORDS primary key (id_rubric, id_record)
);

comment on table rubric_records is
'Çàïèñè ñïğàâî÷íèêîâ, êîòîğûå íàõîäÿòñÿ â ñîîòâåòñòâóşùèõ ğóáğèêàõ

ÂÀÆÍÎ!!!
Â ÍÀÑÒÎßÙÅÅ ÂĞÅÌß POSTGRESQL  ÍÅ ÏÎÄÄÅĞÆÈÂÀÅÒ ÂÍÅØÍÈÅ ÊËŞ×È ÍÀ ÈÅĞÀĞÕÈŞ ÍÀÑËÅÄÎÂÀÍÈß ÒÀÁËÈÖ. ÏÎ İÒÎÉ ÏĞÈ×ÈÍÅ ÄÅËÀÒÜ ÂÍÅØÍÈÉ ÊËŞ× ÈÇ rubric_records ÍÀ Q_BASE_TABLE ÍÅËÜÇß. Â ÈÍÒÅĞÍÅÒÅ ÑÓÙÅÑÒÂÓŞÒ ĞÀÇËÈ×ÍÛÅ ĞÅØÅÍÈß ÄÀÍÍÎÉ ÏĞÎÁËÅÌÛ, ÎÄÍÀÊÎ ÈÕ ĞÀÁÎÒÎÑÏÎÑÎÁÍÎÑÒÜ, À ÎÑÎÁÅÍÍÎ ÁÛÑÒĞÎÄÅÉÑÒÂÈÅ ÂÛÇÛÂÀŞÒ ÑÎÌÍÅÍÈÅ. ÏÎ İÒÎÉ ÏĞÈ×ÈÍÅ ĞÀÑÑÌÀÒĞÈÂÀÅÌÛÉ ÂÍÅØÍÈÉ ÊËŞ× ÍÅ ÃÅÍÅĞÈĞÓÅÒÑß ÑÊĞÈÏÒÎÌ ÑÎÇÄÀÍÈß ÁÄ.
ÄËß ÏÎÄÄÅĞÆÀÍÈß ÑÑÛËÎ×ÍÎÉ ÖÅËÎÑÒÍÎÑÒÈ ÍÀÏÈÑÀÍ ÒĞÈÃÃÅĞ, ÊÎÒÎĞÛÉ ÏĞÎÂÅĞßÅÒ ÔÀÊÒ ÑÓÙÅÑÒÂÎÂÀÍÈß ÇÀÏÈÑÈ, ÍÀ ÊÎÒÎĞÓŞ ÑÑÛËÀÅÒÑß ÑÎÇÄÀÂÀÅÌÀß (ÈÇÌÅÍßÅÌÀß) ÇÀÏÈÑÜ Â rubric_records. ';

alter table rubric_records
   add constraint FK_RUBRIC_R_REFERENCE_RECORD_R foreign key (id_rubric)
      references record_rubricator (id)
      on delete restrict on update restrict;


create table urls_records (
   id_record            INT8                 not null,
   id_url               INT4                 not null,
   name                 VARCHAR              null,
   constraint PK_URLS_RECORDS primary key (id_record, id_url)
);

comment on table urls_records is
'Âçàèìîñâÿçè çàïèñåé ñïğàâî÷íèêîâ ñ ïğèêğåïëåííûìè ê íèì ôàéëàìè.
Ñóùåìñòâóåò âîçìîæíîñòü ïğèêğåïëÿòü ê çàïèñÿì ñïğàâî÷íèêîâ DynamicDocs ğàçëè÷íûå ôàéëû';

alter table urls_records
   add constraint FK_URLS_REC_REFERENCE_IO_URLS foreign key (id_url)
      references io_urls (id)
      on delete restrict on update restrict;


drop function rGetFileSizeByUrl(varchar);
create or replace function rGetFileSizeByUrl(varchar) returns int8
    as '$libdir/libfloader.so', 'rgetfilesizebyurl'  language 'c' strict security definer;

drop function rGetFileSize(int4);

\i ./functions/readd_functions.sql

alter table rec_attrs_values drop constraint fk_rec_attr_reference_q_base_t;
alter table rec_attrs_values rename to tbl_rec_attrs_values;

\i ./functions/security/readd_security.sql

\i ./functions/tasks/zarya22/readd_zarya_func.sql

--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÀ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÊÀÒÅÃÎĞÈÉ (ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_CATEGORIES_TABLE ÍÅ ÍÀÄÎ)
select f_safe_drop_trigger('trgcheckcatforglobal', 'io_categories');
--ñşäà
insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (11, 40, 'false', true, true);--is_archived (ğåäàêòèğîâàòü íåëüçÿ)

select f_create_trigger('trgcheckcatforglobal', 'before', 'insert or update', 'io_categories', 'checkcatforglobal()');


--ÑŞÄÀ (ÌÅÆÄÓ ÓÄÀËÅÍÈÅÌ È ÑÎÇÄÀÍÈÅÌ ÒĞÈÃÃÅĞÎÂ) ÏÎÌÅÙÀÅÌ ÇÀÏĞÎÑÛ ÍÀ ÑÎÇÄÀÍÈÅ ÈÎ (ÍÀÄÎ ÄÎÁÀÂËßÒÜ ÑÒĞÎÊÈ Â ACCESS_TABLE)
select f_safe_drop_trigger('trgioinsert', 'io_objects');
select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');
--ñşäà
select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');
select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');

--ÂÑÅ ÎÑÒÀËÜÍÎÅ ÇÄÅÑÜ

--äåëàåì ıòî òîëüêî äëÿ íåñèñòåìíûõ ñïğàâî÷íèêîâ DynamicDocs
select f_set_all_schemas_visible();
select createTriggerQBaseTableCheck1(table_name) from io_objects where id > 300 and table_name is not null;
--×ÒÎÁÛ Ó×ÅÑÒÜ ÒÀÁËÈÖÛ, "ĞÎÄÍÛÅ" ÄËß DYNAMICDOCS
select createTriggerQBaseTableCheck1('tbl_' || table_name) from io_objects where id > 300 and table_name is not null;

create or replace view rec_attrs_values as select * from f_sel_rec_attrs_values(NULL);
revoke all on tbl_rec_attrs_values from public;
grant all on rec_attrs_values to public;
                                        
create or replace rule "r_ins_rec_attrs_values" as on insert to "rec_attrs_values" do instead select "f_ins_rec_attrs_values"(new.id_record, new.id_attr_category, new.value, new.start_time, new.stop_time, new.meas_time, new.id_io_object_src, new.id_io_object_src1, new.description);
create or replace rule "r_del_rec_attrs_values" as on delete to "rec_attrs_values" do instead select "f_del_rec_attrs_values"(old.id_record, old.id_attr_category, true);
create or replace rule "r_upd_rec_attrs_values" as on update to "rec_attrs_values" do instead select "f_upd_rec_attrs_values"(new.id_record, new.id_attr_category, new.value, new.start_time, new.stop_time, new.meas_time, new.id_io_object_src, new.id_io_object_src1, new.description, old.id_record, old.id_attr_category, true);




--ïî÷åìó-òî èíîãäà ğÿä àòğèáóòîâ íå îáíîâëÿåòñÿ.
--*****
update attributes set id_ref_attr_type = 9 where id_ref_attr_type isnull and column_name is not null;

select setGrantsEx(NULL);

commit;
