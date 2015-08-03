\encoding win1251

begin;

alter table message_journal drop column id_user_sender;
alter table message_journal drop column id_user_receiver;

alter table message_journal alter column id_io_object drop not null;
alter table message_journal alter column message_body drop not null;
alter table message_journal alter column sent_datetime drop not null;

alter table message_journal add column insert_time timestamp;
alter table message_journal alter column insert_time set not null;
--alter table message_journal alter column insert_time set default current_timestamp;
alter table message_journal add column is_outed bool;
alter table message_journal alter column is_outed set default true;
alter table message_journal alter column is_outed set not null;

alter table message_journal add column id_dl_sender int4;
alter table message_journal add column id_dl_receiver int4;
alter table message_journal alter column id_dl_sender set not null;
alter table message_journal alter column id_dl_receiver set not null;
alter table message_journal add constraint fk_message_dl_recv FOREIGN KEY (id_dl_receiver) REFERENCES dls(id) ON UPDATE RESTRICT ON DELETE RESTRICT;
alter table message_journal add constraint fk_message_dl_sender FOREIGN KEY (id_dl_sender) REFERENCES dls(id) ON UPDATE RESTRICT ON DELETE RESTRICT;

insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (39, 16, 'current_timestamp', true, false);

insert into a_views (id, name) values (10, 'Изображение JPG');
insert into a_types (id, id_a_view, name, code) values (15, 10, 'Изображение JPG', 'VARCHAR');

SELECT pg_catalog.setval('a_views_id_seq', 10, true);
SELECT pg_catalog.setval('a_types_id_seq', 15, true);


update attributes set id_a_type = 13 where id in (39, 45);

\i ./functions/readd_functions.sql

commit;
